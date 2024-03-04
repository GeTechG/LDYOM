local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorHudSetCounterFlashWhenFirstDisplayedNode : LDNodeEditorNode
---@field counter integer
---@field state boolean

local HUD_SET_COUNTER_FLASH_WHEN_FIRST_DISPLAYED_NODE_TYPE = "core.hud_set_counter_flash_when_first_displayed"

---@type LDNodeEditorNodeType
local hudSetCounterFlashWhenFirstDisplayedNode = {
    typeName = HUD_SET_COUNTER_FLASH_WHEN_FIRST_DISPLAYED_NODE_TYPE,
    category = "opcode.hud",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorHudSetCounterFlashWhenFirstDisplayedNode
        local newNode = {
            id = newNodeId,
            nodeType = HUD_SET_COUNTER_FLASH_WHEN_FIRST_DISPLAYED_NODE_TYPE,
            inputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.flow",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.number",
                }
				,{
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
                }

            },
			counter = 0,
			state = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorHudSetCounterFlashWhenFirstDisplayedNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "counter", function ()
            node.counter = editor.dataTypes[node.inputs[2].type].drawEditValue(node.counter, "##counterEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "state", function ()
            node.state = editor.dataTypes[node.inputs[3].type].drawEditValue(node.state, "##stateEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorHudSetCounterFlashWhenFirstDisplayedNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local counter = inputValues[2] or node.counter
		local state = inputValues[3] or node.state
		HudOp.setCounterFlashWhenFirstDisplayed(counter, state)
        return {1}
    end
}

return hudSetCounterFlashWhenFirstDisplayedNode
