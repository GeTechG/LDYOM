local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorStreamingHasCarRecordingBeenLoadedNode : LDNodeEditorNode
---@field pathId integer

local STREAMING_HAS_CAR_RECORDING_BEEN_LOADED_NODE_TYPE = "core.streaming_has_car_recording_been_loaded"

---@type LDNodeEditorNodeType
local streamingHasCarRecordingBeenLoadedNode = {
    typeName = STREAMING_HAS_CAR_RECORDING_BEEN_LOADED_NODE_TYPE,
    category = "opcode.streaming",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorStreamingHasCarRecordingBeenLoadedNode
        local newNode = {
            id = newNodeId,
            nodeType = STREAMING_HAS_CAR_RECORDING_BEEN_LOADED_NODE_TYPE,
            inputs = {
                {
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
                    type = "core.bool",
                }

            },
			pathId = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorStreamingHasCarRecordingBeenLoadedNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "pathId", function ()
            node.pathId = editor.dataTypes[node.inputs[1].type].drawEditValue(node.pathId, "##pathIdEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorStreamingHasCarRecordingBeenLoadedNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local pathId = inputValues[1] or node.pathId
		local result = StreamingOp.hasCarRecordingBeenLoaded(pathId)
        return {result}
    end
}

return streamingHasCarRecordingBeenLoadedNode
