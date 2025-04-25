local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorPlaneFlyInDirectionNode : LDNodeEditorNode
---@field self_ integer
---@field heading number
---@field minAltitude number
---@field maxAltitude number

local PLANE_FLY_IN_DIRECTION_NODE_TYPE = "core.plane_fly_in_direction"

---@type LDNodeEditorNodeType
local planeFlyInDirectionNode = {
    typeName = PLANE_FLY_IN_DIRECTION_NODE_TYPE,
    category = "opcode.plane",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorPlaneFlyInDirectionNode
        local newNode = {
            id = newNodeId,
            nodeType = PLANE_FLY_IN_DIRECTION_NODE_TYPE,
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
                    type = "core.Vehicle",
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
			heading = 0,
			minAltitude = 0,
			maxAltitude = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorPlaneFlyInDirectionNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[2].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "heading", function ()
            node.heading = editor.dataTypes[node.inputs[3].type].drawEditValue(node.heading, "##headingEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "minAltitude", function ()
            node.minAltitude = editor.dataTypes[node.inputs[4].type].drawEditValue(node.minAltitude, "##minAltitudeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "maxAltitude", function ()
            node.maxAltitude = editor.dataTypes[node.inputs[5].type].drawEditValue(node.maxAltitude, "##maxAltitudeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorPlaneFlyInDirectionNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local self_ = inputValues[2] or node.self_
		if self_ == nil or self_ == 0 then
            error("Input self is not set")
        end
		local heading = inputValues[3] or node.heading
		local minAltitude = inputValues[4] or node.minAltitude
		local maxAltitude = inputValues[5] or node.maxAltitude
		PlaneOp.flyInDirection(self_, heading, minAltitude, maxAltitude)
        return {1}
    end
}

return planeFlyInDirectionNode
