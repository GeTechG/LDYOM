local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorGameDoWeaponStuffAtStartOf2PGameNode : LDNodeEditorNode


local GAME_DO_WEAPON_STUFF_AT_START_OF2_P_GAME_NODE_TYPE = "core.game_do_weapon_stuff_at_start_of2_p_game"

---@type LDNodeEditorNodeType
local gameDoWeaponStuffAtStartOf2PGameNode = {
    typeName = GAME_DO_WEAPON_STUFF_AT_START_OF2_P_GAME_NODE_TYPE,
    category = "opcode.game",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorGameDoWeaponStuffAtStartOf2PGameNode
        local newNode = {
            id = newNodeId,
            nodeType = GAME_DO_WEAPON_STUFF_AT_START_OF2_P_GAME_NODE_TYPE,
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
    ---@param node LDNodeEditorGameDoWeaponStuffAtStartOf2PGameNode
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
    ---@param node LDNodeEditorGameDoWeaponStuffAtStartOf2PGameNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        GameOp.doWeaponStuffAtStartOf2PGame()
        return {1}
    end
}

return gameDoWeaponStuffAtStartOf2PGameNode
