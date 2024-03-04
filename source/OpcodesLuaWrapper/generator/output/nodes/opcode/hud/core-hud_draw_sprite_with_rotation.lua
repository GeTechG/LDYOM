local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorHudDrawSpriteWithRotationNode : LDNodeEditorNode
---@field memorySlot integer
---@field offsetLeft number
---@field offsetTop number
---@field width number
---@field height number
---@field angle number
---@field red integer
---@field green integer
---@field blue integer
---@field alpha integer

local HUD_DRAW_SPRITE_WITH_ROTATION_NODE_TYPE = "core.hud_draw_sprite_with_rotation"

---@type LDNodeEditorNodeType
local hudDrawSpriteWithRotationNode = {
    typeName = HUD_DRAW_SPRITE_WITH_ROTATION_NODE_TYPE,
    category = "opcode.hud",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorHudDrawSpriteWithRotationNode
        local newNode = {
            id = newNodeId,
            nodeType = HUD_DRAW_SPRITE_WITH_ROTATION_NODE_TYPE,
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

            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.flow",
                }

            },
			memorySlot = 0,
			offsetLeft = 0,
			offsetTop = 0,
			width = 0,
			height = 0,
			angle = 0,
			red = 0,
			green = 0,
			blue = 0,
			alpha = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorHudDrawSpriteWithRotationNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "memorySlot", function ()
            node.memorySlot = editor.dataTypes[node.inputs[2].type].drawEditValue(node.memorySlot, "##memorySlotEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "offsetLeft", function ()
            node.offsetLeft = editor.dataTypes[node.inputs[3].type].drawEditValue(node.offsetLeft, "##offsetLeftEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "offsetTop", function ()
            node.offsetTop = editor.dataTypes[node.inputs[4].type].drawEditValue(node.offsetTop, "##offsetTopEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "width", function ()
            node.width = editor.dataTypes[node.inputs[5].type].drawEditValue(node.width, "##widthEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "height", function ()
            node.height = editor.dataTypes[node.inputs[6].type].drawEditValue(node.height, "##heightEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "angle", function ()
            node.angle = editor.dataTypes[node.inputs[7].type].drawEditValue(node.angle, "##angleEdit", fontScale * 100)
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

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorHudDrawSpriteWithRotationNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local memorySlot = inputValues[2] or node.memorySlot
		local offsetLeft = inputValues[3] or node.offsetLeft
		local offsetTop = inputValues[4] or node.offsetTop
		local width = inputValues[5] or node.width
		local height = inputValues[6] or node.height
		local angle = inputValues[7] or node.angle
		local red = inputValues[8] or node.red
		local green = inputValues[9] or node.green
		local blue = inputValues[10] or node.blue
		local alpha = inputValues[11] or node.alpha
		HudOp.drawSpriteWithRotation(memorySlot, offsetLeft, offsetTop, width, height, angle, red, green, blue, alpha)
        return {1}
    end
}

return hudDrawSpriteWithRotationNode
