local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorWorldAddSetPieceNode : LDNodeEditorNode
---@field type integer
---@field fromX number
---@field fromY number
---@field toX number
---@field toY number
---@field spawnPoliceAAtX number
---@field spawnPoliceAAtY number
---@field headedTowardsAAtX number
---@field headedTowardsAAtY number
---@field spawnPoliceBAtX number
---@field spawnPoliceBAtY number
---@field headedTowardsBAtX number
---@field headedTowardsBAtY number

local WORLD_ADD_SET_PIECE_NODE_TYPE = "core.world_add_set_piece"

---@type LDNodeEditorNodeType
local worldAddSetPieceNode = {
    typeName = WORLD_ADD_SET_PIECE_NODE_TYPE,
    category = "opcode.world",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorWorldAddSetPieceNode
        local newNode = {
            id = newNodeId,
            nodeType = WORLD_ADD_SET_PIECE_NODE_TYPE,
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

            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.flow",
                }

            },
			type = 0,
			fromX = 0,
			fromY = 0,
			toX = 0,
			toY = 0,
			spawnPoliceAAtX = 0,
			spawnPoliceAAtY = 0,
			headedTowardsAAtX = 0,
			headedTowardsAAtY = 0,
			spawnPoliceBAtX = 0,
			spawnPoliceBAtY = 0,
			headedTowardsBAtX = 0,
			headedTowardsBAtY = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorWorldAddSetPieceNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "type", function ()
            node.type = editor.dataTypes[node.inputs[2].type].drawEditValue(node.type, "##typeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "fromX", function ()
            node.fromX = editor.dataTypes[node.inputs[3].type].drawEditValue(node.fromX, "##fromXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "fromY", function ()
            node.fromY = editor.dataTypes[node.inputs[4].type].drawEditValue(node.fromY, "##fromYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "toX", function ()
            node.toX = editor.dataTypes[node.inputs[5].type].drawEditValue(node.toX, "##toXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "toY", function ()
            node.toY = editor.dataTypes[node.inputs[6].type].drawEditValue(node.toY, "##toYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "spawnPoliceAAtX", function ()
            node.spawnPoliceAAtX = editor.dataTypes[node.inputs[7].type].drawEditValue(node.spawnPoliceAAtX, "##spawnPoliceAAtXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "spawnPoliceAAtY", function ()
            node.spawnPoliceAAtY = editor.dataTypes[node.inputs[8].type].drawEditValue(node.spawnPoliceAAtY, "##spawnPoliceAAtYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[9], "headedTowardsAAtX", function ()
            node.headedTowardsAAtX = editor.dataTypes[node.inputs[9].type].drawEditValue(node.headedTowardsAAtX, "##headedTowardsAAtXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[10], "headedTowardsAAtY", function ()
            node.headedTowardsAAtY = editor.dataTypes[node.inputs[10].type].drawEditValue(node.headedTowardsAAtY, "##headedTowardsAAtYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[11], "spawnPoliceBAtX", function ()
            node.spawnPoliceBAtX = editor.dataTypes[node.inputs[11].type].drawEditValue(node.spawnPoliceBAtX, "##spawnPoliceBAtXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[12], "spawnPoliceBAtY", function ()
            node.spawnPoliceBAtY = editor.dataTypes[node.inputs[12].type].drawEditValue(node.spawnPoliceBAtY, "##spawnPoliceBAtYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[13], "headedTowardsBAtX", function ()
            node.headedTowardsBAtX = editor.dataTypes[node.inputs[13].type].drawEditValue(node.headedTowardsBAtX, "##headedTowardsBAtXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[14], "headedTowardsBAtY", function ()
            node.headedTowardsBAtY = editor.dataTypes[node.inputs[14].type].drawEditValue(node.headedTowardsBAtY, "##headedTowardsBAtYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorWorldAddSetPieceNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local type = inputValues[2] or node.type
		local fromX = inputValues[3] or node.fromX
		local fromY = inputValues[4] or node.fromY
		local toX = inputValues[5] or node.toX
		local toY = inputValues[6] or node.toY
		local spawnPoliceAAtX = inputValues[7] or node.spawnPoliceAAtX
		local spawnPoliceAAtY = inputValues[8] or node.spawnPoliceAAtY
		local headedTowardsAAtX = inputValues[9] or node.headedTowardsAAtX
		local headedTowardsAAtY = inputValues[10] or node.headedTowardsAAtY
		local spawnPoliceBAtX = inputValues[11] or node.spawnPoliceBAtX
		local spawnPoliceBAtY = inputValues[12] or node.spawnPoliceBAtY
		local headedTowardsBAtX = inputValues[13] or node.headedTowardsBAtX
		local headedTowardsBAtY = inputValues[14] or node.headedTowardsBAtY
		WorldOp.addSetPiece(type, fromX, fromY, toX, toY, spawnPoliceAAtX, spawnPoliceAAtY, headedTowardsAAtX, headedTowardsAAtY, spawnPoliceBAtX, spawnPoliceBAtY, headedTowardsBAtX, headedTowardsBAtY)
        return {1}
    end
}

return worldAddSetPieceNode
