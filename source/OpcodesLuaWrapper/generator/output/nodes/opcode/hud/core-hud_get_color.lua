local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorHudGetColorNode : LDNodeEditorNode
---@field hudObject integer

local HUD_GET_COLOR_NODE_TYPE = "core.hud_get_color"

---@type LDNodeEditorNodeType
local hudGetColorNode = {
    typeName = HUD_GET_COLOR_NODE_TYPE,
    category = "opcode.hud",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorHudGetColorNode
        local newNode = {
            id = newNodeId,
            nodeType = HUD_GET_COLOR_NODE_TYPE,
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

            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.flow",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }

            },
			hudObject = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorHudGetColorNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "hudObject", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.HudObject) do
                if value == node.hudObject then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##hudObjectSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##hudObjectSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##hudObjectSelect", 0) then
                    if ImGui.BeginListBox("##hudObjectSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.HudObject) do
                            if ImGui.Selectable(key, value == node.hudObject, 0, ImVec2.new()) then
                                node.hudObject = value;
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

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "red");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[3], "green");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[4], "blue");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[5], "alpha");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorHudGetColorNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local hudObject = inputValues[2] or node.hudObject
		local red, green, blue, alpha = HudOp.getColor(hudObject)
        return {1, red, green, blue, alpha}
    end
}

return hudGetColorNode
