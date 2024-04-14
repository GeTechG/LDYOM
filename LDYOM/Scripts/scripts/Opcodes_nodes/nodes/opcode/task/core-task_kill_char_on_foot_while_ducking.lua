local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTaskKillCharOnFootWhileDuckingNode : LDNodeEditorNode
---@field char integer
---@field target integer
---@field flags integer
---@field actionDelay integer
---@field actionChance integer

local TASK_KILL_CHAR_ON_FOOT_WHILE_DUCKING_NODE_TYPE = "core.task_kill_char_on_foot_while_ducking"

---@type LDNodeEditorNodeType
local taskKillCharOnFootWhileDuckingNode = {
    typeName = TASK_KILL_CHAR_ON_FOOT_WHILE_DUCKING_NODE_TYPE,
    category = "opcode.task",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTaskKillCharOnFootWhileDuckingNode
        local newNode = {
            id = newNodeId,
            nodeType = TASK_KILL_CHAR_ON_FOOT_WHILE_DUCKING_NODE_TYPE,
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
			flags = 0,
			actionDelay = 0,
			actionChance = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTaskKillCharOnFootWhileDuckingNode
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "flags", function ()
            node.flags = editor.dataTypes[node.inputs[4].type].drawEditValue(node.flags, "##flagsEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "actionDelay", function ()
            node.actionDelay = editor.dataTypes[node.inputs[5].type].drawEditValue(node.actionDelay, "##actionDelayEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "actionChance", function ()
            node.actionChance = editor.dataTypes[node.inputs[6].type].drawEditValue(node.actionChance, "##actionChanceEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorTaskKillCharOnFootWhileDuckingNode
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
		local flags = inputValues[4] or node.flags
		local actionDelay = inputValues[5] or node.actionDelay
		local actionChance = inputValues[6] or node.actionChance
		TaskOp.killCharOnFootWhileDucking(char, target, flags, actionDelay, actionChance)
        return {1}
    end
}

return taskKillCharOnFootWhileDuckingNode
