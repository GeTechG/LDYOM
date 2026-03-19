import json
import os
import stringcase
from parse_json_opcodes import parseOpcodes

# ---------------------------------------------------------------------------
# Templates — new pin structure:
#   - no default_data, no on_draw
#   - each data-in pin has inline on_render using getInputDefault/setInputDefault
#   - on_execute uses handle:getInput(absolute_pin_idx)
# ---------------------------------------------------------------------------

TEMPLATES = {
    "file": (
        'local TYPE = "opcode.{category_snake}.{node_name_l}"\n'
        "\n"
        "local desc = {{\n"
        "    type = TYPE,\n"
        '    category = "opcode.{category_snake}",\n'
        '    style = "function",\n'
        "{is_pure_line}"
        "    pins = {{{pins}\n"
        "    }},\n"
        "    on_execute = function(_node)\n"
        "{run}        return 0\n"
        "    end\n"
        "}}\n"
        "\n"
        "node_editor.register(desc)\n"
    ),

    # Simple flow/bool pins — no format variables, no {{ }} escaping needed
    "pin_flow_in":  '\n        { title = "", type = "flow", dir = "in" },',
    "pin_flow_out": '\n        { title = "", type = "flow", dir = "out" },',
    "pin_bool_out": '\n        { title = "", type = "bool", dir = "out" },',

    # Output data pin
    "pin_data_out": (
        '\n        {{ title = "{title}", type = "{pin_type}", dir = "out" }},'
    ),

    # Input data pins with inline on_render per type
    "pin_data_in_float": (
        '\n        {{\n'
        '            title = "{title}",\n'
        '            type = "number",\n'
        '            dir = "in",\n'
        '            default = {default},\n'
        '            on_render = function(handle)\n'
        '                if not handle then return end\n'
        '                if not handle:isInputConnected({pin_idx}) then\n'
        '                    local v = handle:getInputDefault({pin_idx})\n'
        '                    ImGui.SetNextItemWidth(80)\n'
        '                    local ok, nv = ImGui.InputFloat("##oe_{field}", v, 0, 0, "%.3f", 0)\n'
        '                    if ok then handle:setInputDefault({pin_idx}, nv) end\n'
        '                end\n'
        '            end\n'
        '        }},'
    ),

    "pin_data_in_int": (
        '\n        {{\n'
        '            title = "{title}",\n'
        '            type = "number",\n'
        '            dir = "in",\n'
        '            default = {default},\n'
        '            on_render = function(handle)\n'
        '                if not handle then return end\n'
        '                if not handle:isInputConnected({pin_idx}) then\n'
        '                    local v = handle:getInputDefault({pin_idx})\n'
        '                    ImGui.SetNextItemWidth(80)\n'
        '                    local ok, nv = ImGui.InputInt("##oe_{field}", v, 0)\n'
        '                    if ok then handle:setInputDefault({pin_idx}, nv) end\n'
        '                end\n'
        '            end\n'
        '        }},'
    ),

    "pin_data_in_string": (
        '\n        {{\n'
        '            title = "{title}",\n'
        '            type = "string",\n'
        '            dir = "in",\n'
        '            default = {default},\n'
        '            on_render = function(handle)\n'
        '                if not handle then return end\n'
        '                if not handle:isInputConnected({pin_idx}) then\n'
        '                    local v = handle:getInputDefault({pin_idx})\n'
        '                    ImGui.SetNextItemWidth(80)\n'
        '                    local ok, nv = ImGui.InputText("##oe_{field}", v, 0, nil, nil)\n'
        '                    if ok then handle:setInputDefault({pin_idx}, nv) end\n'
        '                end\n'
        '            end\n'
        '        }},'
    ),

    "pin_data_in_bool": (
        '\n        {{\n'
        '            title = "{title}",\n'
        '            type = "bool",\n'
        '            dir = "in",\n'
        '            default = {default},\n'
        '            on_render = function(handle)\n'
        '                if not handle then return end\n'
        '                if not handle:isInputConnected({pin_idx}) then\n'
        '                    local v = handle:getInputDefault({pin_idx})\n'
        '                    local ok, nv = ImGui.Checkbox("##oe_{field}", v)\n'
        '                    if ok then handle:setInputDefault({pin_idx}, nv) end\n'
        '                end\n'
        '            end\n'
        '        }},'
    ),

    # on_execute read lines (_node is the node handle, avoids shadowing by opcode param names)
    "run_get_input": (
        "        local {field} = _node:getInput({pin_idx}) or {default}"
    ),
    "run_get_int": (
        "        local {field} = math.floor(_node:getInput({pin_idx}) or {default})"
    ),

    # opcode call lines
    "run_call":     "        {clazz}Op.{method}({args})",
    "run_call_ret": "        local {outs} = {clazz}Op.{method}({args})",
}

