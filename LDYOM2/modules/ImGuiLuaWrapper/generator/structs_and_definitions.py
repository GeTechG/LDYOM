import json
import re
from pathlib import Path

import rtoml

import prettier


def skip_function(func):
    return func in ["igTextUnformatted", "igInputText", "igInputTextMultiline", "igInputTextWithHint", "igCalcTextSize"]


# ─── C-ABI type helpers ────────────────────────────────────────────────────
# cimgui 1.92.x wraps value types in _c structs for C ABI compatibility.
# In generated Lua bindings we want to use the plain C++ types.

_C_TO_CPP_TYPE = {
    'ImVec2_c':       'ImVec2',
    'ImVec4_c':       'ImVec4',
    'ImVec2i_c':      'ImVec2i',
    'ImTextureRef_c': 'ImTextureRef',
    'ImColor_c':      'ImColor',
    'ImRect_c':       'ImRect',   # ImRect is NOT exposed to Lua – skip functions using it
}
_SKIP_C_TYPES = frozenset({'ImRect_c'})
_CPP_TO_C_TYPE = {v: k for k, v in _C_TO_CPP_TYPE.items()}


def _c_types_in_str(s: str) -> set:
    return set(re.findall(r'Im\w+_c\b', s))


def _should_skip_c(func) -> bool:
    """Return True if func uses Im*_c types that are not exposed to Lua (e.g. ImRect_c),
    or has out-pointer args combined with _c types (complex internal functions),
    or is variadic (C variadics can't be wrapped in a C++ lambda)."""
    all_c = _c_types_in_str(func.get('args', '')) | _c_types_in_str(func.get('ret', ''))
    if not all_c:
        return False
    if all_c & _SKIP_C_TYPES:
        return True
    # Skip combined _c + out-pointer case – complex internal functions
    if any('**' in a['type'] for a in func.get('argsT', [])):
        return True
    # Skip variadic functions – C '...' can't be forwarded in a lambda
    if any(a['type'] == '...' for a in func.get('argsT', [])):
        return True
    return False


def _has_c_types(func) -> bool:
    """Return True if func has Im*_c types that we CAN handle with a conversion lambda."""
    return bool(_c_types_in_str(func.get('args', '')) | _c_types_in_str(func.get('ret', '')))


def _arg_call_expr(arg_name: str, argsT_type: str, func_args_str: str) -> str:
    """Return the expression to pass arg to cimgui, converting Im*_c if needed."""
    base = argsT_type.replace('const', '').replace('*', '').strip()
    c_type = _CPP_TO_C_TYPE.get(base)
    if not (c_type and c_type in func_args_str and c_type not in _SKIP_C_TYPES):
        return arg_name
    if '*' in argsT_type:
        # Pointer: same layout → reinterpret_cast
        const_q = 'const ' if 'const' in argsT_type else ''
        return f'reinterpret_cast<{const_q}{c_type}*>({arg_name})'
    else:
        # Value: std::bit_cast (same size/layout, no external helper needed)
        return f'std::bit_cast<{c_type}>({arg_name})'


def _format_param(argsT_type: str, arg_name: str) -> str:
    """Format 'type name', correctly embedding name into function pointer types."""
    # e.g. "float(*)(void* data,int idx)" -> "float(*values_getter)(void* data,int idx)"
    if re.match(r'.+\(\*\)', argsT_type):
        return argsT_type.replace('(*)', f'(*{arg_name})', 1)
    return f'{argsT_type} {arg_name}'


