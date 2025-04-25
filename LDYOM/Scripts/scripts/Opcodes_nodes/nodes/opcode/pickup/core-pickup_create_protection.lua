local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorPickupCreateProtectionNode : LDNodeEditorNode
---@field x number
---@field y number
---@field z number
---@field revenueLimit integer
---@field revenueRate integer

local PICKUP_CREATE_PROTECTION_NODE_TYPE = "core.pickup_create_protection"

---@type LDNodeEditorNodeType
local pickupCreateProtectionNode = {
    typeName = PICKUP_CREATE_PROTECTION_NODE_TYPE,
    category = "opcode.pickup",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorPickupCreateProtectionNode
        local newNode = {
            id = newNodeId,
            nodeType = PICKUP_CREATE_PROTECTION_NODE_TYPE,
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

            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.flow",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.Pickup",
                }

            },
			x = 0,
			y = 0,
			z = 0,
			revenueLimit = 0,
			revenueRate = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorPickupCreateProtectionNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "x", function ()
            node.x = editor.dataTypes[node.inputs[2].type].drawEditValue(node.x, "##xEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "y", function ()
            node.y = editor.dataTypes[node.inputs[3].type].drawEditValue(node.y, "##yEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "z", function ()
            node.z = editor.dataTypes[node.inputs[4].type].drawEditValue(node.z, "##zEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "revenueLimit", function ()
            node.revenueLimit = editor.dataTypes[node.inputs[5].type].drawEditValue(node.revenueLimit, "##revenueLimitEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "revenueRate", function ()
            node.revenueRate = editor.dataTypes[node.inputs[6].type].drawEditValue(node.revenueRate, "##revenueRateEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "handle");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorPickupCreateProtectionNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local x = inputValues[2] or node.x
		local y = inputValues[3] or node.y
		local z = inputValues[4] or node.z
		local revenueLimit = inputValues[5] or node.revenueLimit
		local revenueRate = inputValues[6] or node.revenueRate
		local handle = PickupOp.createProtection(x, y, z, math.floor(revenueLimit), math.floor(revenueRate))
        return {1, handle}
    end
}

return pickupCreateProtectionNode
