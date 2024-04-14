local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTaskCarDriveToCoordNode : LDNodeEditorNode
---@field driver integer
---@field vehicle integer
---@field x number
---@field y number
---@field z number
---@field speed number
---@field _p7 integer
---@field _p8 integer
---@field drivingStyle integer

local TASK_CAR_DRIVE_TO_COORD_NODE_TYPE = "core.task_car_drive_to_coord"

---@type LDNodeEditorNodeType
local taskCarDriveToCoordNode = {
    typeName = TASK_CAR_DRIVE_TO_COORD_NODE_TYPE,
    category = "opcode.task",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTaskCarDriveToCoordNode
        local newNode = {
            id = newNodeId,
            nodeType = TASK_CAR_DRIVE_TO_COORD_NODE_TYPE,
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
			driver = 0,
			vehicle = 0,
			x = 0,
			y = 0,
			z = 0,
			speed = 0,
			_p7 = 0,
			_p8 = 0,
			drivingStyle = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTaskCarDriveToCoordNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "driver", function ()
            node.driver = editor.dataTypes[node.inputs[2].type].drawEditValue(node.driver, "##driverEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "vehicle", function ()
            node.vehicle = editor.dataTypes[node.inputs[3].type].drawEditValue(node.vehicle, "##vehicleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "x", function ()
            node.x = editor.dataTypes[node.inputs[4].type].drawEditValue(node.x, "##xEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "y", function ()
            node.y = editor.dataTypes[node.inputs[5].type].drawEditValue(node.y, "##yEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "z", function ()
            node.z = editor.dataTypes[node.inputs[6].type].drawEditValue(node.z, "##zEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "speed", function ()
            node.speed = editor.dataTypes[node.inputs[7].type].drawEditValue(node.speed, "##speedEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "_p7", function ()
            node._p7 = editor.dataTypes[node.inputs[8].type].drawEditValue(node._p7, "##_p7Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[9], "_p8", function ()
            node._p8 = editor.dataTypes[node.inputs[9].type].drawEditValue(node._p8, "##_p8Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[10], "drivingStyle", function ()
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
    ---@param node LDNodeEditorTaskCarDriveToCoordNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local driver = inputValues[2] or node.driver
		if driver == nil or driver == 0 then
            error("Input driver is not set")
        end
		local vehicle = inputValues[3] or node.vehicle
		if vehicle == nil or vehicle == 0 then
            error("Input vehicle is not set")
        end
		local x = inputValues[4] or node.x
		local y = inputValues[5] or node.y
		local z = inputValues[6] or node.z
		local speed = inputValues[7] or node.speed
		local _p7 = inputValues[8] or node._p7
		local _p8 = inputValues[9] or node._p8
		local drivingStyle = inputValues[10] or node.drivingStyle
		TaskOp.carDriveToCoord(driver, vehicle, x, y, z, speed, _p7, _p8, drivingStyle)
        return {1}
    end
}

return taskCarDriveToCoordNode
