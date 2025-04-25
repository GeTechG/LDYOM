local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorPadIsSkipCutsceneButtonPressedNode : LDNodeEditorNode


local PAD_IS_SKIP_CUTSCENE_BUTTON_PRESSED_NODE_TYPE = "core.pad_is_skip_cutscene_button_pressed"

---@type LDNodeEditorNodeType
local padIsSkipCutsceneButtonPressedNode = {
    typeName = PAD_IS_SKIP_CUTSCENE_BUTTON_PRESSED_NODE_TYPE,
    category = "opcode.pad",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorPadIsSkipCutsceneButtonPressedNode
        local newNode = {
            id = newNodeId,
            nodeType = PAD_IS_SKIP_CUTSCENE_BUTTON_PRESSED_NODE_TYPE,
            inputs = {
                
            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.bool",
                }

            }
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorPadIsSkipCutsceneButtonPressedNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorPadIsSkipCutsceneButtonPressedNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local result = PadOp.isSkipCutsceneButtonPressed()
        return {result}
    end
}

return padIsSkipCutsceneButtonPressedNode
