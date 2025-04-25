local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")

---@class LDNodeEditorIsCurrentObjectiveNode : LDNodeEditorNode
---@field sceneId integer
---@field objectiveUuid string

local IS_CURRENT_OBJECTIVE_NODE_TYPE = "core.is_current_objective"

---@type LDNodeEditorNodeType
local onStartSceneNode = {
    typeName = IS_CURRENT_OBJECTIVE_NODE_TYPE,
    category = "objective",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorIsCurrentObjectiveNode
        local newNode = {
            id = newNodeId,
            nodeType = IS_CURRENT_OBJECTIVE_NODE_TYPE,
            inputs = {},
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.bool",
                }
            },
            sceneId = 0,
            objectiveUuid = "",
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorIsCurrentObjectiveNode
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
            local objectives = scene:getObjectives();
            local indexObjective = ld.indexByUuid(objectives, node.objectiveUuid);

            ImGui.Text(ld.loc.get("objective.title"));
            ImGui.SameLine(0, -1);
            if ImGui.Button((indexObjective ~= -1 and objectives[indexObjective + 1]:getName()) or "<None>", ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##objectiveUuidSelect", 0);
                NodeEditor.Resume();
            end

            if autoBindRequireFields then
                if node.objectiveUuid == "" and #objectives > 0 then
                    node.objectiveUuid = tostring(objectives[#objectives]:getUuid());
                end
            end

            if ImGui.IsPopupOpen("##objectiveUuidSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##objectiveUuidSelect", 0) then
                    if ImGui.BeginListBox("##objectiveUuidSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for i, objective in ipairs(objectives) do
                            if ImGui.Selectable(objective:getName(), node.objectiveUuid == tostring(objective:getUuid()), 0, ImVec2.new()) then
                                node.objectiveUuid = tostring(objective:getUuid());
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
    ---@param node LDNodeEditorIsCurrentObjectiveNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        return {tostring(ld.projectPlayerService:getCurrentObjective():getUuid()) == node.objectiveUuid}
    end
}

return onStartSceneNode