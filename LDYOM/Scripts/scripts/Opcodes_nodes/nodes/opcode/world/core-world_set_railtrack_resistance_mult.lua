local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorWorldSetRailtrackResistanceMultNode : LDNodeEditorNode
---@field mult number

local WORLD_SET_RAILTRACK_RESISTANCE_MULT_NODE_TYPE = "core.world_set_railtrack_resistance_mult"

---@type LDNodeEditorNodeType
local worldSetRailtrackResistanceMultNode = {
    typeName = WORLD_SET_RAILTRACK_RESISTANCE_MULT_NODE_TYPE,
    category = "opcode.world",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorWorldSetRailtrackResistanceMultNode
        local newNode = {
            id = newNodeId,
            nodeType = WORLD_SET_RAILTRACK_RESISTANCE_MULT_NODE_TYPE,
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
			mult = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorWorldSetRailtrackResistanceMultNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "mult", function ()
            node.mult = editor.dataTypes[node.inputs[2].type].drawEditValue(node.mult, "##multEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorWorldSetRailtrackResistanceMultNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local mult = inputValues[2] or node.mult
		WorldOp.setRailtrackResistanceMult(mult)
        return {1}
    end
}

return worldSetRailtrackResistanceMultNode
