local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")

---@class LDNodeEditorInvertNode : LDNodeEditorNode

local INVERT_NODE_TYPE = "core.invert"

---@type LDNodeEditorNodeType
local invertNode = {
    typeName = INVERT_NODE_TYPE,
    category = "main",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorInvertNode
        local newNode = {
            id = newNodeId,
            nodeType = INVERT_NODE_TYPE,
            inputs = {
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
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorInvertNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        builder:Begin(NodeEditor.NodeId(node.id));

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");

        ImGui.SameLine(0,-1);
        ImGui.Text(fa.ICON_FA_TILDE);

        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorInvertNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        if inputValues[1] == nil then
            error("Value is not set!")
        end
        return {not inputValues[1]}
    end
}

return invertNode