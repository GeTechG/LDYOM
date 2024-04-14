local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCarDoesHaveStuckCarCheckNode : LDNodeEditorNode
---@field self_ integer

local CAR_DOES_HAVE_STUCK_CAR_CHECK_NODE_TYPE = "core.car_does_have_stuck_car_check"

---@type LDNodeEditorNodeType
local carDoesHaveStuckCarCheckNode = {
    typeName = CAR_DOES_HAVE_STUCK_CAR_CHECK_NODE_TYPE,
    category = "opcode.car",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCarDoesHaveStuckCarCheckNode
        local newNode = {
            id = newNodeId,
            nodeType = CAR_DOES_HAVE_STUCK_CAR_CHECK_NODE_TYPE,
            inputs = {
                {
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
			self_ = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCarDoesHaveStuckCarCheckNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[1].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCarDoesHaveStuckCarCheckNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local self_ = inputValues[1] or node.self_
		if self_ == nil or self_ == 0 then
            error("Input self is not set")
        end
		local result = CarOp.doesHaveStuckCarCheck(self_)
        return {result}
    end
}

return carDoesHaveStuckCarCheckNode
