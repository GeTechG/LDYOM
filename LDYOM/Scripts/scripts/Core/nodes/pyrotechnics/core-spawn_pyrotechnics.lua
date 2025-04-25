local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")

---@class LDNodeEditorSpawnPyrotechnicsNode : LDNodeEditorNode
---@field sceneId integer
---@field entityUuid string

local SPAWN_PYROTECHNICS_NODE_TYPE = "core.spawn_pyrotechnics"

---@type LDNodeEditorNodeType
local spawnPyrotechnicsNode = {
    typeName = SPAWN_PYROTECHNICS_NODE_TYPE,
    category = "pyrotechnics",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorSpawnPyrotechnicsNode
        local newNode = {
            id = newNodeId,
            nodeType = SPAWN_PYROTECHNICS_NODE_TYPE,
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
    ---@param node LDNodeEditorSpawnPyrotechnicsNode
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
            local pyrotechnics = scene:getPyrotechnics();
            local indexPyrotechnics = ld.indexByUuid(pyrotechnics, node.entityUuid);

            ImGui.Text(ld.loc.get("entities.pyrotechnics"));
            ImGui.SameLine(0, -1);
            if ImGui.Button((indexPyrotechnics ~= -1 and pyrotechnics[indexPyrotechnics + 1]:getName()) or "<None>", ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##entityUuidSelect", 0);
                NodeEditor.Resume();
            end

            if autoBindRequireFields then
                if node.entityUuid == "" and #pyrotechnics > 0 then
                    node.entityUuid = tostring(pyrotechnics[#pyrotechnics]:getUuid());
                end
            end

            if ImGui.IsPopupOpen("##entityUuidSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##entityUuidSelect", 0) then
                    if ImGui.BeginListBox("##entityUuidSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for i, pyrotechnics in ipairs(pyrotechnics) do
                            if ImGui.Selectable(pyrotechnics:getName(), node.entityUuid == tostring(pyrotechnics:getUuid()), 0, ImVec2.new()) then
                                node.entityUuid = tostring(pyrotechnics:getUuid());
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
    ---@param node LDNodeEditorSpawnPyrotechnicsNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        if node.sceneId == 0 then
            error("Scene is not set!")
        end
        if node.entityUuid == "" then
            error("Pyrotechnics is not set!")
        end
        local pyrotechnics = ld.projectsService():getCurrentProject():getScenes()[node.sceneId]:getPyrotechnics();
        local indexPyrotechnics = ld.indexByUuid(pyrotechnics, node.entityUuid);
        if indexPyrotechnics == -1 then
            error("Pyrotechnics not found!")
        end
        local pyrotechnic = pyrotechnics[indexPyrotechnics + 1];
        pyrotechnic:spawnProjectEntity();
        return {1}
    end
}

return spawnPyrotechnicsNode