def _gen_c_lambda(func, deleter: str = '') -> str:
    """Generate a lambda that converts Im*_c ↔ C++ types for args and/or return value."""
    argsT   = func.get('argsT', [])
    args_s  = func.get('args', '')
    ret     = func.get('ret', 'void')
    fn      = func['ov_cimguiname']
    is_ctor = 'constructor' in func

    in_params = [_format_param(a['type'], a['name']) for a in argsT]
    call_args = [_arg_call_expr(a['name'], a['type'], args_s) for a in argsT]
    call_str  = ', '.join(call_args)

    if is_ctor:
        sn = deleter.split('_')[0] if deleter else func.get('stname', '')
        return (f"[]({', '.join(in_params)}) {{\n"
                f"            auto result = {fn}({call_str});\n"
                f"            return std::shared_ptr<{sn}>(result, {deleter});\n"
                f"        }}")

    c_ret = _c_types_in_str(ret) - _SKIP_C_TYPES
    if ret == 'void':
        body = f'{fn}({call_str});'
    elif c_ret:
        cpp_r = _C_TO_CPP_TYPE[next(iter(c_ret))]
        if '*' in ret:
            # Pointer return: reinterpret_cast (same layout)
            const_q = 'const ' if 'const' in ret else ''
            body = f'return reinterpret_cast<{const_q}{cpp_r}*>({fn}({call_str}));'
        else:
            body = f'return std::bit_cast<{cpp_r}>({fn}({call_str}));'
    else:
        body = f'return {fn}({call_str});'

    return f"[]({', '.join(in_params)}) {{\n            {body}\n        }}"


with open('structs_templates.toml', 'r') as f:
    structs_templates = rtoml.load(f)


def parse_union_declaration(union_declaration):
    pattern = r'\s*union\s*\{(.+?)\}\s*'

    match = re.search(pattern, union_declaration, re.DOTALL)

    if match:
        body = match.group(1)

        field_pattern = r'\s*([^;]+);'

        fields = re.findall(field_pattern, body)

        result = []
        for field in fields:
            field = field.strip()
            parts = field.split()

            name = parts[-1]
            type_ = ' '.join(parts[:-1])

            array_match = re.search(r'(.+?)\[(\d+)]$', name)
            if array_match:
                name = array_match.group(1)
                size = int(array_match.group(2))
            else:
                size = None

            result.append({'name': name, 'type': type_, 'size': size})

        return result
    else:
        print("ERROR: union declaration not found")


_CPP_PRIMITIVES_WITH_UNDERSCORE = {
    "unsigned_char", "unsigned_short", "unsigned_int", "unsigned_long",
    "long_long", "unsigned_long_long", "long_double",
}

def extract_type(input_string):
    match = re.search(r'ImVector_(\w+).*?', input_string)

    if match:
        raw = match.group(1)
        if raw.endswith("Ptr"):
            return raw[:-3] + "*"
        if raw in _CPP_PRIMITIVES_WITH_UNDERSCORE:
            return raw.replace("_", " ")
        return raw
    else:
        print("ERROR: ImVector type not found")


def extract_type_of_type(input_string: str) -> tuple[bool, str]:
    if input_string.find("*") != -1:
        if input_string[:-1] in ["unsigned char", "short", "unsigned short", "int", "unsigned int", "long",
                                 "unsigned long", "long long", "unsigned long long", "float", "double", "long double",
                                 "bool"]:
            return True, input_string[:-1]
        else:
            return False, input_string
    else:
        if input_string.find("[") != -1:
            return False, input_string.split("[")[0] + "*"
        else:
            return False, input_string


