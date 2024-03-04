local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorGameSetGunshotSenseRangeForRiot2Node : LDNodeEditorNode
---@field range number

local GAME_SET_GUNSHOT_SENSE_RANGE_FOR_RIOT2_NODE_TYPE = "core.game_set_gunshot_sense_range_for_riot2"

---@type LDNodeEditorNodeType
local gameSetGunshotSenseRangeForRiot2Node = {
    typeName = GAME_SET_GUNSHOT_SENSE_RANGE_FOR_RIOT2_NODE_TYPE,
    category = "opcode.game",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorGameSetGunshotSenseRangeForRiot2Node
        local newNode = {
            id = newNodeId,
            nodeType = GAME_SET_GUNSHOT_SENSE_RANGE_FOR_RIOT2_NODE_TYPE,
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
			range = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorGameSetGunshotSenseRangeForRiot2Node
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "range", function ()
            node.range = editor.dataTypes[node.inputs[2].type].drawEditValue(node.range, "##rangeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorGameSetGunshotSenseRangeForRiot2Node
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local range = inputValues[2] or node.range
		GameOp.setGunshotSenseRangeForRiot2(range)
        return {1}
    end
}

return gameSetGunshotSenseRangeForRiot2Node
