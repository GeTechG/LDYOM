local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorGameIs2PlayerGameGoingOnNode : LDNodeEditorNode


local GAME_IS2_PLAYER_GAME_GOING_ON_NODE_TYPE = "core.game_is2_player_game_going_on"

---@type LDNodeEditorNodeType
local gameIs2PlayerGameGoingOnNode = {
    typeName = GAME_IS2_PLAYER_GAME_GOING_ON_NODE_TYPE,
    category = "opcode.game",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorGameIs2PlayerGameGoingOnNode
        local newNode = {
            id = newNodeId,
            nodeType = GAME_IS2_PLAYER_GAME_GOING_ON_NODE_TYPE,
            inputs = {
                
            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.bool",
                }

            }
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorGameIs2PlayerGameGoingOnNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorGameIs2PlayerGameGoingOnNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local result = GameOp.is2PlayerGameGoingOn()
        return {result}
    end
}

return gameIs2PlayerGameGoingOnNode
