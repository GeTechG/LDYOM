local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorPadGetStateNode : LDNodeEditorNode
---@field pad integer
---@field buttonId integer

local PAD_GET_STATE_NODE_TYPE = "core.pad_get_state"

---@type LDNodeEditorNodeType
local padGetStateNode = {
    typeName = PAD_GET_STATE_NODE_TYPE,
    category = "opcode.pad",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorPadGetStateNode
        local newNode = {
            id = newNodeId,
            nodeType = PAD_GET_STATE_NODE_TYPE,
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
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }

            },
			pad = 0,
			buttonId = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorPadGetStateNode
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "buttonId", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.Button) do
                if value == node.buttonId then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##buttonIdSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##buttonIdSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##buttonIdSelect", 0) then
                    if ImGui.BeginListBox("##buttonIdSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.Button) do
                            if ImGui.Selectable(key, value == node.buttonId, 0, ImVec2.new()) then
                                node.buttonId = value;
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

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "state");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorPadGetStateNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local pad = inputValues[2] or node.pad
		local buttonId = inputValues[3] or node.buttonId
		local state = PadOp.getState(pad, buttonId)
        return {1, state}
    end
}

return padGetStateNode
