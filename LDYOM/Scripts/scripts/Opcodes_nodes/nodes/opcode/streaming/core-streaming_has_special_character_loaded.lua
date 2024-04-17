local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorStreamingHasSpecialCharacterLoadedNode : LDNodeEditorNode
---@field slotId integer

local STREAMING_HAS_SPECIAL_CHARACTER_LOADED_NODE_TYPE = "core.streaming_has_special_character_loaded"

---@type LDNodeEditorNodeType
local streamingHasSpecialCharacterLoadedNode = {
    typeName = STREAMING_HAS_SPECIAL_CHARACTER_LOADED_NODE_TYPE,
    category = "opcode.streaming",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorStreamingHasSpecialCharacterLoadedNode
        local newNode = {
            id = newNodeId,
            nodeType = STREAMING_HAS_SPECIAL_CHARACTER_LOADED_NODE_TYPE,
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
			slotId = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorStreamingHasSpecialCharacterLoadedNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "slotId", function ()
            node.slotId = editor.dataTypes[node.inputs[1].type].drawEditValue(node.slotId, "##slotIdEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorStreamingHasSpecialCharacterLoadedNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local slotId = inputValues[1] or node.slotId
		local result = StreamingOp.hasSpecialCharacterLoaded(math.floor(slotId))
        return {result}
    end
}

return streamingHasSpecialCharacterLoadedNode
