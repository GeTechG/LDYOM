local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTextPrintHelpFormattedNode : LDNodeEditorNode
---@field text string
---@field args integer

local TEXT_PRINT_HELP_FORMATTED_NODE_TYPE = "core.text_print_help_formatted"

---@type LDNodeEditorNodeType
local textPrintHelpFormattedNode = {
    typeName = TEXT_PRINT_HELP_FORMATTED_NODE_TYPE,
    category = "opcode.text",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTextPrintHelpFormattedNode
        local newNode = {
            id = newNodeId,
            nodeType = TEXT_PRINT_HELP_FORMATTED_NODE_TYPE,
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
			text = "",
			args = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTextPrintHelpFormattedNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "text", function ()
            node.text = editor.dataTypes[node.inputs[2].type].drawEditValue(node.text, "##textEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "args", function ()
            node.args = editor.dataTypes[node.inputs[3].type].drawEditValue(node.args, "##argsEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorTextPrintHelpFormattedNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local text = inputValues[2] or node.text
		local args = inputValues[3] or node.args
		TextOp.printHelpFormatted(text, args)
        return {1}
    end
}

return textPrintHelpFormattedNode
