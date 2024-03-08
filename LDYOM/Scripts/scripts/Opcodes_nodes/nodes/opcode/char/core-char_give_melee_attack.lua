local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCharGiveMeleeAttackNode : LDNodeEditorNode
---@field self_ integer
---@field fightStyle integer
---@field moveId integer

local CHAR_GIVE_MELEE_ATTACK_NODE_TYPE = "core.char_give_melee_attack"

---@type LDNodeEditorNodeType
local charGiveMeleeAttackNode = {
    typeName = CHAR_GIVE_MELEE_ATTACK_NODE_TYPE,
    category = "opcode.char",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCharGiveMeleeAttackNode
        local newNode = {
            id = newNodeId,
            nodeType = CHAR_GIVE_MELEE_ATTACK_NODE_TYPE,
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
			fightStyle = 0,
			moveId = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCharGiveMeleeAttackNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[2].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "fightStyle", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.FightStyle) do
                if value == node.fightStyle then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##fightStyleSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##fightStyleSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##fightStyleSelect", 0) then
                    if ImGui.BeginListBox("##fightStyleSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.FightStyle) do
                            if ImGui.Selectable(key, value == node.fightStyle, 0, ImVec2.new()) then
                                node.fightStyle = value;
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "moveId", function ()
            node.moveId = editor.dataTypes[node.inputs[4].type].drawEditValue(node.moveId, "##moveIdEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCharGiveMeleeAttackNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local self_ = inputValues[2] or node.self_
		local fightStyle = inputValues[3] or node.fightStyle
		local moveId = inputValues[4] or node.moveId
		CharOp.giveMeleeAttack(self_, fightStyle, moveId)
        return {1}
    end
}

return charGiveMeleeAttackNode
