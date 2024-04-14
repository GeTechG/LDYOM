local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorBlipAddForPickupNode : LDNodeEditorNode
---@field pickup integer

local BLIP_ADD_FOR_PICKUP_NODE_TYPE = "core.blip_add_for_pickup"

---@type LDNodeEditorNodeType
local blipAddForPickupNode = {
    typeName = BLIP_ADD_FOR_PICKUP_NODE_TYPE,
    category = "opcode.blip",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorBlipAddForPickupNode
        local newNode = {
            id = newNodeId,
            nodeType = BLIP_ADD_FOR_PICKUP_NODE_TYPE,
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
                    type = "core.Pickup",
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
                    type = "core.number",
                }

            },
			pickup = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorBlipAddForPickupNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "pickup", function ()
            node.pickup = editor.dataTypes[node.inputs[2].type].drawEditValue(node.pickup, "##pickupEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "handle");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorBlipAddForPickupNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local pickup = inputValues[2] or node.pickup
		if pickup == nil or pickup == 0 then
            error("Input pickup is not set")
        end
		local handle = BlipOp.addForPickup(pickup)
        return {1, handle}
    end
}

return blipAddForPickupNode
