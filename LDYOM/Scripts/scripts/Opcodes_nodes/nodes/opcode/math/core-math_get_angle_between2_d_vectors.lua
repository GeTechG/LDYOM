local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorMathGetAngleBetween2DVectorsNode : LDNodeEditorNode
---@field x1 number
---@field y1 number
---@field x2 number
---@field y2 number

local MATH_GET_ANGLE_BETWEEN2_D_VECTORS_NODE_TYPE = "core.math_get_angle_between2_d_vectors"

---@type LDNodeEditorNodeType
local mathGetAngleBetween2DVectorsNode = {
    typeName = MATH_GET_ANGLE_BETWEEN2_D_VECTORS_NODE_TYPE,
    category = "opcode.math",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorMathGetAngleBetween2DVectorsNode
        local newNode = {
            id = newNodeId,
            nodeType = MATH_GET_ANGLE_BETWEEN2_D_VECTORS_NODE_TYPE,
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
			x1 = 0,
			y1 = 0,
			x2 = 0,
			y2 = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorMathGetAngleBetween2DVectorsNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "x1", function ()
            node.x1 = editor.dataTypes[node.inputs[2].type].drawEditValue(node.x1, "##x1Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "y1", function ()
            node.y1 = editor.dataTypes[node.inputs[3].type].drawEditValue(node.y1, "##y1Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "x2", function ()
            node.x2 = editor.dataTypes[node.inputs[4].type].drawEditValue(node.x2, "##x2Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "y2", function ()
            node.y2 = editor.dataTypes[node.inputs[5].type].drawEditValue(node.y2, "##y2Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "angle");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorMathGetAngleBetween2DVectorsNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local x1 = inputValues[2] or node.x1
		local y1 = inputValues[3] or node.y1
		local x2 = inputValues[4] or node.x2
		local y2 = inputValues[5] or node.y2
		local angle = MathOp.getAngleBetween2DVectors(x1, y1, x2, y2)
        return {1, angle}
    end
}

return mathGetAngleBetween2DVectorsNode
