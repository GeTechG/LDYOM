local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorWorldGetRandomCarOfTypeInAngledAreaNoSaveNode : LDNodeEditorNode
---@field leftBottomX number
---@field leftBottomY number
---@field rightTopX number
---@field rightTopY number
---@field angle number
---@field modelId integer

local WORLD_GET_RANDOM_CAR_OF_TYPE_IN_ANGLED_AREA_NO_SAVE_NODE_TYPE = "core.world_get_random_car_of_type_in_angled_area_no_save"

---@type LDNodeEditorNodeType
local worldGetRandomCarOfTypeInAngledAreaNoSaveNode = {
    typeName = WORLD_GET_RANDOM_CAR_OF_TYPE_IN_ANGLED_AREA_NO_SAVE_NODE_TYPE,
    category = "opcode.world",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorWorldGetRandomCarOfTypeInAngledAreaNoSaveNode
        local newNode = {
            id = newNodeId,
            nodeType = WORLD_GET_RANDOM_CAR_OF_TYPE_IN_ANGLED_AREA_NO_SAVE_NODE_TYPE,
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
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.Vehicle",
                }

            },
			leftBottomX = 0,
			leftBottomY = 0,
			rightTopX = 0,
			rightTopY = 0,
			angle = 0,
			modelId = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorWorldGetRandomCarOfTypeInAngledAreaNoSaveNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "leftBottomX", function ()
            node.leftBottomX = editor.dataTypes[node.inputs[2].type].drawEditValue(node.leftBottomX, "##leftBottomXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "leftBottomY", function ()
            node.leftBottomY = editor.dataTypes[node.inputs[3].type].drawEditValue(node.leftBottomY, "##leftBottomYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "rightTopX", function ()
            node.rightTopX = editor.dataTypes[node.inputs[4].type].drawEditValue(node.rightTopX, "##rightTopXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "rightTopY", function ()
            node.rightTopY = editor.dataTypes[node.inputs[5].type].drawEditValue(node.rightTopY, "##rightTopYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "angle", function ()
            node.angle = editor.dataTypes[node.inputs[6].type].drawEditValue(node.angle, "##angleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "modelId", function ()
            node.modelId = editor.dataTypes[node.inputs[7].type].drawEditValue(node.modelId, "##modelIdEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "vehicle");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorWorldGetRandomCarOfTypeInAngledAreaNoSaveNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local leftBottomX = inputValues[2] or node.leftBottomX
		local leftBottomY = inputValues[3] or node.leftBottomY
		local rightTopX = inputValues[4] or node.rightTopX
		local rightTopY = inputValues[5] or node.rightTopY
		local angle = inputValues[6] or node.angle
		local modelId = inputValues[7] or node.modelId
		local vehicle = WorldOp.getRandomCarOfTypeInAngledAreaNoSave(leftBottomX, leftBottomY, rightTopX, rightTopY, angle, modelId)
        return {1, vehicle}
    end
}

return worldGetRandomCarOfTypeInAngledAreaNoSaveNode
