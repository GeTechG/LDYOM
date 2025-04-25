local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCarCustomPlateForNextCarNode : LDNodeEditorNode
---@field modelId integer
---@field plateText string

local CAR_CUSTOM_PLATE_FOR_NEXT_CAR_NODE_TYPE = "core.car_custom_plate_for_next_car"

---@type LDNodeEditorNodeType
local carCustomPlateForNextCarNode = {
    typeName = CAR_CUSTOM_PLATE_FOR_NEXT_CAR_NODE_TYPE,
    category = "opcode.car",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCarCustomPlateForNextCarNode
        local newNode = {
            id = newNodeId,
            nodeType = CAR_CUSTOM_PLATE_FOR_NEXT_CAR_NODE_TYPE,
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
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.string",
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
			modelId = 0,
			plateText = ""
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCarCustomPlateForNextCarNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "modelId", function ()
            node.modelId = editor.dataTypes[node.inputs[2].type].drawEditValue(node.modelId, "##modelIdEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "plateText", function ()
            node.plateText = editor.dataTypes[node.inputs[3].type].drawEditValue(node.plateText, "##plateTextEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCarCustomPlateForNextCarNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local modelId = inputValues[2] or node.modelId
		local plateText = inputValues[3] or node.plateText
		CarOp.customPlateForNextCar(modelId, plateText)
        return {1}
    end
}

return carCustomPlateForNextCarNode
