local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")

---@class LDNodeEditorTestNode : LDNodeEditorNode

local TEST_NODE_TYPE = "core.test_node"

---@type LDNodeEditorNodeType
local testNode = {
    typeName = TEST_NODE_TYPE,
    ---@param ctx LDNodeEditorContext
    new = function(ctx)
        local newNodeId = LDNodeEditorContext.getNextUniqueId(ctx)
        ---@type LDNodeEditorTestNode
        local newNode = {
            id = newNodeId,
            nodeType = TEST_NODE_TYPE,
            inputs = {
                {
                    id = LDNodeEditorContext.getNextUniqueId(ctx),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.number",
                }
            },
            outputs = {
                {
                    id = LDNodeEditorContext.getNextUniqueId(ctx),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }
            },
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTestNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(builder, node, nodesColors["function"]);
        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], ld.loc.getArray("nodes.core.test_node.inputs")[1]);
        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], ld.loc.getArray("nodes.core.test_node.outputs")[1]);
        builder:End();
    end,
    ---@param node LDNodeEditorTestNode
    run = function(node)

    end
}

return testNode