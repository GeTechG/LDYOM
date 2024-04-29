local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")

local speedCameraMultiplier = 1;

function waitInGame(time)
    local elapsed = 0
    while elapsed < time do
        coroutine.yield(1)
        elapsed = elapsed + time.getSnTimeInMilliseconds() - time.getSnPreviousTimeInMilliseconds()
    end
end

function editByPlayerCameraTask(pos, pointView, widescreen)
    ld.window.setRenderWindows(false)

    speedCameraMultiplier = ld.settings.getFloat("camera.editByPlayerSpeed") or 1

    PlayerOp.setControl(0, true);

    ld.window.addRender("editByPlayerOverlay", function()
        local windowFlags = bit.bor(ImGuiWindowFlags.NoDecoration, ImGuiWindowFlags.AlwaysAutoResize, ImGuiWindowFlags.NoSavedSettings, ImGuiWindowFlags.NoFocusOnAppearing, ImGuiWindowFlags.NoNav)
        ImGui.SetNextWindowPos(ImVec2.new(0, 0), ImGuiCond.Always, ImVec2.new())
        if ImGui.Begin("##playerEditOverlay", nil, windowFlags) then
            ImGui.PushTextWrapPos(ImGui.GetFontSize() * 16.5)
            ImGui.Text(ld.loc.get("info_overlay.view_camera_mouse"))
            ImGui.Text(ld.loc.get("info_overlay.move_camera"))
            ImGui.Text(string.format(ld.loc.get("info_overlay.speed_move"), speedCameraMultiplier))
            local acceptHotKey = ld.GetHotKeyLib(ld.hotkeyService:getHotKeyByName("accept").functionKeys)
            local cancelHotKey = ld.GetHotKeyLib(ld.hotkeyService:getHotKeyByName("cancel").functionKeys)
            ImGui.Text(string.format(ld.loc.get("info_overlay.accept_cancel"), acceptHotKey, cancelHotKey))
            ImGui.PopTextWrapPos()

            if ImGui.GetMouseWheel() < 0 then
                speedCameraMultiplier = speedCameraMultiplier - 0.01
                speedCameraMultiplier = math.max(speedCameraMultiplier, 0)
                ld.settings.setFloat("camera.editByPlayerSpeed", speedCameraMultiplier)
                ld.settings.save()
            elseif ImGui.GetMouseWheel() > 0 then
                speedCameraMultiplier = speedCameraMultiplier + 0.01
                ld.settings.setFloat("camera.editByPlayerSpeed", speedCameraMultiplier)
                ld.settings.save()
            end
        end
        ImGui.End()
    end)
    
    removePlayerFromWorld()

    local posCam = {pos[1], pos[2], pos[3]}
    local newPointView = {pointView[1], pointView[2], pointView[3]}

    CameraOp.restoreJumpcut()
    CameraOp.persistPos(true)
    CameraOp.setVectorMove(posCam[1], posCam[2], posCam[3], posCam[1], posCam[2], posCam[3], 10, true);
    if widescreen then
        HudOp.switchWidescreen(true)
    end

    HudOp.display(false)
    HudOp.displayRadar(false)

    while true do
        local hotKey = ld.hotkeyService:getHotKey(true)
        if hotKey ~= nil and hotKey.functionName == "cancel" then
            posCam = pos;
            newPointView = pointView;
            break;
        end

        if hotKey ~= nil and hotKey.functionName == "accept" then
            posCam = table.pack(CameraOp.getCoordinates());

            local direction = table.pack(CameraOp.getViewDirection());
            newPointView = {posCam[1] + direction[1], posCam[2] + direction[2], posCam[3] + direction[3]};

            break;
        end

        local rightCamVec = table.pack(CameraOp.getRightCamVector());
        
        KeyCheck.Update();

        if KeyCheck.KeyPressed(0x26) or KeyCheck.KeyPressed(string.byte('W')) then
            local m_vecFront = table.pack(CameraOp.getFrontCamVector());
            posCam[1] = posCam[1] + m_vecFront[1] * speedCameraMultiplier;
            posCam[2] = posCam[2] + m_vecFront[2] * speedCameraMultiplier;
            posCam[3] = posCam[3] + m_vecFront[3] * speedCameraMultiplier;
            CameraOp.setVectorMove(posCam[1], posCam[2], posCam[3], posCam[1], posCam[2], posCam[3], 10, true);
            addPlayerToWorld()
            CharOp.setCoordinates(PlayerOp.getChar(0), posCam[1], posCam[2], posCam[3]);
            removePlayerFromWorld()
        end

        if KeyCheck.KeyPressed(0x28) or KeyCheck.KeyPressed(string.byte('S')) then
            local m_vecFront = table.pack(CameraOp.getFrontCamVector());
            posCam[1] = posCam[1] + m_vecFront[1] * -1 * speedCameraMultiplier;
            posCam[2] = posCam[2] + m_vecFront[2] * -1 * speedCameraMultiplier;
            posCam[3] = posCam[3] + m_vecFront[3] * -1 * speedCameraMultiplier;
            CameraOp.setVectorMove(posCam[1], posCam[2], posCam[3], posCam[1], posCam[2], posCam[3], 10, true);
            addPlayerToWorld()
            CharOp.setCoordinates(PlayerOp.getChar(0), posCam[1], posCam[2], posCam[3]);
            removePlayerFromWorld()
        end

        if KeyCheck.KeyPressed(0x25) or KeyCheck.KeyPressed(string.byte('A')) then
            posCam[1] = posCam[1] + rightCamVec[1] * -1 * speedCameraMultiplier;
            posCam[2] = posCam[2] + rightCamVec[2] * -1 * speedCameraMultiplier;
            posCam[3] = posCam[3] + rightCamVec[3] * -1 * speedCameraMultiplier;
            CameraOp.setVectorMove(posCam[1], posCam[2], posCam[3], posCam[1], posCam[2], posCam[3], 10, true);
            addPlayerToWorld()
            CharOp.setCoordinates(PlayerOp.getChar(0), posCam[1], posCam[2], posCam[3]);
            removePlayerFromWorld()
        end

        if KeyCheck.KeyPressed(0x27) or KeyCheck.KeyPressed(string.byte('D')) then
            posCam[1] = posCam[1] + rightCamVec[1] * speedCameraMultiplier;
            posCam[2] = posCam[2] + rightCamVec[2] * speedCameraMultiplier;
            posCam[3] = posCam[3] + rightCamVec[3] * speedCameraMultiplier;
            CameraOp.setVectorMove(posCam[1], posCam[2], posCam[3], posCam[1], posCam[2], posCam[3], 10, true);
            addPlayerToWorld()
            CharOp.setCoordinates(PlayerOp.getChar(0), posCam[1], posCam[2], posCam[3]);
            removePlayerFromWorld()
        end

        if KeyCheck.KeyPressed(0x51) then
            local m_vecUp = table.pack(CameraOp.getUpCamVector());
            posCam[1] = posCam[1] + m_vecUp[1] * speedCameraMultiplier;
            posCam[2] = posCam[2] + m_vecUp[2] * speedCameraMultiplier;
            posCam[3] = posCam[3] + m_vecUp[3] * speedCameraMultiplier;
            CameraOp.setVectorMove(posCam[1], posCam[2], posCam[3], posCam[1], posCam[2], posCam[3], 10, true);
            addPlayerToWorld()
            CharOp.setCoordinates(PlayerOp.getChar(0), posCam[1], posCam[2], posCam[3]);
            removePlayerFromWorld()
        end

        if KeyCheck.KeyPressed(0x45) then
            local m_vecUp = table.pack(CameraOp.getUpCamVector());
            posCam[1] = posCam[1] + m_vecUp[1] * -1 * speedCameraMultiplier;
            posCam[2] = posCam[2] + m_vecUp[2] * -1 * speedCameraMultiplier;
            posCam[3] = posCam[3] + m_vecUp[3] * -1 * speedCameraMultiplier;
            CameraOp.setVectorMove(posCam[1], posCam[2], posCam[3], posCam[1], posCam[2], posCam[3], 10, true);
            addPlayerToWorld()
            CharOp.setCoordinates(PlayerOp.getChar(0), posCam[1], posCam[2], posCam[3]);
            removePlayerFromWorld()
        end

        coroutine.yield(1)
    end

    CameraOp.persistPos(false)
    CameraOp.restoreJumpcut()
    local z = WorldOp.getGroundZFor3DCoord(posCam[1], posCam[2], posCam[3]);
    addPlayerToWorld()
    CharOp.setCoordinates(PlayerOp.getChar(0), posCam[1], posCam[2], z + 1);
    if widescreen then
        HudOp.switchWidescreen(false)
    end

    HudOp.display(true)
    HudOp.displayRadar(true)

    ld.window.removeRender("editByPlayerOverlay")
    ld.window.setRenderWindows(true)

    return posCam, newPointView
