local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorWorldIsAreaOccupiedNode : LDNodeEditorNode
---@field leftBottomX number
---@field leftBottomY number
---@field leftBottomZ number
---@field rightTopX number
---@field rightTopY number
---@field rightTopZ number
---@field solid boolean
---@field car boolean
---@field char boolean
---@field object boolean
---@field particle boolean

local WORLD_IS_AREA_OCCUPIED_NODE_TYPE = "core.world_is_area_occupied"

---@type LDNodeEditorNodeType
local worldIsAreaOccupiedNode = {
    typeName = WORLD_IS_AREA_OCCUPIED_NODE_TYPE,
    category = "opcode.world",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorWorldIsAreaOccupiedNode
        local newNode = {
            id = newNodeId,
            nodeType = WORLD_IS_AREA_OCCUPIED_NODE_TYPE,
            inputs = {
                {
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
                    type = "core.bool",
                }

            },
			leftBottomX = 0,
			leftBottomY = 0,
			leftBottomZ = 0,
			rightTopX = 0,
			rightTopY = 0,
			rightTopZ = 0,
			solid = false,
			car = false,
			char = false,
			object = false,
			particle = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorWorldIsAreaOccupiedNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "leftBottomX", function ()
            node.leftBottomX = editor.dataTypes[node.inputs[1].type].drawEditValue(node.leftBottomX, "##leftBottomXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "leftBottomY", function ()
            node.leftBottomY = editor.dataTypes[node.inputs[2].type].drawEditValue(node.leftBottomY, "##leftBottomYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "leftBottomZ", function ()
            node.leftBottomZ = editor.dataTypes[node.inputs[3].type].drawEditValue(node.leftBottomZ, "##leftBottomZEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "rightTopX", function ()
            node.rightTopX = editor.dataTypes[node.inputs[4].type].drawEditValue(node.rightTopX, "##rightTopXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "rightTopY", function ()
            node.rightTopY = editor.dataTypes[node.inputs[5].type].drawEditValue(node.rightTopY, "##rightTopYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "rightTopZ", function ()
            node.rightTopZ = editor.dataTypes[node.inputs[6].type].drawEditValue(node.rightTopZ, "##rightTopZEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "solid", function ()
            node.solid = editor.dataTypes[node.inputs[7].type].drawEditValue(node.solid, "##solidEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "car", function ()
            node.car = editor.dataTypes[node.inputs[8].type].drawEditValue(node.car, "##carEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[9], "char", function ()
            node.char = editor.dataTypes[node.inputs[9].type].drawEditValue(node.char, "##charEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[10], "object", function ()
            node.object = editor.dataTypes[node.inputs[10].type].drawEditValue(node.object, "##objectEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[11], "particle", function ()
            node.particle = editor.dataTypes[node.inputs[11].type].drawEditValue(node.particle, "##particleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorWorldIsAreaOccupiedNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local leftBottomX = inputValues[1] or node.leftBottomX
		local leftBottomY = inputValues[2] or node.leftBottomY
		local leftBottomZ = inputValues[3] or node.leftBottomZ
		local rightTopX = inputValues[4] or node.rightTopX
		local rightTopY = inputValues[5] or node.rightTopY
		local rightTopZ = inputValues[6] or node.rightTopZ
		local solid = inputValues[7] or node.solid
		local car = inputValues[8] or node.car
		local char = inputValues[9] or node.char
		local object = inputValues[10] or node.object
		local particle = inputValues[11] or node.particle
		local result = WorldOp.isAreaOccupied(leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ, solid, car, char, object, particle)
        return {result}
    end
}

return worldIsAreaOccupiedNode
