local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTextPrintBigFormattedNode : LDNodeEditorNode
---@field fmt string
---@field time integer
---@field style integer
---@field args integer

local TEXT_PRINT_BIG_FORMATTED_NODE_TYPE = "core.text_print_big_formatted"

---@type LDNodeEditorNodeType
local textPrintBigFormattedNode = {
    typeName = TEXT_PRINT_BIG_FORMATTED_NODE_TYPE,
    category = "opcode.text",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTextPrintBigFormattedNode
        local newNode = {
            id = newNodeId,
            nodeType = TEXT_PRINT_BIG_FORMATTED_NODE_TYPE,
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
                    type = "core.string",
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

            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.flow",
                }

            },
			fmt = "",
			time = 0,
			style = 0,
			args = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTextPrintBigFormattedNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "fmt", function ()
            node.fmt = editor.dataTypes[node.inputs[2].type].drawEditValue(node.fmt, "##fmtEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "time", function ()
            node.time = editor.dataTypes[node.inputs[3].type].drawEditValue(node.time, "##timeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "style", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.TextStyle) do
                if value == node.style then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##styleSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##styleSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##styleSelect", 0) then
                    if ImGui.BeginListBox("##styleSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.TextStyle) do
                            if ImGui.Selectable(key, value == node.style, 0, ImVec2.new()) then
                                node.style = value;
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "args", function ()
            node.args = editor.dataTypes[node.inputs[5].type].drawEditValue(node.args, "##argsEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorTextPrintBigFormattedNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local fmt = inputValues[2] or node.fmt
		local time = inputValues[3] or node.time
		local style = inputValues[4] or node.style
		local args = inputValues[5] or node.args
		TextOp.printBigFormatted(fmt, time, style, args)
        return {1}
    end
}

return textPrintBigFormattedNode
