local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTaskGotoCharOffsetNode : LDNodeEditorNode
---@field char integer
---@field target integer
---@field time integer
---@field radius number
---@field heading number

local TASK_GOTO_CHAR_OFFSET_NODE_TYPE = "core.task_goto_char_offset"

---@type LDNodeEditorNodeType
local taskGotoCharOffsetNode = {
    typeName = TASK_GOTO_CHAR_OFFSET_NODE_TYPE,
    category = "opcode.task",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTaskGotoCharOffsetNode
        local newNode = {
            id = newNodeId,
            nodeType = TASK_GOTO_CHAR_OFFSET_NODE_TYPE,
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
                    type = "core.Char",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.Char",
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
			char = 0,
			target = 0,
			time = 0,
			radius = 0,
			heading = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTaskGotoCharOffsetNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "char", function ()
            node.char = editor.dataTypes[node.inputs[2].type].drawEditValue(node.char, "##charEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "target", function ()
            node.target = editor.dataTypes[node.inputs[3].type].drawEditValue(node.target, "##targetEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "time", function ()
            node.time = editor.dataTypes[node.inputs[4].type].drawEditValue(node.time, "##timeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "radius", function ()
            node.radius = editor.dataTypes[node.inputs[5].type].drawEditValue(node.radius, "##radiusEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "heading", function ()
            node.heading = editor.dataTypes[node.inputs[6].type].drawEditValue(node.heading, "##headingEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorTaskGotoCharOffsetNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local char = inputValues[2] or node.char
		if char == nil or char == 0 then
            error("Input char is not set")
        end
		local target = inputValues[3] or node.target
		if target == nil or target == 0 then
            error("Input target is not set")
        end
		local time = inputValues[4] or node.time
		local radius = inputValues[5] or node.radius
		local heading = inputValues[6] or node.heading
		TaskOp.gotoCharOffset(char, target, time, radius, heading)
        return {1}
    end
}

return taskGotoCharOffsetNode
