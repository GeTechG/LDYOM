local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCharGetWeaponInSlotNode : LDNodeEditorNode
---@field self_ integer
---@field weaponSlotId integer

local CHAR_GET_WEAPON_IN_SLOT_NODE_TYPE = "core.char_get_weapon_in_slot"

---@type LDNodeEditorNodeType
local charGetWeaponInSlotNode = {
    typeName = CHAR_GET_WEAPON_IN_SLOT_NODE_TYPE,
    category = "opcode.char",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCharGetWeaponInSlotNode
        local newNode = {
            id = newNodeId,
            nodeType = CHAR_GET_WEAPON_IN_SLOT_NODE_TYPE,
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
                    type = "core.flow",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }

            },
			self_ = 0,
			weaponSlotId = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCharGetWeaponInSlotNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[2].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "weaponSlotId", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.WeaponSlot) do
                if value == node.weaponSlotId then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##weaponSlotIdSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##weaponSlotIdSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##weaponSlotIdSelect", 0) then
                    if ImGui.BeginListBox("##weaponSlotIdSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.WeaponSlot) do
                            if ImGui.Selectable(key, value == node.weaponSlotId, 0, ImVec2.new()) then
                                node.weaponSlotId = value;
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

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "weaponType");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[3], "weaponAmmo");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[4], "weaponModel");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCharGetWeaponInSlotNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local self_ = inputValues[2] or node.self_
		local weaponSlotId = inputValues[3] or node.weaponSlotId
		local weaponType, weaponAmmo, weaponModel = CharOp.getWeaponInSlot(self_, weaponSlotId)
        return {1, weaponType, weaponAmmo, weaponModel}
    end
}

return charGetWeaponInSlotNode
