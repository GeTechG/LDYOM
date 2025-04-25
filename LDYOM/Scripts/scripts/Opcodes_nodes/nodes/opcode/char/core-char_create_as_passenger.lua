local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCharCreateAsPassengerNode : LDNodeEditorNode
---@field vehicle integer
---@field pedType integer
---@field modelId integer
---@field seatId integer

local CHAR_CREATE_AS_PASSENGER_NODE_TYPE = "core.char_create_as_passenger"

---@type LDNodeEditorNodeType
local charCreateAsPassengerNode = {
    typeName = CHAR_CREATE_AS_PASSENGER_NODE_TYPE,
    category = "opcode.char",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCharCreateAsPassengerNode
        local newNode = {
            id = newNodeId,
            nodeType = CHAR_CREATE_AS_PASSENGER_NODE_TYPE,
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
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.Char",
                }

            },
			vehicle = 0,
			pedType = 0,
			modelId = 0,
			seatId = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCharCreateAsPassengerNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "vehicle", function ()
            node.vehicle = editor.dataTypes[node.inputs[2].type].drawEditValue(node.vehicle, "##vehicleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "pedType", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.PedType) do
                if value == node.pedType then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##pedTypeSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##pedTypeSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##pedTypeSelect", 0) then
                    if ImGui.BeginListBox("##pedTypeSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.PedType) do
                            if ImGui.Selectable(key, value == node.pedType, 0, ImVec2.new()) then
                                node.pedType = value;
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "modelId", function ()
            node.modelId = editor.dataTypes[node.inputs[4].type].drawEditValue(node.modelId, "##modelIdEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "seatId", function ()
            node.seatId = editor.dataTypes[node.inputs[5].type].drawEditValue(node.seatId, "##seatIdEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "handle");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCharCreateAsPassengerNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local vehicle = inputValues[2] or node.vehicle
		if vehicle == nil or vehicle == 0 then
            error("Input vehicle is not set")
        end
		local pedType = inputValues[3] or node.pedType
		local modelId = inputValues[4] or node.modelId
		local seatId = inputValues[5] or node.seatId
		local handle = CharOp.createAsPassenger(vehicle, pedType, modelId, math.floor(seatId))
        return {1, handle}
    end
}

return charCreateAsPassengerNode
