local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorGameLimitTwoPlayerDistanceNode : LDNodeEditorNode
---@field distance number

local GAME_LIMIT_TWO_PLAYER_DISTANCE_NODE_TYPE = "core.game_limit_two_player_distance"

---@type LDNodeEditorNodeType
local gameLimitTwoPlayerDistanceNode = {
    typeName = GAME_LIMIT_TWO_PLAYER_DISTANCE_NODE_TYPE,
    category = "opcode.game",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorGameLimitTwoPlayerDistanceNode
        local newNode = {
            id = newNodeId,
            nodeType = GAME_LIMIT_TWO_PLAYER_DISTANCE_NODE_TYPE,
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
			distance = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorGameLimitTwoPlayerDistanceNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "distance", function ()
            node.distance = editor.dataTypes[node.inputs[2].type].drawEditValue(node.distance, "##distanceEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorGameLimitTwoPlayerDistanceNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local distance = inputValues[2] or node.distance
		GameOp.limitTwoPlayerDistance(distance)
        return {1}
    end
}

return gameLimitTwoPlayerDistanceNode
