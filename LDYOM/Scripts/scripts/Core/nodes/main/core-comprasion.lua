local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")

---@class LDNodeEditorComprasionNode : LDNodeEditorNode
---@field comprasionType string
---@field conditionType MathCondition
---@field compareValues any[]

local COMPRASION_NODE_TYPE = "core.comprasion"

---@type LDNodeEditorNodeType
local comprasionNode = {
    typeName = COMPRASION_NODE_TYPE,
    category = "main",
    icon = nodesIcons["comprasion"],
    color = nodesColors["comprasion"],
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorComprasionNode
        local newNode = {
            id = newNodeId,
            nodeType = COMPRASION_NODE_TYPE,
            inputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.number",
                },
                {
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
                    type = "core.bool",
                }
            },
            comprasionType = "core.number",
            conditionType = MathCondition.Equal,
            compareValues = {0, 0}
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorComprasionNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        builder:SetStage(BlueprintStage.Input);

        ImGui.Text(ld.loc.get("general.type"));
        ImGui.SameLine(0, -1);
        if ImGui.Button(ld.loc.get("nodes.types." .. node.comprasionType), ImVec2.new()) then
            NodeEditor.Suspend();
            ImGui.OpenPopup("##comprasionTypeSelector", 0);
            NodeEditor.Resume();
        end

        if ImGui.IsPopupOpen("##comprasionTypeSelector", 0) then
            NodeEditor.Suspend();
            if ImGui.BeginPopup("##comprasionTypeSelector", 0) then
                if ImGui.BeginListBox("##comprasionTypeSelectorChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                    for id, type in pairs(editor.dataTypes) do
                        if not type.isMakeable then
                            goto continue;
                        end
                        NodeEditor.Icon(ImVec2.new(fontScale * 16, fontScale * 16), NodeEditorIconType.Circle, true, editor.dataTypes[id].colorGetter(), ImVec4.new());
                        ImGui.SameLine(0, -1);
                        if ImGui.Selectable(ld.loc.get("nodes.types." .. id), node.comprasionType == id, 0, ImVec2.new(0, 0)) then
                            node.comprasionType = id;
                            node.compareValues = {type.makeNew(), type.makeNew()};
                            node.inputs[1].type = id;
                            node.inputs[2].type = id;
                            table.erase_if(ctx.__links, function(link)
                                return link.inputId == node.inputs[1].id or link.inputId == node.inputs[2].id;
                            end);
                            ImGui.CloseCurrentPopup();
                        end
                        ::continue::
                    end
                    ImGui.EndListBox();
                end
                ImGui.EndPopup();
            end
            NodeEditor.Resume();
        end

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "A", function ()
            node.compareValues[1] = editor.dataTypes[node.comprasionType].drawEditValue(node.compareValues[1], "##A", fontScale * 150)
        end);

        if node.comprasionType == "core.number" then
            ImGui.Text(ld.loc.get("general.condition"));
            ImGui.SameLine(0, -1);
            local compareTypes = ld.loc.getArray("general.compare_types");
            if ImGui.Button(compareTypes[node.conditionType + 1], ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##conditionTypeSelector", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##conditionTypeSelector", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##conditionTypeSelector", 0) then
                    if ImGui.BeginListBox("##conditionTypeSelectorChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        local compareTypes = ld.loc.getArray("general.compare_types");
                        for id, type in ipairs(compareTypes) do
                            if ImGui.Selectable(type, node.conditionType + 1 == id, 0, ImVec2.new(0, 0)) then
                                node.conditionType = id - 1;
                                ImGui.CloseCurrentPopup();
                            end
                        end
                        ImGui.EndListBox();
                    end
                    ImGui.EndPopup();
                end
                NodeEditor.Resume();
            end
        else
            local lastItemSize = ImVec2.new();
            ImGui.GetItemRectSize(lastItemSize);
            ImGui.SetCursorPosX(ImGui.GetCursorPosX() + lastItemSize.x / 2 - 5 * fontScale);
            ImGui.Text(fa.ICON_FA_EQUALS);
        end

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "B", function ()
            node.compareValues[2] = editor.dataTypes[node.comprasionType].drawEditValue(node.compareValues[2], "##B", fontScale * 150)
        end);

        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], ld.loc.get("general.result"));

        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorComprasionNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local aValue = inputValues[1] or node.compareValues[1];
        local bValue = inputValues[2] or node.compareValues[2];

        local resultCondition = false;
        if node.comprasionType == "core.number" then
            if node.conditionType == MathCondition.Equal then
                resultCondition = aValue == bValue;
            elseif node.conditionType == MathCondition.NotEqual then
                resultCondition = aValue ~= bValue;
            elseif node.conditionType == MathCondition.More then
                resultCondition = aValue > bValue;
            elseif node.conditionType == MathCondition.MoreOrEqual then
                resultCondition = aValue >= bValue;
            elseif node.conditionType == MathCondition.Less then
                resultCondition = aValue < bValue;
            elseif node.conditionType == MathCondition.LessOrEqual then
                resultCondition = aValue <= bValue;
            end
        else
            resultCondition = editor.dataTypes[node.comprasionType].equals(aValue, bValue);
        end

        return {resultCondition}
    end
}

return comprasionNode