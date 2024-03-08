local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorHudDrawSpriteNode : LDNodeEditorNode
---@field memorySlot integer
---@field offsetLeft number
---@field offsetTop number
---@field width number
---@field height number
---@field r integer
---@field g integer
---@field b integer
---@field a integer

local HUD_DRAW_SPRITE_NODE_TYPE = "core.hud_draw_sprite"

---@type LDNodeEditorNodeType
local hudDrawSpriteNode = {
    typeName = HUD_DRAW_SPRITE_NODE_TYPE,
    category = "opcode.hud",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorHudDrawSpriteNode
        local newNode = {
            id = newNodeId,
            nodeType = HUD_DRAW_SPRITE_NODE_TYPE,
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
			r = 0,
			g = 0,
			b = 0,
			a = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorHudDrawSpriteNode
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "r", function ()
            node.r = editor.dataTypes[node.inputs[7].type].drawEditValue(node.r, "##rEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "g", function ()
            node.g = editor.dataTypes[node.inputs[8].type].drawEditValue(node.g, "##gEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[9], "b", function ()
            node.b = editor.dataTypes[node.inputs[9].type].drawEditValue(node.b, "##bEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[10], "a", function ()
            node.a = editor.dataTypes[node.inputs[10].type].drawEditValue(node.a, "##aEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorHudDrawSpriteNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local memorySlot = inputValues[2] or node.memorySlot
		local offsetLeft = inputValues[3] or node.offsetLeft
		local offsetTop = inputValues[4] or node.offsetTop
		local width = inputValues[5] or node.width
		local height = inputValues[6] or node.height
		local r = inputValues[7] or node.r
		local g = inputValues[8] or node.g
		local b = inputValues[9] or node.b
		local a = inputValues[10] or node.a
		HudOp.drawSprite(memorySlot, offsetLeft, offsetTop, width, height, r, g, b, a)
        return {1}
    end
}

return hudDrawSpriteNode
