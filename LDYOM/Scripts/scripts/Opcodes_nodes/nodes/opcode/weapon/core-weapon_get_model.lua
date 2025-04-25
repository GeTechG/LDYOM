local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorWeaponGetModelNode : LDNodeEditorNode
---@field weaponType integer

local WEAPON_GET_MODEL_NODE_TYPE = "core.weapon_get_model"

---@type LDNodeEditorNodeType
local weaponGetModelNode = {
    typeName = WEAPON_GET_MODEL_NODE_TYPE,
    category = "opcode.weapon",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorWeaponGetModelNode
        local newNode = {
            id = newNodeId,
            nodeType = WEAPON_GET_MODEL_NODE_TYPE,
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
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }

            },
			weaponType = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorWeaponGetModelNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "weaponType", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.WeaponType) do
                if value == node.weaponType then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##weaponTypeSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##weaponTypeSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##weaponTypeSelect", 0) then
                    if ImGui.BeginListBox("##weaponTypeSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.WeaponType) do
                            if ImGui.Selectable(key, value == node.weaponType, 0, ImVec2.new()) then
                                node.weaponType = value;
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

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "modelId");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorWeaponGetModelNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local weaponType = inputValues[2] or node.weaponType
		local modelId = WeaponOp.getModel(weaponType)
        return {1, modelId}
    end
}

return weaponGetModelNode
