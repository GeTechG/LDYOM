local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTaskGoToCoordWhileAimingNode : LDNodeEditorNode
---@field char integer
---@field x number
---@field y number
---@field z number
---@field mode integer
---@field turnRadius number
---@field stopRadius number
---@field target integer
---@field xOffset number
---@field yOffset number
---@field zOffset number

local TASK_GO_TO_COORD_WHILE_AIMING_NODE_TYPE = "core.task_go_to_coord_while_aiming"

---@type LDNodeEditorNodeType
local taskGoToCoordWhileAimingNode = {
    typeName = TASK_GO_TO_COORD_WHILE_AIMING_NODE_TYPE,
    category = "opcode.task",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTaskGoToCoordWhileAimingNode
        local newNode = {
            id = newNodeId,
            nodeType = TASK_GO_TO_COORD_WHILE_AIMING_NODE_TYPE,
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
			char = 0,
			x = 0,
			y = 0,
			z = 0,
			mode = 0,
			turnRadius = 0,
			stopRadius = 0,
			target = 0,
			xOffset = 0,
			yOffset = 0,
			zOffset = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTaskGoToCoordWhileAimingNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "char", function ()
            node.char = editor.dataTypes[node.inputs[2].type].drawEditValue(node.char, "##charEdit", fontScale * 100)
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "turnRadius", function ()
            node.turnRadius = editor.dataTypes[node.inputs[7].type].drawEditValue(node.turnRadius, "##turnRadiusEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "stopRadius", function ()
            node.stopRadius = editor.dataTypes[node.inputs[8].type].drawEditValue(node.stopRadius, "##stopRadiusEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[9], "target", function ()
            node.target = editor.dataTypes[node.inputs[9].type].drawEditValue(node.target, "##targetEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[10], "xOffset", function ()
            node.xOffset = editor.dataTypes[node.inputs[10].type].drawEditValue(node.xOffset, "##xOffsetEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[11], "yOffset", function ()
            node.yOffset = editor.dataTypes[node.inputs[11].type].drawEditValue(node.yOffset, "##yOffsetEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[12], "zOffset", function ()
            node.zOffset = editor.dataTypes[node.inputs[12].type].drawEditValue(node.zOffset, "##zOffsetEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorTaskGoToCoordWhileAimingNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local char = inputValues[2] or node.char
		if char == nil or char == 0 then
            error("Input char is not set")
        end
		local x = inputValues[3] or node.x
		local y = inputValues[4] or node.y
		local z = inputValues[5] or node.z
		local mode = inputValues[6] or node.mode
		local turnRadius = inputValues[7] or node.turnRadius
		local stopRadius = inputValues[8] or node.stopRadius
		local target = inputValues[9] or node.target
		if target == nil or target == 0 then
            error("Input target is not set")
        end
		local xOffset = inputValues[10] or node.xOffset
		local yOffset = inputValues[11] or node.yOffset
		local zOffset = inputValues[12] or node.zOffset
		TaskOp.goToCoordWhileAiming(char, x, y, z, math.floor(mode), turnRadius, stopRadius, target, xOffset, yOffset, zOffset)
        return {1}
    end
}

return taskGoToCoordWhileAimingNode
