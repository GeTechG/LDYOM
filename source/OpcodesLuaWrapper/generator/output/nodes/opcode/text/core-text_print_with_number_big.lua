local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTextPrintWithNumberBigNode : LDNodeEditorNode
---@field key string
---@field num integer
---@field duration integer
---@field style integer

local TEXT_PRINT_WITH_NUMBER_BIG_NODE_TYPE = "core.text_print_with_number_big"

---@type LDNodeEditorNodeType
local textPrintWithNumberBigNode = {
    typeName = TEXT_PRINT_WITH_NUMBER_BIG_NODE_TYPE,
    category = "opcode.text",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTextPrintWithNumberBigNode
        local newNode = {
            id = newNodeId,
            nodeType = TEXT_PRINT_WITH_NUMBER_BIG_NODE_TYPE,
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
			key = "",
			num = 0,
			duration = 0,
			style = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTextPrintWithNumberBigNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "key", function ()
            node.key = editor.dataTypes[node.inputs[2].type].drawEditValue(node.key, "##keyEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "num", function ()
            node.num = editor.dataTypes[node.inputs[3].type].drawEditValue(node.num, "##numEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "duration", function ()
            node.duration = editor.dataTypes[node.inputs[4].type].drawEditValue(node.duration, "##durationEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "style", function ()
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
    ---@param node LDNodeEditorTextPrintWithNumberBigNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local key = inputValues[2] or node.key
		local num = inputValues[3] or node.num
		local duration = inputValues[4] or node.duration
		local style = inputValues[5] or node.style
		TextOp.printWithNumberBig(key, num, duration, style)
        return {1}
    end
}

return textPrintWithNumberBigNode