import json
from pathlib import Path

import rtoml

import parse_json_opcodes
import stringcase

with open('nodes_templates.toml', 'r') as f:
    templates = rtoml.load(f)

with open('russian_lang_template.toml', 'r', encoding="utf-8") as f:
    lang_toml = rtoml.load(f)
    lang_toml["nodes"]["core"] = dict()

types = {
    "Checkpoint": "core.Checkpoint",
    "ControllerMode": "core.number",
    "Pickup": "core.Pickup",
    "DecisionMakerChar": "core.DecisionMakerChar",
    "Fx": "core.number",
    "File": "core.number",
    "DecisionMakerGroup": "core.number",
    "gxt_key": "core.string",
    "DynamicLibrary": "core.number",
    "model_object": "core.number",
    "Particle": "core.Particle",
    "Searchlight": "core.Searchlight",
    "Blip": "core.number",
    "model_any": "core.number",
    "Train": "core.Train",
    "SetPieceType": "core.number",
    "ColPoint": "core.number",
    "bool": "core.bool",
    "model_vehicle": "core.number",
    "CleoBlip": "core.number",
    "Char": "core.Char",
    "zone_key": "core.string",
    "DecisionMaker": "core.number",
    "label": "core.string",
    "int": "core.number",
    "AudioStream3D": "core.AudioStream3D",
    "Interior": "core.number",
    "Player": "core.number",
    "string": "core.string",
    "Sphere": "core.number",
    "FindFile": "core.number",
    "CarGenerator": "core.number",
    "arguments": "core.number",
    "Group": "core.number",
    "Menu": "core.number",
    "RenderObject": "core.number",
    "AudioStream": "core.AudioStream",
    "User3DMarker": "core.number",
    "Object": "core.Object",
    "MenuGrid": "core.number",
    "float": "core.number",
    "Plane": "core.Vehicle",
    "script_id": "core.number",
    "Boat": "core.Vehicle",
    "Car": "core.Vehicle",
    "ScriptFire": "core.ScriptFire",
    "Attractor": "core.number",
    "List": "core.number",
    "Trailer": "core.Vehicle",
    "model_char": "core.number",
    "Sequence": "core.number",
    "Heli": "core.Vehicle",
    "any": "core.number",
}

types_lua = {
    "Checkpoint": "integer",
    "ControllerMode": "integer",
    "Pickup": "integer",
    "DecisionMakerChar": "integer",
    "Fx": "integer",
    "File": "integer",
    "DecisionMakerGroup": "integer",
    "gxt_key": "string",
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
    "zone_key": "string",
    "DecisionMaker": "integer",
    "label": "string",
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
    "any": "integer",
}

def_val = {
    "integer": "0",
    "number": "0",
    "string": "\"\"",
    "boolean": "false",
}

enums = ["PedType", "WeaponType", "Town", "Fade", "BodyPart", "AudioStreamState", "FileMode", "SeekOrigin", "Button",
         "AnimGroup", "TimerDirection", "Font", "TextStyle", "CounterDisplay", "CarMission", "FlareType", "CameraMode",
         "HudObject", "ExplosionType", "RelationshipType", "SwitchType", "BlipDisplay", "ShadowTextureType",
         "BlipColor", "ScriptBrainAttachType", "TempAction", "CarDoor", "MissionAudioSlot", "WeatherType",
         "ScriptSound", "FightStyle", "MoveState", "ModSlot", "CarLock", "PickupType", "EntityStatus", "RadarSprite",
         "GarageName", "GangType", "WeaponSlot", "GarageType", "BombType", "RadioChannel", "CoronaType", "PlayerMood",
         "EntryexitsFlag", "DefaultTaskAllocator", "KeyCode", "CarAlarm", "VehicleSubclass", "EntityType", "ImGuiCond",
         "WeaponFire", "DrawEvent", "EaseMode", "EaseWay", "ChangeMoney", "ImGuiDir", "PadId", "ImGuiCol",
         "ImGuiStyleVar", "TaskStatus", "DrivingMode", "HudColors", "Cheats", "Align", "ModelInfoType", "ListType",
         "WeaponSkill", "WeaponState", "AnimGrp", "PedState", "PedStat", "StringFind", "CarAnimGroup", "TaskId",
         "VehicleDummy", "CarNodeDoor", "CarNode", "RwCombine", "PedBone", "SurfaceType", "Platform", "VehicleClass",
         "CarLights", "SeatIndex"]


