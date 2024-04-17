local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorStreamingLoadSpecialCharacterNode : LDNodeEditorNode
---@field slotId integer
---@field modelName string

local STREAMING_LOAD_SPECIAL_CHARACTER_NODE_TYPE = "core.streaming_load_special_character"

---@type LDNodeEditorNodeType
local streamingLoadSpecialCharacterNode = {
    typeName = STREAMING_LOAD_SPECIAL_CHARACTER_NODE_TYPE,
    category = "opcode.streaming",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorStreamingLoadSpecialCharacterNode
        local newNode = {
            id = newNodeId,
            nodeType = STREAMING_LOAD_SPECIAL_CHARACTER_NODE_TYPE,
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
			slotId = 0,
			modelName = ""
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorStreamingLoadSpecialCharacterNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "slotId", function ()
            node.slotId = editor.dataTypes[node.inputs[2].type].drawEditValue(node.slotId, "##slotIdEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "modelName", function ()
            node.modelName = editor.dataTypes[node.inputs[3].type].drawEditValue(node.modelName, "##modelNameEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorStreamingLoadSpecialCharacterNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local slotId = inputValues[2] or node.slotId
		local modelName = inputValues[3] or node.modelName
		StreamingOp.loadSpecialCharacter(math.floor(slotId), modelName)
        return {1}
    end
}

return streamingLoadSpecialCharacterNode
