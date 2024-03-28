local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")

---@class LDNodeEditorFollowCarrecPathVehicleNode : LDNodeEditorNode
---@field pathUuid string
---@field useAI boolean
---@field looped boolean


local FOLLOW_CARREC_PATH_VEHICLE_NODE_TYPE = "core.follow_carrec_path_vehicle"

---@type LDNodeEditorNodeType
local followCarrecPathVehicleNode = {
    typeName = FOLLOW_CARREC_PATH_VEHICLE_NODE_TYPE,
    category = "vehicle",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorFollowCarrecPathVehicleNode
        local newNode = {
            id = newNodeId,
            nodeType = FOLLOW_CARREC_PATH_VEHICLE_NODE_TYPE,
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
                    type = "core.Vehicle",
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
                    type = "core.bool",
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
            sceneId = 0,
            entityUuid = "",
            pathUuid = "",
            useAI = false,
            looped = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorFollowCarrecPathVehicleNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], ld.loc.get("entities.vehicle"));

        ImGui.Text(ld.loc.get("carrec_path.path"));
        ImGui.SameLine(0, -1);
        ---@type CarrecPath
        local path = nil
        for _, p in pairs(ld.carrecPathService:getPaths()) do
            if tostring(p:getUuid()) == node.pathUuid then
                path = p
                break
            end
        end
        if ImGui.Button((path ~= nil and path:getName()) or "<None>", ImVec2.new()) then
            NodeEditor.Suspend();
            ImGui.OpenPopup("##pathUuidSelect", 0);
            NodeEditor.Resume();
        end

        if ImGui.IsPopupOpen("##pathUuidSelect", 0) then
            NodeEditor.Suspend();
            if ImGui.BeginPopup("##pathUuidSelect", 0) then
                if ImGui.BeginListBox("##pathUuidSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                    for i, path in pairs(ld.carrecPathService:getPaths()) do
                        if ImGui.Selectable(path:getName() .. "##" .. tostring(i), node.pathUuid == tostring(path:getUuid()), 0, ImVec2.new()) then
                            node.pathUuid = tostring(path:getUuid());
                            ImGui.CloseCurrentPopup();
                        end
                    end
                    ImGui.EndListBox();
                end
                ImGui.EndPopup();
            end
            NodeEditor.Resume();
        end

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], ld.loc.get("follow_carrec_path_vehicle_objective.use_ai"), function ()
            node.useAI = editor.dataTypes[node.inputs[3].type].drawEditValue(node.useAI, "##useAI", fontScale * 100);
        end);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], ld.loc.get("animation.looped"), function ()
            node.looped = editor.dataTypes[node.inputs[4].type].drawEditValue(node.looped, "##looped", fontScale * 100);
        end);
        
        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");
        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], ld.loc.get("nodes.core.follow_carrec_path_vehicle.on_complete"));

        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorFollowCarrecPathVehicleNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        ld.projectPlayerService:getSceneTasklist():add_task(function ()
            local useAI = inputValues[3] or node.useAI
            local looped = inputValues[4] or node.looped
            local vehicleRef = inputValues[2]
            if vehicleRef == nil then
                error("Vehicle is not set!")
            end

            if node.pathUuid == "" then
                error("Path is not set!")
            end
            ---@type CarrecPath
            local path = nil
            for _, p in pairs(ld.carrecPathService:getPaths()) do
                if tostring(p:getUuid()) == node.pathUuid then
                    path = p
                    break
                end
            end
            if path == nil then
                error("Path not found!")
            end

            ld.carrecPathService.startPlaybackRecordedCar(GetVehicle(vehicleRef), path:getPath(), useAI, looped);

            while CarOp.isPlaybackGoingOn(vehicleRef) do
                coroutine.yield(1)
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

return followCarrecPathVehicleNode
