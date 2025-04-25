local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")

---@enum CalcNumberType
local CalcNumberType = {
    Add = 0,
    Subtract = 1,
    Multiply = 2,
    Divide = 3,
}

local icons = {
    [CalcNumberType.Add] = fa.ICON_FA_PLUS,
    [CalcNumberType.Subtract] = fa.ICON_FA_MINUS,
    [CalcNumberType.Multiply] = fa.ICON_FA_TIMES,
    [CalcNumberType.Divide] = fa.ICON_FA_DIVIDE,
}

---@class LDNodeEditorCalcNumberNode : LDNodeEditorNode
---@field calcType CalcNumberType
---@field aValue number
---@field bValue number

local CALC_NUMBER_NODE_TYPE = "core.calc_number"

---@type LDNodeEditorNodeType
local waitNode = {
    typeName = CALC_NUMBER_NODE_TYPE,
    category = "main",
    icon = fa.ICON_FA_CALCULATOR,
    color = nodesColors["function"],
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCalcNumberNode
        local newNode = {
            id = newNodeId,
            nodeType = CALC_NUMBER_NODE_TYPE,
            inputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.number",
                },
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
                    type = "core.number",
                },
            },
            calcType = CalcNumberType.Add,
            aValue = 0,
            bValue = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCalcNumberNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "A", function ()
            node.aValue = editor.dataTypes[node.inputs[1].type].drawEditValue(node.aValue, "##aEdit", fontScale * 100)
        end);

        ImGui.Dummy(ImVec2.new(fontScale * 28, 0));
        ImGui.SameLine(0,-1);
        ImGui.SetNextItemWidth(95 * fontScale);
        local isEdit, newCalcType = ImGui.SliderInt("##slider", node.calcType, 0, #icons, icons[node.calcType], 0);
        if isEdit then
            node.calcType = newCalcType
        end

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "B", function ()
            node.bValue = editor.dataTypes[node.inputs[2].type].drawEditValue(node.bValue, "##bEdit", fontScale * 100)
        end);

        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCalcNumberNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local a = inputValues[1] or node.aValue
        local b = inputValues[2] or node.bValue
        local result = 0
        if node.calcType == CalcNumberType.Add then
            result = a + b
        elseif node.calcType == CalcNumberType.Subtract then
            result = a - b
        elseif node.calcType == CalcNumberType.Multiply then
            result = a * b
        elseif node.calcType == CalcNumberType.Divide then
            result = a / b
        end
        return {result}
    end
}

return waitNode