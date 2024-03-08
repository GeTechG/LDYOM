local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorGameSetScriptLimitToGangSizeNode : LDNodeEditorNode
---@field maxSize integer

local GAME_SET_SCRIPT_LIMIT_TO_GANG_SIZE_NODE_TYPE = "core.game_set_script_limit_to_gang_size"

---@type LDNodeEditorNodeType
local gameSetScriptLimitToGangSizeNode = {
    typeName = GAME_SET_SCRIPT_LIMIT_TO_GANG_SIZE_NODE_TYPE,
    category = "opcode.game",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorGameSetScriptLimitToGangSizeNode
        local newNode = {
            id = newNodeId,
            nodeType = GAME_SET_SCRIPT_LIMIT_TO_GANG_SIZE_NODE_TYPE,
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
			maxSize = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorGameSetScriptLimitToGangSizeNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "maxSize", function ()
            node.maxSize = editor.dataTypes[node.inputs[2].type].drawEditValue(node.maxSize, "##maxSizeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorGameSetScriptLimitToGangSizeNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local maxSize = inputValues[2] or node.maxSize
		GameOp.setScriptLimitToGangSize(maxSize)
        return {1}
    end
}

return gameSetScriptLimitToGangSizeNode
