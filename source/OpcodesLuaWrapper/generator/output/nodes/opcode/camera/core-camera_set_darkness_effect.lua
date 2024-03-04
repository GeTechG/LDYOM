local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCameraSetDarknessEffectNode : LDNodeEditorNode
---@field enable boolean
---@field pitchBlack integer

local CAMERA_SET_DARKNESS_EFFECT_NODE_TYPE = "core.camera_set_darkness_effect"

---@type LDNodeEditorNodeType
local cameraSetDarknessEffectNode = {
    typeName = CAMERA_SET_DARKNESS_EFFECT_NODE_TYPE,
    category = "opcode.camera",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCameraSetDarknessEffectNode
        local newNode = {
            id = newNodeId,
            nodeType = CAMERA_SET_DARKNESS_EFFECT_NODE_TYPE,
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
                    type = "core.bool",
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
			enable = false,
			pitchBlack = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCameraSetDarknessEffectNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "enable", function ()
            node.enable = editor.dataTypes[node.inputs[2].type].drawEditValue(node.enable, "##enableEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "pitchBlack", function ()
            node.pitchBlack = editor.dataTypes[node.inputs[3].type].drawEditValue(node.pitchBlack, "##pitchBlackEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorCameraSetDarknessEffectNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local enable = inputValues[2] or node.enable
		local pitchBlack = inputValues[3] or node.pitchBlack
		CameraOp.setDarknessEffect(enable, pitchBlack)
        return {1}
    end
}

return cameraSetDarknessEffectNode
