local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")

---@class LDNodeEditorConcatStringNode : LDNodeEditorNode
---@field aValue string
---@field bValue string

local CALC_NUMBER_NODE_TYPE = "core.concat_string"

---@type LDNodeEditorNodeType
local waitNode = {
    typeName = CALC_NUMBER_NODE_TYPE,
    category = "main",
    icon = fa.ICON_FA_PAPERCLIP,
    color = nodesColors["function"],
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorConcatStringNode
        local newNode = {
            id = newNodeId,
            nodeType = CALC_NUMBER_NODE_TYPE,
            inputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.string",
                },
                {
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
                    type = "core.string",
                },
            },
            aValue = "",
            bValue = ""
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorConcatStringNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "A", function ()
            node.aValue = editor.dataTypes[node.inputs[1].type].drawEditValue(node.aValue, "##aEdit", fontScale * 100)
        end);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "B", function ()
            node.bValue = editor.dataTypes[node.inputs[2].type].drawEditValue(node.bValue, "##bEdit", fontScale * 100)
        end);

        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorConcatStringNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local a = inputValues[1] or node.aValue
        local b = inputValues[2] or node.bValue
        return {a .. b}
    end
}

return waitNode