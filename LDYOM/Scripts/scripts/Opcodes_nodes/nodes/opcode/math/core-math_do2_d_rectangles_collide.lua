local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorMathDo2DRectanglesCollideNode : LDNodeEditorNode
---@field rectangle1PositionX number
---@field rectangle1PositionY number
---@field rectangle1SizeX number
---@field rectangle1SizeY number
---@field rectangle2PositionX number
---@field rectangle2PositionY number
---@field rectangle2SizeX number
---@field rectangle2SizeY number

local MATH_DO2_D_RECTANGLES_COLLIDE_NODE_TYPE = "core.math_do2_d_rectangles_collide"

---@type LDNodeEditorNodeType
local mathDo2DRectanglesCollideNode = {
    typeName = MATH_DO2_D_RECTANGLES_COLLIDE_NODE_TYPE,
    category = "opcode.math",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorMathDo2DRectanglesCollideNode
        local newNode = {
            id = newNodeId,
            nodeType = MATH_DO2_D_RECTANGLES_COLLIDE_NODE_TYPE,
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
                    type = "core.bool",
                }

            },
			rectangle1PositionX = 0,
			rectangle1PositionY = 0,
			rectangle1SizeX = 0,
			rectangle1SizeY = 0,
			rectangle2PositionX = 0,
			rectangle2PositionY = 0,
			rectangle2SizeX = 0,
			rectangle2SizeY = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorMathDo2DRectanglesCollideNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "rectangle1PositionX", function ()
            node.rectangle1PositionX = editor.dataTypes[node.inputs[1].type].drawEditValue(node.rectangle1PositionX, "##rectangle1PositionXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "rectangle1PositionY", function ()
            node.rectangle1PositionY = editor.dataTypes[node.inputs[2].type].drawEditValue(node.rectangle1PositionY, "##rectangle1PositionYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "rectangle1SizeX", function ()
            node.rectangle1SizeX = editor.dataTypes[node.inputs[3].type].drawEditValue(node.rectangle1SizeX, "##rectangle1SizeXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "rectangle1SizeY", function ()
            node.rectangle1SizeY = editor.dataTypes[node.inputs[4].type].drawEditValue(node.rectangle1SizeY, "##rectangle1SizeYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "rectangle2PositionX", function ()
            node.rectangle2PositionX = editor.dataTypes[node.inputs[5].type].drawEditValue(node.rectangle2PositionX, "##rectangle2PositionXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "rectangle2PositionY", function ()
            node.rectangle2PositionY = editor.dataTypes[node.inputs[6].type].drawEditValue(node.rectangle2PositionY, "##rectangle2PositionYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "rectangle2SizeX", function ()
            node.rectangle2SizeX = editor.dataTypes[node.inputs[7].type].drawEditValue(node.rectangle2SizeX, "##rectangle2SizeXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "rectangle2SizeY", function ()
            node.rectangle2SizeY = editor.dataTypes[node.inputs[8].type].drawEditValue(node.rectangle2SizeY, "##rectangle2SizeYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorMathDo2DRectanglesCollideNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local rectangle1PositionX = inputValues[1] or node.rectangle1PositionX
		local rectangle1PositionY = inputValues[2] or node.rectangle1PositionY
		local rectangle1SizeX = inputValues[3] or node.rectangle1SizeX
		local rectangle1SizeY = inputValues[4] or node.rectangle1SizeY
		local rectangle2PositionX = inputValues[5] or node.rectangle2PositionX
		local rectangle2PositionY = inputValues[6] or node.rectangle2PositionY
		local rectangle2SizeX = inputValues[7] or node.rectangle2SizeX
		local rectangle2SizeY = inputValues[8] or node.rectangle2SizeY
		local result = MathOp.do2DRectanglesCollide(rectangle1PositionX, rectangle1PositionY, rectangle1SizeX, rectangle1SizeY, rectangle2PositionX, rectangle2PositionY, rectangle2SizeX, rectangle2SizeY)
        return {result}
    end
}

return mathDo2DRectanglesCollideNode
