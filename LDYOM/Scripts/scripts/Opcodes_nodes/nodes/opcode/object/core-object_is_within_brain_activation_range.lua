local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorObjectIsWithinBrainActivationRangeNode : LDNodeEditorNode
---@field self_ integer

local OBJECT_IS_WITHIN_BRAIN_ACTIVATION_RANGE_NODE_TYPE = "core.object_is_within_brain_activation_range"

---@type LDNodeEditorNodeType
local objectIsWithinBrainActivationRangeNode = {
    typeName = OBJECT_IS_WITHIN_BRAIN_ACTIVATION_RANGE_NODE_TYPE,
    category = "opcode.object",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorObjectIsWithinBrainActivationRangeNode
        local newNode = {
            id = newNodeId,
            nodeType = OBJECT_IS_WITHIN_BRAIN_ACTIVATION_RANGE_NODE_TYPE,
            inputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.Object",
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
    ---@param node LDNodeEditorObjectIsWithinBrainActivationRangeNode
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
    ---@param node LDNodeEditorObjectIsWithinBrainActivationRangeNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local self_ = inputValues[1] or node.self_
		local result = ObjectOp.isWithinBrainActivationRange(self_)
        return {result}
    end
}

return objectIsWithinBrainActivationRangeNode