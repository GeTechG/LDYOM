local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCharPerformSequenceFromProgressNode : LDNodeEditorNode
---@field self_ integer
---@field sequence integer
---@field _p3 integer
---@field _p4 integer

local CHAR_PERFORM_SEQUENCE_FROM_PROGRESS_NODE_TYPE = "core.char_perform_sequence_from_progress"

---@type LDNodeEditorNodeType
local charPerformSequenceFromProgressNode = {
    typeName = CHAR_PERFORM_SEQUENCE_FROM_PROGRESS_NODE_TYPE,
    category = "opcode.char",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCharPerformSequenceFromProgressNode
        local newNode = {
            id = newNodeId,
            nodeType = CHAR_PERFORM_SEQUENCE_FROM_PROGRESS_NODE_TYPE,
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

            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.flow",
                }

            },
			self_ = 0,
			sequence = 0,
			_p3 = 0,
			_p4 = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCharPerformSequenceFromProgressNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[2].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "sequence", function ()
            node.sequence = editor.dataTypes[node.inputs[3].type].drawEditValue(node.sequence, "##sequenceEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "_p3", function ()
            node._p3 = editor.dataTypes[node.inputs[4].type].drawEditValue(node._p3, "##_p3Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "_p4", function ()
            node._p4 = editor.dataTypes[node.inputs[5].type].drawEditValue(node._p4, "##_p4Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCharPerformSequenceFromProgressNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local self_ = inputValues[2] or node.self_
		local sequence = inputValues[3] or node.sequence
		local _p3 = inputValues[4] or node._p3
		local _p4 = inputValues[5] or node._p4
		CharOp.performSequenceFromProgress(self_, sequence, _p3, _p4)
        return {1}
    end
}

return charPerformSequenceFromProgressNode