def function_generator(func, struct_name, deleter=""):
    # Skip functions whose _c types can't be safely handled (ImRect_c, combined with **)
    if _should_skip_c(func):
        return None

    list_types_input_args = list(map(extract_type_of_type, map(lambda x: x["type"], func["argsT"])))

    if not any(arg[0] for arg in list_types_input_args):
        if "ret" in func and func["ret"].endswith("**"):
            declaration_function = structs_templates["cpp"]["function_declaration_return_ppointer"].format(
                input_args=func["args"], full_function_name=func["ov_cimguiname"],
                call_args="(self, " + func["call_args"][1:])
        else:
            if "constructor" in func:
                if _has_c_types(func):
                    declaration_function = _gen_c_lambda(func, deleter)
                else:
                    declaration_function = structs_templates["cpp"]["constructor_declaration"].format(
                        input_args=func["args"], full_function_name=func["ov_cimguiname"],
                        call_args=func["call_args"], struct_name=deleter.split("_")[0], deleter=deleter)
            else:
                if _has_c_types(func):
                    declaration_function = _gen_c_lambda(func)
                else:
                    declaration_function = structs_templates["cpp"]["function_declaration"].format(
                        full_function_name=func["ov_cimguiname"])
        return declaration_function
    else:
        input_args = []
        pointers_declaration = []
        input_args_with_pointers = []
        output_vars = []
        if func["ret"] != "void":
            output_vars.append("result")
        for i, arg in enumerate(list_types_input_args):
            arg_name = func["argsT"][i]["name"]
            if not arg[1].endswith("**"):
                arg_ = arg[1]
                inp_name = arg_name
                if arg_.endswith("[]"):
                    arg_ = arg_[:-2]
                    inp_name = arg_name + "[]"
                if arg_ == "bool(*)(void* data,int idx,const char** out_text)":
                    arg_ = "bool(*items_getter)(void* data,int idx,const char** out_text)"
                    inp_name = ""
                if arg_ == "const char*(*)(void* user_data,int idx)":
                    arg_ = "const char*(*getter)(void* user_data,int idx)"
                    inp_name = ""
                if arg[0]:
                    arg_ = "const sol::object&"
                input_args.append(
                    structs_templates["cpp"]["function_input_arg"].format(type=arg_, input_name=inp_name))
            else:
                pointers_declaration.append(
                    structs_templates["cpp"]["function_pointers_pointer_declaration"].format(pointer_name=arg_name,
                                                                                             type=arg[1][:-1],
                                                                                             input_value="nullptr"))
                output_vars.append("std::move(" + arg_name + ")")
            if arg[0]:
                name_ptr_ = arg_name + "Ptr"
                input_args_with_pointers.append(
                    arg_name + ".get_type() == sol::type::lua_nil ? nullptr: " + name_ptr_ + ".get()");
                pointers_declaration.append(
                    structs_templates["cpp"]["function_pointers_declaration"].format(pointer_name=name_ptr_,
                                                                                     var_name=arg_name,
                                                                                     type=arg[1],
                                                                                     input_value=arg_name))
                output_vars.append("*" + name_ptr_)
            else:
                if not arg[1].endswith("**"):
                    input_args_with_pointers.append(arg_name)
                else:
                    input_args_with_pointers.append(arg_name + ".get()")

        call_function = structs_templates["cpp"][
            "call_function_result" if func["ret"] != "void" else "call_function"].format(
            full_function_name=func["ov_cimguiname"],
            input_args_with_pointers=", ".join(input_args_with_pointers))
        declaration_function = structs_templates["cpp"]["function_declaration_with_pointers"].format(
            input_args=", ".join(input_args), pointers_declaration='\n\t'.join(pointers_declaration),
            call_function=call_function, output_vars=", ".join(output_vars))
        return declaration_function


with open('structs_and_enums.json', 'r') as f:
    json_load = json.load(f)
    struct_data = json_load['structs']
    enums_data = json_load['enums']


def convert_type_teal(in_type: str):
    match in_type:
        case "unsigned char":
            return "integer"
        case "short":
            return "integer"
        case "unsigned short":
            return "integer"
        case "int":
            return "integer"
        case "unsigned int":
            return "integer"
        case "long":
            return "integer"
        case "unsigned long":
            return "integer"
        case "long long":
            return "integer"
        case "unsigned long long":
            return "integer"
        case "float":
            return "number"
        case "double":
            return "number"
        case "long double":
            return "number"
        case "bool":
            return "boolean"
        case "ImU32":
            return "integer"
        case "const char*":
            return "string"
    if in_type.endswith("*"):
        in_type = in_type[:-1]
    in_type = in_type.replace("const", "").strip()
    if in_type in struct_data.keys():
        return in_type
    if (in_type + "_") in enums_data.keys() or in_type in enums_data.keys():
        return "integer"
    if in_type.startswith("ImVector"):
        return in_type.replace("*", "")
    return "any"


def convert_type_lua(in_type: str):
    match in_type:
        case "unsigned char":
            return "integer"
        case "short":
            return "integer"
        case "unsigned short":
            return "integer"
        case "int":
            return "integer"
        case "unsigned int":
            return "integer"
        case "long":
            return "integer"
        case "unsigned long":
            return "integer"
        case "long long":
            return "integer"
        case "unsigned long long":
            return "integer"
        case "float":
            return "number"
        case "double":
            return "number"
        case "long double":
            return "number"
        case "bool":
            return "boolean"
        case "ImU32":
            return "integer"
        case "const char*":
            return "string"
        case "...":
            return "any"
    if in_type.endswith("*"):
        in_type = in_type[:-1]
    in_type = in_type.replace("const", "").strip()
    if in_type in struct_data.keys():
        return in_type
    if (in_type + "_") in enums_data.keys() or in_type in enums_data.keys():
        return in_type
    if in_type.startswith("ImVector"):
        return in_type.replace("*", "")
    return "userdata"


