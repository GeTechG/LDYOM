local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCarChangeColorNode : LDNodeEditorNode
---@field self_ integer
---@field primaryColor integer
---@field secondaryColor integer

local CAR_CHANGE_COLOR_NODE_TYPE = "core.car_change_color"

---@type LDNodeEditorNodeType
local carChangeColorNode = {
    typeName = CAR_CHANGE_COLOR_NODE_TYPE,
    category = "opcode.car",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCarChangeColorNode
        local newNode = {
            id = newNodeId,
            nodeType = CAR_CHANGE_COLOR_NODE_TYPE,
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
			primaryColor = 0,
			secondaryColor = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCarChangeColorNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[2].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "primaryColor", function ()
            node.primaryColor = editor.dataTypes[node.inputs[3].type].drawEditValue(node.primaryColor, "##primaryColorEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "secondaryColor", function ()
            node.secondaryColor = editor.dataTypes[node.inputs[4].type].drawEditValue(node.secondaryColor, "##secondaryColorEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCarChangeColorNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local self_ = inputValues[2] or node.self_
		if self_ == nil or self_ == 0 then
            error("Input self is not set")
        end
		local primaryColor = inputValues[3] or node.primaryColor
		local secondaryColor = inputValues[4] or node.secondaryColor
		CarOp.changeColor(self_, primaryColor, secondaryColor)
        return {1}
    end
}

return carChangeColorNode
