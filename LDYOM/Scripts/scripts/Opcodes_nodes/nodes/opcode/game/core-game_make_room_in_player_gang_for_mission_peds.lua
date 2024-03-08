local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorGameMakeRoomInPlayerGangForMissionPedsNode : LDNodeEditorNode
---@field _p1 integer

local GAME_MAKE_ROOM_IN_PLAYER_GANG_FOR_MISSION_PEDS_NODE_TYPE = "core.game_make_room_in_player_gang_for_mission_peds"

---@type LDNodeEditorNodeType
local gameMakeRoomInPlayerGangForMissionPedsNode = {
    typeName = GAME_MAKE_ROOM_IN_PLAYER_GANG_FOR_MISSION_PEDS_NODE_TYPE,
    category = "opcode.game",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorGameMakeRoomInPlayerGangForMissionPedsNode
        local newNode = {
            id = newNodeId,
            nodeType = GAME_MAKE_ROOM_IN_PLAYER_GANG_FOR_MISSION_PEDS_NODE_TYPE,
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

            },
			_p1 = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorGameMakeRoomInPlayerGangForMissionPedsNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "_p1", function ()
            node._p1 = editor.dataTypes[node.inputs[2].type].drawEditValue(node._p1, "##_p1Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorGameMakeRoomInPlayerGangForMissionPedsNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local _p1 = inputValues[2] or node._p1
		GameOp.makeRoomInPlayerGangForMissionPeds(_p1)
        return {1}
    end
}

return gameMakeRoomInPlayerGangForMissionPedsNode
