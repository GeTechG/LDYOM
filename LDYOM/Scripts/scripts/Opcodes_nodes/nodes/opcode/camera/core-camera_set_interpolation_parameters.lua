local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCameraSetInterpolationParametersNode : LDNodeEditorNode
---@field _p1 number
---@field time integer

local CAMERA_SET_INTERPOLATION_PARAMETERS_NODE_TYPE = "core.camera_set_interpolation_parameters"

---@type LDNodeEditorNodeType
local cameraSetInterpolationParametersNode = {
    typeName = CAMERA_SET_INTERPOLATION_PARAMETERS_NODE_TYPE,
    category = "opcode.camera",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCameraSetInterpolationParametersNode
        local newNode = {
            id = newNodeId,
            nodeType = CAMERA_SET_INTERPOLATION_PARAMETERS_NODE_TYPE,
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
			_p1 = 0,
			time = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCameraSetInterpolationParametersNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "_p1", function ()
            node._p1 = editor.dataTypes[node.inputs[2].type].drawEditValue(node._p1, "##_p1Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "time", function ()
            node.time = editor.dataTypes[node.inputs[3].type].drawEditValue(node.time, "##timeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCameraSetInterpolationParametersNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local _p1 = inputValues[2] or node._p1
		local time = inputValues[3] or node.time
		CameraOp.setInterpolationParameters(_p1, math.floor(time))
        return {1}
    end
}

return cameraSetInterpolationParametersNode
