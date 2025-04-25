local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorWorldFireSingleBulletNode : LDNodeEditorNode
---@field fromX number
---@field fromY number
---@field fromZ number
---@field toX number
---@field toY number
---@field toZ number
---@field energy integer

local WORLD_FIRE_SINGLE_BULLET_NODE_TYPE = "core.world_fire_single_bullet"

---@type LDNodeEditorNodeType
local worldFireSingleBulletNode = {
    typeName = WORLD_FIRE_SINGLE_BULLET_NODE_TYPE,
    category = "opcode.world",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorWorldFireSingleBulletNode
        local newNode = {
            id = newNodeId,
            nodeType = WORLD_FIRE_SINGLE_BULLET_NODE_TYPE,
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

            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.flow",
                }

            },
			fromX = 0,
			fromY = 0,
			fromZ = 0,
			toX = 0,
			toY = 0,
			toZ = 0,
			energy = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorWorldFireSingleBulletNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "fromX", function ()
            node.fromX = editor.dataTypes[node.inputs[2].type].drawEditValue(node.fromX, "##fromXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "fromY", function ()
            node.fromY = editor.dataTypes[node.inputs[3].type].drawEditValue(node.fromY, "##fromYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "fromZ", function ()
            node.fromZ = editor.dataTypes[node.inputs[4].type].drawEditValue(node.fromZ, "##fromZEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "toX", function ()
            node.toX = editor.dataTypes[node.inputs[5].type].drawEditValue(node.toX, "##toXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "toY", function ()
            node.toY = editor.dataTypes[node.inputs[6].type].drawEditValue(node.toY, "##toYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "toZ", function ()
            node.toZ = editor.dataTypes[node.inputs[7].type].drawEditValue(node.toZ, "##toZEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "energy", function ()
            node.energy = editor.dataTypes[node.inputs[8].type].drawEditValue(node.energy, "##energyEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorWorldFireSingleBulletNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local fromX = inputValues[2] or node.fromX
		local fromY = inputValues[3] or node.fromY
		local fromZ = inputValues[4] or node.fromZ
		local toX = inputValues[5] or node.toX
		local toY = inputValues[6] or node.toY
		local toZ = inputValues[7] or node.toZ
		local energy = inputValues[8] or node.energy
		WorldOp.fireSingleBullet(fromX, fromY, fromZ, toX, toY, toZ, math.floor(energy))
        return {1}
    end
}

return worldFireSingleBulletNode
