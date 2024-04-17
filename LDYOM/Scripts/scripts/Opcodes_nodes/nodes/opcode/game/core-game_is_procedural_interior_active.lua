local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorGameIsProceduralInteriorActiveNode : LDNodeEditorNode
---@field interiorId integer

local GAME_IS_PROCEDURAL_INTERIOR_ACTIVE_NODE_TYPE = "core.game_is_procedural_interior_active"

---@type LDNodeEditorNodeType
local gameIsProceduralInteriorActiveNode = {
    typeName = GAME_IS_PROCEDURAL_INTERIOR_ACTIVE_NODE_TYPE,
    category = "opcode.game",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorGameIsProceduralInteriorActiveNode
        local newNode = {
            id = newNodeId,
            nodeType = GAME_IS_PROCEDURAL_INTERIOR_ACTIVE_NODE_TYPE,
            inputs = {
                {
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
                    type = "core.bool",
                }

            },
			interiorId = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorGameIsProceduralInteriorActiveNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "interiorId", function ()
            node.interiorId = editor.dataTypes[node.inputs[1].type].drawEditValue(node.interiorId, "##interiorIdEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorGameIsProceduralInteriorActiveNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local interiorId = inputValues[1] or node.interiorId
		local result = GameOp.isProceduralInteriorActive(math.floor(interiorId))
        return {result}
    end
}

return gameIsProceduralInteriorActiveNode
