local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")

---@class LDNodeEditorBranchNode : LDNodeEditorNode

local BRANCH_NODE_TYPE = "core.branch"

---@type LDNodeEditorNodeType
local branchNode = {
    typeName = BRANCH_NODE_TYPE,
    category = "main",
    icon = nodesIcons["branch"],
    color = nodesColors["branch"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorBranchNode
        local newNode = {
            id = newNodeId,
            nodeType = BRANCH_NODE_TYPE,
            inputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.flow",
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
                    type = "core.flow",
                },
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
    ---@param node LDNodeEditorBranchNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], ld.loc.get("general.condition"));

        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], ld.loc.get("general.true"));
        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], ld.loc.get("general.false"));

        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorBranchNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        if inputValues[2] == nil then
            error("Condition is not set!")
        end
        local condition = inputValues[2]
        if condition then
            return {1, 0}
        else
            return {0, 1}
        end
    end
}

return branchNode