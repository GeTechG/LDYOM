local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorPadSetDrunkInputDelayNode : LDNodeEditorNode
---@field pad integer
---@field delay integer

local PAD_SET_DRUNK_INPUT_DELAY_NODE_TYPE = "core.pad_set_drunk_input_delay"

---@type LDNodeEditorNodeType
local padSetDrunkInputDelayNode = {
    typeName = PAD_SET_DRUNK_INPUT_DELAY_NODE_TYPE,
    category = "opcode.pad",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorPadSetDrunkInputDelayNode
        local newNode = {
            id = newNodeId,
            nodeType = PAD_SET_DRUNK_INPUT_DELAY_NODE_TYPE,
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
			pad = 0,
			delay = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorPadSetDrunkInputDelayNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "pad", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.PadId) do
                if value == node.pad then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##padSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##padSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##padSelect", 0) then
                    if ImGui.BeginListBox("##padSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.PadId) do
                            if ImGui.Selectable(key, value == node.pad, 0, ImVec2.new()) then
                                node.pad = value;
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "delay", function ()
            node.delay = editor.dataTypes[node.inputs[3].type].drawEditValue(node.delay, "##delayEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorPadSetDrunkInputDelayNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local pad = inputValues[2] or node.pad
		local delay = inputValues[3] or node.delay
		PadOp.setDrunkInputDelay(pad, delay)
        return {1}
    end
}

return padSetDrunkInputDelayNode