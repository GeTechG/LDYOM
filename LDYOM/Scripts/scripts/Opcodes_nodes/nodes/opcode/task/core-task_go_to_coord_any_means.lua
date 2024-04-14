local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTaskGoToCoordAnyMeansNode : LDNodeEditorNode
---@field char integer
---@field x number
---@field y number
---@field z number
---@field walkSpeed integer
---@field vehicle integer

local TASK_GO_TO_COORD_ANY_MEANS_NODE_TYPE = "core.task_go_to_coord_any_means"

---@type LDNodeEditorNodeType
local taskGoToCoordAnyMeansNode = {
    typeName = TASK_GO_TO_COORD_ANY_MEANS_NODE_TYPE,
    category = "opcode.task",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTaskGoToCoordAnyMeansNode
        local newNode = {
            id = newNodeId,
            nodeType = TASK_GO_TO_COORD_ANY_MEANS_NODE_TYPE,
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
                    type = "core.Vehicle",
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
			walkSpeed = 0,
			vehicle = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTaskGoToCoordAnyMeansNode
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "walkSpeed", function ()
            node.walkSpeed = editor.dataTypes[node.inputs[6].type].drawEditValue(node.walkSpeed, "##walkSpeedEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "vehicle", function ()
            node.vehicle = editor.dataTypes[node.inputs[7].type].drawEditValue(node.vehicle, "##vehicleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorTaskGoToCoordAnyMeansNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local char = inputValues[2] or node.char
		if char == nil or char == 0 then
            error("Input char is not set")
        end
		local x = inputValues[3] or node.x
		local y = inputValues[4] or node.y
		local z = inputValues[5] or node.z
		local walkSpeed = inputValues[6] or node.walkSpeed
		local vehicle = inputValues[7] or node.vehicle
		if vehicle == nil or vehicle == 0 then
            error("Input vehicle is not set")
        end
		TaskOp.goToCoordAnyMeans(char, x, y, z, walkSpeed, vehicle)
        return {1}
    end
}

return taskGoToCoordAnyMeansNode
