local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorGameFindMaxNumberOfGroupMembersNode : LDNodeEditorNode


local GAME_FIND_MAX_NUMBER_OF_GROUP_MEMBERS_NODE_TYPE = "core.game_find_max_number_of_group_members"

---@type LDNodeEditorNodeType
local gameFindMaxNumberOfGroupMembersNode = {
    typeName = GAME_FIND_MAX_NUMBER_OF_GROUP_MEMBERS_NODE_TYPE,
    category = "opcode.game",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorGameFindMaxNumberOfGroupMembersNode
        local newNode = {
            id = newNodeId,
            nodeType = GAME_FIND_MAX_NUMBER_OF_GROUP_MEMBERS_NODE_TYPE,
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
    ---@param node LDNodeEditorGameFindMaxNumberOfGroupMembersNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "maxNum");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorGameFindMaxNumberOfGroupMembersNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local maxNum = GameOp.findMaxNumberOfGroupMembers()
        return {1, maxNum}
    end
}

return gameFindMaxNumberOfGroupMembersNode
