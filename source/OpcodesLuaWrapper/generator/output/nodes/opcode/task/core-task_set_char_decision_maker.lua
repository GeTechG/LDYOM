local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTaskSetCharDecisionMakerNode : LDNodeEditorNode
---@field char integer
---@field decisionMakerChar integer

local TASK_SET_CHAR_DECISION_MAKER_NODE_TYPE = "core.task_set_char_decision_maker"

---@type LDNodeEditorNodeType
local taskSetCharDecisionMakerNode = {
    typeName = TASK_SET_CHAR_DECISION_MAKER_NODE_TYPE,
    category = "opcode.task",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTaskSetCharDecisionMakerNode
        local newNode = {
            id = newNodeId,
            nodeType = TASK_SET_CHAR_DECISION_MAKER_NODE_TYPE,
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
                    type = "core.DecisionMakerChar",
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
			char = 0,
			decisionMakerChar = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTaskSetCharDecisionMakerNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "char", function ()
            node.char = editor.dataTypes[node.inputs[2].type].drawEditValue(node.char, "##charEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "decisionMakerChar", function ()
            node.decisionMakerChar = editor.dataTypes[node.inputs[3].type].drawEditValue(node.decisionMakerChar, "##decisionMakerCharEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorTaskSetCharDecisionMakerNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local char = inputValues[2] or node.char
		local decisionMakerChar = inputValues[3] or node.decisionMakerChar
		TaskOp.setCharDecisionMaker(char, decisionMakerChar)
        return {1}
    end
}

return taskSetCharDecisionMakerNode
