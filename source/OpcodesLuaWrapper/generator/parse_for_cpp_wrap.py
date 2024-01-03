from pathlib import Path

import prettier
import stringcase
import parse_json_opcodes

enums = {"PedType": "int", "WeaponType": "int", "Town": "int", "Fade": "int", "BodyPart": "int",
         "AudioStreamState": "int", "FileMode": "char*", "SeekOrigin": "int", "Button": "int",
         "AnimGroup": "char*", "TimerDirection": "int", "Font": "int", "TextStyle": "int",
         "CounterDisplay": "int", "CarMission": "int", "FlareType": "int", "CameraMode": "int",
         "HudObject": "int", "ExplosionType": "int", "RelationshipType": "int", "SwitchType": "int",
         "BlipDisplay": "int", "ShadowTextureType": "int", "BlipColor": "int",
         "ScriptBrainAttachType": "int", "TempAction": "int", "CarDoor": "int", "MissionAudioSlot": "int",
         "WeatherType": "int", "ScriptSound": "int", "FightStyle": "int", "MoveState": "int",
         "ModSlot": "int", "CarLock": "int", "PickupType": "int", "EntityStatus": "int",
         "RadarSprite": "int", "GarageName": "char*", "GangType": "int", "WeaponSlot": "int",
         "GarageType": "int", "BombType": "int", "RadioChannel": "int", "CoronaType": "int",
         "PlayerMood": "int", "EntryexitsFlag": "int", "DefaultTaskAllocator": "int", "KeyCode": "int",
         "CarAlarm": "int", "VehicleSubclass": "int", "EntityType": "int", "ImGuiCond": "int",
         "WeaponFire": "int", "DrawEvent": "int", "EaseMode": "int", "EaseWay": "int",
         "ChangeMoney": "int", "ImGuiDir": "int", "PadId": "int", "ImGuiCol": "int",
         "ImGuiStyleVar": "int", "TaskStatus": "int", "DrivingMode": "int", "HudColors": "int",
         "Cheats": "int", "Align": "int", "ModelInfoType": "int", "ListType": "int",
         "WeaponSkill": "int", "WeaponState": "int", "AnimGrp": "int", "PedState": "int",
         "PedStat": "int", "StringFind": "int", "CarAnimGroup": "int", "TaskId": "int",
         "VehicleDummy": "int", "CarNodeDoor": "int", "CarNode": "int", "RwCombine": "int",
         "PedBone": "int", "SurfaceType": "int", "Platform": "int", "VehicleClass": "int",
         "CarLights": "int", "SeatIndex": "int"}

types = {
    "Checkpoint": "int",
    "ControllerMode": "int",
    "Pickup": "int",
    "DecisionMakerChar": "int",
    "Fx": "int",
    "File": "int",
    "DecisionMakerGroup": "int",
    "gxt_key": "int",
    "DynamicLibrary": "int",
    "model_object": "int",
    "Particle": "int",
    "Searchlight": "int",
    "Blip": "int",
    "model_any": "int",
    "Train": "int",
    "SetPieceType": "int",
    "ColPoint": "int",
    "bool": "int",
    "model_vehicle": "int",
    "CleoBlip": "int",
    "Char": "int",
    "zone_key": "int",
    "DecisionMaker": "int",
    "label": "int",
    "int": "int",
    "AudioStream3D": "int",
    "Interior": "int",
    "Player": "int",
    "string": "char*",
    "Sphere": "int",
    "FindFile": "int",
    "CarGenerator": "int",
    "arguments": "int",
    "Group": "int",
    "Menu": "int",
    "RenderObject": "int",
    "AudioStream": "int",
    "User3DMarker": "int",
    "Object": "int",
    "MenuGrid": "int",
    "float": "float",
    "Plane": "int",
    "script_id": "int",
    "Boat": "int",
    "Car": "int",
    "ScriptFire": "int",
    "Attractor": "int",
    "List": "int",
    "Trailer": "int",
    "model_char": "int",
    "Sequence": "int",
    "Heli": "int"
}

file_template = """//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void bind{}(sol::state& state) {{
    {}
}}"""