def function_generator_lua(func):
    list_types_input_args = list(map(extract_type_of_type, map(lambda x: x["type"], func["argsT"])))

    if not any(arg[0] for arg in list_types_input_args):
        params = []
        inputs = []
        for i, arg in enumerate(list_types_input_args):
            args_t_i_name_ = func["argsT"][i]["name"]
            if args_t_i_name_ in ["end", "repeat", "in"]:
                args_t_i_name_ += "_"
            inputs.append(args_t_i_name_)
            params.append(structs_templates["lua"]["function_parameter"].format(name=args_t_i_name_,
                                                                                type=convert_type_lua(arg[1])))
        if "destructor" in func:
            declaration_function = structs_templates["lua"]["function_declaration_void"].format(
                name="__gc",
                info="destructor" + func["args"],
                inputs=", ".join(inputs),
                params="\n\t" + "\n\t".join(params) if len(params) > 0 else "")
        elif "ret" in func and func["ret"] == "void":
            declaration_function = structs_templates["lua"]["function_declaration_void"].format(
                name=func["funcname"],
                info=func["funcname"] + func["argsoriginal"],
                inputs=", ".join(inputs),
                params="\n\t" + "\n\t".join(params) if len(params) > 0 else "")
        else:
            if "constructor" in func:
                declaration_function = structs_templates["lua"]["function_declaration_return"].format(
                    name="new",
                    info=func["funcname"] + func["argsoriginal"],
                    inputs=", ".join(inputs),
                    params="\n\t" + "\n\t".join(params) if len(params) > 0 else "",
                    returns=convert_type_lua(func["stname"]))
            else:
                funcname_ = func["funcname"]
                if funcname_ == "end":
                    funcname_ = "[\"end\"]"
                declaration_function = structs_templates["lua"]["function_declaration_return"].format(
                    name=funcname_,
                    info=func["funcname"] + func["argsoriginal"],
                    inputs=", ".join(inputs),
                    params="\n\t" + "\n\t".join(params) if len(params) > 0 else "",
                    returns=convert_type_lua(func["ret"]))
        return declaration_function
    else:
        params = []
        inputs = []
        output_vars = []
        if func["ret"] != "void":
            output_vars.append(convert_type_lua(func["ret"]))
        for i, arg in enumerate(list_types_input_args):
            arg_name = func["argsT"][i]["name"]
            if not arg[1].endswith("**"):
                arg_ = arg[1]
                inp_name = arg_name

                if inp_name in ["end", "repeat", "in"]:
                    inp_name += "_"

                if arg_ == "bool(*)(void* data,int idx,const char** out_text)":
                    arg_ = "userdata"
                elif arg_ == "const char*(*)(void* user_data,int idx)":
                    arg_ = "userdata"
                else:
                    arg_ = convert_type_lua(arg_)
                inputs.append(inp_name)
                if arg[0]:
                    arg_ += "|nil"
                params.append(structs_templates["lua"]["function_parameter"].format(name=inp_name, type=arg_))
            else:
                output_vars.append(convert_type_lua(arg[1]))
            if arg[0]:
                output_vars.append(convert_type_lua(arg[1]))

        func_funcname_ = name = func["funcname"]
        if func_funcname_ == "end":
            func_funcname_ = "[\"end\"]"
        declaration_function = (structs_templates["lua"]["function_declaration_return"]
                                .format(name=func_funcname_, info=func["funcname"] + func["argsoriginal"],
                                        inputs=", ".join(inputs),
                                        params="\n\t" + ",\n\t".join(params) if len(params) > 0 else "",
                                        returns=", ".join(output_vars)))
        return declaration_function


