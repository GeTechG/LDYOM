local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorGameGetCurrentLanguageNode : LDNodeEditorNode


local GAME_GET_CURRENT_LANGUAGE_NODE_TYPE = "core.game_get_current_language"

---@type LDNodeEditorNodeType
local gameGetCurrentLanguageNode = {
    typeName = GAME_GET_CURRENT_LANGUAGE_NODE_TYPE,
    category = "opcode.game",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorGameGetCurrentLanguageNode
        local newNode = {
            id = newNodeId,
            nodeType = GAME_GET_CURRENT_LANGUAGE_NODE_TYPE,
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
    ---@param node LDNodeEditorGameGetCurrentLanguageNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "languageSlot");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorGameGetCurrentLanguageNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local languageSlot = GameOp.getCurrentLanguage()
        return {1, languageSlot}
    end
}

return gameGetCurrentLanguageNode