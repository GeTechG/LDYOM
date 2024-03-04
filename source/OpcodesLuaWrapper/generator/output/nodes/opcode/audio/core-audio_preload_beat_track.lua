local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorAudioPreloadBeatTrackNode : LDNodeEditorNode
---@field trackId integer

local AUDIO_PRELOAD_BEAT_TRACK_NODE_TYPE = "core.audio_preload_beat_track"

---@type LDNodeEditorNodeType
local audioPreloadBeatTrackNode = {
    typeName = AUDIO_PRELOAD_BEAT_TRACK_NODE_TYPE,
    category = "opcode.audio",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorAudioPreloadBeatTrackNode
        local newNode = {
            id = newNodeId,
            nodeType = AUDIO_PRELOAD_BEAT_TRACK_NODE_TYPE,
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
			trackId = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorAudioPreloadBeatTrackNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "trackId", function ()
            node.trackId = editor.dataTypes[node.inputs[2].type].drawEditValue(node.trackId, "##trackIdEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorAudioPreloadBeatTrackNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local trackId = inputValues[2] or node.trackId
		AudioOp.preloadBeatTrack(trackId)
        return {1}
    end
}

return audioPreloadBeatTrackNode
