local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorStreamingRemoveIplDiscreetlyNode : LDNodeEditorNode
---@field iplName string

local STREAMING_REMOVE_IPL_DISCREETLY_NODE_TYPE = "core.streaming_remove_ipl_discreetly"

---@type LDNodeEditorNodeType
local streamingRemoveIplDiscreetlyNode = {
    typeName = STREAMING_REMOVE_IPL_DISCREETLY_NODE_TYPE,
    category = "opcode.streaming",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorStreamingRemoveIplDiscreetlyNode
        local newNode = {
            id = newNodeId,
            nodeType = STREAMING_REMOVE_IPL_DISCREETLY_NODE_TYPE,
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
                    type = "core.string",
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
			iplName = ""
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorStreamingRemoveIplDiscreetlyNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "iplName", function ()
            node.iplName = editor.dataTypes[node.inputs[2].type].drawEditValue(node.iplName, "##iplNameEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorStreamingRemoveIplDiscreetlyNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local iplName = inputValues[2] or node.iplName
		StreamingOp.removeIplDiscreetly(iplName)
        return {1}
    end
}

return streamingRemoveIplDiscreetlyNode
