local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTextPrintWithNumberNowNode : LDNodeEditorNode
---@field key string
---@field num integer
---@field duration integer
---@field flag integer

local TEXT_PRINT_WITH_NUMBER_NOW_NODE_TYPE = "core.text_print_with_number_now"

---@type LDNodeEditorNodeType
local textPrintWithNumberNowNode = {
    typeName = TEXT_PRINT_WITH_NUMBER_NOW_NODE_TYPE,
    category = "opcode.text",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTextPrintWithNumberNowNode
        local newNode = {
            id = newNodeId,
            nodeType = TEXT_PRINT_WITH_NUMBER_NOW_NODE_TYPE,
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
			key = "",
			num = 0,
			duration = 0,
			flag = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTextPrintWithNumberNowNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "key", function ()
            node.key = editor.dataTypes[node.inputs[2].type].drawEditValue(node.key, "##keyEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "num", function ()
            node.num = editor.dataTypes[node.inputs[3].type].drawEditValue(node.num, "##numEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "duration", function ()
            node.duration = editor.dataTypes[node.inputs[4].type].drawEditValue(node.duration, "##durationEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "flag", function ()
            node.flag = editor.dataTypes[node.inputs[5].type].drawEditValue(node.flag, "##flagEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorTextPrintWithNumberNowNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local key = inputValues[2] or node.key
		local num = inputValues[3] or node.num
		local duration = inputValues[4] or node.duration
		local flag = inputValues[5] or node.flag
		TextOp.printWithNumberNow(key, math.floor(num), math.floor(duration), math.floor(flag))
        return {1}
    end
}

return textPrintWithNumberNowNode
