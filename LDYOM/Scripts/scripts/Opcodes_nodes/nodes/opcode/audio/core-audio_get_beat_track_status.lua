local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorAudioGetBeatTrackStatusNode : LDNodeEditorNode


local AUDIO_GET_BEAT_TRACK_STATUS_NODE_TYPE = "core.audio_get_beat_track_status"

---@type LDNodeEditorNodeType
local audioGetBeatTrackStatusNode = {
    typeName = AUDIO_GET_BEAT_TRACK_STATUS_NODE_TYPE,
    category = "opcode.audio",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorAudioGetBeatTrackStatusNode
        local newNode = {
            id = newNodeId,
            nodeType = AUDIO_GET_BEAT_TRACK_STATUS_NODE_TYPE,
            inputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.flow",
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
                    type = "core.number",
                }

            }
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorAudioGetBeatTrackStatusNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "status");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorAudioGetBeatTrackStatusNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local status = AudioOp.getBeatTrackStatus()
        return {1, status}
    end
}

return audioGetBeatTrackStatusNode
