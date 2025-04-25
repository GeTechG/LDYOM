local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")

---@class LDNodeEditorOnStartSceneNode : LDNodeEditorNode
---@field sceneId integer

local ON_START_SCENE_NODE_TYPE = "core.on_start_scene"

---@type LDNodeEditorNodeType
local onStartSceneNode = {
    typeName = ON_START_SCENE_NODE_TYPE,
    category = "scene",
    icon = nodesIcons["event"],
    color = nodesColors["event"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorOnStartSceneNode
        local newNode = {
            id = newNodeId,
            nodeType = ON_START_SCENE_NODE_TYPE,
            inputs = {},
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.flow",
                }
            },
            sceneId = 0,
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorOnStartSceneNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);
        ld.projectsService():getCurrentProject():getScenes();


        local scene = ld.projectsService():getCurrentProject():getScenes()[node.sceneId];

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

        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");
        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorOnStartSceneNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        return {1}
    end
}

return onStartSceneNode