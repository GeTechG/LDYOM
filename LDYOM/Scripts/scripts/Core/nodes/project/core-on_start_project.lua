local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")

---@class LDNodeEditorOnStartProjectNode : LDNodeEditorNode

local ON_START_PROJECT_NODE_TYPE = "core.on_start_project"

---@type LDNodeEditorNodeType
local onStartProjectNode = {
    typeName = ON_START_PROJECT_NODE_TYPE,
    category = "project",
    icon = nodesIcons["event"],
    color = nodesColors["event"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorOnStartProjectNode
        local newNode = {
            id = newNodeId,
            nodeType = ON_START_PROJECT_NODE_TYPE,
            inputs = {},
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.flow",
                }
            },
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorOnStartProjectNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);
        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");
        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorOnStartProjectNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        return {1}
    end
}

return onStartProjectNode