local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorHudFlashObjectNode : LDNodeEditorNode
---@field object integer

local HUD_FLASH_OBJECT_NODE_TYPE = "core.hud_flash_object"

---@type LDNodeEditorNodeType
local hudFlashObjectNode = {
    typeName = HUD_FLASH_OBJECT_NODE_TYPE,
    category = "opcode.hud",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorHudFlashObjectNode
        local newNode = {
            id = newNodeId,
            nodeType = HUD_FLASH_OBJECT_NODE_TYPE,
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
			object = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorHudFlashObjectNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "object", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.HudObject) do
                if value == node.object then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##objectSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##objectSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##objectSelect", 0) then
                    if ImGui.BeginListBox("##objectSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.HudObject) do
                            if ImGui.Selectable(key, value == node.object, 0, ImVec2.new()) then
                                node.object = value;
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
    ---@param node LDNodeEditorHudFlashObjectNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local object = inputValues[2] or node.object
		HudOp.flashObject(object)
        return {1}
    end
}

return hudFlashObjectNode
