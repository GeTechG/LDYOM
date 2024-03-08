local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorWorldIsMoneyPickupAtCoordsNode : LDNodeEditorNode
---@field x number
---@field y number
---@field z number

local WORLD_IS_MONEY_PICKUP_AT_COORDS_NODE_TYPE = "core.world_is_money_pickup_at_coords"

---@type LDNodeEditorNodeType
local worldIsMoneyPickupAtCoordsNode = {
    typeName = WORLD_IS_MONEY_PICKUP_AT_COORDS_NODE_TYPE,
    category = "opcode.world",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorWorldIsMoneyPickupAtCoordsNode
        local newNode = {
            id = newNodeId,
            nodeType = WORLD_IS_MONEY_PICKUP_AT_COORDS_NODE_TYPE,
            inputs = {
                {
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
                    type = "core.bool",
                }

            },
			x = 0,
			y = 0,
			z = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorWorldIsMoneyPickupAtCoordsNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "x", function ()
            node.x = editor.dataTypes[node.inputs[1].type].drawEditValue(node.x, "##xEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "y", function ()
            node.y = editor.dataTypes[node.inputs[2].type].drawEditValue(node.y, "##yEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "z", function ()
            node.z = editor.dataTypes[node.inputs[3].type].drawEditValue(node.z, "##zEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorWorldIsMoneyPickupAtCoordsNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local x = inputValues[1] or node.x
		local y = inputValues[2] or node.y
		local z = inputValues[3] or node.z
		local result = WorldOp.isMoneyPickupAtCoords(x, y, z)
        return {result}
    end
}

return worldIsMoneyPickupAtCoordsNode
