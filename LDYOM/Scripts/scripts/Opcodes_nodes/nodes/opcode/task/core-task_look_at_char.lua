local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTaskLookAtCharNode : LDNodeEditorNode
---@field observer integer
---@field target integer
---@field time integer

local TASK_LOOK_AT_CHAR_NODE_TYPE = "core.task_look_at_char"

---@type LDNodeEditorNodeType
local taskLookAtCharNode = {
    typeName = TASK_LOOK_AT_CHAR_NODE_TYPE,
    category = "opcode.task",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTaskLookAtCharNode
        local newNode = {
            id = newNodeId,
            nodeType = TASK_LOOK_AT_CHAR_NODE_TYPE,
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
                    type = "core.Char",
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
			observer = 0,
			target = 0,
			time = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTaskLookAtCharNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "observer", function ()
            node.observer = editor.dataTypes[node.inputs[2].type].drawEditValue(node.observer, "##observerEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "target", function ()
            node.target = editor.dataTypes[node.inputs[3].type].drawEditValue(node.target, "##targetEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "time", function ()
            node.time = editor.dataTypes[node.inputs[4].type].drawEditValue(node.time, "##timeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorTaskLookAtCharNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local observer = inputValues[2] or node.observer
		if observer == nil or observer == 0 then
            error("Input observer is not set")
        end
		local target = inputValues[3] or node.target
		if target == nil or target == 0 then
            error("Input target is not set")
        end
		local time = inputValues[4] or node.time
		TaskOp.lookAtChar(observer, target, math.floor(time))
        return {1}
    end
}

return taskLookAtCharNode
