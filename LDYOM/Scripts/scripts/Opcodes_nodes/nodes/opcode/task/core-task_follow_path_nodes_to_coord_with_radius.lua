local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTaskFollowPathNodesToCoordWithRadiusNode : LDNodeEditorNode
---@field handle integer
---@field x number
---@field y number
---@field z number
---@field mode integer
---@field time integer
---@field radius number

local TASK_FOLLOW_PATH_NODES_TO_COORD_WITH_RADIUS_NODE_TYPE = "core.task_follow_path_nodes_to_coord_with_radius"

---@type LDNodeEditorNodeType
local taskFollowPathNodesToCoordWithRadiusNode = {
    typeName = TASK_FOLLOW_PATH_NODES_TO_COORD_WITH_RADIUS_NODE_TYPE,
    category = "opcode.task",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTaskFollowPathNodesToCoordWithRadiusNode
        local newNode = {
            id = newNodeId,
            nodeType = TASK_FOLLOW_PATH_NODES_TO_COORD_WITH_RADIUS_NODE_TYPE,
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
			handle = 0,
			x = 0,
			y = 0,
			z = 0,
			mode = 0,
			time = 0,
			radius = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTaskFollowPathNodesToCoordWithRadiusNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "handle", function ()
            node.handle = editor.dataTypes[node.inputs[2].type].drawEditValue(node.handle, "##handleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "x", function ()
            node.x = editor.dataTypes[node.inputs[3].type].drawEditValue(node.x, "##xEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "y", function ()
            node.y = editor.dataTypes[node.inputs[4].type].drawEditValue(node.y, "##yEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "z", function ()
            node.z = editor.dataTypes[node.inputs[5].type].drawEditValue(node.z, "##zEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "mode", function ()
            node.mode = editor.dataTypes[node.inputs[6].type].drawEditValue(node.mode, "##modeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "time", function ()
            node.time = editor.dataTypes[node.inputs[7].type].drawEditValue(node.time, "##timeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "radius", function ()
            node.radius = editor.dataTypes[node.inputs[8].type].drawEditValue(node.radius, "##radiusEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorTaskFollowPathNodesToCoordWithRadiusNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local handle = inputValues[2] or node.handle
		if handle == nil or handle == 0 then
            error("Input handle is not set")
        end
		local x = inputValues[3] or node.x
		local y = inputValues[4] or node.y
		local z = inputValues[5] or node.z
		local mode = inputValues[6] or node.mode
		local time = inputValues[7] or node.time
		local radius = inputValues[8] or node.radius
		TaskOp.followPathNodesToCoordWithRadius(handle, x, y, z, math.floor(mode), math.floor(time), radius)
        return {1}
    end
}

return taskFollowPathNodesToCoordWithRadiusNode
