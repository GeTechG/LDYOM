local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorWorldClearAllScriptRoadblocksNode : LDNodeEditorNode


local WORLD_CLEAR_ALL_SCRIPT_ROADBLOCKS_NODE_TYPE = "core.world_clear_all_script_roadblocks"

---@type LDNodeEditorNodeType
local worldClearAllScriptRoadblocksNode = {
    typeName = WORLD_CLEAR_ALL_SCRIPT_ROADBLOCKS_NODE_TYPE,
    category = "opcode.world",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorWorldClearAllScriptRoadblocksNode
        local newNode = {
            id = newNodeId,
            nodeType = WORLD_CLEAR_ALL_SCRIPT_ROADBLOCKS_NODE_TYPE,
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
                }

            }
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorWorldClearAllScriptRoadblocksNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorWorldClearAllScriptRoadblocksNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        WorldOp.clearAllScriptRoadblocks()
        return {1}
    end
}

return worldClearAllScriptRoadblocksNode
