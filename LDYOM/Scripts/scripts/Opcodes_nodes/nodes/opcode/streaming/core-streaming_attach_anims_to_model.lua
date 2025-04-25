local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorStreamingAttachAnimsToModelNode : LDNodeEditorNode
---@field pedModelId integer
---@field animationFile string

local STREAMING_ATTACH_ANIMS_TO_MODEL_NODE_TYPE = "core.streaming_attach_anims_to_model"

---@type LDNodeEditorNodeType
local streamingAttachAnimsToModelNode = {
    typeName = STREAMING_ATTACH_ANIMS_TO_MODEL_NODE_TYPE,
    category = "opcode.streaming",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorStreamingAttachAnimsToModelNode
        local newNode = {
            id = newNodeId,
            nodeType = STREAMING_ATTACH_ANIMS_TO_MODEL_NODE_TYPE,
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
                    type = "core.string",
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
			pedModelId = 0,
			animationFile = ""
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorStreamingAttachAnimsToModelNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "pedModelId", function ()
            node.pedModelId = editor.dataTypes[node.inputs[2].type].drawEditValue(node.pedModelId, "##pedModelIdEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "animationFile", function ()
            node.animationFile = editor.dataTypes[node.inputs[3].type].drawEditValue(node.animationFile, "##animationFileEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorStreamingAttachAnimsToModelNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local pedModelId = inputValues[2] or node.pedModelId
		local animationFile = inputValues[3] or node.animationFile
		StreamingOp.attachAnimsToModel(math.floor(pedModelId), animationFile)
        return {1}
    end
}

return streamingAttachAnimsToModelNode
