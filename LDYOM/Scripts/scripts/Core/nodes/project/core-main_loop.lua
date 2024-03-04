local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")

---@class LDNodeEditorMainLoopNode : LDNodeEditorNode

local MAIN_LOOP_NODE_TYPE = "core.main_loop"

---@type LDNodeEditorNodeType
local mainLoopNode = {
    typeName = MAIN_LOOP_NODE_TYPE,
    category = "project",
    icon = nodesIcons["loop"],
    color = nodesColors["loop"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorMainLoopNode
        local newNode = {
            id = newNodeId,
            nodeType = MAIN_LOOP_NODE_TYPE,
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
    ---@param node LDNodeEditorMainLoopNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);
        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");
        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorMainLoopNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        return {1}
    end
}

return mainLoopNode