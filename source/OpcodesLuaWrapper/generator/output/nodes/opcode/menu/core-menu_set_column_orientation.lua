local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorMenuSetColumnOrientationNode : LDNodeEditorNode
---@field self_ integer
---@field column integer
---@field alignment integer

local MENU_SET_COLUMN_ORIENTATION_NODE_TYPE = "core.menu_set_column_orientation"

---@type LDNodeEditorNodeType
local menuSetColumnOrientationNode = {
    typeName = MENU_SET_COLUMN_ORIENTATION_NODE_TYPE,
    category = "opcode.menu",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorMenuSetColumnOrientationNode
        local newNode = {
            id = newNodeId,
            nodeType = MENU_SET_COLUMN_ORIENTATION_NODE_TYPE,
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
			alignment = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorMenuSetColumnOrientationNode
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "alignment", function ()
            node.alignment = editor.dataTypes[node.inputs[4].type].drawEditValue(node.alignment, "##alignmentEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorMenuSetColumnOrientationNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local self_ = inputValues[2] or node.self_
		local column = inputValues[3] or node.column
		local alignment = inputValues[4] or node.alignment
		MenuOp.setColumnOrientation(self_, column, alignment)
        return {1}
    end
}

return menuSetColumnOrientationNode