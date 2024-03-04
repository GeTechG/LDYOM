local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorGameSetRelationshipNode : LDNodeEditorNode
---@field relationshipType integer
---@field ofPedType integer
---@field toPedType integer

local GAME_SET_RELATIONSHIP_NODE_TYPE = "core.game_set_relationship"

---@type LDNodeEditorNodeType
local gameSetRelationshipNode = {
    typeName = GAME_SET_RELATIONSHIP_NODE_TYPE,
    category = "opcode.game",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorGameSetRelationshipNode
        local newNode = {
            id = newNodeId,
            nodeType = GAME_SET_RELATIONSHIP_NODE_TYPE,
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

            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.flow",
                }

            },
			relationshipType = 0,
			ofPedType = 0,
			toPedType = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorGameSetRelationshipNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "relationshipType", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.RelationshipType) do
                if value == node.relationshipType then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##relationshipTypeSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##relationshipTypeSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##relationshipTypeSelect", 0) then
                    if ImGui.BeginListBox("##relationshipTypeSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.RelationshipType) do
                            if ImGui.Selectable(key, value == node.relationshipType, 0, ImVec2.new()) then
                                node.relationshipType = value;
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "ofPedType", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.PedType) do
                if value == node.ofPedType then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##ofPedTypeSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##ofPedTypeSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##ofPedTypeSelect", 0) then
                    if ImGui.BeginListBox("##ofPedTypeSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.PedType) do
                            if ImGui.Selectable(key, value == node.ofPedType, 0, ImVec2.new()) then
                                node.ofPedType = value;
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "toPedType", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.PedType) do
                if value == node.toPedType then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##toPedTypeSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##toPedTypeSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##toPedTypeSelect", 0) then
                    if ImGui.BeginListBox("##toPedTypeSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.PedType) do
                            if ImGui.Selectable(key, value == node.toPedType, 0, ImVec2.new()) then
                                node.toPedType = value;
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


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorGameSetRelationshipNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local relationshipType = inputValues[2] or node.relationshipType
		local ofPedType = inputValues[3] or node.ofPedType
		local toPedType = inputValues[4] or node.toPedType
		GameOp.setRelationship(relationshipType, ofPedType, toPedType)
        return {1}
    end
}

return gameSetRelationshipNode
