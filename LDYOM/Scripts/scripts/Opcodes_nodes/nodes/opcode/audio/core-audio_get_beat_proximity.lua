local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorAudioGetBeatProximityNode : LDNodeEditorNode
---@field _p1 integer

local AUDIO_GET_BEAT_PROXIMITY_NODE_TYPE = "core.audio_get_beat_proximity"

---@type LDNodeEditorNodeType
local audioGetBeatProximityNode = {
    typeName = AUDIO_GET_BEAT_PROXIMITY_NODE_TYPE,
    category = "opcode.audio",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorAudioGetBeatProximityNode
        local newNode = {
            id = newNodeId,
            nodeType = AUDIO_GET_BEAT_PROXIMITY_NODE_TYPE,
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
                    type = "core.number",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }

            },
			_p1 = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorAudioGetBeatProximityNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "_p1", function ()
            node._p1 = editor.dataTypes[node.inputs[2].type].drawEditValue(node._p1, "##_p1Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "_p2");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[3], "_p3");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[4], "_p4");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorAudioGetBeatProximityNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local _p1 = inputValues[2] or node._p1
		local _p2, _p3, _p4 = AudioOp.getBeatProximity(math.floor(_p1))
        return {1, _p2, _p3, _p4}
    end
}

return audioGetBeatProximityNode
