local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCameraSetShakeSimulationSimpleNode : LDNodeEditorNode
---@field type integer
---@field timeInMs number
---@field intensity number

local CAMERA_SET_SHAKE_SIMULATION_SIMPLE_NODE_TYPE = "core.camera_set_shake_simulation_simple"

---@type LDNodeEditorNodeType
local cameraSetShakeSimulationSimpleNode = {
    typeName = CAMERA_SET_SHAKE_SIMULATION_SIMPLE_NODE_TYPE,
    category = "opcode.camera",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCameraSetShakeSimulationSimpleNode
        local newNode = {
            id = newNodeId,
            nodeType = CAMERA_SET_SHAKE_SIMULATION_SIMPLE_NODE_TYPE,
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
			type = 0,
			timeInMs = 0,
			intensity = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCameraSetShakeSimulationSimpleNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "type", function ()
            node.type = editor.dataTypes[node.inputs[2].type].drawEditValue(node.type, "##typeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "timeInMs", function ()
            node.timeInMs = editor.dataTypes[node.inputs[3].type].drawEditValue(node.timeInMs, "##timeInMsEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "intensity", function ()
            node.intensity = editor.dataTypes[node.inputs[4].type].drawEditValue(node.intensity, "##intensityEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCameraSetShakeSimulationSimpleNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local type = inputValues[2] or node.type
		local timeInMs = inputValues[3] or node.timeInMs
		local intensity = inputValues[4] or node.intensity
		CameraOp.setShakeSimulationSimple(type, timeInMs, intensity)
        return {1}
    end
}

return cameraSetShakeSimulationSimpleNode
