local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorMathGetDistanceBetweenCoords2DNode : LDNodeEditorNode
---@field fromX number
---@field fromY number
---@field toX number
---@field toZ number

local MATH_GET_DISTANCE_BETWEEN_COORDS2_D_NODE_TYPE = "core.math_get_distance_between_coords2_d"

---@type LDNodeEditorNodeType
local mathGetDistanceBetweenCoords2DNode = {
    typeName = MATH_GET_DISTANCE_BETWEEN_COORDS2_D_NODE_TYPE,
    category = "opcode.math",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorMathGetDistanceBetweenCoords2DNode
        local newNode = {
            id = newNodeId,
            nodeType = MATH_GET_DISTANCE_BETWEEN_COORDS2_D_NODE_TYPE,
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
                    type = "core.number",
                }

            },
			fromX = 0,
			fromY = 0,
			toX = 0,
			toZ = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorMathGetDistanceBetweenCoords2DNode
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "toX", function ()
            node.toX = editor.dataTypes[node.inputs[4].type].drawEditValue(node.toX, "##toXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "toZ", function ()
            node.toZ = editor.dataTypes[node.inputs[5].type].drawEditValue(node.toZ, "##toZEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "distance");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorMathGetDistanceBetweenCoords2DNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local fromX = inputValues[2] or node.fromX
		local fromY = inputValues[3] or node.fromY
		local toX = inputValues[4] or node.toX
		local toZ = inputValues[5] or node.toZ
		local distance = MathOp.getDistanceBetweenCoords2D(fromX, fromY, toX, toZ)
        return {1, distance}
    end
}

return mathGetDistanceBetweenCoords2DNode
