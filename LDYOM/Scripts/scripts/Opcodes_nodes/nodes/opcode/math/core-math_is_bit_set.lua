local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorMathIsBitSetNode : LDNodeEditorNode
---@field number integer
---@field n integer

local MATH_IS_BIT_SET_NODE_TYPE = "core.math_is_bit_set"

---@type LDNodeEditorNodeType
local mathIsBitSetNode = {
    typeName = MATH_IS_BIT_SET_NODE_TYPE,
    category = "opcode.math",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorMathIsBitSetNode
        local newNode = {
            id = newNodeId,
            nodeType = MATH_IS_BIT_SET_NODE_TYPE,
            inputs = {
                {
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
                    type = "core.bool",
                }

            },
			number = 0,
			n = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorMathIsBitSetNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "number", function ()
            node.number = editor.dataTypes[node.inputs[1].type].drawEditValue(node.number, "##numberEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "n", function ()
            node.n = editor.dataTypes[node.inputs[2].type].drawEditValue(node.n, "##nEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorMathIsBitSetNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local number = inputValues[1] or node.number
		local n = inputValues[2] or node.n
		local result = MathOp.isBitSet(number, n)
        return {result}
    end
}

return mathIsBitSetNode