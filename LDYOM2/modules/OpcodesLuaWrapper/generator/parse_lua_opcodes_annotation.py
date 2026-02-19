import json
from pathlib import Path

import parse_json_opcodes
import stringcase

types = {
    "Checkpoint": "integer",
    "ControllerMode": "integer",
    "Pickup": "integer",
    "DecisionMakerChar": "integer",
    "Fx": "integer",
    "File": "integer",
    "DecisionMakerGroup": "integer",
    "gxt_key": "integer",
    "DynamicLibrary": "integer",
    "model_object": "integer",
    "Particle": "integer",
    "Searchlight": "integer",
    "Blip": "integer",
    "model_any": "integer",
    "Train": "integer",
    "SetPieceType": "integer",
    "ColPoint": "integer",
    "bool": "boolean",
    "model_vehicle": "integer",
    "CleoBlip": "integer",
    "Char": "integer",
    "zone_key": "integer",
    "DecisionMaker": "integer",
    "label": "integer",
    "int": "integer",
    "AudioStream3D": "integer",
    "Interior": "integer",
    "Player": "integer",
    "string": "string",
    "Sphere": "integer",
    "FindFile": "integer",
    "CarGenerator": "integer",
    "arguments": "integer",
    "Group": "integer",
    "Menu": "integer",
    "RenderObject": "integer",
    "AudioStream": "integer",
    "User3DMarker": "integer",
    "Object": "integer",
    "MenuGrid": "integer",
    "float": "number",
    "Plane": "integer",
    "script_id": "integer",
    "Boat": "integer",
    "Car": "integer",
    "ScriptFire": "integer",
    "Attractor": "integer",
    "List": "integer",
    "Trailer": "integer",
    "model_char": "integer",
    "Sequence": "integer",
    "Heli": "integer",
}


# new_types = {};
# for name, type in types.items():
#     if name != type:
#         new_types[name] = type

# with open("types_annotation.json", "w") as ann_types:
#     json.dump(new_types, ann_types);

def generate():
    class_template = """{}Op = {{
        {}
    }}"""

    command_template = """---{extension}
        ---| {desc} {params_returns}
        {name} = function({send_params}) end,"""

    alieses: list[str] = []

    for name, type in types.items():
        alieses.append("---@alias {} {}".format(name, type))

    out = "---@meta\n"
    out += '\n'.join(alieses)

    classes = parse_json_opcodes.parseOpcodes()

    tps = set()

    for clazz, commands in classes.items():
        commands_list: list[str] = []
        for command in commands:
            params: list[str] = []
            send_params: list[str] = []
            returns: list[str] = []

            if "attrs" in command and "is_condition" in command["attrs"]:
                returns.append("---@return bool")

            if "input" in command:
                for inp in command["input"]:
                    name = stringcase.camelcase(inp["type"] if inp["name"] == '' else inp["name"])
                    type = inp["type"]
                    tps.add(type)
                    params.append("---@param {} {}".format(name, type))
                    if name in ["end"]:
                        name += "_"
                    send_params.append(name)

            if "output" in command:
                for outp in command["output"]:
                    name = stringcase.camelcase(outp["type"] if outp["name"] == '' else outp["name"])
                    type = outp["type"]
                    tps.add(type)
                    returns.append("---@return {1} {0}".format(name, type))

            params_code = "" if len(params) == 0 else '\n\t' + '\n\t'.join(params)
            returns_code = "" if len(returns) == 0 else '\n\t' + '\n\t'.join(returns)

            name_fmt = stringcase.camelcase(command["member"])
            if name_fmt[0].isdigit() or name_fmt in ["if", "return", "break", "end"]:
                name_fmt = f"[\"{name_fmt}\"]"
            commands_list.append(command_template.format(
                extension=command["extension"],
                desc=command.get("short_desc", ""),
                params_returns=params_code + returns_code,
                name=name_fmt,
                send_params=', '.join(send_params)
            ))
        class_code = class_template.format(clazz, "\n\t".join(commands_list))
        out += "\n" + class_code

    Path("./output/lua").mkdir(parents=True, exist_ok=True)
    with open("./output/lua/opcodes.lua", "w") as lua_ann:
        lua_ann.write(out)

# enums = ["PedType", "WeaponType", "Town", "Fade", "BodyPart", "AudioStreamState", "FileMode", "SeekOrigin", "Button",
#          "AnimGroup", "TimerDirection", "Font", "TextStyle", "CounterDisplay", "CarMission", "FlareType", "CameraMode",
#          "HudObject", "ExplosionType", "RelationshipType", "SwitchType", "BlipDisplay", "ShadowTextureType",
#          "BlipColor", "ScriptBrainAttachType", "TempAction", "CarDoor", "MissionAudioSlot", "WeatherType",
#          "ScriptSound", "FightStyle", "MoveState", "ModSlot", "CarLock", "PickupType", "EntityStatus", "RadarSprite",
#          "GarageName", "GangType", "WeaponSlot", "GarageType", "BombType", "RadioChannel", "CoronaType", "PlayerMood",
#          "EntryexitsFlag", "DefaultTaskAllocator", "KeyCode", "CarAlarm", "VehicleSubclass", "EntityType", "ImGuiCond",
#          "WeaponFire", "DrawEvent", "EaseMode", "EaseWay", "ChangeMoney", "ImGuiDir", "PadId", "ImGuiCol",
#          "ImGuiStyleVar", "TaskStatus", "DrivingMode", "HudColors", "Cheats", "Align", "ModelInfoType", "ListType",
#          "WeaponSkill", "WeaponState", "AnimGrp", "PedState", "PedStat", "StringFind", "CarAnimGroup", "TaskId",
#          "VehicleDummy", "CarNodeDoor", "CarNode", "RwCombine", "PedBone", "SurfaceType", "Platform", "VehicleClass",
#          "CarLights", "SeatIndex"]
# for enum in enums:
#     tps.remove(enum)
# print(json.dumps(list(tps)))
