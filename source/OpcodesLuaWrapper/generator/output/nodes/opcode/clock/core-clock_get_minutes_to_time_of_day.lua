local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorClockGetMinutesToTimeOfDayNode : LDNodeEditorNode
---@field hours integer
---@field minutes integer

local CLOCK_GET_MINUTES_TO_TIME_OF_DAY_NODE_TYPE = "core.clock_get_minutes_to_time_of_day"

---@type LDNodeEditorNodeType
local clockGetMinutesToTimeOfDayNode = {
    typeName = CLOCK_GET_MINUTES_TO_TIME_OF_DAY_NODE_TYPE,
    category = "opcode.clock",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorClockGetMinutesToTimeOfDayNode
        local newNode = {
            id = newNodeId,
            nodeType = CLOCK_GET_MINUTES_TO_TIME_OF_DAY_NODE_TYPE,
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
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }

            },
			hours = 0,
			minutes = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorClockGetMinutesToTimeOfDayNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "hours", function ()
            node.hours = editor.dataTypes[node.inputs[2].type].drawEditValue(node.hours, "##hoursEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "minutes", function ()
            node.minutes = editor.dataTypes[node.inputs[3].type].drawEditValue(node.minutes, "##minutesEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "minutesLeft");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorClockGetMinutesToTimeOfDayNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local hours = inputValues[2] or node.hours
		local minutes = inputValues[3] or node.minutes
		local minutesLeft = ClockOp.getMinutesToTimeOfDay(hours, minutes)
        return {1, minutesLeft}
    end
}

return clockGetMinutesToTimeOfDayNode