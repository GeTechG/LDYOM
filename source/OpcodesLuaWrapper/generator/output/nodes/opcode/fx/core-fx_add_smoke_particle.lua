local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorFxAddSmokeParticleNode : LDNodeEditorNode
---@field x number
---@field y number
---@field z number
---@field velocityX number
---@field velocityY number
---@field velocityZ number
---@field red integer
---@field green integer
---@field blue integer
---@field alpha integer
---@field size number
---@field lastFactor number

local FX_ADD_SMOKE_PARTICLE_NODE_TYPE = "core.fx_add_smoke_particle"

---@type LDNodeEditorNodeType
local fxAddSmokeParticleNode = {
    typeName = FX_ADD_SMOKE_PARTICLE_NODE_TYPE,
    category = "opcode.fx",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorFxAddSmokeParticleNode
        local newNode = {
            id = newNodeId,
            nodeType = FX_ADD_SMOKE_PARTICLE_NODE_TYPE,
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
			x = 0,
			y = 0,
			z = 0,
			velocityX = 0,
			velocityY = 0,
			velocityZ = 0,
			red = 0,
			green = 0,
			blue = 0,
			alpha = 0,
			size = 0,
			lastFactor = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorFxAddSmokeParticleNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "x", function ()
            node.x = editor.dataTypes[node.inputs[2].type].drawEditValue(node.x, "##xEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "y", function ()
            node.y = editor.dataTypes[node.inputs[3].type].drawEditValue(node.y, "##yEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "z", function ()
            node.z = editor.dataTypes[node.inputs[4].type].drawEditValue(node.z, "##zEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "velocityX", function ()
            node.velocityX = editor.dataTypes[node.inputs[5].type].drawEditValue(node.velocityX, "##velocityXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "velocityY", function ()
            node.velocityY = editor.dataTypes[node.inputs[6].type].drawEditValue(node.velocityY, "##velocityYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "velocityZ", function ()
            node.velocityZ = editor.dataTypes[node.inputs[7].type].drawEditValue(node.velocityZ, "##velocityZEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "red", function ()
            node.red = editor.dataTypes[node.inputs[8].type].drawEditValue(node.red, "##redEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[9], "green", function ()
            node.green = editor.dataTypes[node.inputs[9].type].drawEditValue(node.green, "##greenEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[10], "blue", function ()
            node.blue = editor.dataTypes[node.inputs[10].type].drawEditValue(node.blue, "##blueEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[11], "alpha", function ()
            node.alpha = editor.dataTypes[node.inputs[11].type].drawEditValue(node.alpha, "##alphaEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[12], "size", function ()
            node.size = editor.dataTypes[node.inputs[12].type].drawEditValue(node.size, "##sizeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[13], "lastFactor", function ()
            node.lastFactor = editor.dataTypes[node.inputs[13].type].drawEditValue(node.lastFactor, "##lastFactorEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorFxAddSmokeParticleNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local x = inputValues[2] or node.x
		local y = inputValues[3] or node.y
		local z = inputValues[4] or node.z
		local velocityX = inputValues[5] or node.velocityX
		local velocityY = inputValues[6] or node.velocityY
		local velocityZ = inputValues[7] or node.velocityZ
		local red = inputValues[8] or node.red
		local green = inputValues[9] or node.green
		local blue = inputValues[10] or node.blue
		local alpha = inputValues[11] or node.alpha
		local size = inputValues[12] or node.size
		local lastFactor = inputValues[13] or node.lastFactor
		FxOp.addSmokeParticle(x, y, z, velocityX, velocityY, velocityZ, red, green, blue, alpha, size, lastFactor)
        return {1}
    end
}

return fxAddSmokeParticleNode
