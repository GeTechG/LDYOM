local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTextGetStringWidthWithNumberNode : LDNodeEditorNode
---@field gxtEntry string
---@field number integer

local TEXT_GET_STRING_WIDTH_WITH_NUMBER_NODE_TYPE = "core.text_get_string_width_with_number"

---@type LDNodeEditorNodeType
local textGetStringWidthWithNumberNode = {
    typeName = TEXT_GET_STRING_WIDTH_WITH_NUMBER_NODE_TYPE,
    category = "opcode.text",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTextGetStringWidthWithNumberNode
        local newNode = {
            id = newNodeId,
            nodeType = TEXT_GET_STRING_WIDTH_WITH_NUMBER_NODE_TYPE,
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
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }

            },
			gxtEntry = "",
			number = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTextGetStringWidthWithNumberNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "gxtEntry", function ()
            node.gxtEntry = editor.dataTypes[node.inputs[2].type].drawEditValue(node.gxtEntry, "##gxtEntryEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "number", function ()
            node.number = editor.dataTypes[node.inputs[3].type].drawEditValue(node.number, "##numberEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "width");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorTextGetStringWidthWithNumberNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local gxtEntry = inputValues[2] or node.gxtEntry
		local number = inputValues[3] or node.number
		local width = TextOp.getStringWidthWithNumber(gxtEntry, number)
        return {1, width}
    end
}

return textGetStringWidthWithNumberNode
