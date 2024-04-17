local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTextPrintHelpForeverWithNumberNode : LDNodeEditorNode
---@field gxt string
---@field number integer

local TEXT_PRINT_HELP_FOREVER_WITH_NUMBER_NODE_TYPE = "core.text_print_help_forever_with_number"

---@type LDNodeEditorNodeType
local textPrintHelpForeverWithNumberNode = {
    typeName = TEXT_PRINT_HELP_FOREVER_WITH_NUMBER_NODE_TYPE,
    category = "opcode.text",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTextPrintHelpForeverWithNumberNode
        local newNode = {
            id = newNodeId,
            nodeType = TEXT_PRINT_HELP_FOREVER_WITH_NUMBER_NODE_TYPE,
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
			gxt = "",
			number = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTextPrintHelpForeverWithNumberNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "gxt", function ()
            node.gxt = editor.dataTypes[node.inputs[2].type].drawEditValue(node.gxt, "##gxtEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "number", function ()
            node.number = editor.dataTypes[node.inputs[3].type].drawEditValue(node.number, "##numberEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorTextPrintHelpForeverWithNumberNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local gxt = inputValues[2] or node.gxt
		local number = inputValues[3] or node.number
		TextOp.printHelpForeverWithNumber(gxt, math.floor(number))
        return {1}
    end
}

return textPrintHelpForeverWithNumberNode
