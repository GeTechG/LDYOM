local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCameraAttachToCharNode : LDNodeEditorNode
---@field handle integer
---@field xOffset number
---@field yOffset number
---@field zOffset number
---@field xRotation number
---@field yRotation number
---@field zRotation number
---@field tilt number
---@field switchStyle integer

local CAMERA_ATTACH_TO_CHAR_NODE_TYPE = "core.camera_attach_to_char"

---@type LDNodeEditorNodeType
local cameraAttachToCharNode = {
    typeName = CAMERA_ATTACH_TO_CHAR_NODE_TYPE,
    category = "opcode.camera",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCameraAttachToCharNode
        local newNode = {
            id = newNodeId,
            nodeType = CAMERA_ATTACH_TO_CHAR_NODE_TYPE,
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
			handle = 0,
			xOffset = 0,
			yOffset = 0,
			zOffset = 0,
			xRotation = 0,
			yRotation = 0,
			zRotation = 0,
			tilt = 0,
			switchStyle = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCameraAttachToCharNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "handle", function ()
            node.handle = editor.dataTypes[node.inputs[2].type].drawEditValue(node.handle, "##handleEdit", fontScale * 100)
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "xRotation", function ()
            node.xRotation = editor.dataTypes[node.inputs[6].type].drawEditValue(node.xRotation, "##xRotationEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "yRotation", function ()
            node.yRotation = editor.dataTypes[node.inputs[7].type].drawEditValue(node.yRotation, "##yRotationEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "zRotation", function ()
            node.zRotation = editor.dataTypes[node.inputs[8].type].drawEditValue(node.zRotation, "##zRotationEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[9], "tilt", function ()
            node.tilt = editor.dataTypes[node.inputs[9].type].drawEditValue(node.tilt, "##tiltEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[10], "switchStyle", function ()
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
    ---@param node LDNodeEditorCameraAttachToCharNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local handle = inputValues[2] or node.handle
		local xOffset = inputValues[3] or node.xOffset
		local yOffset = inputValues[4] or node.yOffset
		local zOffset = inputValues[5] or node.zOffset
		local xRotation = inputValues[6] or node.xRotation
		local yRotation = inputValues[7] or node.yRotation
		local zRotation = inputValues[8] or node.zRotation
		local tilt = inputValues[9] or node.tilt
		local switchStyle = inputValues[10] or node.switchStyle
		CameraOp.attachToChar(handle, xOffset, yOffset, zOffset, xRotation, yRotation, zRotation, tilt, switchStyle)
        return {1}
    end
}

return cameraAttachToCharNode
