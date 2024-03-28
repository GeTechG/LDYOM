local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")

---@class LDNodeEditorStopCarrecPathVehicleNode : LDNodeEditorNode


local STOP_CARREC_PATH_VEHICLE_NODE_TYPE = "core.stop_carrec_path_vehicle"

---@type LDNodeEditorNodeType
local stopCarrecPathVehicleNode = {
    typeName = STOP_CARREC_PATH_VEHICLE_NODE_TYPE,
    category = "vehicle",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorStopCarrecPathVehicleNode
        local newNode = {
            id = newNodeId,
            nodeType = STOP_CARREC_PATH_VEHICLE_NODE_TYPE,
            buffer = "",
            inputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.flow",
                },
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.Vehicle",
                }
            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.flow",
                }
            }
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorStopCarrecPathVehicleNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], ld.loc.get("entities.vehicle"));
        
        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorStopCarrecPathVehicleNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local vehicleRef = inputValues[2]
        if vehicleRef == nil then
            error("Vehicle is not set!")
        end

        ld.carrecPathService.stopPlaybackRecordedCar(GetVehicle(vehicleRef));

        return {1}
    end
}

return stopCarrecPathVehicleNode
