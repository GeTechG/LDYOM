from os import mkdir
from pathlib import Path
import parse_json_opcodes
import stringcase
import toml

enums = ["PedType", "WeaponType", "Town", "Fade", "BodyPart", "AudioStreamState", "FileMode", "SeekOrigin", "Button", "AnimGroup", "TimerDirection", "Font", "TextStyle", "CounterDisplay", "CarDrivingStyle", "CarMission", "FlareType", "CameraMode", "HudObject", "ExplosionType", "RelationshipType", "SwitchType", "BlipDisplay", "ShadowTextureType", "BlipColor", "ScriptBrainAttachType", "TempAction", "CarDoor", "MissionAudioSlot", "WeatherType", "ScriptSound", "FightStyle", "MoveState", "ModSlot", "CarLock", "PickupType", "EntityStatus", "RadarSprite", "GarageName", "GangType", "WeaponSlot", "GarageType", "BombType", "RadioChannel", "CoronaType", "PlayerMood", "EntryexitsFlag", "DefaultTaskAllocator", "KeyCode", "CarAlarm", "VehicleSubclass", "EntityType", "ImGuiCond", "WeaponFire", "DrawEvent", "EaseMode", "EaseWay", "ChangeMoney", "ImGuiDir", "PadId", "ImGuiCol", "ImGuiStyleVar"]

types = {
    "Searchlight": "Number",
    "CleoBlip": "Number",
    "Player": "Number",
    "Trailer": "Number",
    "DecisionMakerGroup": "Number",
    "File": "Number",
    "CarGenerator": "Number",
    "DynamicLibrary": "Number",
    "DecisionMaker": "Number",
    "model_char": "Number",
    "Attractor": "Number",
    "bool": "Boolean",
    "AudioStream": "Number",
    "zone_key": "Number",
    "arguments": "Number",
    "Blip": "Number",
    "Sphere": "Number",
    "Plane": "Number",
    "Boat": "Number",
    "DecisionMakerChar": "Number",
    "float": "Number",
    "Checkpoint": "Number",
    "Group": "Number",
    "model_object": "Number",
    "User3DMarker": "Number",
    "FindFile": "Number",
    "SetPieceType": "Number",
    "label": "Number",
    "Car": "Number",
    "Pickup": "Number",
    "Object": "Number",
    "string128": "String",
    "string": "String",
    "Interior": "Number",
    "Menu": "Number",
    "model_any": "Number",
    "Heli": "Number",
    "script_id": "Number",
    "Particle": "Number",
    "gxt_key": "String",
    "Sequence": "Number",
    "model_vehicle": "Number",
    "int": "Number",
    "Train": "Number",
    "Char": "Number",
    "ScriptFire": "Number",
    "ControllerMode": "Number",
    "AudioStream3D": "Number",
    "MenuGrid": "Number"
}

file_format: str;
with open("file_template.lua", "r") as file:
    file_format = file.read();

allow_extensions = ["default","CLEO","bitwise","file","ini",""]

categories_names = {};
nodes_names = {};
pins_names = {};
node_descriptions = {};

