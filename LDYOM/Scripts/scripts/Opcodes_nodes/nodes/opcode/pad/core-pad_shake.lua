local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorPadShakeNode : LDNodeEditorNode
---@field pad integer
---@field time integer
---@field intensity integer

local PAD_SHAKE_NODE_TYPE = "core.pad_shake"

---@type LDNodeEditorNodeType
local padShakeNode = {
    typeName = PAD_SHAKE_NODE_TYPE,
    category = "opcode.pad",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorPadShakeNode
        local newNode = {
            id = newNodeId,
            nodeType = PAD_SHAKE_NODE_TYPE,
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
			time = 0,
			intensity = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorPadShakeNode
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "time", function ()
            node.time = editor.dataTypes[node.inputs[3].type].drawEditValue(node.time, "##timeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "intensity", function ()
            node.intensity = editor.dataTypes[node.inputs[4].type].drawEditValue(node.intensity, "##intensityEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorPadShakeNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local pad = inputValues[2] or node.pad
		local time = inputValues[3] or node.time
		local intensity = inputValues[4] or node.intensity
		PadOp.shake(pad, math.floor(time), math.floor(intensity))
        return {1}
    end
}

return padShakeNode
