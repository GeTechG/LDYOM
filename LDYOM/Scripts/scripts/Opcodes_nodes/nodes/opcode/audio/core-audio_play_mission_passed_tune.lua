local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorAudioPlayMissionPassedTuneNode : LDNodeEditorNode
---@field soundId integer

local AUDIO_PLAY_MISSION_PASSED_TUNE_NODE_TYPE = "core.audio_play_mission_passed_tune"

---@type LDNodeEditorNodeType
local audioPlayMissionPassedTuneNode = {
    typeName = AUDIO_PLAY_MISSION_PASSED_TUNE_NODE_TYPE,
    category = "opcode.audio",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorAudioPlayMissionPassedTuneNode
        local newNode = {
            id = newNodeId,
            nodeType = AUDIO_PLAY_MISSION_PASSED_TUNE_NODE_TYPE,
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

            },
			soundId = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorAudioPlayMissionPassedTuneNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "soundId", function ()
            node.soundId = editor.dataTypes[node.inputs[2].type].drawEditValue(node.soundId, "##soundIdEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorAudioPlayMissionPassedTuneNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local soundId = inputValues[2] or node.soundId
		AudioOp.playMissionPassedTune(soundId)
        return {1}
    end
}

return audioPlayMissionPassedTuneNode