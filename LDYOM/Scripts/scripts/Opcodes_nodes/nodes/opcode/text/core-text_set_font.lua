local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTextSetFontNode : LDNodeEditorNode
---@field font integer

local TEXT_SET_FONT_NODE_TYPE = "core.text_set_font"

---@type LDNodeEditorNodeType
local textSetFontNode = {
    typeName = TEXT_SET_FONT_NODE_TYPE,
    category = "opcode.text",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTextSetFontNode
        local newNode = {
            id = newNodeId,
            nodeType = TEXT_SET_FONT_NODE_TYPE,
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

            },
			font = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTextSetFontNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "font", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.Font) do
                if value == node.font then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##fontSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##fontSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##fontSelect", 0) then
                    if ImGui.BeginListBox("##fontSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.Font) do
                            if ImGui.Selectable(key, value == node.font, 0, ImVec2.new()) then
                                node.font = value;
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
    ---@param node LDNodeEditorTextSetFontNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local font = inputValues[2] or node.font
		TextOp.setFont(font)
        return {1}
    end
}

return textSetFontNode
