local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")

---@class LDNodeEditorBooleanOperationsNode : LDNodeEditorNode
---@field operationType integer
---@field aValue boolean
---@field bValue boolean

local operations = {"AND", "OR", "XOR"}

local BOOLEAN_OPERATIONS_NODE_TYPE = "core.boolean_operations"

---@type LDNodeEditorNodeType
local booleanOperationsNode = {
    typeName = BOOLEAN_OPERATIONS_NODE_TYPE,
    category = "main",
    icon = nodesIcons["comprasion"],
    color = nodesColors["comprasion"],
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorBooleanOperationsNode
        local newNode = {
            id = newNodeId,
            nodeType = BOOLEAN_OPERATIONS_NODE_TYPE,
            inputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.bool",
                },
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.bool",
                }
            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.bool",
                },
            },
            operationType = 1,
            aValue = false,
            bValue = false,
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorBooleanOperationsNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "A", function ()
            node.aValue = editor.dataTypes[node.inputs[1].type].drawEditValue(node.aValue, "##aEdit", fontScale * 100)
        end);

        ImGui.SetNextItemWidth(fontScale * 100);
        local isEdit, newValue = ImGui.SliderInt("##operationType", node.operationType, 1, #operations, operations[node.operationType], 0);
        if isEdit then
            node.operationType = newValue
        end

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "B", function ()
            node.bValue = editor.dataTypes[node.inputs[2].type].drawEditValue(node.bValue, "##bEdit", fontScale * 100)
        end);

        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorBooleanOperationsNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local a = inputValues[1]
        local b = inputValues[2]
        if node.operationType == 1 then
            return a and b
        elseif node.operationType == 2 then
            return a or b
        elseif node.operationType == 3 then
            return (a == true and b == false) or (a == false and b == true)
        end
    end
}

return booleanOperationsNode