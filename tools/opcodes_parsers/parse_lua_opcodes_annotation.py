from distutils import extension
import json
from multiprocessing.reduction import send_handle
import parse_json_opcodes
import stringcase

types = {
    "Searchlight": "integer",
    "CleoBlip": "integer",
    "Player": "integer",
    "Trailer": "integer",
    "DecisionMakerGroup": "integer",
    "File": "integer",
    "CarGenerator": "integer",
    "DynamicLibrary": "integer",
    "DecisionMaker": "integer",
    "model_char": "integer",
    "Attractor": "integer",
    "bool": "boolean",
    "AudioStream": "integer",
    "zone_key": "integer",
    "arguments": "integer",
    "Blip": "integer",
    "Sphere": "integer",
    "Plane": "integer",
    "Boat": "integer",
    "DecisionMakerChar": "integer",
    "float": "number",
    "Checkpoint": "integer",
    "Group": "integer",
    "model_object": "integer",
    "User3DMarker": "integer",
    "FindFile": "integer",
    "SetPieceType": "integer",
    "label": "integer",
    "Car": "integer",
    "Pickup": "integer",
    "Object": "integer",
    "string128": "string",
    "Interior": "integer",
    "Menu": "integer",
    "model_any": "integer",
    "Heli": "integer",
    "script_id": "integer",
    "Particle": "integer",
    "gxt_key": "string",
    "Sequence": "integer",
    "model_vehicle": "integer",
    "int": "integer",
    "Train": "integer",
    "Char": "integer",
    "ScriptFire": "integer",
    "ControllerMode": "integer",
    "AudioStream3D": "integer",
    "MenuGrid": "integer"
}

# new_types = {};
# for name, type in types.items():
#     if name != type:
#         new_types[name] = type

# with open("types_annotation.json", "w") as ann_types:
#     json.dump(new_types, ann_types);

class_template = """{}Op = {{
    {}
}}"""

command_template = """---{extension}
    ---| {desc} {params_returns}
    [\"{name}\"] = function({send_params}) end,"""

alieses: list[str] = [];

for name, type in types.items():
    alieses.append("---@alias {} {}".format(name, type))

out = "---@meta\n"
out += '\n'.join(alieses);

classes = parse_json_opcodes.parseOpcodes();

for clazz, commands in classes.items():
    commands_list: list[str] = [];
    for command in commands:
        params: list[str] = [];
        send_params: list[str] = [];
        returns: list[str] = [];
        if "input" in command:
            for inp in command["input"]:
                name = stringcase.camelcase(inp["type"] if inp["name"] == '' else inp["name"])
                type = inp["type"];
                params.append("---@param {} {}".format(name, type));
                send_params.append(name);
        
        if "output" in command:
            for outp in command["output"]:
                name = stringcase.camelcase(outp["type"] if outp["name"] == '' else outp["name"])
                type = outp["type"];
                returns.append("---@return {1} {0}".format(name, type));

        params_code = "" if len(params) == 0 else '\n\t' + '\n\t'.join(params);
        returns_code = "" if len(returns) == 0 else '\n\t' + '\n\t'.join(returns);

        commands_list.append(command_template.format(
            extension=command["extension"], 
            desc=command.get("short_desc", ""),
            params_returns=params_code + returns_code,
            name=stringcase.camelcase(command["member"]),
            send_params=', '.join(send_params)
        ));
    class_code = class_template.format(clazz, "\n\t".join(commands_list));
    out += "\n" + class_code;

with open("lua_opcodes_annotation.lua", "w") as lua_ann:
    lua_ann.write(out);