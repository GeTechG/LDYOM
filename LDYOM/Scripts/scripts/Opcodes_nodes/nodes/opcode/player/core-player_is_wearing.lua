local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorPlayerIsWearingNode : LDNodeEditorNode
---@field self_ integer
---@field modelName string
---@field bodyPart integer

local PLAYER_IS_WEARING_NODE_TYPE = "core.player_is_wearing"

---@type LDNodeEditorNodeType
local playerIsWearingNode = {
    typeName = PLAYER_IS_WEARING_NODE_TYPE,
    category = "opcode.player",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorPlayerIsWearingNode
        local newNode = {
            id = newNodeId,
            nodeType = PLAYER_IS_WEARING_NODE_TYPE,
            inputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.number",
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

            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.bool",
                }

            },
			self_ = 0,
			modelName = "",
			bodyPart = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorPlayerIsWearingNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[1].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "modelName", function ()
            node.modelName = editor.dataTypes[node.inputs[2].type].drawEditValue(node.modelName, "##modelNameEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "bodyPart", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.BodyPart) do
                if value == node.bodyPart then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##bodyPartSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##bodyPartSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##bodyPartSelect", 0) then
                    if ImGui.BeginListBox("##bodyPartSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.BodyPart) do
                            if ImGui.Selectable(key, value == node.bodyPart, 0, ImVec2.new()) then
                                node.bodyPart = value;
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
    ---@param node LDNodeEditorPlayerIsWearingNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local self_ = inputValues[1] or node.self_
		local modelName = inputValues[2] or node.modelName
		local bodyPart = inputValues[3] or node.bodyPart
		local result = PlayerOp.isWearing(self_, modelName, bodyPart)
        return {result}
    end
}

return playerIsWearingNode
