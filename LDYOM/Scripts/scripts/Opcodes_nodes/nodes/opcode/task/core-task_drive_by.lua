local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTaskDriveByNode : LDNodeEditorNode
---@field handle integer
---@field targetChar integer
---@field targetVehicle integer
---@field x number
---@field y number
---@field z number
---@field radius number
---@field style integer
---@field rightHandCarSeat boolean
---@field fireRate integer

local TASK_DRIVE_BY_NODE_TYPE = "core.task_drive_by"

---@type LDNodeEditorNodeType
local taskDriveByNode = {
    typeName = TASK_DRIVE_BY_NODE_TYPE,
    category = "opcode.task",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTaskDriveByNode
        local newNode = {
            id = newNodeId,
            nodeType = TASK_DRIVE_BY_NODE_TYPE,
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
			targetChar = 0,
			targetVehicle = 0,
			x = 0,
			y = 0,
			z = 0,
			radius = 0,
			style = 0,
			rightHandCarSeat = false,
			fireRate = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTaskDriveByNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "handle", function ()
            node.handle = editor.dataTypes[node.inputs[2].type].drawEditValue(node.handle, "##handleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "targetChar", function ()
            node.targetChar = editor.dataTypes[node.inputs[3].type].drawEditValue(node.targetChar, "##targetCharEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "targetVehicle", function ()
            node.targetVehicle = editor.dataTypes[node.inputs[4].type].drawEditValue(node.targetVehicle, "##targetVehicleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "x", function ()
            node.x = editor.dataTypes[node.inputs[5].type].drawEditValue(node.x, "##xEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "y", function ()
            node.y = editor.dataTypes[node.inputs[6].type].drawEditValue(node.y, "##yEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "z", function ()
            node.z = editor.dataTypes[node.inputs[7].type].drawEditValue(node.z, "##zEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "radius", function ()
            node.radius = editor.dataTypes[node.inputs[8].type].drawEditValue(node.radius, "##radiusEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[9], "style", function ()
            node.style = editor.dataTypes[node.inputs[9].type].drawEditValue(node.style, "##styleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[10], "rightHandCarSeat", function ()
            node.rightHandCarSeat = editor.dataTypes[node.inputs[10].type].drawEditValue(node.rightHandCarSeat, "##rightHandCarSeatEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[11], "fireRate", function ()
            node.fireRate = editor.dataTypes[node.inputs[11].type].drawEditValue(node.fireRate, "##fireRateEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorTaskDriveByNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local handle = inputValues[2] or node.handle
		local targetChar = inputValues[3] or node.targetChar
		local targetVehicle = inputValues[4] or node.targetVehicle
		local x = inputValues[5] or node.x
		local y = inputValues[6] or node.y
		local z = inputValues[7] or node.z
		local radius = inputValues[8] or node.radius
		local style = inputValues[9] or node.style
		local rightHandCarSeat = inputValues[10] or node.rightHandCarSeat
		local fireRate = inputValues[11] or node.fireRate
		TaskOp.driveBy(handle, targetChar, targetVehicle, x, y, z, radius, style, rightHandCarSeat, fireRate)
        return {1}
    end
}

return taskDriveByNode
