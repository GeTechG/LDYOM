local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorAudioLoadMissionAudioNode : LDNodeEditorNode
---@field slotId integer
---@field audioId integer

local AUDIO_LOAD_MISSION_AUDIO_NODE_TYPE = "core.audio_load_mission_audio"

---@type LDNodeEditorNodeType
local audioLoadMissionAudioNode = {
    typeName = AUDIO_LOAD_MISSION_AUDIO_NODE_TYPE,
    category = "opcode.audio",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorAudioLoadMissionAudioNode
        local newNode = {
            id = newNodeId,
            nodeType = AUDIO_LOAD_MISSION_AUDIO_NODE_TYPE,
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

            },
			slotId = 0,
			audioId = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorAudioLoadMissionAudioNode
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "audioId", function ()
            node.audioId = editor.dataTypes[node.inputs[3].type].drawEditValue(node.audioId, "##audioIdEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorAudioLoadMissionAudioNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local slotId = inputValues[2] or node.slotId
		local audioId = inputValues[3] or node.audioId
		AudioOp.loadMissionAudio(slotId, audioId)
        return {1}
    end
}

return audioLoadMissionAudioNode
