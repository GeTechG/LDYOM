local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTaskToggleDuckNode : LDNodeEditorNode
---@field handle integer
---@field state boolean

local TASK_TOGGLE_DUCK_NODE_TYPE = "core.task_toggle_duck"

---@type LDNodeEditorNodeType
local taskToggleDuckNode = {
    typeName = TASK_TOGGLE_DUCK_NODE_TYPE,
    category = "opcode.task",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTaskToggleDuckNode
        local newNode = {
            id = newNodeId,
            nodeType = TASK_TOGGLE_DUCK_NODE_TYPE,
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
                    type = "core.bool",
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
			handle = 0,
			state = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTaskToggleDuckNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "handle", function ()
            node.handle = editor.dataTypes[node.inputs[2].type].drawEditValue(node.handle, "##handleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "state", function ()
            node.state = editor.dataTypes[node.inputs[3].type].drawEditValue(node.state, "##stateEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorTaskToggleDuckNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local handle = inputValues[2] or node.handle
		local state = inputValues[3] or node.state
		TaskOp.toggleDuck(handle, state)
        return {1}
    end
}

return taskToggleDuckNode