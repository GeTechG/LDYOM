local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTextStringFormatNode : LDNodeEditorNode
---@field buffer integer
---@field format string
---@field args integer

local TEXT_STRING_FORMAT_NODE_TYPE = "core.text_string_format"

---@type LDNodeEditorNodeType
local textStringFormatNode = {
    typeName = TEXT_STRING_FORMAT_NODE_TYPE,
    category = "opcode.text",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTextStringFormatNode
        local newNode = {
            id = newNodeId,
            nodeType = TEXT_STRING_FORMAT_NODE_TYPE,
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
			buffer = 0,
			format = "",
			args = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTextStringFormatNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "buffer", function ()
            node.buffer = editor.dataTypes[node.inputs[2].type].drawEditValue(node.buffer, "##bufferEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "format", function ()
            node.format = editor.dataTypes[node.inputs[3].type].drawEditValue(node.format, "##formatEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "args", function ()
            node.args = editor.dataTypes[node.inputs[4].type].drawEditValue(node.args, "##argsEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorTextStringFormatNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local buffer = inputValues[2] or node.buffer
		local format = inputValues[3] or node.format
		local args = inputValues[4] or node.args
		TextOp.stringFormat(buffer, format, args)
        return {1}
    end
}

return textStringFormatNode
