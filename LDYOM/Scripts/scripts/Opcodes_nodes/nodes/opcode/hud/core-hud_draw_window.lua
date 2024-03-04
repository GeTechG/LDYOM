local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorHudDrawWindowNode : LDNodeEditorNode
---@field leftTopX number
---@field leftTopY number
---@field rightBottomX number
---@field rightBottomY number
---@field header string
---@field zIndex integer

local HUD_DRAW_WINDOW_NODE_TYPE = "core.hud_draw_window"

---@type LDNodeEditorNodeType
local hudDrawWindowNode = {
    typeName = HUD_DRAW_WINDOW_NODE_TYPE,
    category = "opcode.hud",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorHudDrawWindowNode
        local newNode = {
            id = newNodeId,
            nodeType = HUD_DRAW_WINDOW_NODE_TYPE,
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
                    type = "core.string",
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
			leftTopX = 0,
			leftTopY = 0,
			rightBottomX = 0,
			rightBottomY = 0,
			header = "",
			zIndex = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorHudDrawWindowNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "leftTopX", function ()
            node.leftTopX = editor.dataTypes[node.inputs[2].type].drawEditValue(node.leftTopX, "##leftTopXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "leftTopY", function ()
            node.leftTopY = editor.dataTypes[node.inputs[3].type].drawEditValue(node.leftTopY, "##leftTopYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "rightBottomX", function ()
            node.rightBottomX = editor.dataTypes[node.inputs[4].type].drawEditValue(node.rightBottomX, "##rightBottomXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "rightBottomY", function ()
            node.rightBottomY = editor.dataTypes[node.inputs[5].type].drawEditValue(node.rightBottomY, "##rightBottomYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "header", function ()
            node.header = editor.dataTypes[node.inputs[6].type].drawEditValue(node.header, "##headerEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "zIndex", function ()
            node.zIndex = editor.dataTypes[node.inputs[7].type].drawEditValue(node.zIndex, "##zIndexEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorHudDrawWindowNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local leftTopX = inputValues[2] or node.leftTopX
		local leftTopY = inputValues[3] or node.leftTopY
		local rightBottomX = inputValues[4] or node.rightBottomX
		local rightBottomY = inputValues[5] or node.rightBottomY
		local header = inputValues[6] or node.header
		local zIndex = inputValues[7] or node.zIndex
		HudOp.drawWindow(leftTopX, leftTopY, rightBottomX, rightBottomY, header, zIndex)
        return {1}
    end
}

return hudDrawWindowNode
