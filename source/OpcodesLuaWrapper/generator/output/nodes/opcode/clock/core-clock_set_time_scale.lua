local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorClockSetTimeScaleNode : LDNodeEditorNode
---@field scale number

local CLOCK_SET_TIME_SCALE_NODE_TYPE = "core.clock_set_time_scale"

---@type LDNodeEditorNodeType
local clockSetTimeScaleNode = {
    typeName = CLOCK_SET_TIME_SCALE_NODE_TYPE,
    category = "opcode.clock",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorClockSetTimeScaleNode
        local newNode = {
            id = newNodeId,
            nodeType = CLOCK_SET_TIME_SCALE_NODE_TYPE,
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

            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.flow",
                }

            },
			scale = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorClockSetTimeScaleNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "scale", function ()
            node.scale = editor.dataTypes[node.inputs[2].type].drawEditValue(node.scale, "##scaleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorClockSetTimeScaleNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local scale = inputValues[2] or node.scale
		ClockOp.setTimeScale(scale)
        return {1}
    end
}

return clockSetTimeScaleNode
