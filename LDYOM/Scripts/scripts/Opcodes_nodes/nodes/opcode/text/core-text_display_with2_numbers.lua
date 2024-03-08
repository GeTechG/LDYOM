local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTextDisplayWith2NumbersNode : LDNodeEditorNode
---@field offsetLeft number
---@field offsetTop number
---@field key string
---@field num1 integer
---@field num2 integer

local TEXT_DISPLAY_WITH2_NUMBERS_NODE_TYPE = "core.text_display_with2_numbers"

---@type LDNodeEditorNodeType
local textDisplayWith2NumbersNode = {
    typeName = TEXT_DISPLAY_WITH2_NUMBERS_NODE_TYPE,
    category = "opcode.text",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTextDisplayWith2NumbersNode
        local newNode = {
            id = newNodeId,
            nodeType = TEXT_DISPLAY_WITH2_NUMBERS_NODE_TYPE,
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
			offsetLeft = 0,
			offsetTop = 0,
			key = "",
			num1 = 0,
			num2 = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTextDisplayWith2NumbersNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "offsetLeft", function ()
            node.offsetLeft = editor.dataTypes[node.inputs[2].type].drawEditValue(node.offsetLeft, "##offsetLeftEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "offsetTop", function ()
            node.offsetTop = editor.dataTypes[node.inputs[3].type].drawEditValue(node.offsetTop, "##offsetTopEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "key", function ()
            node.key = editor.dataTypes[node.inputs[4].type].drawEditValue(node.key, "##keyEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "num1", function ()
            node.num1 = editor.dataTypes[node.inputs[5].type].drawEditValue(node.num1, "##num1Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "num2", function ()
            node.num2 = editor.dataTypes[node.inputs[6].type].drawEditValue(node.num2, "##num2Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorTextDisplayWith2NumbersNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local offsetLeft = inputValues[2] or node.offsetLeft
		local offsetTop = inputValues[3] or node.offsetTop
		local key = inputValues[4] or node.key
		local num1 = inputValues[5] or node.num1
		local num2 = inputValues[6] or node.num2
		TextOp.displayWith2Numbers(offsetLeft, offsetTop, key, num1, num2)
        return {1}
    end
}

return textDisplayWith2NumbersNode
