local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorGameSetNamedEntryExitFlagNode : LDNodeEditorNode
---@field name string
---@field entryexitsFlag integer
---@field state boolean

local GAME_SET_NAMED_ENTRY_EXIT_FLAG_NODE_TYPE = "core.game_set_named_entry_exit_flag"

---@type LDNodeEditorNodeType
local gameSetNamedEntryExitFlagNode = {
    typeName = GAME_SET_NAMED_ENTRY_EXIT_FLAG_NODE_TYPE,
    category = "opcode.game",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorGameSetNamedEntryExitFlagNode
        local newNode = {
            id = newNodeId,
            nodeType = GAME_SET_NAMED_ENTRY_EXIT_FLAG_NODE_TYPE,
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
                    type = "core.string",
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
			name = "",
			entryexitsFlag = 0,
			state = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorGameSetNamedEntryExitFlagNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "name", function ()
            node.name = editor.dataTypes[node.inputs[2].type].drawEditValue(node.name, "##nameEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "entryexitsFlag", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.EntryexitsFlag) do
                if value == node.entryexitsFlag then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##entryexitsFlagSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##entryexitsFlagSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##entryexitsFlagSelect", 0) then
                    if ImGui.BeginListBox("##entryexitsFlagSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.EntryexitsFlag) do
                            if ImGui.Selectable(key, value == node.entryexitsFlag, 0, ImVec2.new()) then
                                node.entryexitsFlag = value;
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "state", function ()
            node.state = editor.dataTypes[node.inputs[4].type].drawEditValue(node.state, "##stateEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorGameSetNamedEntryExitFlagNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local name = inputValues[2] or node.name
		local entryexitsFlag = inputValues[3] or node.entryexitsFlag
		local state = inputValues[4] or node.state
		GameOp.setNamedEntryExitFlag(name, entryexitsFlag, state)
        return {1}
    end
}

return gameSetNamedEntryExitFlagNode
