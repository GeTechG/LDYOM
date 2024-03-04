local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorZoneGetCurrentPopulationZoneTypeNode : LDNodeEditorNode


local ZONE_GET_CURRENT_POPULATION_ZONE_TYPE_NODE_TYPE = "core.zone_get_current_population_zone_type"

---@type LDNodeEditorNodeType
local zoneGetCurrentPopulationZoneTypeNode = {
    typeName = ZONE_GET_CURRENT_POPULATION_ZONE_TYPE_NODE_TYPE,
    category = "opcode.zone",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorZoneGetCurrentPopulationZoneTypeNode
        local newNode = {
            id = newNodeId,
            nodeType = ZONE_GET_CURRENT_POPULATION_ZONE_TYPE_NODE_TYPE,
            inputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.flow",
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

            }
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorZoneGetCurrentPopulationZoneTypeNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "type");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorZoneGetCurrentPopulationZoneTypeNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local type = ZoneOp.getCurrentPopulationZoneType()
        return {1, type}
    end
}

return zoneGetCurrentPopulationZoneTypeNode
