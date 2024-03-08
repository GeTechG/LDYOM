local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTextSetColorNode : LDNodeEditorNode
---@field red integer
---@field green integer
---@field blue integer
---@field alpha integer

local TEXT_SET_COLOR_NODE_TYPE = "core.text_set_color"

---@type LDNodeEditorNodeType
local textSetColorNode = {
    typeName = TEXT_SET_COLOR_NODE_TYPE,
    category = "opcode.text",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTextSetColorNode
        local newNode = {
            id = newNodeId,
            nodeType = TEXT_SET_COLOR_NODE_TYPE,
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

            },
			red = 0,
			green = 0,
			blue = 0,
			alpha = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTextSetColorNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "red", function ()
            node.red = editor.dataTypes[node.inputs[2].type].drawEditValue(node.red, "##redEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "green", function ()
            node.green = editor.dataTypes[node.inputs[3].type].drawEditValue(node.green, "##greenEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "blue", function ()
            node.blue = editor.dataTypes[node.inputs[4].type].drawEditValue(node.blue, "##blueEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "alpha", function ()
            node.alpha = editor.dataTypes[node.inputs[5].type].drawEditValue(node.alpha, "##alphaEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorTextSetColorNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local red = inputValues[2] or node.red
		local green = inputValues[3] or node.green
		local blue = inputValues[4] or node.blue
		local alpha = inputValues[5] or node.alpha
		TextOp.setColor(red, green, blue, alpha)
        return {1}
    end
}

return textSetColorNode
