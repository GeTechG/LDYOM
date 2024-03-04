local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorWorldIsLineOfSightClearNode : LDNodeEditorNode
---@field fromX number
---@field fromY number
---@field fromZ number
---@field toX number
---@field toY number
---@field toZ number
---@field buildings boolean
---@field cars boolean
---@field chars boolean
---@field objects boolean
---@field particles boolean

local WORLD_IS_LINE_OF_SIGHT_CLEAR_NODE_TYPE = "core.world_is_line_of_sight_clear"

---@type LDNodeEditorNodeType
local worldIsLineOfSightClearNode = {
    typeName = WORLD_IS_LINE_OF_SIGHT_CLEAR_NODE_TYPE,
    category = "opcode.world",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorWorldIsLineOfSightClearNode
        local newNode = {
            id = newNodeId,
            nodeType = WORLD_IS_LINE_OF_SIGHT_CLEAR_NODE_TYPE,
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
                    type = "core.bool",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.bool",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.bool",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.bool",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.bool",
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
                    type = "core.bool",
                }

            },
			fromX = 0,
			fromY = 0,
			fromZ = 0,
			toX = 0,
			toY = 0,
			toZ = 0,
			buildings = false,
			cars = false,
			chars = false,
			objects = false,
			particles = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorWorldIsLineOfSightClearNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "fromX", function ()
            node.fromX = editor.dataTypes[node.inputs[2].type].drawEditValue(node.fromX, "##fromXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "fromY", function ()
            node.fromY = editor.dataTypes[node.inputs[3].type].drawEditValue(node.fromY, "##fromYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "fromZ", function ()
            node.fromZ = editor.dataTypes[node.inputs[4].type].drawEditValue(node.fromZ, "##fromZEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "toX", function ()
            node.toX = editor.dataTypes[node.inputs[5].type].drawEditValue(node.toX, "##toXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "toY", function ()
            node.toY = editor.dataTypes[node.inputs[6].type].drawEditValue(node.toY, "##toYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "toZ", function ()
            node.toZ = editor.dataTypes[node.inputs[7].type].drawEditValue(node.toZ, "##toZEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "buildings", function ()
            node.buildings = editor.dataTypes[node.inputs[8].type].drawEditValue(node.buildings, "##buildingsEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[9], "cars", function ()
            node.cars = editor.dataTypes[node.inputs[9].type].drawEditValue(node.cars, "##carsEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[10], "chars", function ()
            node.chars = editor.dataTypes[node.inputs[10].type].drawEditValue(node.chars, "##charsEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[11], "objects", function ()
            node.objects = editor.dataTypes[node.inputs[11].type].drawEditValue(node.objects, "##objectsEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[12], "particles", function ()
            node.particles = editor.dataTypes[node.inputs[12].type].drawEditValue(node.particles, "##particlesEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorWorldIsLineOfSightClearNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local fromX = inputValues[2] or node.fromX
		local fromY = inputValues[3] or node.fromY
		local fromZ = inputValues[4] or node.fromZ
		local toX = inputValues[5] or node.toX
		local toY = inputValues[6] or node.toY
		local toZ = inputValues[7] or node.toZ
		local buildings = inputValues[8] or node.buildings
		local cars = inputValues[9] or node.cars
		local chars = inputValues[10] or node.chars
		local objects = inputValues[11] or node.objects
		local particles = inputValues[12] or node.particles
		local result = WorldOp.isLineOfSightClear(fromX, fromY, fromZ, toX, toY, toZ, buildings, cars, chars, objects, particles)
        return {1, result}
    end
}

return worldIsLineOfSightClearNode
