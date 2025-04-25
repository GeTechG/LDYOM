local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorStreamingRemoveAnimationNode : LDNodeEditorNode
---@field animationFile string

local STREAMING_REMOVE_ANIMATION_NODE_TYPE = "core.streaming_remove_animation"

---@type LDNodeEditorNodeType
local streamingRemoveAnimationNode = {
    typeName = STREAMING_REMOVE_ANIMATION_NODE_TYPE,
    category = "opcode.streaming",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorStreamingRemoveAnimationNode
        local newNode = {
            id = newNodeId,
            nodeType = STREAMING_REMOVE_ANIMATION_NODE_TYPE,
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
			animationFile = ""
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorStreamingRemoveAnimationNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "animationFile", function ()
            node.animationFile = editor.dataTypes[node.inputs[2].type].drawEditValue(node.animationFile, "##animationFileEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorStreamingRemoveAnimationNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local animationFile = inputValues[2] or node.animationFile
		StreamingOp.removeAnimation(animationFile)
        return {1}
    end
}

return streamingRemoveAnimationNode
