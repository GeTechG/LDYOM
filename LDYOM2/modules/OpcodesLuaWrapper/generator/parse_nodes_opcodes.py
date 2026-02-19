import json
import os
import stringcase
from parse_json_opcodes import parseOpcodes

# ---------------------------------------------------------------------------
# Templates (mirrors nodes_templates.toml — canonical reference)
# ---------------------------------------------------------------------------

TEMPLATES = {
    "file": (
        'local TYPE = "opcode.{category_snake}.{node_name_l}"\n'
        "\n"
        "local desc = {{\n"
        "    type = TYPE,\n"
        '    category = "opcode.{category_snake}",\n'
        '    style = "function",\n'
        "    pins = {{{pins}\n"
        "    }},\n"
        "    default_data = {{{default_data}\n"
        "    }},\n"
        "    on_draw = function(handle)\n"
        "        if not handle then return end{draw}\n"
        "    end,\n"
        "    on_execute = function(handle)\n"
        "{run}        return 0\n"
        "    end\n"
        "}}\n"
        "\n"
        "node_editor.register(desc)\n"
    ),

    "pin_flow_in":  '\n        {{ title = "", type = "flow", dir = "in" }},',
    "pin_flow_out": '\n        {{ title = "", type = "flow", dir = "out" }},',
    "pin_bool_out": '\n        {{ title = "", type = "bool", dir = "out" }},',

    "pin_data_in": (
        '\n        {{ title = "{title}", type = "{pin_type}", dir = "in",'
        " default = {default} }},"
    ),

    "pin_data_out": (
        '\n        {{ title = "{title}", type = "{pin_type}", dir = "out" }},'
    ),

    "default_data_entry": "\n        {field} = {default},",

    "draw_float": (
        "\n        do"
        '\n            local v = handle:getData("{field}") or {default}'
        "\n            ImGui.SetNextItemWidth(80)"
        '\n            local ok, nv = ImGui.InputFloat("##oe_{field}", v, 0, 0, "%.3f", 0)'
        '\n            if ok then handle:setData("{field}", nv) end'
        "\n        end"
    ),

    "draw_int": (
        "\n        do"
        '\n            local v = handle:getData("{field}") or {default}'
        "\n            ImGui.SetNextItemWidth(80)"
        '\n            local ok, nv = ImGui.InputInt("##oe_{field}", v, 0)'
        '\n            if ok then handle:setData("{field}", nv) end'
        "\n        end"
    ),

    "draw_bool": (
        "\n        do"
        '\n            local v = handle:getData("{field}") or false'
        '\n            local ok, nv = ImGui.Checkbox("##oe_{field}", v)'
        '\n            if ok then handle:setData("{field}", nv) end'
        "\n        end"
    ),

    "draw_string": (
        "\n        do"
        '\n            local v = handle:getData("{field}") or ""'
        "\n            ImGui.SetNextItemWidth(80)"
        '\n            local ok, nv = ImGui.InputText("##oe_{field}", v, 0, nil, nil)'
        '\n            if ok then handle:setData("{field}", nv) end'
        "\n        end"
    ),

    "run_get_input": (
        '        local {field} = handle:getInput({idx})'
        ' or handle:getData("{field}") or {default}'
    ),

    "run_get_int": (
        "        local {field} = math.floor("
        'handle:getInput({idx}) or handle:getData("{field}") or {default})'
    ),

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
    return "float"


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


# ---------------------------------------------------------------------------
# Generator
# ---------------------------------------------------------------------------


def generate():
    classes = parseOpcodes(only_required=True)

    out_nodes_dir = "output/nodes/opcode"
    os.makedirs(out_nodes_dir, exist_ok=True)

    lang_patch: dict[str, dict[str, str]] = {}
    generated = 0

    for class_name, commands in classes.items():
        category_snake = to_snake(class_name)
        class_dir = os.path.join(out_nodes_dir, category_snake)
        os.makedirs(class_dir, exist_ok=True)

        for cmd in commands:
            is_condition = cmd.get("attrs", {}).get("is_condition", False)
            method = cmd.get("member", "")
            if not method:
                continue

            node_name_l = to_snake(method)
            inputs  = cmd.get("input",  [])
            outputs = cmd.get("output", [])

            pins_str         = ""
            draw_str         = ""
            run_str          = ""
            default_data_str = ""
            args_list        = []

            # Flow in (non-condition only)
            if not is_condition:
                pins_str += T["pin_flow_in"].format()

            # Data input pins
            data_idx = 1
            for param in inputs:
                field    = to_snake(param["name"])
                ptype    = param["type"]
                pin_type = get_pin_type(ptype)
                default  = get_lua_default(ptype)
                title    = param["name"]

                pins_str += T["pin_data_in"].format(
                    title=title, pin_type=pin_type, default=default
                )
                default_data_str += T["default_data_entry"].format(
                    field=field, default=default
                )

                # Draw widget
                if pin_type == "bool":
                    draw_str += T["draw_bool"].format(field=field, default=default)
                elif pin_type == "string":
                    draw_str += T["draw_string"].format(field=field, default=default)
                elif needs_floor(ptype):
                    draw_str += T["draw_int"].format(field=field, default=default)
                else:
                    draw_str += T["draw_float"].format(field=field, default=default)

                # Run: read input value
                if needs_floor(ptype):
                    run_str += T["run_get_int"].format(
                        field=field, idx=data_idx, default=default
                    ) + "\n"
                else:
                    run_str += T["run_get_input"].format(
                        field=field, idx=data_idx, default=default
                    ) + "\n"

                args_list.append(field)
                data_idx += 1

            # Flow out or bool out
            if not is_condition:
                pins_str += T["pin_flow_out"].format()
            else:
                pins_str += T["pin_bool_out"].format()

            # Output data pins (non-condition only)
            out_vars = []
            if not is_condition:
                for param in outputs:
                    field    = to_snake(param["name"])
                    ptype    = param["type"]
                    pin_type = get_pin_type(ptype)
                    default  = get_lua_default(ptype)
                    title    = param["name"]

                    pins_str += T["pin_data_out"].format(
                        title=title, pin_type=pin_type
                    )
                    default_data_str += T["default_data_entry"].format(
                        field=field, default=default
                    )
                    out_vars.append((field, default))

            # Build opcode call
            args_str = ", ".join(args_list)

            if is_condition:
                run_str += T["run_call_ret"].format(
                    outs="result",
                    clazz=class_name,
                    method=method,
                    args=args_str,
                ) + "\n"
                run_str += '        handle:setData("_result", result)\n'
            elif out_vars:
                outs_str = ", ".join(f for f, _ in out_vars)
                run_str += T["run_call_ret"].format(
                    outs=outs_str,
                    clazz=class_name,
                    method=method,
                    args=args_str,
                ) + "\n"
                for field, _ in out_vars:
                    run_str += f'        handle:setData("{field}", {field})\n'
            else:
                run_str += T["run_call"].format(
                    clazz=class_name,
                    method=method,
                    args=args_str,
                ) + "\n"

            # Render file content
            content = T["file"].format(
                category_snake=category_snake,
                node_name_l=node_name_l,
                pins=pins_str,
                default_data=default_data_str,
                draw=draw_str,
                run=run_str,
            )

            filepath = os.path.join(
                class_dir, f"opcode.{category_snake}.{node_name_l}.lua"
            )
            with open(filepath, "w", encoding="utf-8") as f:
                f.write(content)

            # Accumulate lang patch
            if category_snake not in lang_patch:
                lang_patch[category_snake] = {}
            lang_patch[category_snake][node_name_l] = f"{class_name}.{method}"

            generated += 1

    # Write lang patch JSON
    lang_output = {"nodes_titles": {"opcode": lang_patch}}
    os.makedirs("output/nodes", exist_ok=True)
    with open("output/nodes/lang_en_patch.json", "w", encoding="utf-8") as f:
        json.dump(lang_output, f, indent=2, ensure_ascii=False)

    print(f"Generated {generated} node files across {len(classes)} classes")
    print("Lang patch: output/nodes/lang_en_patch.json")
