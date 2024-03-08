local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTaskCarMissionNode : LDNodeEditorNode
---@field char integer
---@field vehicle integer
---@field targetVehicle integer
---@field missionId integer
---@field cruiseSpeed number
---@field drivingStyle integer

local TASK_CAR_MISSION_NODE_TYPE = "core.task_car_mission"

---@type LDNodeEditorNodeType
local taskCarMissionNode = {
    typeName = TASK_CAR_MISSION_NODE_TYPE,
    category = "opcode.task",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTaskCarMissionNode
        local newNode = {
            id = newNodeId,
            nodeType = TASK_CAR_MISSION_NODE_TYPE,
            inputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.flow",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.Char",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.Vehicle",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.Vehicle",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.number",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.number",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.number",
                }

            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.flow",
                }

            },
			char = 0,
			vehicle = 0,
			targetVehicle = 0,
			missionId = 0,
			cruiseSpeed = 0,
			drivingStyle = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTaskCarMissionNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "char", function ()
            node.char = editor.dataTypes[node.inputs[2].type].drawEditValue(node.char, "##charEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "vehicle", function ()
            node.vehicle = editor.dataTypes[node.inputs[3].type].drawEditValue(node.vehicle, "##vehicleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "targetVehicle", function ()
            node.targetVehicle = editor.dataTypes[node.inputs[4].type].drawEditValue(node.targetVehicle, "##targetVehicleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "missionId", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.CarMission) do
                if value == node.missionId then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##missionIdSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##missionIdSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##missionIdSelect", 0) then
                    if ImGui.BeginListBox("##missionIdSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.CarMission) do
                            if ImGui.Selectable(key, value == node.missionId, 0, ImVec2.new()) then
                                node.missionId = value;
                                ImGui.CloseCurrentPopup();
                            end
                        end
                        ImGui.EndListBox();
                    end
                    ImGui.EndPopup();
                end
                NodeEditor.Resume();
            end
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "cruiseSpeed", function ()
            node.cruiseSpeed = editor.dataTypes[node.inputs[6].type].drawEditValue(node.cruiseSpeed, "##cruiseSpeedEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "drivingStyle", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.DrivingMode) do
                if value == node.drivingStyle then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##drivingStyleSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##drivingStyleSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##drivingStyleSelect", 0) then
                    if ImGui.BeginListBox("##drivingStyleSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.DrivingMode) do
                            if ImGui.Selectable(key, value == node.drivingStyle, 0, ImVec2.new()) then
                                node.drivingStyle = value;
                                ImGui.CloseCurrentPopup();
                            end
                        end
                        ImGui.EndListBox();
                    end
                    ImGui.EndPopup();
                end
                NodeEditor.Resume();
            end
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorTaskCarMissionNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local char = inputValues[2] or node.char
		local vehicle = inputValues[3] or node.vehicle
		local targetVehicle = inputValues[4] or node.targetVehicle
		local missionId = inputValues[5] or node.missionId
		local cruiseSpeed = inputValues[6] or node.cruiseSpeed
		local drivingStyle = inputValues[7] or node.drivingStyle
		TaskOp.carMission(char, vehicle, targetVehicle, missionId, cruiseSpeed, drivingStyle)
        return {1}
    end
}

return taskCarMissionNode
