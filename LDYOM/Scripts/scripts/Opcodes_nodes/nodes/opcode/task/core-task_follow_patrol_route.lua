local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTaskFollowPatrolRouteNode : LDNodeEditorNode
---@field handle integer
---@field walkSpeed integer
---@field routeMode integer

local TASK_FOLLOW_PATROL_ROUTE_NODE_TYPE = "core.task_follow_patrol_route"

---@type LDNodeEditorNodeType
local taskFollowPatrolRouteNode = {
    typeName = TASK_FOLLOW_PATROL_ROUTE_NODE_TYPE,
    category = "opcode.task",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTaskFollowPatrolRouteNode
        local newNode = {
            id = newNodeId,
            nodeType = TASK_FOLLOW_PATROL_ROUTE_NODE_TYPE,
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
			walkSpeed = 0,
			routeMode = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTaskFollowPatrolRouteNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "handle", function ()
            node.handle = editor.dataTypes[node.inputs[2].type].drawEditValue(node.handle, "##handleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "walkSpeed", function ()
            node.walkSpeed = editor.dataTypes[node.inputs[3].type].drawEditValue(node.walkSpeed, "##walkSpeedEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "routeMode", function ()
            node.routeMode = editor.dataTypes[node.inputs[4].type].drawEditValue(node.routeMode, "##routeModeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorTaskFollowPatrolRouteNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local handle = inputValues[2] or node.handle
		if handle == nil or handle == 0 then
            error("Input handle is not set")
        end
		local walkSpeed = inputValues[3] or node.walkSpeed
		local routeMode = inputValues[4] or node.routeMode
		TaskOp.followPatrolRoute(handle, math.floor(walkSpeed), math.floor(routeMode))
        return {1}
    end
}

return taskFollowPatrolRouteNode
