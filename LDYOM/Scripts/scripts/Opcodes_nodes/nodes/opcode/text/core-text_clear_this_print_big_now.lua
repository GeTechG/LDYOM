local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTextClearThisPrintBigNowNode : LDNodeEditorNode
---@field textStyle integer

local TEXT_CLEAR_THIS_PRINT_BIG_NOW_NODE_TYPE = "core.text_clear_this_print_big_now"

---@type LDNodeEditorNodeType
local textClearThisPrintBigNowNode = {
    typeName = TEXT_CLEAR_THIS_PRINT_BIG_NOW_NODE_TYPE,
    category = "opcode.text",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTextClearThisPrintBigNowNode
        local newNode = {
            id = newNodeId,
            nodeType = TEXT_CLEAR_THIS_PRINT_BIG_NOW_NODE_TYPE,
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
			textStyle = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTextClearThisPrintBigNowNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "textStyle", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.TextStyle) do
                if value == node.textStyle then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##textStyleSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##textStyleSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##textStyleSelect", 0) then
                    if ImGui.BeginListBox("##textStyleSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.TextStyle) do
                            if ImGui.Selectable(key, value == node.textStyle, 0, ImVec2.new()) then
                                node.textStyle = value;
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
    ---@param node LDNodeEditorTextClearThisPrintBigNowNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local textStyle = inputValues[2] or node.textStyle
		TextOp.clearThisPrintBigNow(textStyle)
        return {1}
    end
}

return textClearThisPrintBigNowNode