def function_generator_teal(func):
    list_types_input_args = list(map(extract_type_of_type, map(lambda x: x["type"], func["argsT"])))

    if not any(arg[0] for arg in list_types_input_args):
        input_args = []
        for i, arg in enumerate(list_types_input_args):
            args_t_i_name_ = func["argsT"][i]["name"]
            if args_t_i_name_ in ["end", "repeat", "in"]:
                args_t_i_name_ += "_"
            input_args.append(structs_templates["teal"]["struct_record_item"].format(args_t_i_name_,
                                                                                     convert_type_teal(arg[1])))
        if "destructor" in func:
            declaration_function = structs_templates["teal"]["function_declaration_void"].format(
                name="__gc",
                input_args=", ".join(input_args))
        elif "ret" in func and func["ret"] == "void":
            declaration_function = structs_templates["teal"]["function_declaration_void"].format(
                name=func["funcname"],
                input_args=", ".join(input_args))
        else:
            if "constructor" in func:
                declaration_function = structs_templates["teal"]["function_declaration_return"].format(
                    name="new",
                    input_args=", ".join(input_args),
                    returns=convert_type_teal(func["stname"]))
            else:
                funcname_ = func["funcname"]
                if funcname_ in ["end", "repeat", "in"]:
                    funcname_ += "_"
                declaration_function = structs_templates["teal"]["function_declaration_return"].format(
                    name=funcname_,
                    input_args=", ".join(input_args),
                    returns=convert_type_teal(func["ret"]))
        return declaration_function
    else:
        input_args = []
        output_vars = []
        if func["ret"] != "void":
            output_vars.append(convert_type_teal(func["ret"]))
        for i, arg in enumerate(list_types_input_args):
            arg_name = func["argsT"][i]["name"]
            if not arg[1].endswith("**"):
                arg_ = arg[1]
                inp_name = arg_name

                if inp_name in ["end", "repeat", "in"]:
                    inp_name += "_"

                if arg_.endswith("[]"):
                    arg_ = "{" + convert_type_teal(arg_[:-2]) + "}"
                elif arg_ == "bool(*)(void* data,int idx,const char** out_text)":
                    arg_ = "any"
                elif arg_ == "const char*(*)(void* user_data,int idx)":
                    arg_ = "any"
                else:
                    arg_ = convert_type_teal(arg_)
                input_args.append(structs_templates["teal"]["struct_record_item"].format(inp_name, arg_))
            else:
                output_vars.append(convert_type_teal(arg[1]))
            if arg[0]:
                output_vars.append(convert_type_teal(arg[1]))

        func_funcname_ = name = func["funcname"]
        if func_funcname_ in ["end", "repeat", "in"]:
            func_funcname_ += "_"
        declaration_function = (structs_templates["teal"]["function_declaration_return"]
                                .format(name=func_funcname_, input_args=", ".join(input_args),
                                        returns=", ".join(output_vars)))
        return declaration_function


_SKIP_STRUCTS = {
    "ImColor",             # handled manually in customImColor.cpp
    "ImStbTexteditState",  # internal, STB_TexteditState undefined for sol2
    "ImGuiInputTextState", # internal, contains ImStbTexteditState*
    "ImRect",              # internal, uses ImVec2_c which conflicts with C++ ImVec2
    "ImGuiStyleMod",       # internal, uses ImVec2_c
    "ImVec2ih",            # internal, uses ImVec2_c
}

def cpp_skip_struct(struct_name):
    return struct_name in _SKIP_STRUCTS

