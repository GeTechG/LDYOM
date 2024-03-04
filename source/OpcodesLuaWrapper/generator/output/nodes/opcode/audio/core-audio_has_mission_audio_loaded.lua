local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorAudioHasMissionAudioLoadedNode : LDNodeEditorNode
---@field slotId integer

local AUDIO_HAS_MISSION_AUDIO_LOADED_NODE_TYPE = "core.audio_has_mission_audio_loaded"

---@type LDNodeEditorNodeType
local audioHasMissionAudioLoadedNode = {
    typeName = AUDIO_HAS_MISSION_AUDIO_LOADED_NODE_TYPE,
    category = "opcode.audio",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorAudioHasMissionAudioLoadedNode
        local newNode = {
            id = newNodeId,
            nodeType = AUDIO_HAS_MISSION_AUDIO_LOADED_NODE_TYPE,
            inputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.flow",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.number",
                }

            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.flow",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.bool",
                }

            },
			slotId = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorAudioHasMissionAudioLoadedNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "slotId", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.MissionAudioSlot) do
                if value == node.slotId then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##slotIdSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##slotIdSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##slotIdSelect", 0) then
                    if ImGui.BeginListBox("##slotIdSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.MissionAudioSlot) do
                            if ImGui.Selectable(key, value == node.slotId, 0, ImVec2.new()) then
                                node.slotId = value;
                                ImGui.CloseCurrentPopup();
                            end
                        end
                        ImGui.EndListBox();
                    end
                    ImGui.EndPopup();
                end
                NodeEditor.Resume();
            end
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorAudioHasMissionAudioLoadedNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local slotId = inputValues[2] or node.slotId
		local result = AudioOp.hasMissionAudioLoaded(slotId)
        return {1, result}
    end
}

return audioHasMissionAudioLoadedNode
