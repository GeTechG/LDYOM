local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorAudioSetRadioToPlayersFavouriteStationNode : LDNodeEditorNode


local AUDIO_SET_RADIO_TO_PLAYERS_FAVOURITE_STATION_NODE_TYPE = "core.audio_set_radio_to_players_favourite_station"

---@type LDNodeEditorNodeType
local audioSetRadioToPlayersFavouriteStationNode = {
    typeName = AUDIO_SET_RADIO_TO_PLAYERS_FAVOURITE_STATION_NODE_TYPE,
    category = "opcode.audio",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorAudioSetRadioToPlayersFavouriteStationNode
        local newNode = {
            id = newNodeId,
            nodeType = AUDIO_SET_RADIO_TO_PLAYERS_FAVOURITE_STATION_NODE_TYPE,
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

            }
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorAudioSetRadioToPlayersFavouriteStationNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorAudioSetRadioToPlayersFavouriteStationNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        AudioOp.setRadioToPlayersFavouriteStation()
        return {1}
    end
}

return audioSetRadioToPlayersFavouriteStationNode
