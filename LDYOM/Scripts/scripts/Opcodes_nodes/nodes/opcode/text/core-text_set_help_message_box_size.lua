local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTextSetHelpMessageBoxSizeNode : LDNodeEditorNode
---@field size integer

local TEXT_SET_HELP_MESSAGE_BOX_SIZE_NODE_TYPE = "core.text_set_help_message_box_size"

---@type LDNodeEditorNodeType
local textSetHelpMessageBoxSizeNode = {
    typeName = TEXT_SET_HELP_MESSAGE_BOX_SIZE_NODE_TYPE,
    category = "opcode.text",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTextSetHelpMessageBoxSizeNode
        local newNode = {
            id = newNodeId,
            nodeType = TEXT_SET_HELP_MESSAGE_BOX_SIZE_NODE_TYPE,
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

            },
			size = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTextSetHelpMessageBoxSizeNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "size", function ()
            node.size = editor.dataTypes[node.inputs[2].type].drawEditValue(node.size, "##sizeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorTextSetHelpMessageBoxSizeNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local size = inputValues[2] or node.size
		TextOp.setHelpMessageBoxSize(size)
        return {1}
    end
}

return textSetHelpMessageBoxSizeNode
