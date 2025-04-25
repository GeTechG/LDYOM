local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTextAddLabelNode : LDNodeEditorNode
---@field dynamicKey string
---@field text string

local TEXT_ADD_LABEL_NODE_TYPE = "core.text_add_label"

---@type LDNodeEditorNodeType
local textAddLabelNode = {
    typeName = TEXT_ADD_LABEL_NODE_TYPE,
    category = "opcode.text",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTextAddLabelNode
        local newNode = {
            id = newNodeId,
            nodeType = TEXT_ADD_LABEL_NODE_TYPE,
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
                    type = "core.string",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.string",
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
			dynamicKey = "",
			text = ""
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTextAddLabelNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "dynamicKey", function ()
            node.dynamicKey = editor.dataTypes[node.inputs[2].type].drawEditValue(node.dynamicKey, "##dynamicKeyEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "text", function ()
            node.text = editor.dataTypes[node.inputs[3].type].drawEditValue(node.text, "##textEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorTextAddLabelNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local dynamicKey = inputValues[2] or node.dynamicKey
		local text = inputValues[3] or node.text
		TextOp.addLabel(dynamicKey, text)
        return {1}
    end
}

return textAddLabelNode
