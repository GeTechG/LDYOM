local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorZoneGetGangStrengthNode : LDNodeEditorNode
---@field zone string
---@field gangId integer

local ZONE_GET_GANG_STRENGTH_NODE_TYPE = "core.zone_get_gang_strength"

---@type LDNodeEditorNodeType
local zoneGetGangStrengthNode = {
    typeName = ZONE_GET_GANG_STRENGTH_NODE_TYPE,
    category = "opcode.zone",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorZoneGetGangStrengthNode
        local newNode = {
            id = newNodeId,
            nodeType = ZONE_GET_GANG_STRENGTH_NODE_TYPE,
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
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }

            },
			zone = "",
			gangId = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorZoneGetGangStrengthNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "zone", function ()
            node.zone = editor.dataTypes[node.inputs[2].type].drawEditValue(node.zone, "##zoneEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "gangId", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.GangType) do
                if value == node.gangId then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##gangIdSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##gangIdSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##gangIdSelect", 0) then
                    if ImGui.BeginListBox("##gangIdSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.GangType) do
                            if ImGui.Selectable(key, value == node.gangId, 0, ImVec2.new()) then
                                node.gangId = value;
                                ImGui.CloseCurrentPopup();
                            end
                        end
                        ImGui.EndListBox();
                    end
                    ImGui.EndPopup();
                end
                NodeEditor.Resume();
            end
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "density");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorZoneGetGangStrengthNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local zone = inputValues[2] or node.zone
		local gangId = inputValues[3] or node.gangId
		local density = ZoneOp.getGangStrength(zone, gangId)
        return {1, density}
    end
}

return zoneGetGangStrengthNode
