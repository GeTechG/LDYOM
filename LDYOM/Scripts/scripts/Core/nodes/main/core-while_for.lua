local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")

---@class LDNodeEditorWhileForNode : LDNodeEditorNode

local WHILE_FOR_NODE_TYPE = "core.while_for"

---@type LDNodeEditorNodeType
local whileForNode = {
    typeName = WHILE_FOR_NODE_TYPE,
    category = "main",
    icon = nodesIcons["loop"],
    color = nodesColors["loop"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorWhileForNode
        local newNode = {
            id = newNodeId,
            nodeType = WHILE_FOR_NODE_TYPE,
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
    ---@param node LDNodeEditorWhileForNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], ld.loc.get("general.condition"));

        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], ld.loc.get("nodes.core.while_for.to"));
        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], ld.loc.get("nodes.core.while_for.on_true"));

        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorWhileForNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local condition = inputValues[2]
        if condition == nil then
            error("Condition is not set")
        end
        while not condition do
            local pinsInputs = {}
            for _, link in ipairs(context.__links) do
                if link.outputId == node.outputs[1].id then
                    local nodeCallId = math.floor(link.inputId / 100) * 100
                    LDNodeEditor.runNode(editor, context, context.nodes[nodeCallId], pinsInputs)
                end
            end
            coroutine.yield(1)
            inputValues = LDNodeEditor.getInputValues(editor, context, node, pinsInputs)
            condition = inputValues[2]
            if condition == nil then
                error("Condition is not set")
            end
        end
        return {0, 1}
    end
}

return whileForNode