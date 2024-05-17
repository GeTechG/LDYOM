local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")

---@class LDNodeEditorDeltaTimeNode : LDNodeEditorNode

local DELTA_TIME_NODE_TYPE = "core.delta_time"

---@type LDNodeEditorNodeType
local deltaTimeNode = {
    typeName = DELTA_TIME_NODE_TYPE,
    category = "main",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorDeltaTimeNode
        local newNode = {
            id = newNodeId,
            nodeType = DELTA_TIME_NODE_TYPE,
            inputs = {
            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                },
            },
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

        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "deltaTime");

        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorWaitNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        return {getDeltaTime()}
    end
}

return deltaTimeNode