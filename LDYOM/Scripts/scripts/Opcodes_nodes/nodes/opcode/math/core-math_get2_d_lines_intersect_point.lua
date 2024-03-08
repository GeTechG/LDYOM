local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorMathGet2DLinesIntersectPointNode : LDNodeEditorNode
---@field line1StartX number
---@field line1StartY number
---@field line1EndX number
---@field line1EndY number
---@field line2StartX number
---@field line2StartY number
---@field line2EndX number
---@field line2EndY number

local MATH_GET2_D_LINES_INTERSECT_POINT_NODE_TYPE = "core.math_get2_d_lines_intersect_point"

---@type LDNodeEditorNodeType
local mathGet2DLinesIntersectPointNode = {
    typeName = MATH_GET2_D_LINES_INTERSECT_POINT_NODE_TYPE,
    category = "opcode.math",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorMathGet2DLinesIntersectPointNode
        local newNode = {
            id = newNodeId,
            nodeType = MATH_GET2_D_LINES_INTERSECT_POINT_NODE_TYPE,
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
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }

            },
			line1StartX = 0,
			line1StartY = 0,
			line1EndX = 0,
			line1EndY = 0,
			line2StartX = 0,
			line2StartY = 0,
			line2EndX = 0,
			line2EndY = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorMathGet2DLinesIntersectPointNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "line1StartX", function ()
            node.line1StartX = editor.dataTypes[node.inputs[2].type].drawEditValue(node.line1StartX, "##line1StartXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "line1StartY", function ()
            node.line1StartY = editor.dataTypes[node.inputs[3].type].drawEditValue(node.line1StartY, "##line1StartYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "line1EndX", function ()
            node.line1EndX = editor.dataTypes[node.inputs[4].type].drawEditValue(node.line1EndX, "##line1EndXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "line1EndY", function ()
            node.line1EndY = editor.dataTypes[node.inputs[5].type].drawEditValue(node.line1EndY, "##line1EndYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "line2StartX", function ()
            node.line2StartX = editor.dataTypes[node.inputs[6].type].drawEditValue(node.line2StartX, "##line2StartXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "line2StartY", function ()
            node.line2StartY = editor.dataTypes[node.inputs[7].type].drawEditValue(node.line2StartY, "##line2StartYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "line2EndX", function ()
            node.line2EndX = editor.dataTypes[node.inputs[8].type].drawEditValue(node.line2EndX, "##line2EndXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[9], "line2EndY", function ()
            node.line2EndY = editor.dataTypes[node.inputs[9].type].drawEditValue(node.line2EndY, "##line2EndYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[3], "intersectPointX");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[4], "intersectPointY");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorMathGet2DLinesIntersectPointNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local line1StartX = inputValues[2] or node.line1StartX
		local line1StartY = inputValues[3] or node.line1StartY
		local line1EndX = inputValues[4] or node.line1EndX
		local line1EndY = inputValues[5] or node.line1EndY
		local line2StartX = inputValues[6] or node.line2StartX
		local line2StartY = inputValues[7] or node.line2StartY
		local line2EndX = inputValues[8] or node.line2EndX
		local line2EndY = inputValues[9] or node.line2EndY
		local result, intersectPointX, intersectPointY = MathOp.get2DLinesIntersectPoint(line1StartX, line1StartY, line1EndX, line1EndY, line2StartX, line2StartY, line2EndX, line2EndY)
        return {1, result, intersectPointX, intersectPointY}
    end
}

return mathGet2DLinesIntersectPointNode
