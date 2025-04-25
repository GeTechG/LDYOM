local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTaskKillCharOnFootNode : LDNodeEditorNode
---@field killer integer
---@field target integer

local TASK_KILL_CHAR_ON_FOOT_NODE_TYPE = "core.task_kill_char_on_foot"

---@type LDNodeEditorNodeType
local taskKillCharOnFootNode = {
    typeName = TASK_KILL_CHAR_ON_FOOT_NODE_TYPE,
    category = "opcode.task",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTaskKillCharOnFootNode
        local newNode = {
            id = newNodeId,
            nodeType = TASK_KILL_CHAR_ON_FOOT_NODE_TYPE,
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

            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.flow",
                }

            },
			killer = 0,
			target = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTaskKillCharOnFootNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "killer", function ()
            node.killer = editor.dataTypes[node.inputs[2].type].drawEditValue(node.killer, "##killerEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "target", function ()
            node.target = editor.dataTypes[node.inputs[3].type].drawEditValue(node.target, "##targetEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorTaskKillCharOnFootNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local killer = inputValues[2] or node.killer
		if killer == nil or killer == 0 then
            error("Input killer is not set")
        end
		local target = inputValues[3] or node.target
		if target == nil or target == 0 then
            error("Input target is not set")
        end
		TaskOp.killCharOnFoot(killer, target)
        return {1}
    end
}

return taskKillCharOnFootNode
