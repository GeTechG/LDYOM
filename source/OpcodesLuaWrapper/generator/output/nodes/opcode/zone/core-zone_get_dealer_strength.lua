local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorZoneGetDealerStrengthNode : LDNodeEditorNode
---@field zone string

local ZONE_GET_DEALER_STRENGTH_NODE_TYPE = "core.zone_get_dealer_strength"

---@type LDNodeEditorNodeType
local zoneGetDealerStrengthNode = {
    typeName = ZONE_GET_DEALER_STRENGTH_NODE_TYPE,
    category = "opcode.zone",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorZoneGetDealerStrengthNode
        local newNode = {
            id = newNodeId,
            nodeType = ZONE_GET_DEALER_STRENGTH_NODE_TYPE,
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
                    type = "core.string",
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
			zone = ""
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorZoneGetDealerStrengthNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "zone", function ()
            node.zone = editor.dataTypes[node.inputs[2].type].drawEditValue(node.zone, "##zoneEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "density");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorZoneGetDealerStrengthNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local zone = inputValues[2] or node.zone
		local density = ZoneOp.getDealerStrength(zone)
        return {1, density}
    end
}

return zoneGetDealerStrengthNode