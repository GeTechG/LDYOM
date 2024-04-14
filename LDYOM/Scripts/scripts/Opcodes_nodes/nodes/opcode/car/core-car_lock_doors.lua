local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCarLockDoorsNode : LDNodeEditorNode
---@field self_ integer
---@field lockStatus integer

local CAR_LOCK_DOORS_NODE_TYPE = "core.car_lock_doors"

---@type LDNodeEditorNodeType
local carLockDoorsNode = {
    typeName = CAR_LOCK_DOORS_NODE_TYPE,
    category = "opcode.car",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCarLockDoorsNode
        local newNode = {
            id = newNodeId,
            nodeType = CAR_LOCK_DOORS_NODE_TYPE,
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
                    type = "core.Vehicle",
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
			lockStatus = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCarLockDoorsNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[2].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "lockStatus", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.CarLock) do
                if value == node.lockStatus then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##lockStatusSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##lockStatusSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##lockStatusSelect", 0) then
                    if ImGui.BeginListBox("##lockStatusSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.CarLock) do
                            if ImGui.Selectable(key, value == node.lockStatus, 0, ImVec2.new()) then
                                node.lockStatus = value;
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
    ---@param node LDNodeEditorCarLockDoorsNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local self_ = inputValues[2] or node.self_
		if self_ == nil or self_ == 0 then
            error("Input self is not set")
        end
		local lockStatus = inputValues[3] or node.lockStatus
		CarOp.lockDoors(self_, lockStatus)
        return {1}
    end
}

return carLockDoorsNode
