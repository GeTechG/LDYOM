local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCharAnimationNode : LDNodeEditorNode
---@field pack integer
---@field animation integer
---@field playType integer
---@field looped boolean
---@field blend number
---@field unlockX boolean
---@field unlockY boolean
---@field lockF boolean
---@field time number
---@field disableForce boolean
---@field disableLockZ boolean


local CHAR_ANIMATION_NODE_TYPE = "core.char_animation"

---@type LDNodeEditorNodeType
local charAnimationNode = {
    typeName = CHAR_ANIMATION_NODE_TYPE,
    category = "char",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCharAnimationNode
        local newNode = {
            id = newNodeId,
            nodeType = CHAR_ANIMATION_NODE_TYPE,
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
            pack = 1,
            animation = 1,
            playType = 1,
            looped = false,
            blend = 0,
            unlockX = false,
            unlockY = false,
            lockF = false,
            time = 0,
            disableForce = false,
            disableLockZ = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCharAnimationNode
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

        local packsNames = ld.models.getPacksNames()
        local play = false

        ImGui.Text(ld.loc.get("animation.pack"));
        ImGui.SameLine(0, -1);
        if ImGui.Button(packsNames[node.pack], ImVec2.new()) then
            NodeEditor.Suspend();
            ImGui.OpenPopup("##packSelect", 0);
            NodeEditor.Resume();
        end

        if ImGui.IsPopupOpen("##packSelect", 0) then
            NodeEditor.Suspend();
            if ImGui.BeginPopup("##packSelect", 0) then
                if ImGui.BeginListBox("##packSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                    for i, packName in ipairs(packsNames) do
                        if ImGui.Selectable(packName, node.pack == i, 0, ImVec2.new()) then
                            node.pack = i;
                            node.animation = 1;
                            play = true;
                            ImGui.CloseCurrentPopup();
                        end
                    end
                    ImGui.EndListBox();
                end
                ImGui.EndPopup();
            end
            NodeEditor.Resume();
        end
        
        local animations = ld.models.getAnimations()
        
        ImGui.Text(ld.loc.get("animation.title"));
        ImGui.SameLine(0, -1);
        if ImGui.Button(animations[packsNames[node.pack]][node.animation], ImVec2.new()) then
            NodeEditor.Suspend();
            ImGui.OpenPopup("##animationSelect", 0);
            NodeEditor.Resume();
        end
        
        if ImGui.IsPopupOpen("##animationSelect", 0) then
            NodeEditor.Suspend();
            if ImGui.BeginPopup("##animationSelect", 0) then
                if ImGui.BeginListBox("##animationSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                    for i, animation in ipairs(animations[packsNames[node.pack]]) do
                        if ImGui.Selectable(animation, node.animation == i, 0, ImVec2.new()) then
                            node.animation = i;
                            play = true;
                            ImGui.CloseCurrentPopup();
                        end
                    end
                    ImGui.EndListBox();
                end
                ImGui.EndPopup();
            end
            NodeEditor.Resume();
        end

        ImGui.SetNextItemWidth(fontScale * 100);
        ---@type boolean, any
        local isEdit, newValue = ImGui.SliderInt(ld.loc.get("nodes.core.char_animation.play_type"), node.playType, 1, 4, ld.loc.getArray("nodes.core.char_animation.play_types")[node.playType], 0);
        if isEdit then
            node.playType = newValue;
            play = true;
        end

        if ImGui.IsItemHovered(0) then
            NodeEditor.Suspend();
            if ImGui.BeginTooltip() then
                ImGui.PushTextWrapPos(ImGui.GetFontSize() * 20.0);
                ImGui.TextUnformatted(ld.loc.get("nodes.core.char_animation.play_types_info"));
                ImGui.PopTextWrapPos();
                ImGui.EndTooltip();
            end 
            NodeEditor.Resume();
        end

        isEdit, newValue = ImGui.Checkbox(ld.loc.get("animation.looped"), node.looped);
        if isEdit then
            node.looped = newValue;
        end
        ImGui.SetNextItemWidth(fontScale * 100);
        isEdit, newValue = ImGui.DragFloat(ld.loc.get("animation.smoothness"), node.blend, 0.01, 0.00001, 1, "%.3f", 0);
        if isEdit then
            node.blend = newValue;
        end

        isEdit, newValue = ImGui.Checkbox(ld.loc.get("nodes.core.char_animation.unlock_x"), node.unlockX);
        if isEdit then
            node.unlockX = newValue;
            play = true;
        end

        ImGui.BeginDisabled(not node.unlockX);
        isEdit, newValue = ImGui.Checkbox(ld.loc.get("nodes.core.char_animation.unlock_y"), node.unlockY);
        if isEdit then
            node.unlockY = newValue;
            play = true;
        end
        ImGui.EndDisabled();

        isEdit, newValue = ImGui.Checkbox(ld.loc.get("nodes.core.char_animation.lock_f"), node.lockF);
        if isEdit then
            node.lockF = newValue;
            play = true;
        end

        ImGui.SetNextItemWidth(fontScale * 100);
        isEdit, newValue = ImGui.DragFloat(ld.loc.get("general.time"), node.time, 1, -1, 999999999999, "%.3f", 0);
        if isEdit then
            node.time = newValue;
            play = true;
        end

        if node.playType == 4 then
            isEdit, newValue = ImGui.Checkbox(ld.loc.get("nodes.core.char_animation.disable_force"), node.disableForce);
            if isEdit then
                node.disableForce = newValue;
                play = true;
            end
    
            isEdit, newValue = ImGui.Checkbox(ld.loc.get("nodes.core.char_animation.disable_lock_z"), node.disableLockZ);
            if isEdit then
                node.disableLockZ = newValue;
                play = true;
            end
        end
        
        ImGui.EndGroup();

        local playerPed = PlayerOp.getChar(0)
        if play and CharOp.doesExist(playerPed) then
            if not StreamingOp.hasAnimationLoaded(packsNames[node.pack]) then
                StreamingOp.requestAnimation(packsNames[node.pack])
            end
            local anims = animations[packsNames[node.pack]]
            if node.playType == 1 then
                TaskOp.playAnim(playerPed, anims[node.animation], packsNames[node.pack], 10 * (1 - (node.blend - 0.00001)), node.looped, node.unlockX, node.unlockY, node.lockF, math.floor(node.time * 1000))
            elseif node.playType == 2 then
                TaskOp.playAnimNonInterruptable(playerPed, anims[node.animation], packsNames[node.pack], 10 * (1 - (node.blend - 0.00001)), node.looped, node.unlockX, node.unlockY, node.lockF, math.floor(node.time * 1000))
            elseif node.playType == 3 then
                TaskOp.playAnimSecondary(playerPed, anims[node.animation], packsNames[node.pack], 10 * (1 - (node.blend - 0.00001)), node.looped, node.unlockX, node.unlockY, node.lockF, math.floor(node.time * 1000))
            elseif node.playType == 4 then
                TaskOp.playAnimWithFlags(playerPed, anims[node.animation], packsNames[node.pack], 10 * (1 - (node.blend - 0.00001)), node.looped, node.unlockX, node.unlockY, node.lockF, math.floor(node.time * 1000), node.disableForce, node.disableLockZ)
            end
        end

        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");
        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], ld.loc.get("nodes.core.char_animation.on_complete"));

        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCharAnimationNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        ld.projectPlayerService:getSceneTasklist():add_task(function ()
            if inputValues[2] == nil then
                error("Char not found")
            end
            ---@type integer
            local ped = inputValues[2]

            local packsNames = ld.models.getPacksNames()

            if not StreamingOp.hasAnimationLoaded(packsNames[node.pack]) then
                StreamingOp.requestAnimation(packsNames[node.pack])
            end

            local animations = ld.models.getAnimations()
            local anims = animations[packsNames[node.pack]]

            if node.playType == 1 then
                TaskOp.playAnim(ped, anims[node.animation], packsNames[node.pack], 10 * (1 - (node.blend - 0.00001)), node.looped, node.unlockX, node.unlockY, node.lockF, math.floor(node.time * 1000))
            elseif node.playType == 2 then
                TaskOp.playAnimNonInterruptable(ped, anims[node.animation], packsNames[node.pack], 10 * (1 - (node.blend - 0.00001)), node.looped, node.unlockX, node.unlockY, node.lockF, math.floor(node.time * 1000))
            elseif node.playType == 3 then
                TaskOp.playAnimSecondary(ped, anims[node.animation], packsNames[node.pack], 10 * (1 - (node.blend - 0.00001)), node.looped, node.unlockX, node.unlockY, node.lockF, math.floor(node.time * 1000))
            elseif node.playType == 4 then
                TaskOp.playAnimWithFlags(ped, anims[node.animation], packsNames[node.pack], 10 * (1 - (node.blend - 0.00001)), node.looped, node.unlockX, node.unlockY, node.lockF, math.floor(node.time * 1000), node.disableForce, node.disableLockZ)
            end

            while not CharOp.isPlayingAnim(ped, anims[node.animation]) do
                coroutine.yield(1)
            end

            while CharOp.isPlayingAnim(ped, anims[node.animation]) do
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

return charAnimationNode