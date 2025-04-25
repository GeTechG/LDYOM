local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorMenuSetItemWith2NumbersNode : LDNodeEditorNode
---@field self_ integer
---@field column integer
---@field row integer
---@field gxt string
---@field number1 integer
---@field number2 integer

local MENU_SET_ITEM_WITH2_NUMBERS_NODE_TYPE = "core.menu_set_item_with2_numbers"

---@type LDNodeEditorNodeType
local menuSetItemWith2NumbersNode = {
    typeName = MENU_SET_ITEM_WITH2_NUMBERS_NODE_TYPE,
    category = "opcode.menu",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorMenuSetItemWith2NumbersNode
        local newNode = {
            id = newNodeId,
            nodeType = MENU_SET_ITEM_WITH2_NUMBERS_NODE_TYPE,
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
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.number",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.number",
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
			self_ = 0,
			column = 0,
			row = 0,
			gxt = "",
			number1 = 0,
			number2 = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorMenuSetItemWith2NumbersNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[2].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "column", function ()
            node.column = editor.dataTypes[node.inputs[3].type].drawEditValue(node.column, "##columnEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "row", function ()
            node.row = editor.dataTypes[node.inputs[4].type].drawEditValue(node.row, "##rowEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "gxt", function ()
            node.gxt = editor.dataTypes[node.inputs[5].type].drawEditValue(node.gxt, "##gxtEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "number1", function ()
            node.number1 = editor.dataTypes[node.inputs[6].type].drawEditValue(node.number1, "##number1Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "number2", function ()
            node.number2 = editor.dataTypes[node.inputs[7].type].drawEditValue(node.number2, "##number2Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorMenuSetItemWith2NumbersNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local self_ = inputValues[2] or node.self_
		local column = inputValues[3] or node.column
		local row = inputValues[4] or node.row
		local gxt = inputValues[5] or node.gxt
		local number1 = inputValues[6] or node.number1
		local number2 = inputValues[7] or node.number2
		MenuOp.setItemWith2Numbers(self_, math.floor(column), math.floor(row), gxt, math.floor(number1), math.floor(number2))
        return {1}
    end
}

return menuSetItemWith2NumbersNode
