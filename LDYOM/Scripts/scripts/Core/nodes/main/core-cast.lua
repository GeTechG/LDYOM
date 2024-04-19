local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")

---@class LDNodeEditorCastNode : LDNodeEditorNode
---@field typeIn string
---@field valueIn any
---@field typeCast string

local CAST_NODE_TYPE = "core.cast"

---@type LDNodeEditorNodeType
local castNode = {
    typeName = CAST_NODE_TYPE,
    category = "main",
    icon = nodesIcons["cast"],
    color = nodesColors["cast"],
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCastNode
        local newNode = {
            id = newNodeId,
            nodeType = CAST_NODE_TYPE,
            inputs = {
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
                    type = "core.number",
                }
            },
            typeIn = "core.number",
            typeCast = "core.number",
            valueIn = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCastNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        builder:SetStage(BlueprintStage.Input);

        ImGui.Text(ld.loc.get("general.type"));
        ImGui.SameLine(0, -1);
        if ImGui.Button(ld.loc.get("nodes.types." .. node.typeIn), ImVec2.new()) then
            NodeEditor.Suspend();
            ImGui.OpenPopup("##castTypeSelector", 0);
            NodeEditor.Resume();
        end

        if ImGui.IsPopupOpen("##castTypeSelector", 0) then
            NodeEditor.Suspend();
            if ImGui.BeginPopup("##castTypeSelector", 0) then
                if ImGui.BeginListBox("##castTypeSelectorChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                    for id, type in pairs(editor.dataTypes) do
                        if not type.isMakeable then
                            goto continue;
                        end
                        NodeEditor.Icon(ImVec2.new(fontScale * 16, fontScale * 16), NodeEditorIconType.Circle, true, editor.dataTypes[id].colorGetter(), ImVec4.new());
                        ImGui.SameLine(0, -1);
                        if ImGui.Selectable(ld.loc.get("nodes.types." .. id), node.typeIn == id, 0, ImVec2.new(0, 0)) then
                            node.typeIn = id;
                            node.valueIn = type.makeNew();
                            node.inputs[1].type = id;
                            table.erase_if(ctx.__links, function(link)
                                return link.inputId == node.inputs[1].id;
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

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "Value", function ()
            node.valueIn = editor.dataTypes[node.typeIn].drawEditValue(node.valueIn, "##Value", fontScale * 150)
        end);

        builder:SetStage(BlueprintStage.Middle);

        ImGui.Text(fa.ICON_FA_CHEVRON_DOUBLE_RIGHT);
        ImGui.TextColored(ImVec4.new(1, 1, 0, 1), fa.ICON_FA_EXCLAMATION_TRIANGLE);

        if ImGui.IsItemHovered(0) then
            NodeEditor.Suspend();
            ImGui.SetTooltip(ld.loc.get("nodes.core.cast.warning"));
            NodeEditor.Resume();
        end

        builder:SetStage(BlueprintStage.Output);

        ImGui.Text(ld.loc.get("general.type"));
        ImGui.SameLine(0, -1);
        if ImGui.Button(ld.loc.get("nodes.types." .. node.typeCast), ImVec2.new()) then
            NodeEditor.Suspend();
            ImGui.OpenPopup("##castTypeSelector", 0);
            NodeEditor.Resume();
        end

        if ImGui.IsPopupOpen("##castTypeSelector", 0) then
            NodeEditor.Suspend();
            if ImGui.BeginPopup("##castTypeSelector", 0) then
                if ImGui.BeginListBox("##castTypeSelectorChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                    for id, type in pairs(editor.dataTypes) do
                        if not type.isMakeable then
                            goto continue;
                        end
                        NodeEditor.Icon(ImVec2.new(fontScale * 16, fontScale * 16), NodeEditorIconType.Circle, true, editor.dataTypes[id].colorGetter(), ImVec4.new());
                        ImGui.SameLine(0, -1);
                        if ImGui.Selectable(ld.loc.get("nodes.types." .. id), node.typeCast == id, 0, ImVec2.new(0, 0)) then
                            node.typeCast = id;
                            node.outputs[1].type = id;
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

        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCastNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local value = inputValues[1] or node.valueIn

        return {value}
    end
}

return castNode