end

---@class LDNodeEditorCutsceneNode : LDNodeEditorNode
---@field text string
---@field textTime number
---@field position number[]
---@field pointPosition number[]
---@field moveType integer
---@field followType integer
---@field followSceneId integer
---@field followUuid string
---@field startFadeOut boolean
---@field startFadeOutTime number
---@field endFadeIn boolean
---@field endFadeInTime number
---@field wideScreen boolean
---@field lockPlayerControl boolean


local lastQ = {0,0,0,0}

local CUTSCENE_NODE_TYPE = "core.cutscene"

---@type LDNodeEditorNodeType
local cutsceneForNode = {
    typeName = CUTSCENE_NODE_TYPE,
    category = "main",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        local px, py, pz = CharOp.getCoordinates(PlayerOp.getChar(0))
        ---@type LDNodeEditorCutsceneNode
        local newNode = {
            id = newNodeId,
            nodeType = CUTSCENE_NODE_TYPE,
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
                    type = "core.string",
                },
                { -- time
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.number",
                },
                { -- x
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.number",
                },
                { -- y
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.number",
                },
                { -- z
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.number",
                },
                { -- x point
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.number",
                },
                { -- y point
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.number",
                },
                { -- z point
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.number",
                },
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
            text = "",
            textTime = 0,
            position = {px, py, pz},
            pointPosition = {px + 1, py, pz},
            moveType = 0,
            followType = 0,
            followSceneId = 0,
            followUuid = "",
            startFadeOut = false,
            startFadeOutTime = 0,
            endFadeIn = false,
            endFadeInTime = 0,
            wideScreen = false,
            lockPlayerControl = true,
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCutsceneNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], ld.loc.get("general.text"), function()
            ImGui.SetNextItemWidth(fontScale * 100);
            local isEdit, newValue = ImGui.InputText("##editText", node.text, 0);
            if isEdit then
                node.text = newValue
            end
        end);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], ld.loc.get("general.time"), function()
            ImGui.SetNextItemWidth(fontScale * 100);
            local isEdit, newValue = ImGui.DragFloat("##editTime", node.textTime, 0.001, 0, 0, "%.3f", 0);
            if isEdit then
                node.textTime = newValue
            end
        end);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "X", function()
            node.position[1] = editor.dataTypes[node.inputs[4].type].drawEditValue(node.position[1], "##xEdit", fontScale * 100)
        end);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "Y", function()
            node.position[2] = editor.dataTypes[node.inputs[5].type].drawEditValue(node.position[2], "##yEdit", fontScale * 100)
        end);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "Z", function()
            node.position[3] = editor.dataTypes[node.inputs[6].type].drawEditValue(node.position[3], "##zEdit", fontScale * 100)
        end);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], ld.loc.get("nodes.core.cutscene.point_to_x"), function()
            node.pointPosition[1] = editor.dataTypes[node.inputs[7].type].drawEditValue(node.pointPosition[1], "##xPointEdit", fontScale * 100)
        end);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], ld.loc.get("nodes.core.cutscene.point_to_y"), function()
            node.pointPosition[2] = editor.dataTypes[node.inputs[8].type].drawEditValue(node.pointPosition[2], "##yPointEdit", fontScale * 100)
        end);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[9], ld.loc.get("nodes.core.cutscene.point_to_z"), function()
            node.pointPosition[3] = editor.dataTypes[node.inputs[9].type].drawEditValue(node.pointPosition[3], "##zPointEdit", fontScale * 100)
        end);

        if ImGui.Button(ld.loc.get("general.edit_manually"), ImVec2.new()) then
            ld.tasker:addTask("editByPlayerActorPath", function ()
                node.position, node.pointPosition = editByPlayerCameraTask(node.position, node.pointPosition, node.wideScreen);
            end)
        end

        if ImGui.Button(ld.loc.get("cutscene_objective.move_type") .. ": " .. ld.loc.getArray("cutscene_objective.move_types")[node.moveType + 1], ImVec2.new()) then
            NodeEditor.Suspend();
            ImGui.OpenPopup("##moveTypeSelect", 0);
            NodeEditor.Resume();
        end

        if ImGui.IsPopupOpen("##moveTypeSelect", 0) then
            NodeEditor.Suspend();
            if ImGui.BeginPopup("##moveTypeSelect", 0) then
                if ImGui.BeginListBox("##moveTypeSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                    for i, moveType in ipairs(ld.loc.getArray("cutscene_objective.move_types")) do
                        if ImGui.Selectable(moveType, node.moveType == i - 1, 0, ImVec2.new()) then
                            node.moveType = i - 1;
                            ImGui.CloseCurrentPopup();
                        end
                    end
                    ImGui.EndListBox();
                end
                ImGui.EndPopup();
            end
            NodeEditor.Resume();
        end

        local entitySelector = function (name, uuid, entities)
            local index = ld.indexByUuid(entities, uuid)
            if index == -1 then
                if #entities > 0 then
                    uuid = tostring(entities[#entities]:getUuid())
                end
            end
            if ImGui.Button((index ~= -1 and entities[index + 1]:getName()) or "<None>", ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##entityUuidSelect", 0);
                NodeEditor.Resume();
            end
            if ImGui.IsPopupOpen("##entityUuidSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##entityUuidSelect", 0) then
                    if ImGui.BeginListBox("##entityUuidSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for i, entity in ipairs(entities) do
                            if ImGui.Selectable(entity:getName(), uuid == tostring(entity:getUuid()), 0, ImVec2.new()) then
                                uuid = tostring(entity:getUuid())
                                ImGui.CloseCurrentPopup();
                            end
                        end
                        ImGui.EndListBox();
                    end
                    ImGui.EndPopup();
                end
                NodeEditor.Resume();
            end
            return uuid
        end

        ImGui.BeginDisabled(node.moveType ~= 0)

        ImGui.PushID("followGroup");

        ImGui.SetNextItemWidth(fontScale * 100);
        ---@type boolean, any
        local isEdit, newValue = ImGui.SliderInt(ld.loc.get("cutscene_objective.follow"), node.followType, 0, 3, ld.loc.getArray("nodes.core.cutscene.entities_types")[node.followType + 1], 0)
        if isEdit then
            node.followType = newValue
            node.followUuid = ""
        end

        if node.followType > 0 and node.followType < 4 then
            local scene = ld.projectsService():getCurrentProject():getScenes()[node.followSceneId];

            ImGui.Text(ld.loc.get("scenes.scene"));
            ImGui.SameLine(0, -1);
            if ImGui.Button((scene ~= nil and scene:getName()) or "<None>", ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##sceneIdSelectFollow", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##sceneIdSelectFollow", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##sceneIdSelectFollow", 0) then
                    if ImGui.BeginListBox("##sceneIdSelectFollowChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for id, scene in pairs(ld.projectsService():getCurrentProject():getScenes()) do
                            if ImGui.Selectable(scene:getName(), node.followSceneId == id, 0, ImVec2.new()) then
                                node.followSceneId = id;
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
                if node.followSceneId == 0 then
                    node.followSceneId = ld.projectsService():getCurrentProject():getCurrentSceneIndex()
                end
            end

            if scene then
                if node.followType == 1 then
                    node.followUuid = entitySelector(ld.loc.get("entities.actor"), node.followUuid, scene:getActors())
                elseif node.followType == 2 then
                    node.followUuid = entitySelector(ld.loc.get("entities.vehicle"), node.followUuid, scene:getVehicles())
                end
            end
        end
        ImGui.PopID();
        ImGui.EndDisabled();

        isEdit, newValue = ImGui.Checkbox(ld.loc.get("cutscene_objective.start_fade_out"), node.startFadeOut)
        if isEdit then
            node.startFadeOut = newValue
        end
        ImGui.BeginDisabled(not node.startFadeOut)
        ImGui.SetNextItemWidth(fontScale * 100);
        isEdit, newValue = ImGui.InputFloat(ld.loc.get("cutscene_objective.fade_out_time"), node.startFadeOutTime, 0,  0, "%.3f", 0)
        if isEdit then
            node.startFadeOutTime = newValue
        end
        ImGui.EndDisabled()

        isEdit, newValue = ImGui.Checkbox(ld.loc.get("cutscene_objective.end_fade_in"), node.endFadeIn)
        if isEdit then
            node.endFadeIn = newValue
        end
        ImGui.BeginDisabled(not node.endFadeIn)
        ImGui.SetNextItemWidth(fontScale * 100);
        isEdit, newValue = ImGui.InputFloat(ld.loc.get("cutscene_objective.fade_in_time"), node.endFadeInTime, 0,  0, "%.3f", 0)
        if isEdit then
            node.endFadeInTime = newValue
        end
        ImGui.EndDisabled()

        isEdit, newValue = ImGui.Checkbox(ld.loc.get("cutscene_objective.widescreen"), node.wideScreen)
        if isEdit then
            node.wideScreen = newValue
        end

        isEdit, newValue = ImGui.Checkbox(ld.loc.get("cutscene_objective.lock_player_control"), node.lockPlayerControl)
        if isEdit then
            node.lockPlayerControl = newValue
        end

        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");
        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], ld.loc.get("nodes.core.cutscene.on_complete"));

        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCutsceneNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local time = inputValues[3] or node.textTime
        local pos_x = inputValues[4] or node.position[1]
        local pos_y = inputValues[5] or node.position[2]
        local pos_z = inputValues[6] or node.position[3]
        local point_x = inputValues[7] or node.pointPosition[1]
        local point_y = inputValues[8] or node.pointPosition[2]
        local point_z = inputValues[9] or node.pointPosition[3]


        local lockPlayerControl = node.lockPlayerControl and PlayerOp.isControlOn(0)

        if lockPlayerControl then
            PlayerOp.setControl(0, false)
        end

        if node.wideScreen then
            HudOp.switchWidescreen(true)
        end

        local targetFollow = nil
        local indexFollow = -1

        if node.followType > 0 then
            local scene = ld.projectsService():getCurrentProject():getScenes()[node.followSceneId]

            if node.followType == 1 then
                indexFollow = ld.indexByUuid(scene:getActors(), node.followUuid) + 1;
                if indexFollow ~= -1 then
                    local ped = scene:getActors()[indexFollow]:getProjectPed()
                    if ped then
                        targetFollow = GetPedRef(ped)
                    end
                end
            elseif node.followType == 2 then
                indexFollow = ld.indexByUuid(scene:getVehicles(), node.followUuid) + 1;
                if indexFollow ~= -1 then
                    local vehicle = scene:getVehicles()[indexFollow]:getProjectVehicle()
                    if vehicle then
                        targetFollow = GetVehicleRef(vehicle)
                    end
                end
            elseif node.followType == 3 then
                local ped = findPlayerPed()
                if ped then
                    indexFollow = 0
                    targetFollow = GetPedRef(ped)
                end
            end

            if indexFollow == -1 then
                error("The follow entity for the objective is not selected.")
            end
            if targetFollow == nil then
                error("The follow entity does not exist.")
            end
        end

        TextOp.printString(node.text, math.floor(node.textTime * 1000))

        HudOp.display(false)
        HudOp.displayRadar(true)

        ld.projectPlayerService:getSceneTasklist():add_task("cutscene", function ()
            if node.startFadeOut then
                CameraOp.doFade(math.floor(node.startFadeOutTime * 1000), 1);
            end

            if node.moveType == 0 then
                CameraOp.setFixedPosition(pos_x, pos_y, pos_z, 0, 0, 0);
                if node.followType == 0 then
                    CameraOp.pointAtPoint(point_x, point_y, point_z, 2);
                elseif node.followType == 1 and targetFollow then
                    CameraOp.pointAtChar(targetFollow, 15, 2)
                elseif node.followType == 2 and targetFollow then
                    CameraOp.pointAtVehicle(targetFollow, 15, 2)
                elseif node.followType == 3 and targetFollow then
                    CameraOp.pointAtChar(targetFollow, 15, 2)
                end
            else
                local startPos = table.pack(CameraOp.getCoordinates());
                local direction = table.pack(CameraOp.getViewDirection());

                CameraOp.setVectorTrack(point_x, point_y, point_z, direction[1], direction[2], direction[3], math.floor(time * 1000), node.moveType == 2);
                CameraOp.setVectorMove(pos_x, pos_y, pos_z, startPos[1], startPos[2], startPos[3], math.floor(time * 1000), node.moveType == 2);
            end

            local duration = math.floor(time * 1000)
            if node.endFadeIn then
                local fadeInDuration = math.floor(node.endFadeInTime * 1000)
                ld.tasker:addTask("cutsceneFadeIn", function ()
                    waitInGame(duration - fadeInDuration)
                    CameraOp.doFade(fadeInDuration, 0)
                    waitInGame(fadeInDuration)
                end)
            else
                waitInGame(duration)
            end

            if lockPlayerControl then
                PlayerOp.setControl(0, true)
            end

            if node.wideScreen then
                HudOp.switchWidescreen(false)
            end

            HudOp.display(true)
            HudOp.displayRadar(true)
            CameraOp.restoreJumpcut();

            for _, link in ipairs(context.__links) do
                if link.outputId == node.outputs[2].id then
                    local nodeCallId = math.floor(link.inputId / 100) * 100
                    LDNodeEditor.runNode(editor, context, context.nodes[nodeCallId])
                end
            end
        end)

        return {1, 0}
    end
}

return cutsceneForNode