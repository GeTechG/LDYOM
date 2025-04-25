local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorPlayerGiveClothesNode : LDNodeEditorNode
---@field self_ integer
---@field textureHash integer
---@field modelHash integer
---@field bodyPart integer

local PLAYER_GIVE_CLOTHES_NODE_TYPE = "core.player_give_clothes"

---@type LDNodeEditorNodeType
local playerGiveClothesNode = {
    typeName = PLAYER_GIVE_CLOTHES_NODE_TYPE,
    category = "opcode.player",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorPlayerGiveClothesNode
        local newNode = {
            id = newNodeId,
            nodeType = PLAYER_GIVE_CLOTHES_NODE_TYPE,
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
			self_ = 0,
			textureHash = 0,
			modelHash = 0,
			bodyPart = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorPlayerGiveClothesNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[2].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "textureHash", function ()
            node.textureHash = editor.dataTypes[node.inputs[3].type].drawEditValue(node.textureHash, "##textureHashEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "modelHash", function ()
            node.modelHash = editor.dataTypes[node.inputs[4].type].drawEditValue(node.modelHash, "##modelHashEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "bodyPart", function ()
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
    ---@param node LDNodeEditorPlayerGiveClothesNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local self_ = inputValues[2] or node.self_
		local textureHash = inputValues[3] or node.textureHash
		local modelHash = inputValues[4] or node.modelHash
		local bodyPart = inputValues[5] or node.bodyPart
		PlayerOp.giveClothes(self_, math.floor(textureHash), math.floor(modelHash), bodyPart)
        return {1}
    end
}

return playerGiveClothesNode
