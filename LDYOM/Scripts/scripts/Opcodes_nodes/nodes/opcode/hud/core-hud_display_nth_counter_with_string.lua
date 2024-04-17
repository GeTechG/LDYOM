local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorHudDisplayNthCounterWithStringNode : LDNodeEditorNode
---@field counter integer
---@field display integer
---@field slot integer
---@field text string

local HUD_DISPLAY_NTH_COUNTER_WITH_STRING_NODE_TYPE = "core.hud_display_nth_counter_with_string"

---@type LDNodeEditorNodeType
local hudDisplayNthCounterWithStringNode = {
    typeName = HUD_DISPLAY_NTH_COUNTER_WITH_STRING_NODE_TYPE,
    category = "opcode.hud",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorHudDisplayNthCounterWithStringNode
        local newNode = {
            id = newNodeId,
            nodeType = HUD_DISPLAY_NTH_COUNTER_WITH_STRING_NODE_TYPE,
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
                    type = "core.number",
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
                    type = "core.string",
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
			display = 0,
			slot = 0,
			text = ""
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorHudDisplayNthCounterWithStringNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "counter", function ()
            node.counter = editor.dataTypes[node.inputs[2].type].drawEditValue(node.counter, "##counterEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "display", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.CounterDisplay) do
                if value == node.display then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##displaySelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##displaySelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##displaySelect", 0) then
                    if ImGui.BeginListBox("##displaySelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.CounterDisplay) do
                            if ImGui.Selectable(key, value == node.display, 0, ImVec2.new()) then
                                node.display = value;
                                ImGui.CloseCurrentPopup();
                            end
                        end
                        ImGui.EndListBox();
                    end
                    ImGui.EndPopup();
                end
                NodeEditor.Resume();
            end
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "slot", function ()
            node.slot = editor.dataTypes[node.inputs[4].type].drawEditValue(node.slot, "##slotEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "text", function ()
            node.text = editor.dataTypes[node.inputs[5].type].drawEditValue(node.text, "##textEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorHudDisplayNthCounterWithStringNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local counter = inputValues[2] or node.counter
		local display = inputValues[3] or node.display
		local slot = inputValues[4] or node.slot
		local text = inputValues[5] or node.text
		HudOp.displayNthCounterWithString(math.floor(counter), display, math.floor(slot), text)
        return {1}
    end
}

return hudDisplayNthCounterWithStringNode
