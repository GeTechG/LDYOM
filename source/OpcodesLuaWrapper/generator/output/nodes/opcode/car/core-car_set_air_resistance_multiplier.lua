local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCarSetAirResistanceMultiplierNode : LDNodeEditorNode
---@field self_ integer
---@field multiplier number

local CAR_SET_AIR_RESISTANCE_MULTIPLIER_NODE_TYPE = "core.car_set_air_resistance_multiplier"

---@type LDNodeEditorNodeType
local carSetAirResistanceMultiplierNode = {
    typeName = CAR_SET_AIR_RESISTANCE_MULTIPLIER_NODE_TYPE,
    category = "opcode.car",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCarSetAirResistanceMultiplierNode
        local newNode = {
            id = newNodeId,
            nodeType = CAR_SET_AIR_RESISTANCE_MULTIPLIER_NODE_TYPE,
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
                    type = "core.Vehicle",
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
			self_ = 0,
			multiplier = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCarSetAirResistanceMultiplierNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[2].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "multiplier", function ()
            node.multiplier = editor.dataTypes[node.inputs[3].type].drawEditValue(node.multiplier, "##multiplierEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorCarSetAirResistanceMultiplierNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local self_ = inputValues[2] or node.self_
		local multiplier = inputValues[3] or node.multiplier
		CarOp.setAirResistanceMultiplier(self_, multiplier)
        return {1}
    end
}

return carSetAirResistanceMultiplierNode