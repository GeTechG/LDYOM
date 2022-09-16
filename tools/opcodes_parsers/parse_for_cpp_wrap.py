import os
import shutil
import utils
import stringcase
import parse_json_opcodes

types = {
    "EntityStatus": "int",
    "RadioChannel": "int",
    "MoveState": "int",
    "CarDrivingStyle": "int",
    "any": "int",
    "ExplosionType": "int",
    "CoronaType": "int",
    "Searchlight": "int",
    "MissionAudioSlot": "int",
    "CarLock": "int",
    "CleoBlip": "int",
    "CarMission": "int",
    "CounterDisplay": "int",
    "Fade": "int",
    "Player": "int",
    "FlareType": "int",
    "HudObject": "int",
    "Trailer": "int",
    "PedType": "int",
    "Font": "int",
    "DecisionMakerGroup": "int",
    "File": "int",
    "CarGenerator": "int",
    "CarAlarm": "int",
    "WeaponType": "int",
    "DynamicLibrary": "int",
    "DecisionMaker": "int",
    "AnimGroup": "char*",
    "DefaultTaskAllocator": "int",
    "RelationshipType": "int",
    "ModSlot": "int",
    "EaseWay": "int",
    "EntryexitsFlag": "int",
    "model_char": "int",
    "Attractor": "int",
    "TempAction": "int",
    "bool": "bool",
    "AudioStream": "int",
    "ImGuiDir": "int",
    "zone_key": "int",
    "arguments": "int",
    "Blip": "int",
    "KeyCode": "int",
    "ChangeMoney": "int",
    "SwitchType": "int",
    "Town": "int",
    "Sphere": "int",
    "GarageType": "int",
    "AudioStreamState": "int",
    "Plane": "int",
    "GarageName": "char*",
    "ImGuiCond": "int",
    "Boat": "int",
    "BombType": "int",
    "DecisionMakerChar": "int",
    "WeaponSlot": "int",
    "PickupType": "int",
    "float": "float",
    "WeaponFire": "int",
    "Checkpoint": "int",
    "Group": "int",
    "DrawEvent": "int",
    "ScriptSound": "int",
    "ImGuiCol": "int",
    "CameraMode": "int",
    "model_object": "int",
    "User3DMarker": "int",
    "FindFile": "int",
    "SetPieceType": "int",
    "BlipColor": "int",
    "BlipDisplay": "int",
    "WeatherType": "int",
    "string": "char*",
    "PlayerMood": "int",
    "label": "int",
    "Car": "int",
    "Pickup": "int",
    "GangType": "int",
    "EaseMode": "int",
    "PadId": "int",
    "Object": "int",
    "string128": "char*",
    "Interior": "int",
    "Menu": "int",
    "FightStyle": "int",
    "model_any": "int",
    "ScriptBrainAttachType": "int",
    "Heli": "int",
    "ShadowTextureType": "int",
    "script_id": "int",
    "Particle": "int",
    "gxt_key": "char*",
    "Button": "int",
    "RadarSprite": "int",
    "TextStyle": "int",
    "Sequence": "int",
    "FileMode": "char*",
    "model_vehicle": "int",
    "int": "int",
    "Train": "int",
    "ImGuiStyleVar": "int",
    "TimerDirection": "int",
    "Char": "int",
    "SeekOrigin": "int",
    "BodyPart": "int",
    "ScriptFire": "int",
    "CarDoor": "int",
    "ControllerMode": "int",
    "AudioStream3D": "int",
    "EntityType": "int",
    "MenuGrid": "int",
    "VehicleSubclass": "int"
}

file_template = """//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void {}OpcodesLuaWrapper(sol::state& state) {{
    {}
}}"""

class_template = """auto table = state.create_table(\"{}Op\");
    {}
"""

command_template = "table.set_function(\"{}\", []({}) {{{} auto result = Command<0x{}>({}); return std::make_tuple({});}});"

try:
    shutil.rmtree("Opcodes");
except OSError as e:
    print("Error: %s" % (e.strerror));

os.mkdir("Opcodes");

classes = parse_json_opcodes.parseOpcodes();

classes_list: list[str] = [];
for clazz, commands in classes.items():
    commands_list: list[str] = [];
    lastExtension: str = "";
    for command in commands:
        args: list[str] = [];
        temp_vars: list[str] = [];
        send_args: list[str] = [];
        returns: list[str] = [];
        
        if "input" in command:
            for inp in command["input"]:
                name = "_" + stringcase.camelcase(inp["type"] if inp["name"] == '' else inp["name"])
                type = types[inp["type"]];
                inp_code = "";
                if type == "char*":
                    inp_code = "const {} {}".format(type, name)
                else:
                    inp_code = "const {}& {}".format(type, name)
                args.append(inp_code);
                send_args.append(name)
        
        if "output" in command:
            for outp in command["output"]:
                name = stringcase.camelcase(outp["type"] if outp["name"] == '' else outp["name"]) + "_"
                type = types[outp["type"]]
                if type == 'char*':
                    type = "const " + type;
                temp_vars.append('{} {};'.format(type, name))
                send_arg = '&{}'.format(name);
                if type != "int" and type != "float":
                    send_arg = '(int*){}'.format(send_arg);
                send_args.append(send_arg);
                returns.append(name);
        
        returns.append("result");

        if command["extension"] != lastExtension:
            commands_list.append((("" if lastExtension == '' else "\n\t") + "//{}").format(command["extension"]))
            lastExtension = command["extension"];
        command_code = command_template.format(stringcase.camelcase(command["member"]), ', '.join(args), ''.join(temp_vars), command["id"], ', '.join(send_args), ', '.join(returns))
        commands_list.append(command_code);
    class_code = class_template.format(clazz, '\n\t'.join(commands_list))
    out = file_template.format(clazz, class_code)
    with open("Opcodes/{}OpcodesLuaWrapper.cpp".format(clazz), "w") as cpp_file:
        cpp_file.write(out);
    classes_list.append("extern void {}OpcodesLuaWrapper(sol::state& state);".format(clazz))

out = "//This code is generating, it cannot be changed.\n"
out += "\n".join(classes_list);
out += "\n\n";

call_list = [];
for class_name in classes.keys():
    call_list.append("{}OpcodesLuaWrapper(state);".format(class_name));

out += """void OpcodesLuaWrapper::wrap(sol::state& state) {{
    {}
}}""".format('\n\t'.join(call_list));

with open("cpp_opcodes_wrapper.cpp", "w") as cpp_file:
    cpp_file.write(out);

# for type in types_set:
#     if not type in types:
#         types[type] = "int"

# with open("types.json", "w") as write_file:
#     json.dump(types, write_file); 