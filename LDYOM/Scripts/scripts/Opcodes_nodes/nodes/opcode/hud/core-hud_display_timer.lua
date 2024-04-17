local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorHudDisplayTimerNode : LDNodeEditorNode
---@field timer integer
---@field direction integer

local HUD_DISPLAY_TIMER_NODE_TYPE = "core.hud_display_timer"

---@type LDNodeEditorNodeType
local hudDisplayTimerNode = {
    typeName = HUD_DISPLAY_TIMER_NODE_TYPE,
    category = "opcode.hud",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorHudDisplayTimerNode
        local newNode = {
            id = newNodeId,
            nodeType = HUD_DISPLAY_TIMER_NODE_TYPE,
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

            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.flow",
                }

            },
			timer = 0,
			direction = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorHudDisplayTimerNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "timer", function ()
            node.timer = editor.dataTypes[node.inputs[2].type].drawEditValue(node.timer, "##timerEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "direction", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.TimerDirection) do
                if value == node.direction then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##directionSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##directionSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##directionSelect", 0) then
                    if ImGui.BeginListBox("##directionSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.TimerDirection) do
                            if ImGui.Selectable(key, value == node.direction, 0, ImVec2.new()) then
                                node.direction = value;
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

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorHudDisplayTimerNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local timer = inputValues[2] or node.timer
		local direction = inputValues[3] or node.direction
		HudOp.displayTimer(math.floor(timer), direction)
        return {1}
    end
}

return hudDisplayTimerNode
