local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTaskFallAndGetUpNode : LDNodeEditorNode
---@field handle integer
---@field fallDown boolean
---@field timeOnGround integer

local TASK_FALL_AND_GET_UP_NODE_TYPE = "core.task_fall_and_get_up"

---@type LDNodeEditorNodeType
local taskFallAndGetUpNode = {
    typeName = TASK_FALL_AND_GET_UP_NODE_TYPE,
    category = "opcode.task",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTaskFallAndGetUpNode
        local newNode = {
            id = newNodeId,
            nodeType = TASK_FALL_AND_GET_UP_NODE_TYPE,
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
			handle = 0,
			fallDown = false,
			timeOnGround = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTaskFallAndGetUpNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "handle", function ()
            node.handle = editor.dataTypes[node.inputs[2].type].drawEditValue(node.handle, "##handleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "fallDown", function ()
            node.fallDown = editor.dataTypes[node.inputs[3].type].drawEditValue(node.fallDown, "##fallDownEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "timeOnGround", function ()
            node.timeOnGround = editor.dataTypes[node.inputs[4].type].drawEditValue(node.timeOnGround, "##timeOnGroundEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorTaskFallAndGetUpNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local handle = inputValues[2] or node.handle
		if handle == nil or handle == 0 then
            error("Input handle is not set")
        end
		local fallDown = inputValues[3] or node.fallDown
		local timeOnGround = inputValues[4] or node.timeOnGround
		TaskOp.fallAndGetUp(handle, fallDown, timeOnGround)
        return {1}
    end
}

return taskFallAndGetUpNode
