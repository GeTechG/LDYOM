import json
from pathlib import Path

import rtoml

import prettier

with open('enums_templates.toml', 'r') as f:
    templates = rtoml.load(f)


def generate():
    Path(f"./output/cpp/enums").mkdir(parents=True, exist_ok=True)

    enums_extern = []

    with open('structs_and_enums.json', 'r') as f:
        loaded_data = json.load(f)
        enums_data = loaded_data['enums']

        for k in enums_data:
            is_runtime = len(enums_data[k]) > 20

            enumsList = []
            for enum in enums_data[k]:
                if is_runtime:
                    deli = enum["name"].find("_") + 1
                    if not enum["name"].startswith(k):
                        deli = 0
                    enumsList.append(templates["cpp"]["enum_item_runtime"].format(enum["name"][deli:], enum["name"]))
                else:
                    deli = enum["name"].find("_") + 1
                    if not enum["name"].startswith(k):
                        deli = 0
                    enumsList.append(templates["cpp"]["enum_item"].format(enum["name"][deli:], enum["name"]))

            enum_name = k.replace("_", "")

            if is_runtime:
                enum_declaration = templates["cpp"]["enum_declaration_runtime"].format(k, enum_name,
                                                                                       ",\n\t\t".join(enumsList))
            else:
                enum_declaration = templates["cpp"]["enum_declaration"].format(enum_name,
                                                                               ",\n\t\t".join(enumsList))

            enum_function = templates["cpp"]["enum_file"].format("bind" + enum_name, enum_declaration)

            with open(f"./output/cpp/enums/{enum_name}.cpp", 'w', encoding='utf-8') as fw:
                fw.write(prettier.format_code(enum_function))

            enums_extern.append(f"extern void bind{enum_name}(sol::state &state);")

    with open(f"./output/cpp/enums.cpp", 'w') as f:
        enums_binds = []
        for enumName in enums_data:
            enums_binds.append(templates["cpp"]["enum_bind"].format(enumName.replace('_', '')))

        out = templates["cpp"]["enums_file_bind"].format("\n".join(enums_extern), "\n\t".join(enums_binds))
        f.write(prettier.format_code(out))


def generate_lua():
    lua_declarations = []

    with open('structs_and_enums.json', 'r') as f:
        enums_data = json.load(f)['enums']

        for k in enums_data:
            enumsValuesLua = []
            for enum in enums_data[k]:
                deli = enum["name"].find("_") + 1
                if not enum["name"].startswith(k):
                    deli = 0
                item_enum_name = enum["name"][deli:]
                if item_enum_name[0].isdigit():
                    enumsValuesLua.append(
                        templates["lua"]["enum_record_item_digit"].format(item_enum_name, enum["calc_value"]))
                else:
                    enumsValuesLua.append(
                        templates["lua"]["enum_record_item"].format(item_enum_name, enum["calc_value"]))

            enum_name = k.replace("_", "")

            lua_declarations.append(
                templates["lua"]["enum_record"].format(enum_name, enum_name, ",\n\t".join(enumsValuesLua)))

    return lua_declarations


def generate_teal():
    teal_declarations = []

    with open('structs_and_enums.json', 'r') as f:
        enums_data = json.load(f)['enums']

        for k in enums_data:
            enums_values_teal = []
            for enum in enums_data[k]:
                deli = enum["name"].find("_") + 1
                if not enum["name"].startswith(k):
                    deli = 0
                item_enum_name = enum["name"][deli:]
                if item_enum_name[0].isdigit():
                    enums_values_teal.append(templates["teal"]["enum_record_item_digit"].format(item_enum_name))
                else:
                    enums_values_teal.append(templates["teal"]["enum_record_item"].format(item_enum_name))

            enum_name = k.replace("_", "")

            teal_declarations.append(templates["teal"]["enum_record"].format(enum_name, "\n\t".join(enums_values_teal)))

    return teal_declarations
