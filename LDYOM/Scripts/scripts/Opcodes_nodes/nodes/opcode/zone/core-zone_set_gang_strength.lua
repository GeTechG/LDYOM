local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorZoneSetGangStrengthNode : LDNodeEditorNode
---@field zoneId string
---@field gangId integer
---@field density integer

local ZONE_SET_GANG_STRENGTH_NODE_TYPE = "core.zone_set_gang_strength"

---@type LDNodeEditorNodeType
local zoneSetGangStrengthNode = {
    typeName = ZONE_SET_GANG_STRENGTH_NODE_TYPE,
    category = "opcode.zone",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorZoneSetGangStrengthNode
        local newNode = {
            id = newNodeId,
            nodeType = ZONE_SET_GANG_STRENGTH_NODE_TYPE,
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
			zoneId = "",
			gangId = 0,
			density = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorZoneSetGangStrengthNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "zoneId", function ()
            node.zoneId = editor.dataTypes[node.inputs[2].type].drawEditValue(node.zoneId, "##zoneIdEdit", fontScale * 100)
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "density", function ()
            node.density = editor.dataTypes[node.inputs[4].type].drawEditValue(node.density, "##densityEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorZoneSetGangStrengthNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local zoneId = inputValues[2] or node.zoneId
		local gangId = inputValues[3] or node.gangId
		local density = inputValues[4] or node.density
		ZoneOp.setGangStrength(zoneId, gangId, math.floor(density))
        return {1}
    end
}

return zoneSetGangStrengthNode