# new_types = {};
# for name, type in types.items():
#     if name != type:
#         new_types[name] = type

# with open("types_annotation.json", "w") as ann_types:
#     json.dump(new_types, ann_types);

def generate():
    classes = parse_json_opcodes.parseOpcodes(True)

    for clazz, commands in classes.items():
        if clazz not in ["Audio", "Boat", "Camera", "Car", "Char", "Checkpoint", "Clock", "Fx", "Game", "Heli", "Hud", "Menu", "MenuGrid", "Object", "Pad", "Particle", "Pickup", "Plane", "Player", "Rc", "Task", "Text", "Trailer", "Train", "Weapon", "Weather", "World", "Zone", "Math"]:
            continue

        commands_list: list[str] = []
        for command in commands:
            fields = []
            fields_init = []
            draws = []
            run = []
            run_ret = []

            inputs = []

            if not ("attrs" in command and "is_condition" in command["attrs"]):
                inputs.append(templates["input"].format(type="core.flow"))
                draws.append("LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], \"\");")

            inps = []
            if "input" in command:
                for inp in command["input"]:
                    name = inp["name"] if inp["name"] != "" else ("inp" + str(len(inputs) - 1))
                    if name == "self":
                        name = "self_"
                    if inp["type"] in enums:
                        inputs.append(templates["input"].format(type="core.number"))
                        fields.append("---@field {} {}".format(name, "integer"))
                        fields_init.append("{} = {}".format(name, def_val["integer"]))
                        draws.append(templates["input_enum_draw"].format(index=len(inputs), field=name, name=inp["name"], enum=inp["type"]))
                    else:
                        inputs.append(templates["input"].format(type=types[inp["type"]]))
                        type_lua_c = types_lua[inp["type"]]
                        fields.append("---@field {} {}".format(name, type_lua_c))
                        fields_init.append("{} = {}".format(name, def_val[type_lua_c]))
                        draws.append(templates["input_draw"].format(index=len(inputs), field=name, name=inp["name"]))

                    run.append(templates["input_run"].format(index=len(inputs), field=name))
                    inps.append(name)

            outputs = []
            if not ("attrs" in command and "is_condition" in command["attrs"]):
                outputs.append(templates["output"].format(type="core.flow"))
                draws.append(templates["output_draw"].format(index=len(outputs), name=""))

            outs = []

            if "attrs" in command and "is_condition" in command["attrs"]:
                outputs.append(templates["output"].format(type="core.bool"))
                draws.append(templates["output_draw"].format(index=len(outputs), name=""))
                outs.append("result")

            if "output" in command:
                for outp in command["output"]:
                    if outp["type"] in enums:
                        outputs.append(templates["output"].format(type="core.number"))
                    else:
                        outputs.append(templates["output"].format(type=types[outp["type"]]))

                    name = outp["name"] if outp["name"] != "" else ("out" + str(len(outputs) - 1))
                    if name == "self":
                        name = "self_"
                    draws.append(templates["output_draw"].format(index=len(outputs), name=outp["name"]))
                    outs.append(name)

            if len(outs) > 0:
                o = templates["run_call_ret"].format(outs=", ".join(outs), clazz=command["class"], method=stringcase.camelcase(command["member"]), inps=", ".join(inps))
                o = o.replace(".goto(", "[\"goto\"](")
                o = o.replace(".break(", "[\"break\"](")
                run.append(o)
            else:
                o = templates["run_call"].format(clazz=command["class"], method=stringcase.camelcase(command["member"]), inps=", ".join(inps))
                o = o.replace(".goto(", "[\"goto\"](")
                o = o.replace(".break(", "[\"break\"](")
                run.append(o)

            if not ("attrs" in command and "is_condition" in command["attrs"]):
                run_ret.append("1")

            run_ret += outs

            node_name = command["class"] + command["member"]
            result_com = templates["file"].format(
                fields="\n".join(fields),
                node_name_u=stringcase.constcase(node_name),
                node_name_l=stringcase.snakecase(node_name),
                node_name=node_name,
                node_name_var=stringcase.camelcase(node_name),
                category="opcode." + stringcase.snakecase(command["class"]),
                inputs="\t\t\t\t,".join(inputs),
                outputs="\t\t\t\t,".join(outputs),
                fieldsInit= (",\n\t\t\t" + ",\n\t\t\t".join(fields_init)) if len(fields_init) > 0 else "",
                draw="\n\t\t".join(draws),
                run="\n\t\t".join(run),
                run_ret=", ".join(run_ret),
                is_callable="false" if ("attrs" in command and "is_condition" in command["attrs"]) else "true",
                short_desc="\n\tdescription = true," if "short_desc" in command and command["short_desc"] != "" else "",
            )
            lang_toml["nodes"]["node_editor"]["categories"][stringcase.snakecase(command["class"])] = command["class"]
            lang_toml["nodes"]["core"][stringcase.snakecase(node_name)] = dict()
            lang_toml["nodes"]["core"][stringcase.snakecase(node_name)]["title"] = command["member"]
            if "short_desc" in command and command["short_desc"] != "":
                lang_toml["nodes"]["core"][stringcase.snakecase(node_name)]["short_desc"] = command["short_desc"]

            class_path = "./output/nodes/opcode/" + stringcase.snakecase(command["class"]) + "/"
            Path(class_path).mkdir(parents=True, exist_ok=True)
            with open(class_path + "core-" + stringcase.snakecase(node_name) + ".lua", "w") as out:
                out.write(result_com)

