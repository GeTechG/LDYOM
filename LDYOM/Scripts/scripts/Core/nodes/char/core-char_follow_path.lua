local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@param path number[][]
function editByPlayerActorPath(path)
    ld.window.setRenderWindows(false);

    CameraOp.restoreJumpcut();

    PlayerOp.setControl(0, true);

    newPath = table.deepcopy(path);

    ---@type number[][]
    local newPath = table.deepcopy(path);

    local currentIndexPoint = 0;

    ld.window.addRender("editByPlayerOverlay", function()
        local windowFlags = bit.bor(ImGuiWindowFlags.NoDecoration, ImGuiWindowFlags.AlwaysAutoResize, ImGuiWindowFlags.NoSavedSettings, ImGuiWindowFlags.NoFocusOnAppearing, ImGuiWindowFlags.NoNav)
        ImGui.SetNextWindowPos(ImVec2.new(0, 0), ImGuiCond.Always, ImVec2.new())
        if ImGui.Begin("##playerEditOverlay", nil, windowFlags) then
            ImGui.PushTextWrapPos(ImGui.GetFontSize() * 16.5)
            ImGui.Text(ld.loc.get("info_overlay.delete_all_points"))
            ImGui.Text(ld.loc.get("info_overlay.move_point"))
            ImGui.Text(ld.loc.get("info_overlay.select_point"))
            ImGui.Text(ld.loc.get("info_overlay.add_point"))
            local acceptHotKey = ld.GetHotKeyLib(ld.hotkeyService:getHotKeyByName("accept").functionKeys)
            local cancelHotKey = ld.GetHotKeyLib(ld.hotkeyService:getHotKeyByName("cancel").functionKeys)
            ImGui.Text(string.format(ld.loc.get("info_overlay.accept_cancel"), acceptHotKey, cancelHotKey))
            ImGui.PopTextWrapPos()
    
            local drawList = ImGui.GetBackgroundDrawList()
    
            if #newPath > 0 then
                local lastPoint = {0, 0, 0};
                local lastVisible, w, h = CalcScreenCoors(newPath[1], lastPoint, true, true)
                if lastVisible then
                    drawList:AddCircleFilled(ImVec2.new(lastPoint[1], lastPoint[2]), 5, currentIndexPoint == 1 and ImGui.ColorConvertFloat4ToU32(ImVec4.new(1, 0, 0, 1)) or 0xFFFFFFFF, 0)
                end
                for i = 2, #newPath do
                    local currentPoint = {0, 0, 0}
                    local currentVisible, cw, ch = CalcScreenCoors(newPath[i], currentPoint, true, true)
                    if lastVisible and currentVisible then
                        drawList:AddLine(ImVec2.new(lastPoint[1], lastPoint[2]), ImVec2.new(currentPoint[1], currentPoint[2]), 0xFFFFFFFF, 0)
                    end
                    if currentVisible then
                        drawList:AddCircleFilled(ImVec2.new(currentPoint[1], currentPoint[2]), 5, currentIndexPoint == i and ImGui.ColorConvertFloat4ToU32(ImVec4.new(1, 0, 0, 1)) or 0xFFFFFFFF, 0)
                    end
                    lastPoint = currentPoint
                    lastVisible = currentVisible
                end
            end

            local exit = false

            local hotKey = ld.hotkeyService:getHotKey(true)
            if hotKey ~= nil and hotKey.functionName == "cancel" then
                exit = true
            end

            if hotKey ~= nil and hotKey.functionName == "accept" then
                for i = 1, #newPath do
                    path[i] = newPath[i]
                end
                for i = #newPath + 1, #path do
                    path[i] = nil
                end
                exit = true
            end

            if exit then
                ld.window.removeRender("editByPlayerOverlay")
                ld.window.setRenderWindows(true);
            end

            KeyCheck.Update();

            if KeyCheck.CheckJustDown(string.byte("L")) then
                newPath = {}
            end

            if KeyCheck.CheckJustDown(string.byte("M")) then
                local px, py, pz = CharOp.getCoordinates(PlayerOp.getChar(0))
                if #newPath > 0 then
                    newPath[currentIndexPoint] = {px, py, pz}
                end
            end

            if KeyCheck.CheckJustDown(string.byte("I")) then
                currentIndexPoint = currentIndexPoint + 1
                currentIndexPoint = math.min(currentIndexPoint, #newPath)
            end

            if KeyCheck.CheckJustDown(string.byte("O")) then
                currentIndexPoint = currentIndexPoint - 1
                currentIndexPoint = math.max(currentIndexPoint, 1)
            end

            if KeyCheck.CheckJustDown(string.byte("P")) then
                local px, py, pz = CharOp.getCoordinates(PlayerOp.getChar(0))
                table.insert(newPath, {px, py, pz})
                currentIndexPoint = #newPath
            end
        end
        ImGui.End()
    end)
end

local moveState = {4, 6, 7};

---@class LDNodeEditorCharFollowPathNode : LDNodeEditorNode
---@field moveType integer
---@field pathType integer
---@field executeTime number
---@field path number[][]
---@field startFromNearest boolean

local CHAR_FOLLOW_PATH_NODE_TYPE = "core.char_follow_path"

---@type LDNodeEditorNodeType
local charFollowPathNode = {
    typeName = CHAR_FOLLOW_PATH_NODE_TYPE,
    category = "char",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCharFollowPathNode
        local newNode = {
            id = newNodeId,
            nodeType = CHAR_FOLLOW_PATH_NODE_TYPE,
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
                    type = "core.Char",
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
            moveType = 0,
            pathType = 0,
            executeTime = 0,
            path = {},
            startFromNearest = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCharFollowPathNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "");

        builder:Middle();

        ImGui.SameLine(0,-1);
        ImGui.BeginGroup();
        ImGui.SetNextItemWidth(fontScale * 100);
        ---@type boolean, any
        local isEdit, newValue = ImGui.SliderInt(ld.loc.get("follow_path_actor_objective.move_type"), node.moveType, 0, 2, ld.loc.getArray("follow_path_actor_objective.move_states")[node.moveType + 1], 0);
        if isEdit then
            node.moveType = newValue
        end

        ImGui.SetNextItemWidth(fontScale * 100);
        isEdit, newValue = ImGui.SliderInt(ld.loc.get("follow_path_actor_objective.path_type"), node.pathType, 0, 2, ld.loc.getArray("follow_path_actor_objective.path_types")[node.pathType + 1], 0);
        if isEdit then
            node.pathType = newValue
        end

        if node.pathType > 0 then
            ImGui.SetNextItemWidth(fontScale * 100);
            isEdit, newValue = ImGui.DragFloat(ld.loc.get("follow_path_actor_objective.execute_time"), node.executeTime, 0.1, 0, 9999999, "%.2f", 0);
            if isEdit then
                node.executeTime = newValue
            end
        end

        isEdit, newValue = ImGui.Checkbox(ld.loc.get("nodes.core.char_follow_path.start_from_nearest"), node.startFromNearest);
        if isEdit then
            node.startFromNearest = newValue
        end
        
        if ImGui.Button(ld.loc.get("general.record_path"), ImVec2.new()) then
            editByPlayerActorPath(node.path)
        end
        ImGui.EndGroup();

        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");
        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], ld.loc.get("nodes.core.char_follow_path.on_complete"));

        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCharFollowPathNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        ld.projectPlayerService:getSceneTasklist():add_task(function ()
            if inputValues[2] == nil then
                error("Char not found")
            end
            ---@type integer
            local ped = inputValues[2]
            local execute = true
            local step = 1
            local index = 1
            local startTime = time.snTimeInMilliseconds

            if node.startFromNearest then
                local x, y, z = CharOp.getCoordinates(ped)
                local nearestIndex = 1
                local nearestDistance = distanceBetweenPoints(x, y, z, table.unpack(node.path[1]))
                for i = 2, #node.path do
                    local distance = distanceBetweenPoints(x, y, z, table.unpack(node.path[i]))
                    if distance < nearestDistance then
                        nearestIndex = i
                        nearestDistance = distance
                    end
                end
                index = nearestIndex
            end

            while execute do
                local x = node.path[index][1]
                local y = node.path[index][2]
                local z = node.path[index][3]

                if not CharOp.doesExist(ped) then
                    return;
                end

                local moveType = node.moveType
                local pathType = node.pathType
                local executeTime = node.executeTime

                TaskOp.goStraightToCoord(ped, x, y, z, moveState[moveType + 1], -1)

                local timeCondition = pathType == 0 or time.snTimeInMilliseconds - startTime < executeTime * 1000
                local reachDistance = moveType == 0 and 0.5 or 1

                while CharOp.doesExist(ped) and distanceBetweenPoints(x, y, z, CharOp.getCoordinates(ped)) > reachDistance and timeCondition do
                    timeCondition = (pathType == 0) or (time.snTimeInMilliseconds - startTime < executeTime * 1000)
                    coroutine.yield(1)
                end

                index = index + step

                if not timeCondition then
                    execute = false
                end

                if pathType == 0 then
                    if index > #node.path then
                        execute = false
                    end
                elseif pathType == 1 then
                    if index > #node.path then
                        index = 1
                    end
                elseif pathType == 2 then
                    if index > #node.path then
                        step = -1
                        index = index - 2
                    elseif index < 1 then
                        step = 1
                        index = index + 2
                    end
                end
            end

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

return charFollowPathNode