T = TEMPLATES  # shorthand

# ---------------------------------------------------------------------------
# Type helpers
# ---------------------------------------------------------------------------

STRING_TYPES = {"string", "gxt_key", "label", "zone_key"}
FLOAT_TYPES  = {"float"}
BOOL_TYPES   = {"bool"}


def get_pin_type(param_type: str) -> str:
    if param_type in BOOL_TYPES:
        return "bool"
    if param_type in STRING_TYPES:
        return "string"
    return "number"


def needs_floor(param_type: str) -> bool:
    """True for int, handles, enums — anything that needs math.floor in Lua."""
    return (
        param_type not in FLOAT_TYPES
        and param_type not in BOOL_TYPES
        and param_type not in STRING_TYPES
    )


def get_lua_default(param_type: str) -> str:
    if param_type in BOOL_TYPES:
        return "false"
    if param_type in STRING_TYPES:
        return '""'
    return "0"


def to_snake(name: str) -> str:
    return stringcase.snakecase(name).lower()


def param_field(param: dict) -> str:
    """Derive a Lua-safe field name from a parameter dict."""
    raw = param["name"] if param["name"] else param["type"]
    return to_snake(raw)


# ---------------------------------------------------------------------------
# Generator
# ---------------------------------------------------------------------------


def class_to_display(name: str) -> str:
    import re
    return re.sub(r'(?<=[a-z])(?=[A-Z])|(?<=[A-Z])(?=[A-Z][a-z])', ' ', name)


