local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorPlayerGetWheelieStatsNode : LDNodeEditorNode
---@field self_ integer

local PLAYER_GET_WHEELIE_STATS_NODE_TYPE = "core.player_get_wheelie_stats"

---@type LDNodeEditorNodeType
local playerGetWheelieStatsNode = {
    typeName = PLAYER_GET_WHEELIE_STATS_NODE_TYPE,
    category = "opcode.player",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorPlayerGetWheelieStatsNode
        local newNode = {
            id = newNodeId,
            nodeType = PLAYER_GET_WHEELIE_STATS_NODE_TYPE,
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
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }

            },
			self_ = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorPlayerGetWheelieStatsNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[2].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "twoWheelsTime");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[3], "twoWheelsDistance");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[4], "wheelieTime");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[5], "wheelieDistance");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[6], "stoppieTime");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[7], "stoppieDistance");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorPlayerGetWheelieStatsNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local self_ = inputValues[2] or node.self_
		local twoWheelsTime, twoWheelsDistance, wheelieTime, wheelieDistance, stoppieTime, stoppieDistance = PlayerOp.getWheelieStats(self_)
        return {1, twoWheelsTime, twoWheelsDistance, wheelieTime, wheelieDistance, stoppieTime, stoppieDistance}
    end
}

return playerGetWheelieStatsNode
