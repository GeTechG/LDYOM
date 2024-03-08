local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCarApplyForceNode : LDNodeEditorNode
---@field self_ integer
---@field xOffset number
---@field yOffset number
---@field zOffset number
---@field xRotation number
---@field yRotation number
---@field zRotation number

local CAR_APPLY_FORCE_NODE_TYPE = "core.car_apply_force"

---@type LDNodeEditorNodeType
local carApplyForceNode = {
    typeName = CAR_APPLY_FORCE_NODE_TYPE,
    category = "opcode.car",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCarApplyForceNode
        local newNode = {
            id = newNodeId,
            nodeType = CAR_APPLY_FORCE_NODE_TYPE,
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
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.number",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.number",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.number",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.number",
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
			xOffset = 0,
			yOffset = 0,
			zOffset = 0,
			xRotation = 0,
			yRotation = 0,
			zRotation = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCarApplyForceNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[2].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "xOffset", function ()
            node.xOffset = editor.dataTypes[node.inputs[3].type].drawEditValue(node.xOffset, "##xOffsetEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "yOffset", function ()
            node.yOffset = editor.dataTypes[node.inputs[4].type].drawEditValue(node.yOffset, "##yOffsetEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "zOffset", function ()
            node.zOffset = editor.dataTypes[node.inputs[5].type].drawEditValue(node.zOffset, "##zOffsetEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "xRotation", function ()
            node.xRotation = editor.dataTypes[node.inputs[6].type].drawEditValue(node.xRotation, "##xRotationEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "yRotation", function ()
            node.yRotation = editor.dataTypes[node.inputs[7].type].drawEditValue(node.yRotation, "##yRotationEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "zRotation", function ()
            node.zRotation = editor.dataTypes[node.inputs[8].type].drawEditValue(node.zRotation, "##zRotationEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCarApplyForceNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local self_ = inputValues[2] or node.self_
		local xOffset = inputValues[3] or node.xOffset
		local yOffset = inputValues[4] or node.yOffset
		local zOffset = inputValues[5] or node.zOffset
		local xRotation = inputValues[6] or node.xRotation
		local yRotation = inputValues[7] or node.yRotation
		local zRotation = inputValues[8] or node.zRotation
		CarOp.applyForce(self_, xOffset, yOffset, zOffset, xRotation, yRotation, zRotation)
        return {1}
    end
}

return carApplyForceNode
