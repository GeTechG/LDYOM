local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTextIsThisHelpMessageBeingDisplayedNode : LDNodeEditorNode
---@field gxt string

local TEXT_IS_THIS_HELP_MESSAGE_BEING_DISPLAYED_NODE_TYPE = "core.text_is_this_help_message_being_displayed"

---@type LDNodeEditorNodeType
local textIsThisHelpMessageBeingDisplayedNode = {
    typeName = TEXT_IS_THIS_HELP_MESSAGE_BEING_DISPLAYED_NODE_TYPE,
    category = "opcode.text",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTextIsThisHelpMessageBeingDisplayedNode
        local newNode = {
            id = newNodeId,
            nodeType = TEXT_IS_THIS_HELP_MESSAGE_BEING_DISPLAYED_NODE_TYPE,
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
			gxt = ""
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTextIsThisHelpMessageBeingDisplayedNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "gxt", function ()
            node.gxt = editor.dataTypes[node.inputs[2].type].drawEditValue(node.gxt, "##gxtEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorTextIsThisHelpMessageBeingDisplayedNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local gxt = inputValues[2] or node.gxt
		local result = TextOp.isThisHelpMessageBeingDisplayed(gxt)
        return {1, result}
    end
}

return textIsThisHelpMessageBeingDisplayedNode