def generate():
    Path(f"./output/cpp/structs").mkdir(parents=True, exist_ok=True)

    with open('structs_and_enums.json', 'r') as f:
        struct_data = json.load(f)['structs']
    with open('definitions.json', 'r') as f:
        definitions = json.load(f)

    im_vectors_types = set()

    for struct_name in struct_data:

        if cpp_skip_struct(struct_name):
            continue

        fields_list = []
        for field in struct_data[struct_name]:
            if field["name"] == "":
                union_fields = parse_union_declaration(field["type"])
                for union_field in union_fields:
                    field_name = union_field["name"] if union_field["size"] is None \
                        else union_field["name"].split("[")[0]
                    if union_field["size"] is None:
                        fields_list.append(
                            structs_templates["cpp"]["field_declaration"].format(field_name=field_name,
                                                                                 struct_name=struct_name))
                    else:
                        fields_list.append(
                            structs_templates["cpp"]["field_declarations_array"].format(field_name=field_name,
                                                                                        struct_name=struct_name,
                                                                                        type=union_field["type"]))
                    if union_field["type"].startswith("ImVector"):
                        im_vectors_types.add(union_field["type"].replace("*", ""))
                continue
            field_name = field["name"] if "size" not in field else field["name"].split("[")[0]
            if "size" not in field:
                if "bitfield" not in field:
                    fields_list.append(
                        structs_templates["cpp"]["field_declaration"].format(field_name=field_name,
                                                                             struct_name=struct_name))
                else:
                    fields_list.append(
                        structs_templates["cpp"]["field_declarations_bitfield"].format(field_name=field_name,
                                                                                       struct_name=struct_name,
                                                                                       type=field["type"]))
            else:
                fields_list.append(
                    structs_templates["cpp"]["field_declarations_array"].format(field_name=field_name,
                                                                                struct_name=struct_name,
                                                                                type=field["type"]))
            if field["type"].startswith("ImVector"):
                im_vectors_types.add(field["type"].replace("*", ""))

        name_deleter = ""
        for k in definitions:
            if not k.split("_")[0] == struct_name:
                continue
            if len(definitions[k]) == 1:
                if "destructor" in definitions[k][0]:
                    name_deleter = definitions[k][0]["ov_cimguiname"]

        factories_list = "sol::no_constructor"
        functions_declaration = []
        for k in definitions:
            if not k.split("_")[0] == struct_name:
                continue
            if len(definitions[k]) == 1:
                func = definitions[k][0]

                if ("templated" in func) and func["templated"]:
                    return None

                if "destructor" in func:
                    continue

                funcname_ = func["funcname"] if "destructor" not in func else "__gc"
                function = function_generator(func, struct_name, name_deleter)
                if function is None:
                    continue
                if "constructor" in func:
                    factories_list = f"sol::factories({function})"
                    continue
                declaration_function = structs_templates["cpp"]["function_set"] \
                    .format(struct_name=struct_name, function_name=funcname_,
                            function=function if "destructor" not in func else f"sol::destructor({function})")
                functions_declaration.append(declaration_function)
            else:
                list_overloading = []
                for func in definitions[k]:
                    if ("templated" in func) and func["templated"]:
                        return None
                    result = function_generator(func, struct_name, name_deleter)
                    if result is not None:
                        list_overloading.append(result)
                if not list_overloading:
                    continue
                overloading_functions = structs_templates["cpp"]["function_declaration_overloading"].format(
                    overloading_functions=',\n\t\t'.join(list_overloading))
                if "constructor" in definitions[k][0]:
                    overloading_joined = ',\n\t\t'.join(list_overloading)
                    factories_list = f"sol::factories({overloading_joined})"
                    continue
                declaration_function = structs_templates["cpp"]["function_set"] \
                    .format(struct_name=struct_name, function_name=definitions[k][0]["funcname"],
                            function=overloading_functions if len(list_overloading) > 1 else list_overloading[0])
                functions_declaration.append(declaration_function)

        struct_file__format = (structs_templates["cpp"]["struct_file"]
                               .format(struct_name=struct_name,
                                       field_declarations='\n\t'.join(
                                           fields_list),
                                       function_declarations='\n\t'.join(
                                           functions_declaration),
                                       constructor=factories_list))

        with open(f"./output/cpp/structs/{struct_name}.cpp", 'w') as fw:
            fw.write(prettier.format_code(struct_file__format))

    functions_declaration = []
    for k in definitions:

        if skip_function(k):
            continue

        if "namespace" in definitions[k][0]:
            if definitions[k][0]["namespace"] != "ImGui":
                continue
            if len(definitions[k]) == 1:
                func = definitions[k][0]

                if ("templated" in func) and func["templated"]:
                    return None

                function = function_generator(func, func["namespace"], "")
                if function is None:
                    continue
                declaration_function = structs_templates["cpp"]["function_set"] \
                    .format(struct_name=func["namespace"], function_name=func["funcname"],
                            function=function)
                functions_declaration.append(declaration_function)
            else:
                list_overloading = []
                for func in definitions[k]:
                    if ("templated" in func) and func["templated"]:
                        return None
                    result = function_generator(func, func["namespace"], "")
                    if result is not None:
                        list_overloading.append(result)
                if not list_overloading:
                    continue
                overloading_functions = structs_templates["cpp"]["function_declaration_overloading"].format(
                    overloading_functions=',\n\t\t'.join(list_overloading))
                declaration_function = structs_templates["cpp"]["function_set"] \
                    .format(struct_name=definitions[k][0]["namespace"], function_name=definitions[k][0]["funcname"],
                            function=overloading_functions if len(list_overloading) > 1 else list_overloading[0])
                functions_declaration.append(declaration_function)

    with open(f"./output/cpp/structs/ImGui.cpp", 'w') as fw:
        file_declaration__format = structs_templates["cpp"]["imgui_file_declaration"].format(
            function_declarations='\n\t'.join(functions_declaration))
        fw.write(prettier.format_code(file_declaration__format))

    _SKIP_IM_VECTOR_TYPES = {"ImVector_stbrp_node_im"}

    declarations_im_vector = []
    for im_vector_type in im_vectors_types:
        if im_vector_type in _SKIP_IM_VECTOR_TYPES:
            continue
        im_vector_template_type = extract_type(im_vector_type)
        declaration_im_vector = structs_templates["cpp"]["im_vector_declaration"].format(im_vector=im_vector_type,
                                                                                         type=im_vector_template_type)
        declarations_im_vector.append(declaration_im_vector)

    with open(f"./output/cpp/structs/ImVectors.cpp", 'w') as fw:
        im_vectors_file__format = structs_templates["cpp"]["im_vectors_file"].format(
            '\n\t'.join(declarations_im_vector))
        fw.write(prettier.format_code(im_vectors_file__format))

    list_extern = []
    list_bind = []
    for struct_name in struct_data:
        if cpp_skip_struct(struct_name):
            continue
        list_extern.append(structs_templates["cpp"]["extern_bind"].format(struct_name=struct_name))
        list_bind.append(structs_templates["cpp"]["struct_bind"].format(struct_name=struct_name))

    with open(f"./output/cpp/structs.cpp", 'w') as fw:
        binds__format = structs_templates["cpp"]["structs_file_binds"].format('\n'.join(list_extern),
                                                                              '\n\t'.join(list_bind))
        fw.write(prettier.format_code(binds__format))


