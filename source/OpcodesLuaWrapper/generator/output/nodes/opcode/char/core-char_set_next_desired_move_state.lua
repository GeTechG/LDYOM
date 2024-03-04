local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCharSetNextDesiredMoveStateNode : LDNodeEditorNode
---@field moveState integer

local CHAR_SET_NEXT_DESIRED_MOVE_STATE_NODE_TYPE = "core.char_set_next_desired_move_state"

---@type LDNodeEditorNodeType
local charSetNextDesiredMoveStateNode = {
    typeName = CHAR_SET_NEXT_DESIRED_MOVE_STATE_NODE_TYPE,
    category = "opcode.char",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCharSetNextDesiredMoveStateNode
        local newNode = {
            id = newNodeId,
            nodeType = CHAR_SET_NEXT_DESIRED_MOVE_STATE_NODE_TYPE,
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

            },
			moveState = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCharSetNextDesiredMoveStateNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "moveState", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.MoveState) do
                if value == node.moveState then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##moveStateSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##moveStateSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##moveStateSelect", 0) then
                    if ImGui.BeginListBox("##moveStateSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.MoveState) do
                            if ImGui.Selectable(key, value == node.moveState, 0, ImVec2.new()) then
                                node.moveState = value;
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
    ---@param node LDNodeEditorCharSetNextDesiredMoveStateNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local moveState = inputValues[2] or node.moveState
		CharOp.setNextDesiredMoveState(moveState)
        return {1}
    end
}

return charSetNextDesiredMoveStateNode
