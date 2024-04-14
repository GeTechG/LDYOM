local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorParticleCreateOnCarWithDirectionNode : LDNodeEditorNode
---@field name string
---@field vehicle integer
---@field xOffset number
---@field yOffset number
---@field zOffset number
---@field xDirection number
---@field yDirection number
---@field zDirection number
---@field type integer

local PARTICLE_CREATE_ON_CAR_WITH_DIRECTION_NODE_TYPE = "core.particle_create_on_car_with_direction"

---@type LDNodeEditorNodeType
local particleCreateOnCarWithDirectionNode = {
    typeName = PARTICLE_CREATE_ON_CAR_WITH_DIRECTION_NODE_TYPE,
    category = "opcode.particle",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorParticleCreateOnCarWithDirectionNode
        local newNode = {
            id = newNodeId,
            nodeType = PARTICLE_CREATE_ON_CAR_WITH_DIRECTION_NODE_TYPE,
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
                    type = "core.string",
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
                    type = "core.Particle",
                }

            },
			name = "",
			vehicle = 0,
			xOffset = 0,
			yOffset = 0,
			zOffset = 0,
			xDirection = 0,
			yDirection = 0,
			zDirection = 0,
			type = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorParticleCreateOnCarWithDirectionNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "name", function ()
            node.name = editor.dataTypes[node.inputs[2].type].drawEditValue(node.name, "##nameEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "vehicle", function ()
            node.vehicle = editor.dataTypes[node.inputs[3].type].drawEditValue(node.vehicle, "##vehicleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "xOffset", function ()
            node.xOffset = editor.dataTypes[node.inputs[4].type].drawEditValue(node.xOffset, "##xOffsetEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "yOffset", function ()
            node.yOffset = editor.dataTypes[node.inputs[5].type].drawEditValue(node.yOffset, "##yOffsetEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "zOffset", function ()
            node.zOffset = editor.dataTypes[node.inputs[6].type].drawEditValue(node.zOffset, "##zOffsetEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "xDirection", function ()
            node.xDirection = editor.dataTypes[node.inputs[7].type].drawEditValue(node.xDirection, "##xDirectionEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "yDirection", function ()
            node.yDirection = editor.dataTypes[node.inputs[8].type].drawEditValue(node.yDirection, "##yDirectionEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[9], "zDirection", function ()
            node.zDirection = editor.dataTypes[node.inputs[9].type].drawEditValue(node.zDirection, "##zDirectionEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[10], "type", function ()
            node.type = editor.dataTypes[node.inputs[10].type].drawEditValue(node.type, "##typeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "handle");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorParticleCreateOnCarWithDirectionNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local name = inputValues[2] or node.name
		local vehicle = inputValues[3] or node.vehicle
		if vehicle == nil or vehicle == 0 then
            error("Input vehicle is not set")
        end
		local xOffset = inputValues[4] or node.xOffset
		local yOffset = inputValues[5] or node.yOffset
		local zOffset = inputValues[6] or node.zOffset
		local xDirection = inputValues[7] or node.xDirection
		local yDirection = inputValues[8] or node.yDirection
		local zDirection = inputValues[9] or node.zDirection
		local type = inputValues[10] or node.type
		local handle = ParticleOp.createOnCarWithDirection(name, vehicle, xOffset, yOffset, zOffset, xDirection, yDirection, zDirection, type)
        return {1, handle}
    end
}

return particleCreateOnCarWithDirectionNode
