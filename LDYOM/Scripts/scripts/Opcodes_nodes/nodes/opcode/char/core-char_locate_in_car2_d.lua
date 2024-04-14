local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCharLocateInCar2DNode : LDNodeEditorNode
---@field self_ integer
---@field x number
---@field y number
---@field xRadius number
---@field yRadius number
---@field drawSphere boolean

local CHAR_LOCATE_IN_CAR2_D_NODE_TYPE = "core.char_locate_in_car2_d"

---@type LDNodeEditorNodeType
local charLocateInCar2DNode = {
    typeName = CHAR_LOCATE_IN_CAR2_D_NODE_TYPE,
    category = "opcode.char",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCharLocateInCar2DNode
        local newNode = {
            id = newNodeId,
            nodeType = CHAR_LOCATE_IN_CAR2_D_NODE_TYPE,
            inputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.Char",
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

            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.bool",
                }

            },
			self_ = 0,
			x = 0,
			y = 0,
			xRadius = 0,
			yRadius = 0,
			drawSphere = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCharLocateInCar2DNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[1].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "x", function ()
            node.x = editor.dataTypes[node.inputs[2].type].drawEditValue(node.x, "##xEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "y", function ()
            node.y = editor.dataTypes[node.inputs[3].type].drawEditValue(node.y, "##yEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "xRadius", function ()
            node.xRadius = editor.dataTypes[node.inputs[4].type].drawEditValue(node.xRadius, "##xRadiusEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "yRadius", function ()
            node.yRadius = editor.dataTypes[node.inputs[5].type].drawEditValue(node.yRadius, "##yRadiusEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "drawSphere", function ()
            node.drawSphere = editor.dataTypes[node.inputs[6].type].drawEditValue(node.drawSphere, "##drawSphereEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCharLocateInCar2DNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local self_ = inputValues[1] or node.self_
		if self_ == nil or self_ == 0 then
            error("Input self is not set")
        end
		local x = inputValues[2] or node.x
		local y = inputValues[3] or node.y
		local xRadius = inputValues[4] or node.xRadius
		local yRadius = inputValues[5] or node.yRadius
		local drawSphere = inputValues[6] or node.drawSphere
		local result = CharOp.locateInCar2D(self_, x, y, xRadius, yRadius, drawSphere)
        return {result}
    end
}

return charLocateInCar2DNode