classes = parse_json_opcodes.parseOpcodes();
for clazz, commands in classes.items():
    if clazz == "Keywords":
        continue;
    for command in commands:
        if command["extension"] in allow_extensions and command["id"]:# in ["01AB","02F8","0AAD","0AB9","0ACD"]:
            classname = clazz + command["member"];
            is_condition = False;
            if "attrs" in command and "is_condition" in command["attrs"]:
                is_condition = True;
            type = "Function" if not is_condition else "Condition";
            pins = [];
            draw_inp = [];
            draw_out = [];
            get_params = [];
            method_params = [];
            returns = [];
            copy_str = [];

            if not is_condition:
                pins.append("ed.Pin:new(id + 1, \"\", ed.PinType.In, ed.ValueType.Void)");
                draw_inp.append("""ImNodes.beginInputAttribute(self.pins[1]);
    ImGui.text(self.pins[1].name);
    ImNodes.endInputAttribute();""");

            if "input" in command:
                for inp in command["input"]:
                    inp_type = "Number";
                    if inp["type"] in types:
                        inp_type = types[inp["type"]];
                    id = len(pins)+1;
                    pin_name = "core.pins." + stringcase.snakecase(inp["name"])
                    pins_names[stringcase.snakecase(inp["name"])] = stringcase.titlecase(inp["name"])
                    if inp_type != "String":
                        pins.append("ed.Pin:new(id + {}, \"{}\", ed.PinType.In, ed.ValueType.{})".format(id, pin_name, inp_type))
                    else:
                        pins.append("ed.Pin:new(id + {}, \"{}\", ed.PinType.In, ed.ValueType.{}, \"\", constants.TEXT_SIZE)".format(id, pin_name, inp_type))
                    draw_inp_c: str;
                    if inp["type"] in enums:
                        type_snakecase = stringcase.snakecase(inp["type"]);
                        arr = "enums." + type_snakecase;
                        draw_inp_c = f"""if not self.pins[{id}].connected then
        ImGui.setNextItemWidth(120);
        ImNodes.enum(loc.get(self.pins[{id}].name), self.pins[{id}].value, loc.getArray(\"{arr}\"), enums_id.{type_snakecase});
    else
        ImGui.text(loc.get(self.pins[{id}].name));
    end"""
                    elif inp["type"] != "int" and inp["type"] != "string" and inp["type"] != "float" and inp["type"] != "bool":
                        draw_inp_c = f"ImGui.text(loc.get(self.pins[{id}].name));"
                    elif inp_type == "Number":
                        draw_inp_c = f"""if not self.pins[{id}].connected then
        ImGui.setNextItemWidth(120);
        ImGui.inputFloat(loc.get(self.pins[{id}].name), self.pins[{id}].value);
    else
        ImGui.text(loc.get(self.pins[{id}].name));
    end"""
                    elif inp_type == "String":
                        draw_inp_c = f"""if not self.pins[{id}].connected then
        ImGui.setNextItemWidth(120);
        ImGui.inputText(loc.get(self.pins[{id}].name), self.pins[{id}].value, constants.TEXT_SIZE);
    end"""
                    elif inp_type == "Boolean":
                        draw_inp_c = f"""if not self.pins[{id}].connected then
        ImGui.checkbox(loc.get(self.pins[{id}].name), self.pins[{id}].value);
    else
        ImGui.text(loc.get(self.pins[{id}].name));
    end"""
                    draw_inp.append(f"""\n\tImNodes.beginInputAttribute(self.pins[{id}]);
    {draw_inp_c}
    ImNodes.endInputAttribute();""")
                    if inp_type == "Number" or inp_type == "Boolean":
                        get_params.append("local {} = context:getValue(scene, tasklist, self.id + {}, queryId)[0];".format(inp["name"] + "_", id))
                    elif inp_type == "String":
                        get_params.append("local {} = gxtEncode(ffi.string(context:getValue(scene, tasklist, self.id + {}, queryId)));".format(inp["name"] + "_", id))
                    if inp["type"] in enums:
                        method_params.append("{}".format(inp["name"] + "_"));
                    method_params.append(inp["name"] + "_");

            call_next = "";
            if not is_condition:
                id = len(pins)+1
                pins.append(f"ed.Pin:new(id + {id}, \"\", ed.PinType.Out, ed.ValueType.Void)");
                call_next = f"context:callNodes(scene, tasklist, self.id + {id});"
            else:
                pins.append(f"ed.Pin:new(id + {len(pins)+1}, \"general.result\", ed.PinType.Out, ed.ValueType.Boolean)")
                returns.append(f"self.pins[{len(pins)}].value[0]")

            draw_out.append(f"""ImNodes.beginOutputAttribute(self.pins[{len(pins)}]);
    ImGui.text(loc.get(self.pins[{len(pins)}].name));
    ImNodes.endOutputAttribute();""");

            if "output" in command:
                for out in command["output"]:
                    out_type = "Number";
                    if out["type"] in types:
                        out_type = types[out["type"]];
                    id = len(pins)+1;
                    type_snakecase = stringcase.snakecase(out["type"]);
                    pin_name = "core.pins." + stringcase.snakecase(out["name"])
                    pins_names[stringcase.snakecase(out["name"])] = stringcase.titlecase(out["name"])
                    if out_type != "String":
                        pins.append("ed.Pin:new(id + {}, \"{}\", ed.PinType.Out, ed.ValueType.{})".format(id, pin_name, out_type))
                    else:
                        pins.append("ed.Pin:new(id + {}, \"{}\", ed.PinType.Out, ed.ValueType.{}, \"\", constants.TEXT_SIZE)".format(id, pin_name, out_type))
                    
                    tooltip = "";
                    if out["type"] in enums:
                            tooltip = f"""\n\tif ImGui.isItemHovered() then
            ImGui.beginTooltip();
            local names = loc.getArray("enums.{type_snakecase}");
            for i = 1, #names do
                ImGui.text(names[i] .. " : " .. enums_id.{type_snakecase}[i])
            end
            ImGui.endTooltip();
        end"""
                    
                    draw_out.append(f"""\n\tImNodes.beginOutputAttribute(self.pins[{id}]);
    ImGui.dummy(math.max(ImNodes.getNodeDimensions(self.id).x - (ImGui.calcTextSize(loc.get(self.pins[{id}].name)).x + inp_size.x) - ImGui.getFontSize() * 1.75, 0), 0)
    ImGui.sameLine();
    ImGui.text(loc.get(self.pins[{id}].name));{tooltip}
    ImNodes.endOutputAttribute();""")
                    if out_type == "Number" or out_type == "Boolean":
                        returns.append(f"self.pins[{len(pins)}].value[0]")
                    elif out_type == "String":
                        str_name = out["name"] + "_"
                        returns.append(str_name);
                        copy_str.append(f"ffi.copy(self.pins[{len(pins)}].value, {str_name})")


            out_draw = "";
            if len(draw_inp) > 0:
                out_draw += "ImGui.beginGroup();\n\t{}\n\tImGui.endGroup();".format("\n\t".join(draw_inp))
                if len(draw_out) > 0:
                    out_draw += "\n\n\tImGui.sameLine();\n\n\t"
                    out_draw += "local inp_size = ImGui.getItemRectSize();\n\n\t"
            if len(draw_out) > 0:
                out_draw += "ImGui.beginGroup();\n\t{}\n\tImGui.endGroup();".format("\n\t".join(draw_out))

            if not is_condition:
                returns.append("_")

            description = "nil";
            if "short_desc" in command and command["short_desc"] != "": 
                description = f"\"core.descriptions.{stringcase.snakecase(classname)}\"";
                node_descriptions[stringcase.snakecase(classname)] = command["short_desc"];

            out = file_format.format(classname = classname,
                                     classname_snake = stringcase.snakecase(classname),
                                     type = type,
                                     activation = ("false" if is_condition else "true"),
                                     pins=',\n\t\t'.join(pins),
                                     draw=out_draw,
                                     get_params='\n\t'.join(get_params),
                                     returns=",".join(returns),
                                     copy_str='\n'.join(copy_str),
                                     call_next=call_next,
                                     clazz=clazz,
                                     method=stringcase.camelcase(command["member"]),
                                     method_params=','.join(method_params),
                                     clazz_snake=stringcase.snakecase(clazz),
                                     description=description)
            if not Path(f"nodes/{clazz}").exists():
                mkdir(f"nodes/{clazz}");
            with open("nodes/{}/{}.lua".format(clazz, command["member"]), "w") as file:
                file.write(out);
            
            nodes_names[stringcase.snakecase(classname)] = stringcase.titlecase(classname)
            categories_names[stringcase.snakecase(clazz)] = stringcase.titlecase(clazz);

with open("nodes_names.toml", "w") as file:
    toml.dump(nodes_names, file);
with open("nodes_pins_names.toml", "w") as file:
    toml.dump(pins_names, file);
with open("nodes_categories_names.toml", "w") as file:
    toml.dump(categories_names, file);
with open("nodes_descriptions.toml", "w") as file:
    toml.dump(node_descriptions, file);