local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorStreamingHasAnimationLoadedNode : LDNodeEditorNode
---@field animationFile string

local STREAMING_HAS_ANIMATION_LOADED_NODE_TYPE = "core.streaming_has_animation_loaded"

---@type LDNodeEditorNodeType
local streamingHasAnimationLoadedNode = {
    typeName = STREAMING_HAS_ANIMATION_LOADED_NODE_TYPE,
    category = "opcode.streaming",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorStreamingHasAnimationLoadedNode
        local newNode = {
            id = newNodeId,
            nodeType = STREAMING_HAS_ANIMATION_LOADED_NODE_TYPE,
            inputs = {
                {
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
                    type = "core.bool",
                }

            },
			animationFile = ""
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorStreamingHasAnimationLoadedNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "animationFile", function ()
            node.animationFile = editor.dataTypes[node.inputs[1].type].drawEditValue(node.animationFile, "##animationFileEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorStreamingHasAnimationLoadedNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local animationFile = inputValues[1] or node.animationFile
		local result = StreamingOp.hasAnimationLoaded(animationFile)
        return {result}
    end
}

return streamingHasAnimationLoadedNode
