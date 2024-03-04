local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorGameHasGameJustReturnedFromFrontendNode : LDNodeEditorNode


local GAME_HAS_GAME_JUST_RETURNED_FROM_FRONTEND_NODE_TYPE = "core.game_has_game_just_returned_from_frontend"

---@type LDNodeEditorNodeType
local gameHasGameJustReturnedFromFrontendNode = {
    typeName = GAME_HAS_GAME_JUST_RETURNED_FROM_FRONTEND_NODE_TYPE,
    category = "opcode.game",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorGameHasGameJustReturnedFromFrontendNode
        local newNode = {
            id = newNodeId,
            nodeType = GAME_HAS_GAME_JUST_RETURNED_FROM_FRONTEND_NODE_TYPE,
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
                    type = "core.bool",
                }

            }
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorGameHasGameJustReturnedFromFrontendNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorGameHasGameJustReturnedFromFrontendNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local result = GameOp.hasGameJustReturnedFromFrontend()
        return {1, result}
    end
}

return gameHasGameJustReturnedFromFrontendNode
