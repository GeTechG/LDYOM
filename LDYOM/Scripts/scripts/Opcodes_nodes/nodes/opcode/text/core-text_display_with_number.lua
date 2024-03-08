local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTextDisplayWithNumberNode : LDNodeEditorNode
---@field offsetLeft number
---@field offsetTop number
---@field key string
---@field num integer

local TEXT_DISPLAY_WITH_NUMBER_NODE_TYPE = "core.text_display_with_number"

---@type LDNodeEditorNodeType
local textDisplayWithNumberNode = {
    typeName = TEXT_DISPLAY_WITH_NUMBER_NODE_TYPE,
    category = "opcode.text",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTextDisplayWithNumberNode
        local newNode = {
            id = newNodeId,
            nodeType = TEXT_DISPLAY_WITH_NUMBER_NODE_TYPE,
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
			num = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTextDisplayWithNumberNode
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "num", function ()
            node.num = editor.dataTypes[node.inputs[5].type].drawEditValue(node.num, "##numEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorTextDisplayWithNumberNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local offsetLeft = inputValues[2] or node.offsetLeft
		local offsetTop = inputValues[3] or node.offsetTop
		local key = inputValues[4] or node.key
		local num = inputValues[5] or node.num
		TextOp.displayWithNumber(offsetLeft, offsetTop, key, num)
        return {1}
    end
}

return textDisplayWithNumberNode
