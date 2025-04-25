local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorStreamingIsModelInCdimageNode : LDNodeEditorNode
---@field modeId integer

local STREAMING_IS_MODEL_IN_CDIMAGE_NODE_TYPE = "core.streaming_is_model_in_cdimage"

---@type LDNodeEditorNodeType
local streamingIsModelInCdimageNode = {
    typeName = STREAMING_IS_MODEL_IN_CDIMAGE_NODE_TYPE,
    category = "opcode.streaming",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorStreamingIsModelInCdimageNode
        local newNode = {
            id = newNodeId,
            nodeType = STREAMING_IS_MODEL_IN_CDIMAGE_NODE_TYPE,
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
			modeId = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorStreamingIsModelInCdimageNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "modeId", function ()
            node.modeId = editor.dataTypes[node.inputs[1].type].drawEditValue(node.modeId, "##modeIdEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorStreamingIsModelInCdimageNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local modeId = inputValues[1] or node.modeId
		local result = StreamingOp.isModelInCdimage(modeId)
        return {result}
    end
}

return streamingIsModelInCdimageNode
