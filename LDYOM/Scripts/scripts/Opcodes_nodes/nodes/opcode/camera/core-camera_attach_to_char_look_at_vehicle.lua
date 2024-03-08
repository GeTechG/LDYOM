local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCameraAttachToCharLookAtVehicleNode : LDNodeEditorNode
---@field char integer
---@field xOffset number
---@field yOffset number
---@field zOffset number
---@field vehicle integer
---@field tilt number
---@field switchStyle integer

local CAMERA_ATTACH_TO_CHAR_LOOK_AT_VEHICLE_NODE_TYPE = "core.camera_attach_to_char_look_at_vehicle"

---@type LDNodeEditorNodeType
local cameraAttachToCharLookAtVehicleNode = {
    typeName = CAMERA_ATTACH_TO_CHAR_LOOK_AT_VEHICLE_NODE_TYPE,
    category = "opcode.camera",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCameraAttachToCharLookAtVehicleNode
        local newNode = {
            id = newNodeId,
            nodeType = CAMERA_ATTACH_TO_CHAR_LOOK_AT_VEHICLE_NODE_TYPE,
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
			xOffset = 0,
			yOffset = 0,
			zOffset = 0,
			vehicle = 0,
			tilt = 0,
			switchStyle = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCameraAttachToCharLookAtVehicleNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "char", function ()
            node.char = editor.dataTypes[node.inputs[2].type].drawEditValue(node.char, "##charEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "xOffset", function ()
            node.xOffset = editor.dataTypes[node.inputs[3].type].drawEditValue(node.xOffset, "##xOffsetEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "yOffset", function ()
            node.yOffset = editor.dataTypes[node.inputs[4].type].drawEditValue(node.yOffset, "##yOffsetEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "zOffset", function ()
            node.zOffset = editor.dataTypes[node.inputs[5].type].drawEditValue(node.zOffset, "##zOffsetEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "vehicle", function ()
            node.vehicle = editor.dataTypes[node.inputs[6].type].drawEditValue(node.vehicle, "##vehicleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "tilt", function ()
            node.tilt = editor.dataTypes[node.inputs[7].type].drawEditValue(node.tilt, "##tiltEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "switchStyle", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.SwitchType) do
                if value == node.switchStyle then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##switchStyleSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##switchStyleSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##switchStyleSelect", 0) then
                    if ImGui.BeginListBox("##switchStyleSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.SwitchType) do
                            if ImGui.Selectable(key, value == node.switchStyle, 0, ImVec2.new()) then
                                node.switchStyle = value;
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
    ---@param node LDNodeEditorCameraAttachToCharLookAtVehicleNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local char = inputValues[2] or node.char
		local xOffset = inputValues[3] or node.xOffset
		local yOffset = inputValues[4] or node.yOffset
		local zOffset = inputValues[5] or node.zOffset
		local vehicle = inputValues[6] or node.vehicle
		local tilt = inputValues[7] or node.tilt
		local switchStyle = inputValues[8] or node.switchStyle
		CameraOp.attachToCharLookAtVehicle(char, xOffset, yOffset, zOffset, vehicle, tilt, switchStyle)
        return {1}
    end
}

return cameraAttachToCharLookAtVehicleNode