def generate_lua():
    with open('definitions.json', 'r') as f:
        definitions = json.load(f)

    im_vectors_types = set()

    lua_declarations = []
    for struct_name in struct_data:
        fields_list = []
        for field in struct_data[struct_name]:
            if field["name"] == "":
                union_fields = parse_union_declaration(field["type"])
                for union_field in union_fields:
                    field_name = union_field["name"] if union_field["size"] is None \
                        else union_field["name"].split("[")[0]
                    fields_list.append(
                        structs_templates["lua"]["struct_field_item"].format(name=field_name,
                                                                             type=convert_type_lua(
                                                                                 union_field["type"])))
                    if union_field["type"].startswith("ImVector"):
                        im_vectors_types.add(union_field["type"].replace("*", ""))
                continue
            field_name = field["name"] if "size" not in field else field["name"].split("[")[0]
            # if field_name == "end":
            #     field_name = "[\"end\"]"
            fields_list.append(
                structs_templates["lua"]["struct_field_item"].format(name=field_name,
                                                                     type=convert_type_lua(field["type"])))
            if field["type"].startswith("ImVector"):
                im_vectors_types.add(field["type"].replace("*", ""))

        functions_declaration = []
        for k in definitions:
            if not k.split("_")[0] == struct_name:
                continue
            if len(definitions[k]) == 1:
                func = definitions[k][0]

                if ("templated" in func) and func["templated"]:
                    return None

                if "destructor" in func:
                    continue

                function = function_generator_lua(func)
                functions_declaration.append(function)
            else:
                for func in definitions[k]:
                    if ("templated" in func) and func["templated"]:
                        return None
                    functions_declaration.append(function_generator_lua(func))

        struct_file__format = structs_templates["lua"]["struct_record"].format(struct_name=struct_name,
                                                                               fields='\n'.join(fields_list),
                                                                               functions=',\n\t'.join(
                                                                                   functions_declaration))

        lua_declarations.append(struct_file__format)

    declarations_im_vector = []
    for im_vector_type in im_vectors_types:
        im_vector_template_type = extract_type(im_vector_type)
        declaration_im_vector = structs_templates["lua"]["im_vector_record"].format(name=im_vector_type,
                                                                                    type=convert_type_lua(
                                                                                        im_vector_template_type))
        declarations_im_vector.append(declaration_im_vector)

    lua_declarations.append('\n'.join(declarations_im_vector))

    functions_declaration = []
    for k in definitions:

        if skip_function(k):
            continue

        if "namespace" in definitions[k][0]:
            if len(definitions[k]) == 1:
                func = definitions[k][0]

                if ("templated" in func) and func["templated"]:
                    return None

                if "destructor" in func:
                    continue

                function = function_generator_lua(func)
                functions_declaration.append(function)
            else:
                for func in definitions[k]:
                    if ("templated" in func) and func["templated"]:
                        return None
                    functions_declaration.append(function_generator_lua(func))

    file_declaration__format = structs_templates["lua"]["imgui_record"].format(
        functions=',\n\t'.join(functions_declaration))
    lua_declarations.append(file_declaration__format)

    lua_declarations.append(structs_templates["lua"]["custom_declaration"])

    return lua_declarations


