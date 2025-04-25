local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCharSetRelationshipNode : LDNodeEditorNode
---@field self_ integer
---@field relationshipType integer
---@field pedType integer

local CHAR_SET_RELATIONSHIP_NODE_TYPE = "core.char_set_relationship"

---@type LDNodeEditorNodeType
local charSetRelationshipNode = {
    typeName = CHAR_SET_RELATIONSHIP_NODE_TYPE,
    category = "opcode.char",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCharSetRelationshipNode
        local newNode = {
            id = newNodeId,
            nodeType = CHAR_SET_RELATIONSHIP_NODE_TYPE,
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
                    type = "core.Char",
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
			self_ = 0,
			relationshipType = 0,
			pedType = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCharSetRelationshipNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[2].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "relationshipType", function ()
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "pedType", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.PedType) do
                if value == node.pedType then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##pedTypeSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##pedTypeSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##pedTypeSelect", 0) then
                    if ImGui.BeginListBox("##pedTypeSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.PedType) do
                            if ImGui.Selectable(key, value == node.pedType, 0, ImVec2.new()) then
                                node.pedType = value;
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
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCharSetRelationshipNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local self_ = inputValues[2] or node.self_
		if self_ == nil or self_ == 0 then
            error("Input self is not set")
        end
		local relationshipType = inputValues[3] or node.relationshipType
		local pedType = inputValues[4] or node.pedType
		CharOp.setRelationship(self_, relationshipType, pedType)
        return {1}
    end
}

return charSetRelationshipNode
