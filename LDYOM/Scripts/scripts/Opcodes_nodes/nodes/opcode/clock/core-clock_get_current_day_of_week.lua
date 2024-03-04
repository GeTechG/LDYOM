local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorClockGetCurrentDayOfWeekNode : LDNodeEditorNode


local CLOCK_GET_CURRENT_DAY_OF_WEEK_NODE_TYPE = "core.clock_get_current_day_of_week"

---@type LDNodeEditorNodeType
local clockGetCurrentDayOfWeekNode = {
    typeName = CLOCK_GET_CURRENT_DAY_OF_WEEK_NODE_TYPE,
    category = "opcode.clock",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorClockGetCurrentDayOfWeekNode
        local newNode = {
            id = newNodeId,
            nodeType = CLOCK_GET_CURRENT_DAY_OF_WEEK_NODE_TYPE,
            inputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.flow",
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

            }
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorClockGetCurrentDayOfWeekNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "day");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorClockGetCurrentDayOfWeekNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local day = ClockOp.getCurrentDayOfWeek()
        return {1, day}
    end
}

return clockGetCurrentDayOfWeekNode
