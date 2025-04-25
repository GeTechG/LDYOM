local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorStreamingMarkMissionTrainsAsNoLongerNeededNode : LDNodeEditorNode


local STREAMING_MARK_MISSION_TRAINS_AS_NO_LONGER_NEEDED_NODE_TYPE = "core.streaming_mark_mission_trains_as_no_longer_needed"

---@type LDNodeEditorNodeType
local streamingMarkMissionTrainsAsNoLongerNeededNode = {
    typeName = STREAMING_MARK_MISSION_TRAINS_AS_NO_LONGER_NEEDED_NODE_TYPE,
    category = "opcode.streaming",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorStreamingMarkMissionTrainsAsNoLongerNeededNode
        local newNode = {
            id = newNodeId,
            nodeType = STREAMING_MARK_MISSION_TRAINS_AS_NO_LONGER_NEEDED_NODE_TYPE,
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

            }
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorStreamingMarkMissionTrainsAsNoLongerNeededNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorStreamingMarkMissionTrainsAsNoLongerNeededNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        StreamingOp.markMissionTrainsAsNoLongerNeeded()
        return {1}
    end
}

return streamingMarkMissionTrainsAsNoLongerNeededNode
