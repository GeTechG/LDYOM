local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorZoneSetPopulationTypeNode : LDNodeEditorNode
---@field zone string
---@field type integer

local ZONE_SET_POPULATION_TYPE_NODE_TYPE = "core.zone_set_population_type"

---@type LDNodeEditorNodeType
local zoneSetPopulationTypeNode = {
    typeName = ZONE_SET_POPULATION_TYPE_NODE_TYPE,
    category = "opcode.zone",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorZoneSetPopulationTypeNode
        local newNode = {
            id = newNodeId,
            nodeType = ZONE_SET_POPULATION_TYPE_NODE_TYPE,
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
			zone = "",
			type = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorZoneSetPopulationTypeNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "zone", function ()
            node.zone = editor.dataTypes[node.inputs[2].type].drawEditValue(node.zone, "##zoneEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "type", function ()
            node.type = editor.dataTypes[node.inputs[3].type].drawEditValue(node.type, "##typeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorZoneSetPopulationTypeNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local zone = inputValues[2] or node.zone
		local type = inputValues[3] or node.type
		ZoneOp.setPopulationType(zone, type)
        return {1}
    end
}

return zoneSetPopulationTypeNode