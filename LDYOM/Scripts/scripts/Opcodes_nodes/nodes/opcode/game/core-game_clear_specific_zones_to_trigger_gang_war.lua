local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorGameClearSpecificZonesToTriggerGangWarNode : LDNodeEditorNode


local GAME_CLEAR_SPECIFIC_ZONES_TO_TRIGGER_GANG_WAR_NODE_TYPE = "core.game_clear_specific_zones_to_trigger_gang_war"

---@type LDNodeEditorNodeType
local gameClearSpecificZonesToTriggerGangWarNode = {
    typeName = GAME_CLEAR_SPECIFIC_ZONES_TO_TRIGGER_GANG_WAR_NODE_TYPE,
    category = "opcode.game",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorGameClearSpecificZonesToTriggerGangWarNode
        local newNode = {
            id = newNodeId,
            nodeType = GAME_CLEAR_SPECIFIC_ZONES_TO_TRIGGER_GANG_WAR_NODE_TYPE,
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
    ---@param node LDNodeEditorGameClearSpecificZonesToTriggerGangWarNode
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
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorGameClearSpecificZonesToTriggerGangWarNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        GameOp.clearSpecificZonesToTriggerGangWar()
        return {1}
    end
}

return gameClearSpecificZonesToTriggerGangWarNode
