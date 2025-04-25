local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorBlipAddForCarOldNode : LDNodeEditorNode
---@field vehicle integer
---@field color integer
---@field display integer

local BLIP_ADD_FOR_CAR_OLD_NODE_TYPE = "core.blip_add_for_car_old"

---@type LDNodeEditorNodeType
local blipAddForCarOldNode = {
    typeName = BLIP_ADD_FOR_CAR_OLD_NODE_TYPE,
    category = "opcode.blip",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorBlipAddForCarOldNode
        local newNode = {
            id = newNodeId,
            nodeType = BLIP_ADD_FOR_CAR_OLD_NODE_TYPE,
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
                    type = "core.Vehicle",
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
                    type = "core.number",
                }

            },
			vehicle = 0,
			color = 0,
			display = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorBlipAddForCarOldNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "vehicle", function ()
            node.vehicle = editor.dataTypes[node.inputs[2].type].drawEditValue(node.vehicle, "##vehicleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "color", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.BlipColor) do
                if value == node.color then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##colorSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##colorSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##colorSelect", 0) then
                    if ImGui.BeginListBox("##colorSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.BlipColor) do
                            if ImGui.Selectable(key, value == node.color, 0, ImVec2.new()) then
                                node.color = value;
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "display", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.BlipDisplay) do
                if value == node.display then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##displaySelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##displaySelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##displaySelect", 0) then
                    if ImGui.BeginListBox("##displaySelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.BlipDisplay) do
                            if ImGui.Selectable(key, value == node.display, 0, ImVec2.new()) then
                                node.display = value;
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

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "handle");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorBlipAddForCarOldNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local vehicle = inputValues[2] or node.vehicle
		if vehicle == nil or vehicle == 0 then
            error("Input vehicle is not set")
        end
		local color = inputValues[3] or node.color
		local display = inputValues[4] or node.display
		local handle = BlipOp.addForCarOld(vehicle, color, display)
        return {1, handle}
    end
}

return blipAddForCarOldNode
