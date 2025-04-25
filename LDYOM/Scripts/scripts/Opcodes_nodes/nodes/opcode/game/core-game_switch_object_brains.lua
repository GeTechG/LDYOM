local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorGameSwitchObjectBrainsNode : LDNodeEditorNode
---@field type integer
---@field state boolean

local GAME_SWITCH_OBJECT_BRAINS_NODE_TYPE = "core.game_switch_object_brains"

---@type LDNodeEditorNodeType
local gameSwitchObjectBrainsNode = {
    typeName = GAME_SWITCH_OBJECT_BRAINS_NODE_TYPE,
    category = "opcode.game",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorGameSwitchObjectBrainsNode
        local newNode = {
            id = newNodeId,
            nodeType = GAME_SWITCH_OBJECT_BRAINS_NODE_TYPE,
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
                    type = "core.bool",
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
			type = 0,
			state = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorGameSwitchObjectBrainsNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "type", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.ScriptBrainAttachType) do
                if value == node.type then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##typeSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##typeSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##typeSelect", 0) then
                    if ImGui.BeginListBox("##typeSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.ScriptBrainAttachType) do
                            if ImGui.Selectable(key, value == node.type, 0, ImVec2.new()) then
                                node.type = value;
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "state", function ()
            node.state = editor.dataTypes[node.inputs[3].type].drawEditValue(node.state, "##stateEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorGameSwitchObjectBrainsNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local type = inputValues[2] or node.type
		local state = inputValues[3] or node.state
		GameOp.switchObjectBrains(type, state)
        return {1}
    end
}

return gameSwitchObjectBrainsNode
