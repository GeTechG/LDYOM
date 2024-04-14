local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCarPopDoorNode : LDNodeEditorNode
---@field self_ integer
---@field door integer
---@field visibility boolean

local CAR_POP_DOOR_NODE_TYPE = "core.car_pop_door"

---@type LDNodeEditorNodeType
local carPopDoorNode = {
    typeName = CAR_POP_DOOR_NODE_TYPE,
    category = "opcode.car",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCarPopDoorNode
        local newNode = {
            id = newNodeId,
            nodeType = CAR_POP_DOOR_NODE_TYPE,
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
			self_ = 0,
			door = 0,
			visibility = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCarPopDoorNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[2].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "door", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.CarDoor) do
                if value == node.door then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##doorSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##doorSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##doorSelect", 0) then
                    if ImGui.BeginListBox("##doorSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.CarDoor) do
                            if ImGui.Selectable(key, value == node.door, 0, ImVec2.new()) then
                                node.door = value;
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "visibility", function ()
            node.visibility = editor.dataTypes[node.inputs[4].type].drawEditValue(node.visibility, "##visibilityEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCarPopDoorNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local self_ = inputValues[2] or node.self_
		if self_ == nil or self_ == 0 then
            error("Input self is not set")
        end
		local door = inputValues[3] or node.door
		local visibility = inputValues[4] or node.visibility
		CarOp.popDoor(self_, door, visibility)
        return {1}
    end
}

return carPopDoorNode
