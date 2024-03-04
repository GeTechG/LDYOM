local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorPlayerGiveClothesOutsideShopNode : LDNodeEditorNode
---@field self_ integer
---@field textureName string
---@field modelName string
---@field bodyPart integer

local PLAYER_GIVE_CLOTHES_OUTSIDE_SHOP_NODE_TYPE = "core.player_give_clothes_outside_shop"

---@type LDNodeEditorNodeType
local playerGiveClothesOutsideShopNode = {
    typeName = PLAYER_GIVE_CLOTHES_OUTSIDE_SHOP_NODE_TYPE,
    category = "opcode.player",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorPlayerGiveClothesOutsideShopNode
        local newNode = {
            id = newNodeId,
            nodeType = PLAYER_GIVE_CLOTHES_OUTSIDE_SHOP_NODE_TYPE,
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
                    type = "core.string",
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
                    type = "core.flow",
                }

            },
			self_ = 0,
			textureName = "",
			modelName = "",
			bodyPart = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorPlayerGiveClothesOutsideShopNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[2].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "textureName", function ()
            node.textureName = editor.dataTypes[node.inputs[3].type].drawEditValue(node.textureName, "##textureNameEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "modelName", function ()
            node.modelName = editor.dataTypes[node.inputs[4].type].drawEditValue(node.modelName, "##modelNameEdit", fontScale * 100)
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
    ---@param node LDNodeEditorPlayerGiveClothesOutsideShopNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local self_ = inputValues[2] or node.self_
		local textureName = inputValues[3] or node.textureName
		local modelName = inputValues[4] or node.modelName
		local bodyPart = inputValues[5] or node.bodyPart
		PlayerOp.giveClothesOutsideShop(self_, textureName, modelName, bodyPart)
        return {1}
    end
}

return playerGiveClothesOutsideShopNode
