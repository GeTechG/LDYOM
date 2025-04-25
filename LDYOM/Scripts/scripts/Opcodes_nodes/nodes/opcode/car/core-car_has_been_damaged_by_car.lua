local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCarHasBeenDamagedByCarNode : LDNodeEditorNode
---@field self_ integer
---@field other integer

local CAR_HAS_BEEN_DAMAGED_BY_CAR_NODE_TYPE = "core.car_has_been_damaged_by_car"

---@type LDNodeEditorNodeType
local carHasBeenDamagedByCarNode = {
    typeName = CAR_HAS_BEEN_DAMAGED_BY_CAR_NODE_TYPE,
    category = "opcode.car",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCarHasBeenDamagedByCarNode
        local newNode = {
            id = newNodeId,
            nodeType = CAR_HAS_BEEN_DAMAGED_BY_CAR_NODE_TYPE,
            inputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.Vehicle",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.Vehicle",
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
			self_ = 0,
			other = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCarHasBeenDamagedByCarNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[1].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "other", function ()
            node.other = editor.dataTypes[node.inputs[2].type].drawEditValue(node.other, "##otherEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCarHasBeenDamagedByCarNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local self_ = inputValues[1] or node.self_
		if self_ == nil or self_ == 0 then
            error("Input self is not set")
        end
		local other = inputValues[2] or node.other
		if other == nil or other == 0 then
            error("Input other is not set")
        end
		local result = CarOp.hasBeenDamagedByCar(self_, other)
        return {result}
    end
}

return carHasBeenDamagedByCarNode
