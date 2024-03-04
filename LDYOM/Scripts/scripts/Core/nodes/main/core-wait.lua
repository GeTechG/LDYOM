local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")

---@class LDNodeEditorWaitNode : LDNodeEditorNode
---@field waitTime number

local WAIT_NODE_TYPE = "core.wait"

---@type LDNodeEditorNodeType
local waitNode = {
    typeName = WAIT_NODE_TYPE,
    category = "main",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorWaitNode
        local newNode = {
            id = newNodeId,
            nodeType = WAIT_NODE_TYPE,
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
                },
            },
            waitTime = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorWaitNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");

        ImGui.SameLine(0,-1);
        ImGui.SetNextItemWidth(100 * fontScale);
        local isEdit, newWaitTime = ImGui.InputFloat(ld.loc.get("general.time"), node.waitTime, 0, 0, "%.3f", 0);
        if isEdit then
            node.waitTime = newWaitTime
        end

        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorWaitNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        _ = coroutine.yield(node.waitTime * 1000)
        return {1}
    end
}

return waitNode