enums = ["PedType", "WeaponType", "Town", "Fade", "BodyPart", "AudioStreamState", "FileMode", "SeekOrigin", "Button",
         "AnimGroup", "TimerDirection", "Font", "TextStyle", "CounterDisplay", "CarMission", "FlareType", "CameraMode",
         "HudObject", "ExplosionType", "RelationshipType", "SwitchType", "BlipDisplay", "ShadowTextureType",
         "BlipColor", "ScriptBrainAttachType", "TempAction", "CarDoor", "MissionAudioSlot", "WeatherType",
         "ScriptSound", "FightStyle", "MoveState", "ModSlot", "CarLock", "PickupType", "EntityStatus", "RadarSprite",
         "GarageName", "GangType", "WeaponSlot", "GarageType", "BombType", "RadioChannel", "CoronaType", "PlayerMood",
         "EntryexitsFlag", "DefaultTaskAllocator", "KeyCode", "CarAlarm", "VehicleSubclass", "EntityType", "ImGuiCond",
         "WeaponFire", "DrawEvent", "EaseMode", "EaseWay", "ChangeMoney", "ImGuiDir", "PadId", "ImGuiCol",
         "ImGuiStyleVar", "TaskStatus", "DrivingMode", "HudColors", "Cheats", "Align", "ModelInfoType", "ListType",
         "WeaponSkill", "WeaponState", "AnimGrp", "PedState", "PedStat", "StringFind", "CarAnimGroup", "TaskId",
         "VehicleDummy", "CarNodeDoor", "CarNode", "RwCombine", "PedBone", "SurfaceType", "Platform", "VehicleClass",
         "CarLights", "SeatIndex"]
# for enum in enums:
#     tps.remove(enum)
# print(json.dumps(list(tps)))

generate()

with open("./output/nodes/lang.toml", "w", encoding="utf-8") as lang:
    rtoml.dump(lang_toml, lang)
