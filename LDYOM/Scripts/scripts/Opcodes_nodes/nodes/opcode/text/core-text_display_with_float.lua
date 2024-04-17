local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTextDisplayWithFloatNode : LDNodeEditorNode
---@field leftTopX number
---@field leftTopY number
---@field key string
---@field value number
---@field precision integer

local TEXT_DISPLAY_WITH_FLOAT_NODE_TYPE = "core.text_display_with_float"

---@type LDNodeEditorNodeType
local textDisplayWithFloatNode = {
    typeName = TEXT_DISPLAY_WITH_FLOAT_NODE_TYPE,
    category = "opcode.text",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTextDisplayWithFloatNode
        local newNode = {
            id = newNodeId,
            nodeType = TEXT_DISPLAY_WITH_FLOAT_NODE_TYPE,
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
			leftTopX = 0,
			leftTopY = 0,
			key = "",
			value = 0,
			precision = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTextDisplayWithFloatNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "leftTopX", function ()
            node.leftTopX = editor.dataTypes[node.inputs[2].type].drawEditValue(node.leftTopX, "##leftTopXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "leftTopY", function ()
            node.leftTopY = editor.dataTypes[node.inputs[3].type].drawEditValue(node.leftTopY, "##leftTopYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "key", function ()
            node.key = editor.dataTypes[node.inputs[4].type].drawEditValue(node.key, "##keyEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "value", function ()
            node.value = editor.dataTypes[node.inputs[5].type].drawEditValue(node.value, "##valueEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "precision", function ()
            node.precision = editor.dataTypes[node.inputs[6].type].drawEditValue(node.precision, "##precisionEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorTextDisplayWithFloatNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local leftTopX = inputValues[2] or node.leftTopX
		local leftTopY = inputValues[3] or node.leftTopY
		local key = inputValues[4] or node.key
		local value = inputValues[5] or node.value
		local precision = inputValues[6] or node.precision
		TextOp.displayWithFloat(leftTopX, leftTopY, key, value, math.floor(precision))
        return {1}
    end
}

return textDisplayWithFloatNode