class_template = """auto table = state.create_table(\"{}Op\");
    {}
"""

command_template_no_return = "table.set_function(\"{}\", []({}) {{{} Command<0x{}>({});}});"
command_template_no_result = "table.set_function(\"{}\", []({}) {{{} Command<0x{}>({}); return std::make_tuple({});}});"
command_template = "table.set_function(\"{}\", []({}) {{{} auto result = Command<0x{}>({}); return std::make_tuple({});}});"


def generate():
    classes = parse_json_opcodes.parseOpcodes()

    classes_list: list[str] = []
    for clazz, commands in classes.items():
        commands_list: list[str] = []
        lastExtension: str = ""
        for command in commands:
            args: list[str] = []
            temp_vars: list[str] = []
            send_args: list[str] = []
            returns: list[str] = []

            if "attrs" in command and "is_condition" in command["attrs"]:
                returns.append("result")

            if "input" in command:
                for inp in command["input"]:
                    name = "_" + stringcase.camelcase(inp["type"] if inp["name"] == '' else inp["name"])

                    type = "int"
                    if inp["type"] in types:
                        type = types[inp["type"]]
                    if inp["type"] in enums:
                        type = enums[inp["type"]]

                    inp_code = ""
                    if type == "char*":
                        inp_code = "const {} {}".format(type, name)
                    else:
                        inp_code = "const {}& {}".format(type, name)
                    args.append(inp_code)
                    send_args.append(name)

            if "output" in command:
                for outp in command["output"]:
                    name = stringcase.camelcase(outp["type"] if outp["name"] == '' else outp["name"]) + "_"

                    type = "int"
                    if outp["type"] in types:
                        type = types[outp["type"]]
                    if outp["type"] in enums:
                        type = enums[outp["type"]]

                    if type == 'char*':
                        type = "const " + type
                    temp_vars.append('{} {};'.format(type, name))
                    send_arg = '&{}'.format(name)
                    if type != "int" and type != "float":
                        send_arg = '(int*){}'.format(send_arg)
                    send_args.append(send_arg)
                    returns.append(name)

            if command["extension"] != lastExtension:
                commands_list.append((("" if lastExtension == '' else "\n\t") + "//{}").format(command["extension"]))
                lastExtension = command["extension"]
            if "attrs" in command and "is_condition" in command["attrs"]:
                command_code = command_template.format(stringcase.camelcase(command["member"]), ', '.join(args),
                                                       ''.join(temp_vars), command["id"], ', '.join(send_args),
                                                       ', '.join(returns))
            elif len(returns) > 0:
                command_code = command_template_no_result.format(stringcase.camelcase(command["member"]),
                                                                 ', '.join(args),
                                                                 ''.join(temp_vars), command["id"],
                                                                 ', '.join(send_args),
                                                                 ', '.join(returns))
            else:
                command_code = command_template_no_return.format(stringcase.camelcase(command["member"]),
                                                                 ', '.join(args),
                                                                 ''.join(temp_vars), command["id"],
                                                                 ', '.join(send_args))
            commands_list.append(command_code)
        class_code = class_template.format(clazz, '\n\t'.join(commands_list))
        out = file_template.format(clazz, class_code)
        Path(f"./output/cpp/opcodes").mkdir(parents=True, exist_ok=True)
        with open("./output/cpp/opcodes/{}.cpp".format(clazz), "w") as cpp_file:
            cpp_file.write(prettier.format_code(out))
        classes_list.append("extern void bind{}(sol::state& state);".format(clazz))

    out = "//This code is generating, it cannot be changed.\n"
    out += "#include <sol.hpp>\n"
    out += "\n".join(classes_list)
    out += "\n\n"

    call_list = []
    for class_name in classes.keys():
        call_list.append("bind{}(state);".format(class_name))

    out += """void opcodesLuaWrapper(sol::state &state) {{
        {}
    }}""".format('\n\t'.join(call_list))

    Path(f"./output/cpp").mkdir(parents=True, exist_ok=True)
    with open("./output/cpp/OpcodesLuaWrapper.cpp", "w") as cpp_file:
        cpp_file.write(prettier.format_code(out))
