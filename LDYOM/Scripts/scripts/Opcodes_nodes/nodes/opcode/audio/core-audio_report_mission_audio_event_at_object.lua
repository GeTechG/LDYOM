local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorAudioReportMissionAudioEventAtObjectNode : LDNodeEditorNode
---@field handle integer
---@field soundId integer

local AUDIO_REPORT_MISSION_AUDIO_EVENT_AT_OBJECT_NODE_TYPE = "core.audio_report_mission_audio_event_at_object"

---@type LDNodeEditorNodeType
local audioReportMissionAudioEventAtObjectNode = {
    typeName = AUDIO_REPORT_MISSION_AUDIO_EVENT_AT_OBJECT_NODE_TYPE,
    category = "opcode.audio",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorAudioReportMissionAudioEventAtObjectNode
        local newNode = {
            id = newNodeId,
            nodeType = AUDIO_REPORT_MISSION_AUDIO_EVENT_AT_OBJECT_NODE_TYPE,
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
                    type = "core.Object",
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
			handle = 0,
			soundId = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorAudioReportMissionAudioEventAtObjectNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "handle", function ()
            node.handle = editor.dataTypes[node.inputs[2].type].drawEditValue(node.handle, "##handleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "soundId", function ()
            node.soundId = editor.dataTypes[node.inputs[3].type].drawEditValue(node.soundId, "##soundIdEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorAudioReportMissionAudioEventAtObjectNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local handle = inputValues[2] or node.handle
		local soundId = inputValues[3] or node.soundId
		AudioOp.reportMissionAudioEventAtObject(handle, soundId)
        return {1}
    end
}

return audioReportMissionAudioEventAtObjectNode
