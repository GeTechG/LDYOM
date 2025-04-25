local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorStreamingCustomPlateDesignForNextCarNode : LDNodeEditorNode
---@field modelId integer
---@field townId integer

local STREAMING_CUSTOM_PLATE_DESIGN_FOR_NEXT_CAR_NODE_TYPE = "core.streaming_custom_plate_design_for_next_car"

---@type LDNodeEditorNodeType
local streamingCustomPlateDesignForNextCarNode = {
    typeName = STREAMING_CUSTOM_PLATE_DESIGN_FOR_NEXT_CAR_NODE_TYPE,
    category = "opcode.streaming",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorStreamingCustomPlateDesignForNextCarNode
        local newNode = {
            id = newNodeId,
            nodeType = STREAMING_CUSTOM_PLATE_DESIGN_FOR_NEXT_CAR_NODE_TYPE,
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
			modelId = 0,
			townId = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorStreamingCustomPlateDesignForNextCarNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "modelId", function ()
            node.modelId = editor.dataTypes[node.inputs[2].type].drawEditValue(node.modelId, "##modelIdEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "townId", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.Town) do
                if value == node.townId then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##townIdSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##townIdSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##townIdSelect", 0) then
                    if ImGui.BeginListBox("##townIdSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.Town) do
                            if ImGui.Selectable(key, value == node.townId, 0, ImVec2.new()) then
                                node.townId = value;
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
    ---@param node LDNodeEditorStreamingCustomPlateDesignForNextCarNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local modelId = inputValues[2] or node.modelId
		local townId = inputValues[3] or node.townId
		StreamingOp.customPlateDesignForNextCar(modelId, townId)
        return {1}
    end
}

return streamingCustomPlateDesignForNextCarNode