def generate():
    classes = parseOpcodes(only_required=True)

    out_nodes_dir = "output/nodes/opcode"
    os.makedirs(out_nodes_dir, exist_ok=True)

    lang_patch: dict[str, dict[str, str]] = {}
    categories_lang: dict[str, str] = {}
    descriptions_patch: dict[str, dict[str, str]] = {}
    generated = 0

    for class_name, commands in classes.items():
        category_snake = to_snake(class_name)
        categories_lang[category_snake] = class_to_display(class_name)
        class_dir = os.path.join(out_nodes_dir, category_snake)
        os.makedirs(class_dir, exist_ok=True)

        for cmd in commands:
            is_condition = cmd.get("attrs", {}).get("is_condition", False)
            method = stringcase.camelcase(cmd.get("member", ""))
            if not method:
                continue

            node_name_l = to_snake(method)
            inputs  = cmd.get("input",  [])
            outputs = cmd.get("output", [])

            pins_str  = ""
            run_str   = ""
            args_list = []

            # Track absolute 1-based pin index (matches handle:getInput / getInputDefault)
            pin_idx = 1

            # -- Flow in (non-condition only) ---------------------------------
            if not is_condition:
                pins_str += T["pin_flow_in"]
                pin_idx += 1          # data pins start at 2

            # -- Data input pins ----------------------------------------------
            for param in inputs:
                field   = param_field(param)
                ptype   = param["type"]
                default = get_lua_default(ptype)
                title   = param["name"]

                if ptype in BOOL_TYPES:
                    pins_str += T["pin_data_in_bool"].format(
                        title=title, default=default, field=field, pin_idx=pin_idx
                    )
                elif ptype in STRING_TYPES:
                    pins_str += T["pin_data_in_string"].format(
                        title=title, default=default, field=field, pin_idx=pin_idx
                    )
                elif needs_floor(ptype):
                    pins_str += T["pin_data_in_int"].format(
                        title=title, default=default, field=field, pin_idx=pin_idx
                    )
                else:
                    pins_str += T["pin_data_in_float"].format(
                        title=title, default=default, field=field, pin_idx=pin_idx
                    )

                if needs_floor(ptype):
                    run_str += T["run_get_int"].format(
                        field=field, pin_idx=pin_idx, default=default
                    ) + "\n"
                else:
                    run_str += T["run_get_input"].format(
                        field=field, pin_idx=pin_idx, default=default
                    ) + "\n"

                args_list.append(field)
                pin_idx += 1

            # -- Flow out / bool out ------------------------------------------
            if not is_condition:
                pins_str += T["pin_flow_out"]
                pin_idx += 1
            else:
                bool_out_pin_idx = pin_idx
                pins_str += T["pin_bool_out"]
                pin_idx += 1

            # -- Output data pins (non-condition only) ------------------------
            out_vars = []  # list of (field, pin_idx)
            if not is_condition:
                for param in outputs:
                    field    = param_field(param)
                    ptype    = param["type"]
                    pin_type = get_pin_type(ptype)
                    title    = param["name"]

                    pins_str += T["pin_data_out"].format(
                        title=title, pin_type=pin_type
                    )
                    out_vars.append((field, pin_idx))
                    pin_idx += 1

            # -- Build opcode call -------------------------------------------
            args_str = ", ".join(args_list)

            if is_condition:
                run_str += T["run_call_ret"].format(
                    outs="result",
                    clazz=class_name,
                    method=method,
                    args=args_str,
                ) + "\n"
                run_str += f'        _node:setOutput({bool_out_pin_idx}, result)\n'
            elif out_vars:
                outs_str = ", ".join(f for f, _ in out_vars)
                run_str += T["run_call_ret"].format(
                    outs=outs_str,
                    clazz=class_name,
                    method=method,
                    args=args_str,
                ) + "\n"
                for field, out_pin_idx in out_vars:
                    run_str += f'        _node:setOutput({out_pin_idx}, {field})\n'
            else:
                run_str += T["run_call"].format(
                    clazz=class_name,
                    method=method,
                    args=args_str,
                ) + "\n"

            # -- Render file content -----------------------------------------
            is_pure_line = "    is_pure = true,\n" if is_condition else ""
            content = T["file"].format(
                category_snake=category_snake,
                node_name_l=node_name_l,
                pins=pins_str,
                run=run_str,
                is_pure_line=is_pure_line,
            )

            filepath = os.path.join(
                class_dir, f"opcode.{category_snake}.{node_name_l}.lua"
            )
            with open(filepath, "w", encoding="utf-8") as f:
                f.write(content)

            # Accumulate lang patch
            if category_snake not in lang_patch:
                lang_patch[category_snake] = {}
            lang_patch[category_snake][node_name_l] = cmd.get("member", method)

            short_desc = cmd.get("short_desc", "")
            if short_desc:
                if category_snake not in descriptions_patch:
                    descriptions_patch[category_snake] = {}
                descriptions_patch[category_snake][node_name_l] = short_desc

            generated += 1

    # Write lang patch JSON
    lang_output = {
        "nodes_titles": {"opcode": lang_patch},
        "nodes_categories": {"opcode": categories_lang},
        "nodes_descriptions": {"opcode": descriptions_patch},
    }
    os.makedirs("output/nodes", exist_ok=True)
    with open("output/nodes/lang_en_patch.json", "w", encoding="utf-8") as f:
        json.dump(lang_output, f, indent=2, ensure_ascii=False)

    print(f"Generated {generated} node files across {len(classes)} classes")
    print("Lang patch: output/nodes/lang_en_patch.json")
