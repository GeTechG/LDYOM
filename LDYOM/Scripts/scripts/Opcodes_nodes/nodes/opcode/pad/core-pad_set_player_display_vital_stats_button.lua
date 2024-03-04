local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorPadSetPlayerDisplayVitalStatsButtonNode : LDNodeEditorNode
---@field playerId integer
---@field state boolean

local PAD_SET_PLAYER_DISPLAY_VITAL_STATS_BUTTON_NODE_TYPE = "core.pad_set_player_display_vital_stats_button"

---@type LDNodeEditorNodeType
local padSetPlayerDisplayVitalStatsButtonNode = {
    typeName = PAD_SET_PLAYER_DISPLAY_VITAL_STATS_BUTTON_NODE_TYPE,
    category = "opcode.pad",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorPadSetPlayerDisplayVitalStatsButtonNode
        local newNode = {
            id = newNodeId,
            nodeType = PAD_SET_PLAYER_DISPLAY_VITAL_STATS_BUTTON_NODE_TYPE,
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
                    type = "core.bool",
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
			playerId = 0,
			state = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorPadSetPlayerDisplayVitalStatsButtonNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "playerId", function ()
            node.playerId = editor.dataTypes[node.inputs[2].type].drawEditValue(node.playerId, "##playerIdEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "state", function ()
            node.state = editor.dataTypes[node.inputs[3].type].drawEditValue(node.state, "##stateEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorPadSetPlayerDisplayVitalStatsButtonNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local playerId = inputValues[2] or node.playerId
		local state = inputValues[3] or node.state
		PadOp.setPlayerDisplayVitalStatsButton(playerId, state)
        return {1}
    end
}

return padSetPlayerDisplayVitalStatsButtonNode
