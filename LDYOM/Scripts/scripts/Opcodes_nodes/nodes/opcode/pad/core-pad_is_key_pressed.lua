local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorPadIsKeyPressedNode : LDNodeEditorNode
---@field keyCode integer

local PAD_IS_KEY_PRESSED_NODE_TYPE = "core.pad_is_key_pressed"

---@type LDNodeEditorNodeType
local padIsKeyPressedNode = {
    typeName = PAD_IS_KEY_PRESSED_NODE_TYPE,
    category = "opcode.pad",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorPadIsKeyPressedNode
        local newNode = {
            id = newNodeId,
            nodeType = PAD_IS_KEY_PRESSED_NODE_TYPE,
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
                    type = "core.bool",
                }

            },
			keyCode = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorPadIsKeyPressedNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "keyCode", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.KeyCode) do
                if value == node.keyCode then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##keyCodeSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##keyCodeSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##keyCodeSelect", 0) then
                    if ImGui.BeginListBox("##keyCodeSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.KeyCode) do
                            if ImGui.Selectable(key, value == node.keyCode, 0, ImVec2.new()) then
                                node.keyCode = value;
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

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorPadIsKeyPressedNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local keyCode = inputValues[2] or node.keyCode
		local result = PadOp.isKeyPressed(keyCode)
        return {1, result}
    end
}

return padIsKeyPressedNode
