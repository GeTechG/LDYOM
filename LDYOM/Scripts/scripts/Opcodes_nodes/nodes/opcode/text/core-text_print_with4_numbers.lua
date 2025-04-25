local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTextPrintWith4NumbersNode : LDNodeEditorNode
---@field key string
---@field num1 integer
---@field num2 integer
---@field num3 integer
---@field num4 integer
---@field duration integer
---@field style integer

local TEXT_PRINT_WITH4_NUMBERS_NODE_TYPE = "core.text_print_with4_numbers"

---@type LDNodeEditorNodeType
local textPrintWith4NumbersNode = {
    typeName = TEXT_PRINT_WITH4_NUMBERS_NODE_TYPE,
    category = "opcode.text",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTextPrintWith4NumbersNode
        local newNode = {
            id = newNodeId,
            nodeType = TEXT_PRINT_WITH4_NUMBERS_NODE_TYPE,
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
			key = "",
			num1 = 0,
			num2 = 0,
			num3 = 0,
			num4 = 0,
			duration = 0,
			style = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTextPrintWith4NumbersNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "key", function ()
            node.key = editor.dataTypes[node.inputs[2].type].drawEditValue(node.key, "##keyEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "num1", function ()
            node.num1 = editor.dataTypes[node.inputs[3].type].drawEditValue(node.num1, "##num1Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "num2", function ()
            node.num2 = editor.dataTypes[node.inputs[4].type].drawEditValue(node.num2, "##num2Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "num3", function ()
            node.num3 = editor.dataTypes[node.inputs[5].type].drawEditValue(node.num3, "##num3Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "num4", function ()
            node.num4 = editor.dataTypes[node.inputs[6].type].drawEditValue(node.num4, "##num4Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "duration", function ()
            node.duration = editor.dataTypes[node.inputs[7].type].drawEditValue(node.duration, "##durationEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "style", function ()
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

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorTextPrintWith4NumbersNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local key = inputValues[2] or node.key
		local num1 = inputValues[3] or node.num1
		local num2 = inputValues[4] or node.num2
		local num3 = inputValues[5] or node.num3
		local num4 = inputValues[6] or node.num4
		local duration = inputValues[7] or node.duration
		local style = inputValues[8] or node.style
		TextOp.printWith4Numbers(key, math.floor(num1), math.floor(num2), math.floor(num3), math.floor(num4), math.floor(duration), style)
        return {1}
    end
}

return textPrintWith4NumbersNode
