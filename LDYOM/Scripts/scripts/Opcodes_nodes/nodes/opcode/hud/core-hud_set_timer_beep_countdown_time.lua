local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorHudSetTimerBeepCountdownTimeNode : LDNodeEditorNode
---@field timer integer
---@field timeInSec integer

local HUD_SET_TIMER_BEEP_COUNTDOWN_TIME_NODE_TYPE = "core.hud_set_timer_beep_countdown_time"

---@type LDNodeEditorNodeType
local hudSetTimerBeepCountdownTimeNode = {
    typeName = HUD_SET_TIMER_BEEP_COUNTDOWN_TIME_NODE_TYPE,
    category = "opcode.hud",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorHudSetTimerBeepCountdownTimeNode
        local newNode = {
            id = newNodeId,
            nodeType = HUD_SET_TIMER_BEEP_COUNTDOWN_TIME_NODE_TYPE,
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

            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.flow",
                }

            },
			timer = 0,
			timeInSec = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorHudSetTimerBeepCountdownTimeNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "timer", function ()
            node.timer = editor.dataTypes[node.inputs[2].type].drawEditValue(node.timer, "##timerEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "timeInSec", function ()
            node.timeInSec = editor.dataTypes[node.inputs[3].type].drawEditValue(node.timeInSec, "##timeInSecEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorHudSetTimerBeepCountdownTimeNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local timer = inputValues[2] or node.timer
		local timeInSec = inputValues[3] or node.timeInSec
		HudOp.setTimerBeepCountdownTime(math.floor(timer), math.floor(timeInSec))
        return {1}
    end
}

return hudSetTimerBeepCountdownTimeNode
