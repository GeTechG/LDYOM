local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")

---@class LDNodeEditorGetCharNode : LDNodeEditorNode
---@field sceneId integer
---@field entityUuid string

local GET_CHAR_NODE_TYPE = "core.get_char"

---@type LDNodeEditorNodeType
local spawnActorNode = {
    typeName = GET_CHAR_NODE_TYPE,
    category = "actor",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorGetCharNode
        local newNode = {
            id = newNodeId,
            nodeType = GET_CHAR_NODE_TYPE,
            buffer = "",
            inputs = {
            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.Char",
                }
            },
            sceneId = 0,
            entityUuid = ""
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorGetCharNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        builder:SetStage(BlueprintStage.Input);

        ImGui.BeginGroup();

        local scene = ld.projectsService():getCurrentProject():getScenes()[node.sceneId];

        ImGui.Text(ld.loc.get("scenes.scene"));
        ImGui.SameLine(0, -1);
        if ImGui.Button((scene ~= nil and scene:getName()) or "<None>", ImVec2.new()) then
            NodeEditor.Suspend();
            ImGui.OpenPopup("##sceneIdSelect", 0);
            NodeEditor.Resume();
        end

        if ImGui.IsPopupOpen("##sceneIdSelect", 0) then
            NodeEditor.Suspend();
            if ImGui.BeginPopup("##sceneIdSelect", 0) then
                if ImGui.BeginListBox("##sceneIdSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                    for id, scene in pairs(ld.projectsService():getCurrentProject():getScenes()) do
                        if ImGui.Selectable(scene:getName(), node.id == id, 0, ImVec2.new()) then
                            node.sceneId = id;
                            ImGui.CloseCurrentPopup();
                        end
                    end
                    ImGui.EndListBox();
                end
                ImGui.EndPopup();
            end
            NodeEditor.Resume();
        end

        local autoBindRequireFields = ld.settings.getBool("main.autoBindRequireFields");
        if autoBindRequireFields == nil then
            autoBindRequireFields = true;
        end

        if autoBindRequireFields then
            if node.sceneId == 0 then
                node.sceneId = ld.projectsService():getCurrentProject():getCurrentSceneIndex()
            end
        end

        if scene then
            local actors = scene:getActors();
            local indexActor = ld.indexByUuid(actors, node.entityUuid);

            ImGui.Text(ld.loc.get("entities.actor"));
            ImGui.SameLine(0, -1);
            if ImGui.Button((indexActor ~= -1 and actors[indexActor + 1]:getName()) or "<None>", ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##entityUuidSelect", 0);
                NodeEditor.Resume();
            end

            if autoBindRequireFields then
                if node.entityUuid == "" and #actors > 0 then
                    node.entityUuid = tostring(actors[#actors]:getUuid());
                end
            end

            if ImGui.IsPopupOpen("##entityUuidSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##entityUuidSelect", 0) then
                    if ImGui.BeginListBox("##entityUuidSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for i, actor in ipairs(actors) do
                            if ImGui.Selectable(actor:getName(), node.entityUuid == tostring(actor:getUuid()), 0, ImVec2.new()) then
                                node.entityUuid = tostring(actor:getUuid());
                                ImGui.CloseCurrentPopup();
                            end
                        end
                        ImGui.EndListBox();
                    end
                    ImGui.EndPopup();
                end
                NodeEditor.Resume();
            end
        end

        ImGui.EndGroup();
        
        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorGetCharNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        if node.sceneId == 0 then
            error("Scene is not set!")
        end
        if node.entityUuid == "" then
            error("Actor is not set!")
        end
        local actors = ld.projectsService():getCurrentProject():getScenes()[node.sceneId]:getActors();
        local indexActor = ld.indexByUuid(actors, node.entityUuid);
        if indexActor == -1 then
            error("Actor not found!")
        end
        local actor = actors[indexActor + 1];
        local ped = actor:getProjectPed();
        if ped == nil then
            error("Actor has no ped!")
        end
        return {GetPedRef(ped)};
    end
}

return spawnActorNode