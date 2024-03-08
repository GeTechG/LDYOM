local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorMenuSetColumnNode : LDNodeEditorNode
---@field self_ integer
---@field column integer
---@field title string
---@field row0 string
---@field row1 string
---@field row2 string
---@field row3 string
---@field row4 string
---@field row5 string
---@field row6 string
---@field row7 string
---@field row8 string
---@field row9 string
---@field row10 string
---@field row11 string

local MENU_SET_COLUMN_NODE_TYPE = "core.menu_set_column"

---@type LDNodeEditorNodeType
local menuSetColumnNode = {
    typeName = MENU_SET_COLUMN_NODE_TYPE,
    category = "opcode.menu",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorMenuSetColumnNode
        local newNode = {
            id = newNodeId,
            nodeType = MENU_SET_COLUMN_NODE_TYPE,
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
                    type = "core.string",
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
                    type = "core.string",
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
                    type = "core.string",
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
                    type = "core.string",
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
                    type = "core.string",
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
                    type = "core.string",
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
			self_ = 0,
			column = 0,
			title = "",
			row0 = "",
			row1 = "",
			row2 = "",
			row3 = "",
			row4 = "",
			row5 = "",
			row6 = "",
			row7 = "",
			row8 = "",
			row9 = "",
			row10 = "",
			row11 = ""
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorMenuSetColumnNode
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "title", function ()
            node.title = editor.dataTypes[node.inputs[4].type].drawEditValue(node.title, "##titleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "row0", function ()
            node.row0 = editor.dataTypes[node.inputs[5].type].drawEditValue(node.row0, "##row0Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "row1", function ()
            node.row1 = editor.dataTypes[node.inputs[6].type].drawEditValue(node.row1, "##row1Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "row2", function ()
            node.row2 = editor.dataTypes[node.inputs[7].type].drawEditValue(node.row2, "##row2Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "row3", function ()
            node.row3 = editor.dataTypes[node.inputs[8].type].drawEditValue(node.row3, "##row3Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[9], "row4", function ()
            node.row4 = editor.dataTypes[node.inputs[9].type].drawEditValue(node.row4, "##row4Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[10], "row5", function ()
            node.row5 = editor.dataTypes[node.inputs[10].type].drawEditValue(node.row5, "##row5Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[11], "row6", function ()
            node.row6 = editor.dataTypes[node.inputs[11].type].drawEditValue(node.row6, "##row6Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[12], "row7", function ()
            node.row7 = editor.dataTypes[node.inputs[12].type].drawEditValue(node.row7, "##row7Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[13], "row8", function ()
            node.row8 = editor.dataTypes[node.inputs[13].type].drawEditValue(node.row8, "##row8Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[14], "row9", function ()
            node.row9 = editor.dataTypes[node.inputs[14].type].drawEditValue(node.row9, "##row9Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[15], "row10", function ()
            node.row10 = editor.dataTypes[node.inputs[15].type].drawEditValue(node.row10, "##row10Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[16], "row11", function ()
            node.row11 = editor.dataTypes[node.inputs[16].type].drawEditValue(node.row11, "##row11Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorMenuSetColumnNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local self_ = inputValues[2] or node.self_
		local column = inputValues[3] or node.column
		local title = inputValues[4] or node.title
		local row0 = inputValues[5] or node.row0
		local row1 = inputValues[6] or node.row1
		local row2 = inputValues[7] or node.row2
		local row3 = inputValues[8] or node.row3
		local row4 = inputValues[9] or node.row4
		local row5 = inputValues[10] or node.row5
		local row6 = inputValues[11] or node.row6
		local row7 = inputValues[12] or node.row7
		local row8 = inputValues[13] or node.row8
		local row9 = inputValues[14] or node.row9
		local row10 = inputValues[15] or node.row10
		local row11 = inputValues[16] or node.row11
		MenuOp.setColumn(self_, column, title, row0, row1, row2, row3, row4, row5, row6, row7, row8, row9, row10, row11)
        return {1}
    end
}

return menuSetColumnNode
