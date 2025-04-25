local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")

---@class LDNodeEditorNumberToStringNode : LDNodeEditorNode

local NUMBER_TO_STRING_NODE_TYPE = "core.number_to_string"

---@type LDNodeEditorNodeType
local waitNode = {
    typeName = NUMBER_TO_STRING_NODE_TYPE,
    category = "main",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorNumberToStringNode
        local newNode = {
            id = newNodeId,
            nodeType = NUMBER_TO_STRING_NODE_TYPE,
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
                    type = "core.string",
                },
            },
            waitTime = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorNumberToStringNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");

        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorNumberToStringNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local value = inputValues[1] or 0
        return {tostring(value)}
    end
}

return waitNode