local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCameraPointAtCharNode : LDNodeEditorNode
---@field char integer
---@field mode integer
---@field switchStyle integer

local CAMERA_POINT_AT_CHAR_NODE_TYPE = "core.camera_point_at_char"

---@type LDNodeEditorNodeType
local cameraPointAtCharNode = {
    typeName = CAMERA_POINT_AT_CHAR_NODE_TYPE,
    category = "opcode.camera",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCameraPointAtCharNode
        local newNode = {
            id = newNodeId,
            nodeType = CAMERA_POINT_AT_CHAR_NODE_TYPE,
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
			char = 0,
			mode = 0,
			switchStyle = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCameraPointAtCharNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "char", function ()
            node.char = editor.dataTypes[node.inputs[2].type].drawEditValue(node.char, "##charEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "mode", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.CameraMode) do
                if value == node.mode then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##modeSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##modeSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##modeSelect", 0) then
                    if ImGui.BeginListBox("##modeSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.CameraMode) do
                            if ImGui.Selectable(key, value == node.mode, 0, ImVec2.new()) then
                                node.mode = value;
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "switchStyle", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.SwitchType) do
                if value == node.switchStyle then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##switchStyleSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##switchStyleSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##switchStyleSelect", 0) then
                    if ImGui.BeginListBox("##switchStyleSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.SwitchType) do
                            if ImGui.Selectable(key, value == node.switchStyle, 0, ImVec2.new()) then
                                node.switchStyle = value;
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
    ---@param node LDNodeEditorCameraPointAtCharNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local char = inputValues[2] or node.char
		local mode = inputValues[3] or node.mode
		local switchStyle = inputValues[4] or node.switchStyle
		CameraOp.pointAtChar(char, mode, switchStyle)
        return {1}
    end
}

return cameraPointAtCharNode
