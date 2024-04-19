local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorVehicleRegisterNode : LDNodeEditorNode

local VEHICLE_REGISTER_NODE_TYPE = "core.vehicle_register"

---@type LDNodeEditorNodeType
local vehicleRegisterNode = {
    typeName = VEHICLE_REGISTER_NODE_TYPE,
    category = "vehicle",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorVehicleRegisterNode
        local newNode = {
            id = newNodeId,
            nodeType = VEHICLE_REGISTER_NODE_TYPE,
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
            },
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorVehicleRegisterNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "");

        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorVehicleRegisterNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local vehicle = inputValues[2]
        if vehicle == nil then
            error("Vehicle is nil")
        end

        LDNodeEditor.registerCallbackProjectClear(editor, function ()
            CarOp.delete(vehicle)
        end)

        return {1}
    end
}

return vehicleRegisterNode
