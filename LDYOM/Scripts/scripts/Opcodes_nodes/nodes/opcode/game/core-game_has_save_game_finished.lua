local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorGameHasSaveGameFinishedNode : LDNodeEditorNode


local GAME_HAS_SAVE_GAME_FINISHED_NODE_TYPE = "core.game_has_save_game_finished"

---@type LDNodeEditorNodeType
local gameHasSaveGameFinishedNode = {
    typeName = GAME_HAS_SAVE_GAME_FINISHED_NODE_TYPE,
    category = "opcode.game",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorGameHasSaveGameFinishedNode
        local newNode = {
            id = newNodeId,
            nodeType = GAME_HAS_SAVE_GAME_FINISHED_NODE_TYPE,
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
    ---@param node LDNodeEditorGameHasSaveGameFinishedNode
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
    ---@param node LDNodeEditorGameHasSaveGameFinishedNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local result = GameOp.hasSaveGameFinished()
        return {result}
    end
}

return gameHasSaveGameFinishedNode
