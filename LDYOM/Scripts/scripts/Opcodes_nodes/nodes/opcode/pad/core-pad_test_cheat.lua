local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorPadTestCheatNode : LDNodeEditorNode
---@field input string

local PAD_TEST_CHEAT_NODE_TYPE = "core.pad_test_cheat"

---@type LDNodeEditorNodeType
local padTestCheatNode = {
    typeName = PAD_TEST_CHEAT_NODE_TYPE,
    category = "opcode.pad",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorPadTestCheatNode
        local newNode = {
            id = newNodeId,
            nodeType = PAD_TEST_CHEAT_NODE_TYPE,
            inputs = {
                {
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
                    type = "core.bool",
                }

            },
			input = ""
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorPadTestCheatNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "input", function ()
            node.input = editor.dataTypes[node.inputs[1].type].drawEditValue(node.input, "##inputEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorPadTestCheatNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local input = inputValues[1] or node.input
		local result = PadOp.testCheat(input)
        return {result}
    end
}

return padTestCheatNode