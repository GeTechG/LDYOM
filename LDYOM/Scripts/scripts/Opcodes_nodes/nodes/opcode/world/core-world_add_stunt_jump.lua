local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorWorldAddStuntJumpNode : LDNodeEditorNode
---@field startX number
---@field startY number
---@field startZ number
---@field startRadiusX number
---@field startRadiusY number
---@field startRadiusZ number
---@field finishX number
---@field finishY number
---@field finishZ number
---@field finishRadiusX number
---@field finishRadiusY number
---@field finishRadiusZ number
---@field cameraX number
---@field cameraY number
---@field cameraZ number
---@field reward integer

local WORLD_ADD_STUNT_JUMP_NODE_TYPE = "core.world_add_stunt_jump"

---@type LDNodeEditorNodeType
local worldAddStuntJumpNode = {
    typeName = WORLD_ADD_STUNT_JUMP_NODE_TYPE,
    category = "opcode.world",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorWorldAddStuntJumpNode
        local newNode = {
            id = newNodeId,
            nodeType = WORLD_ADD_STUNT_JUMP_NODE_TYPE,
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
			startX = 0,
			startY = 0,
			startZ = 0,
			startRadiusX = 0,
			startRadiusY = 0,
			startRadiusZ = 0,
			finishX = 0,
			finishY = 0,
			finishZ = 0,
			finishRadiusX = 0,
			finishRadiusY = 0,
			finishRadiusZ = 0,
			cameraX = 0,
			cameraY = 0,
			cameraZ = 0,
			reward = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorWorldAddStuntJumpNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "startX", function ()
            node.startX = editor.dataTypes[node.inputs[2].type].drawEditValue(node.startX, "##startXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "startY", function ()
            node.startY = editor.dataTypes[node.inputs[3].type].drawEditValue(node.startY, "##startYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "startZ", function ()
            node.startZ = editor.dataTypes[node.inputs[4].type].drawEditValue(node.startZ, "##startZEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "startRadiusX", function ()
            node.startRadiusX = editor.dataTypes[node.inputs[5].type].drawEditValue(node.startRadiusX, "##startRadiusXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "startRadiusY", function ()
            node.startRadiusY = editor.dataTypes[node.inputs[6].type].drawEditValue(node.startRadiusY, "##startRadiusYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "startRadiusZ", function ()
            node.startRadiusZ = editor.dataTypes[node.inputs[7].type].drawEditValue(node.startRadiusZ, "##startRadiusZEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "finishX", function ()
            node.finishX = editor.dataTypes[node.inputs[8].type].drawEditValue(node.finishX, "##finishXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[9], "finishY", function ()
            node.finishY = editor.dataTypes[node.inputs[9].type].drawEditValue(node.finishY, "##finishYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[10], "finishZ", function ()
            node.finishZ = editor.dataTypes[node.inputs[10].type].drawEditValue(node.finishZ, "##finishZEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[11], "finishRadiusX", function ()
            node.finishRadiusX = editor.dataTypes[node.inputs[11].type].drawEditValue(node.finishRadiusX, "##finishRadiusXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[12], "finishRadiusY", function ()
            node.finishRadiusY = editor.dataTypes[node.inputs[12].type].drawEditValue(node.finishRadiusY, "##finishRadiusYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[13], "finishRadiusZ", function ()
            node.finishRadiusZ = editor.dataTypes[node.inputs[13].type].drawEditValue(node.finishRadiusZ, "##finishRadiusZEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[14], "cameraX", function ()
            node.cameraX = editor.dataTypes[node.inputs[14].type].drawEditValue(node.cameraX, "##cameraXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[15], "cameraY", function ()
            node.cameraY = editor.dataTypes[node.inputs[15].type].drawEditValue(node.cameraY, "##cameraYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[16], "cameraZ", function ()
            node.cameraZ = editor.dataTypes[node.inputs[16].type].drawEditValue(node.cameraZ, "##cameraZEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[17], "reward", function ()
            node.reward = editor.dataTypes[node.inputs[17].type].drawEditValue(node.reward, "##rewardEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorWorldAddStuntJumpNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local startX = inputValues[2] or node.startX
		local startY = inputValues[3] or node.startY
		local startZ = inputValues[4] or node.startZ
		local startRadiusX = inputValues[5] or node.startRadiusX
		local startRadiusY = inputValues[6] or node.startRadiusY
		local startRadiusZ = inputValues[7] or node.startRadiusZ
		local finishX = inputValues[8] or node.finishX
		local finishY = inputValues[9] or node.finishY
		local finishZ = inputValues[10] or node.finishZ
		local finishRadiusX = inputValues[11] or node.finishRadiusX
		local finishRadiusY = inputValues[12] or node.finishRadiusY
		local finishRadiusZ = inputValues[13] or node.finishRadiusZ
		local cameraX = inputValues[14] or node.cameraX
		local cameraY = inputValues[15] or node.cameraY
		local cameraZ = inputValues[16] or node.cameraZ
		local reward = inputValues[17] or node.reward
		WorldOp.addStuntJump(startX, startY, startZ, startRadiusX, startRadiusY, startRadiusZ, finishX, finishY, finishZ, finishRadiusX, finishRadiusY, finishRadiusZ, cameraX, cameraY, cameraZ, reward)
        return {1}
    end
}

return worldAddStuntJumpNode
