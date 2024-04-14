local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTaskDrivePointRouteAdvancedNode : LDNodeEditorNode
---@field char integer
---@field vehicle integer
---@field speed number
---@field _p4 integer
---@field _p5 integer
---@field _p6 integer

local TASK_DRIVE_POINT_ROUTE_ADVANCED_NODE_TYPE = "core.task_drive_point_route_advanced"

---@type LDNodeEditorNodeType
local taskDrivePointRouteAdvancedNode = {
    typeName = TASK_DRIVE_POINT_ROUTE_ADVANCED_NODE_TYPE,
    category = "opcode.task",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTaskDrivePointRouteAdvancedNode
        local newNode = {
            id = newNodeId,
            nodeType = TASK_DRIVE_POINT_ROUTE_ADVANCED_NODE_TYPE,
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
			char = 0,
			vehicle = 0,
			speed = 0,
			_p4 = 0,
			_p5 = 0,
			_p6 = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTaskDrivePointRouteAdvancedNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "char", function ()
            node.char = editor.dataTypes[node.inputs[2].type].drawEditValue(node.char, "##charEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "vehicle", function ()
            node.vehicle = editor.dataTypes[node.inputs[3].type].drawEditValue(node.vehicle, "##vehicleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "speed", function ()
            node.speed = editor.dataTypes[node.inputs[4].type].drawEditValue(node.speed, "##speedEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "_p4", function ()
            node._p4 = editor.dataTypes[node.inputs[5].type].drawEditValue(node._p4, "##_p4Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "_p5", function ()
            node._p5 = editor.dataTypes[node.inputs[6].type].drawEditValue(node._p5, "##_p5Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "_p6", function ()
            node._p6 = editor.dataTypes[node.inputs[7].type].drawEditValue(node._p6, "##_p6Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorTaskDrivePointRouteAdvancedNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local char = inputValues[2] or node.char
		if char == nil or char == 0 then
            error("Input char is not set")
        end
		local vehicle = inputValues[3] or node.vehicle
		if vehicle == nil or vehicle == 0 then
            error("Input vehicle is not set")
        end
		local speed = inputValues[4] or node.speed
		local _p4 = inputValues[5] or node._p4
		local _p5 = inputValues[6] or node._p5
		local _p6 = inputValues[7] or node._p6
		TaskOp.drivePointRouteAdvanced(char, vehicle, speed, _p4, _p5, _p6)
        return {1}
    end
}

return taskDrivePointRouteAdvancedNode
