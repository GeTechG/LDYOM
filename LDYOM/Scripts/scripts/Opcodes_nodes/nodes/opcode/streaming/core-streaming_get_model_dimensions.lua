local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorStreamingGetModelDimensionsNode : LDNodeEditorNode
---@field modelId integer

local STREAMING_GET_MODEL_DIMENSIONS_NODE_TYPE = "core.streaming_get_model_dimensions"

---@type LDNodeEditorNodeType
local streamingGetModelDimensionsNode = {
    typeName = STREAMING_GET_MODEL_DIMENSIONS_NODE_TYPE,
    category = "opcode.streaming",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorStreamingGetModelDimensionsNode
        local newNode = {
            id = newNodeId,
            nodeType = STREAMING_GET_MODEL_DIMENSIONS_NODE_TYPE,
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
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }

            },
			modelId = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorStreamingGetModelDimensionsNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "modelId", function ()
            node.modelId = editor.dataTypes[node.inputs[2].type].drawEditValue(node.modelId, "##modelIdEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "leftBottomBackX");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[3], "leftBottomBackY");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[4], "leftBottomBackZ");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[5], "rightTopFrontX");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[6], "rightTopFrontY");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[7], "rightTopFrontZ");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorStreamingGetModelDimensionsNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local modelId = inputValues[2] or node.modelId
		local leftBottomBackX, leftBottomBackY, leftBottomBackZ, rightTopFrontX, rightTopFrontY, rightTopFrontZ = StreamingOp.getModelDimensions(modelId)
        return {1, leftBottomBackX, leftBottomBackY, leftBottomBackZ, rightTopFrontX, rightTopFrontY, rightTopFrontZ}
    end
}

return streamingGetModelDimensionsNode