def generate_teal():
    with open('definitions.json', 'r') as f:
        definitions = json.load(f)

    im_vectors_types = set()

    lua_declarations = []
    for struct_name in struct_data:
        fields_list = []
        for field in struct_data[struct_name]:
            if field["name"] == "":
                union_fields = parse_union_declaration(field["type"])
                for union_field in union_fields:
                    field_name = union_field["name"] if union_field["size"] is None \
                        else union_field["name"].split("[")[0]
                    fields_list.append(
                        structs_templates["teal"]["struct_record_item"].format(field_name,
                                                                               convert_type_teal(union_field["type"])))
                    if union_field["type"].startswith("ImVector"):
                        im_vectors_types.add(union_field["type"].replace("*", ""))
                continue
            field_name = field["name"] if "size" not in field else field["name"].split("[")[0]
            if field_name == "end":
                field_name = "[\"end\"]"
            fields_list.append(
                structs_templates["teal"]["struct_record_item"].format(field_name, convert_type_teal(field["type"])))
            if field["type"].startswith("ImVector"):
                im_vectors_types.add(field["type"].replace("*", ""))

        functions_declaration = []
        for k in definitions:
            if not k.split("_")[0] == struct_name:
                continue
            if len(definitions[k]) == 1:
                func = definitions[k][0]

                if ("templated" in func) and func["templated"]:
                    return None

                function = function_generator_teal(func)
                functions_declaration.append(function)
            else:
                for func in definitions[k]:
                    if ("templated" in func) and func["templated"]:
                        return None
                    functions_declaration.append(function_generator_teal(func))

        struct_file__format = structs_templates["teal"]["struct_record"].format(struct_name, '\n\t'.join(fields_list),
                                                                                '\n\t'.join(functions_declaration))

        lua_declarations.append(struct_file__format)

    declarations_im_vector = []
    for im_vector_type in im_vectors_types:
        im_vector_template_type = extract_type(im_vector_type)
        declaration_im_vector = structs_templates["teal"]["im_vector_record"].format(im_vector_type,
                                                                                     convert_type_teal(
                                                                                         im_vector_template_type))
        declarations_im_vector.append(declaration_im_vector)

    lua_declarations.append('\n'.join(declarations_im_vector))

    functions_declaration = []
    for k in definitions:

        if skip_function(k):
            continue

        if "namespace" in definitions[k][0]:
            if len(definitions[k]) == 1:
                func = definitions[k][0]

                if ("templated" in func) and func["templated"]:
                    return None

                function = function_generator_teal(func)
                functions_declaration.append(function)
            else:
                for func in definitions[k]:
                    if ("templated" in func) and func["templated"]:
                        return None
                    functions_declaration.append(function_generator_teal(func))

    file_declaration__format = structs_templates["teal"]["struct_record"].format("ImGui", "",
                                                                                 '\n\t'.join(functions_declaration))
    lua_declarations.append(file_declaration__format)

    return lua_declarations
