local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorStreamingGetRandomCarModelInMemoryNode : LDNodeEditorNode
---@field _p1 boolean

local STREAMING_GET_RANDOM_CAR_MODEL_IN_MEMORY_NODE_TYPE = "core.streaming_get_random_car_model_in_memory"

---@type LDNodeEditorNodeType
local streamingGetRandomCarModelInMemoryNode = {
    typeName = STREAMING_GET_RANDOM_CAR_MODEL_IN_MEMORY_NODE_TYPE,
    category = "opcode.streaming",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorStreamingGetRandomCarModelInMemoryNode
        local newNode = {
            id = newNodeId,
            nodeType = STREAMING_GET_RANDOM_CAR_MODEL_IN_MEMORY_NODE_TYPE,
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

            },
			_p1 = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorStreamingGetRandomCarModelInMemoryNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "_p1", function ()
            node._p1 = editor.dataTypes[node.inputs[2].type].drawEditValue(node._p1, "##_p1Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "modelId");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[3], "class");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorStreamingGetRandomCarModelInMemoryNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local _p1 = inputValues[2] or node._p1
		local modelId, class = StreamingOp.getRandomCarModelInMemory(_p1)
        return {1, modelId, class}
    end
}

return streamingGetRandomCarModelInMemoryNode
