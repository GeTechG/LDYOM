local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCharIsShootingInAreaNode : LDNodeEditorNode
---@field self_ integer
---@field leftBottomX number
---@field leftBottomY number
---@field topRightX number
---@field topRightY number
---@field drawSphere boolean

local CHAR_IS_SHOOTING_IN_AREA_NODE_TYPE = "core.char_is_shooting_in_area"

---@type LDNodeEditorNodeType
local charIsShootingInAreaNode = {
    typeName = CHAR_IS_SHOOTING_IN_AREA_NODE_TYPE,
    category = "opcode.char",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCharIsShootingInAreaNode
        local newNode = {
            id = newNodeId,
            nodeType = CHAR_IS_SHOOTING_IN_AREA_NODE_TYPE,
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
                    type = "core.flow",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.bool",
                }

            },
			self_ = 0,
			leftBottomX = 0,
			leftBottomY = 0,
			topRightX = 0,
			topRightY = 0,
			drawSphere = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCharIsShootingInAreaNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[2].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "leftBottomX", function ()
            node.leftBottomX = editor.dataTypes[node.inputs[3].type].drawEditValue(node.leftBottomX, "##leftBottomXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "leftBottomY", function ()
            node.leftBottomY = editor.dataTypes[node.inputs[4].type].drawEditValue(node.leftBottomY, "##leftBottomYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "topRightX", function ()
            node.topRightX = editor.dataTypes[node.inputs[5].type].drawEditValue(node.topRightX, "##topRightXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "topRightY", function ()
            node.topRightY = editor.dataTypes[node.inputs[6].type].drawEditValue(node.topRightY, "##topRightYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "drawSphere", function ()
            node.drawSphere = editor.dataTypes[node.inputs[7].type].drawEditValue(node.drawSphere, "##drawSphereEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCharIsShootingInAreaNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local self_ = inputValues[2] or node.self_
		local leftBottomX = inputValues[3] or node.leftBottomX
		local leftBottomY = inputValues[4] or node.leftBottomY
		local topRightX = inputValues[5] or node.topRightX
		local topRightY = inputValues[6] or node.topRightY
		local drawSphere = inputValues[7] or node.drawSphere
		local result = CharOp.isShootingInArea(self_, leftBottomX, leftBottomY, topRightX, topRightY, drawSphere)
        return {1, result}
    end
}

return charIsShootingInAreaNode
