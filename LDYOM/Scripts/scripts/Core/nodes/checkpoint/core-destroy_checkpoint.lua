local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")

---@class LDNodeEditorDestroyCheckpointNode : LDNodeEditorNode
---@field sceneId integer
---@field entityUuid string

local DESTROY_CHECKPOINT_NODE_TYPE = "core.destroy_checkpoint"

---@type LDNodeEditorNodeType
local destroyCheckpointNode = {
    typeName = DESTROY_CHECKPOINT_NODE_TYPE,
    category = "checkpoint",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorDestroyCheckpointNode
        local newNode = {
            id = newNodeId,
            nodeType = DESTROY_CHECKPOINT_NODE_TYPE,
            buffer = "",
            inputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.flow",
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
            sceneId = 0,
            entityUuid = ""
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorDestroyCheckpointNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");

        ImGui.SameLine(0, -1);

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
            local checkpoints = scene:getCheckpoints();
            local indexCheckpoint = ld.indexByUuid(checkpoints, node.entityUuid);

            ImGui.Text(ld.loc.get("entities.checkpoint"));
            ImGui.SameLine(0, -1);
            if ImGui.Button((indexCheckpoint ~= -1 and checkpoints[indexCheckpoint + 1]:getName()) or "<None>", ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##entityUuidSelect", 0);
                NodeEditor.Resume();
            end

            if autoBindRequireFields then
                if node.entityUuid == "" and #checkpoints > 0 then
                    node.entityUuid = tostring(checkpoints[#checkpoints]:getUuid());
                end
            end

            if ImGui.IsPopupOpen("##entityUuidSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##entityUuidSelect", 0) then
                    if ImGui.BeginListBox("##entityUuidSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for i, checkpoint in ipairs(checkpoints) do
                            if ImGui.Selectable(checkpoint:getName(), node.entityUuid == tostring(checkpoint:getUuid()), 0, ImVec2.new()) then
                                node.entityUuid = tostring(checkpoint:getUuid());
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
    ---@param node LDNodeEditorDestroyCheckpointNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        if node.sceneId == 0 then
            error("Scene is not set!")
        end
        if node.entityUuid == "" then
            error("Checkpoint is not set!")
        end
        local checkpoints = ld.projectsService():getCurrentProject():getScenes()[node.sceneId]:getCheckpoints();
        local indexCheckpoint = ld.indexByUuid(checkpoints, node.entityUuid);
        if indexCheckpoint == -1 then
            error("Checkpoint not found!")
        end
        local checkpoint = checkpoints[indexCheckpoint + 1];
        checkpoint:deleteProjectEntity();
        return {1}
    end
}

return destroyCheckpointNode
