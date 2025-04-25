local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorStreamingIsThisModelAPlaneNode : LDNodeEditorNode
---@field modelId integer

local STREAMING_IS_THIS_MODEL_A_PLANE_NODE_TYPE = "core.streaming_is_this_model_a_plane"

---@type LDNodeEditorNodeType
local streamingIsThisModelAPlaneNode = {
    typeName = STREAMING_IS_THIS_MODEL_A_PLANE_NODE_TYPE,
    category = "opcode.streaming",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorStreamingIsThisModelAPlaneNode
        local newNode = {
            id = newNodeId,
            nodeType = STREAMING_IS_THIS_MODEL_A_PLANE_NODE_TYPE,
            inputs = {
                {
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
                    type = "core.bool",
                }

            },
			modelId = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorStreamingIsThisModelAPlaneNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "modelId", function ()
            node.modelId = editor.dataTypes[node.inputs[1].type].drawEditValue(node.modelId, "##modelIdEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorStreamingIsThisModelAPlaneNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local modelId = inputValues[1] or node.modelId
		local result = StreamingOp.isThisModelAPlane(modelId)
        return {result}
    end
}

return streamingIsThisModelAPlaneNode
