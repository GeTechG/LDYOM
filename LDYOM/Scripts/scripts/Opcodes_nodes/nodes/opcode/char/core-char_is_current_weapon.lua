local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCharIsCurrentWeaponNode : LDNodeEditorNode
---@field self_ integer
---@field weaponType integer

local CHAR_IS_CURRENT_WEAPON_NODE_TYPE = "core.char_is_current_weapon"

---@type LDNodeEditorNodeType
local charIsCurrentWeaponNode = {
    typeName = CHAR_IS_CURRENT_WEAPON_NODE_TYPE,
    category = "opcode.char",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCharIsCurrentWeaponNode
        local newNode = {
            id = newNodeId,
            nodeType = CHAR_IS_CURRENT_WEAPON_NODE_TYPE,
            inputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.Char",
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
			weaponType = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCharIsCurrentWeaponNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[1].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

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


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCharIsCurrentWeaponNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local self_ = inputValues[1] or node.self_
		local weaponType = inputValues[2] or node.weaponType
		local result = CharOp.isCurrentWeapon(self_, weaponType)
        return {result}
    end
}

return charIsCurrentWeaponNode
