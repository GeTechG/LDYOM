local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")

---@class LDNodeEditorPlayCameraPathNode : LDNodeEditorNode
---@field sceneId integer
---@field entityUuid string
---@field startFadeOut boolean
---@field startFadeOutTime number
---@field endFadeIn boolean
---@field endFadeInTime number
---@field lockPlayerControl boolean

local PLAY_CAMERA_PATH_NODE_TYPE = "core.play_camera_path"

---@type LDNodeEditorNodeType
local playCameraPathNode = {
    typeName = PLAY_CAMERA_PATH_NODE_TYPE,
    category = "camera_path",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorPlayCameraPathNode
        local newNode = {
            id = newNodeId,
            nodeType = PLAY_CAMERA_PATH_NODE_TYPE,
            buffer = "",
            inputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.flow",
                },
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.bool",
                },
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
                    type = "core.bool",
                },
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
                    type = "core.bool",
                }
            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.flow",
                },
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.flow",
                }
            },
            sceneId = 0,
            entityUuid = "",
            startFadeOut = false,
            startFadeOutTime = 0,
            endFadeIn = false,
            endFadeInTime = 0,
            lockPlayerControl = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorPlayCameraPathNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");

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
            local camerPaths = scene:getCameraPaths();
            local indexCameraPath = ld.indexByUuid(camerPaths, node.entityUuid);

            ImGui.Text(ld.loc.get("entities.camera_path"));
            ImGui.SameLine(0, -1);
            if ImGui.Button((indexCameraPath ~= -1 and camerPaths[indexCameraPath + 1]:getName()) or "<None>", ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##entityUuidSelect", 0);
                NodeEditor.Resume();
            end

            if autoBindRequireFields then
                if node.entityUuid == "" and #camerPaths > 0 then
                    node.entityUuid = tostring(camerPaths[#camerPaths]:getUuid());
                end
            end

            if ImGui.IsPopupOpen("##entityUuidSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##entityUuidSelect", 0) then
                    if ImGui.BeginListBox("##entityUuidSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for i, vehicle in ipairs(camerPaths) do
                            if ImGui.Selectable(vehicle:getName(), node.entityUuid == tostring(vehicle:getUuid()), 0, ImVec2.new()) then
                                node.entityUuid = tostring(vehicle:getUuid());
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

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], ld.loc.get("cutscene_objective.start_fade_out"), function (value)
            node.startFadeOut = editor.dataTypes[node.inputs[2].type].drawEditValue(node.startFadeOut, "##startFadeOutEdit", fontScale * 100)
        end);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], ld.loc.get("cutscene_objective.fade_out_time"), function (value)
            ImGui.BeginDisabled(not node.startFadeOut);
            node.startFadeOutTime = editor.dataTypes[node.inputs[3].type].drawEditValue(node.startFadeOutTime, "##startFadeOutTimeEdit", fontScale * 100)
            ImGui.EndDisabled();
        end);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], ld.loc.get("cutscene_objective.end_fade_in"), function (value)
            node.endFadeIn = editor.dataTypes[node.inputs[4].type].drawEditValue(node.endFadeIn, "##endFadeInEdit", fontScale * 100)
        end);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], ld.loc.get("cutscene_objective.fade_in_time"), function (value)
            ImGui.BeginDisabled(not node.endFadeIn);
            node.endFadeInTime = editor.dataTypes[node.inputs[5].type].drawEditValue(node.endFadeInTime, "##endFadeInTimeEdit", fontScale * 100)
            ImGui.EndDisabled();
        end);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], ld.loc.get("cutscene_objective.lock_player_control"), function (value)
            node.lockPlayerControl = editor.dataTypes[node.inputs[6].type].drawEditValue(node.lockPlayerControl, "##lockPlayerControlEdit", fontScale * 100)
        end);
        
        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");
        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], ld.loc.get("nodes.core.play_camera_path.on_complete"));

        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorPlayCameraPathNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local startFadeOut = inputValues[2] or node.startFadeOut;
        local startFadeOutTime = inputValues[3] or node.startFadeOutTime;
        local endFadeIn = inputValues[4] or node.endFadeIn;
        local endFadeInTime = inputValues[5] or node.endFadeInTime;
        local lockPlayerControlVal = inputValues[6] or node.lockPlayerControl;

        if node.sceneId == 0 then
            error("Scene is not set!")
        end

        local scene = ld.projectsService():getCurrentProject():getScenes()[node.sceneId];
        local indexCameraPath = ld.indexByUuid(scene:getCameraPaths(), node.entityUuid);
        if indexCameraPath == -1 then
            error("Camera path is not set!")
        end
        local cameraPath = scene:getCameraPaths()[indexCameraPath + 1];

        while ld.CameraMutex.isLocked() do
            coroutine.yield(1)
        end

        
        local lockPlayerControl = lockPlayerControlVal and PlayerOp.isControlOn(0);

        if lockPlayerControl then
            PlayerOp.setControl(0, false);
        end

        HudOp.display(false);
        HudOp.displayRadar(false);
        
        ld.projectPlayerService:getSceneTasklist():add_task("playCameraPath", function ()
            (function ()
                local guard = ld.createCutsceneGuard();
                if startFadeOut then
                    CameraOp.doFade(startFadeOutTime * 1000, 1);
                end

                ld.TheCameraExtend:setExtendMode(true);

                ld.TheCameraExtend:playCameraPath(cameraPath);

                local startTime = time.getSnTimeInMilliseconds();
                local fadeInDuration = endFadeInTime * 1000;
                local duration = cameraPath:getTime() * 1000;
                local endFadeInStarted = false;
                print(startTime);
                while ld.TheCameraExtend:isPlayingPath() do
                    if endFadeIn and not endFadeInStarted and duration - fadeInDuration <= time.getSnTimeInMilliseconds() - startTime then
                        CameraOp.doFade(fadeInDuration, 0);
                        endFadeInStarted = true;
                    end
                    coroutine.yield(1);
                end

                ld.TheCameraExtend:stopCameraPath();
                ld.TheCameraExtend:setExtendMode(false);

                if lockPlayerControl then
                    PlayerOp.setControl(0, true);
                end

                HudOp.display(true);
                HudOp.displayRadar(true);

                ld.destroyCutsceneGuard(guard);
            end)()

            for _, link in ipairs(context.__links) do
                if link.outputId == node.outputs[2].id then
                    local nodeCallId = math.floor(link.inputId / 100) * 100
                    LDNodeEditor.runNode(editor, context, context.nodes[nodeCallId])
                end
            end
        end);

        return {1, 0}
    end
}

return playCameraPathNode
