local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCameraGetPlayerInCarModeNode : LDNodeEditorNode


local CAMERA_GET_PLAYER_IN_CAR_MODE_NODE_TYPE = "core.camera_get_player_in_car_mode"

---@type LDNodeEditorNodeType
local cameraGetPlayerInCarModeNode = {
    typeName = CAMERA_GET_PLAYER_IN_CAR_MODE_NODE_TYPE,
    category = "opcode.camera",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCameraGetPlayerInCarModeNode
        local newNode = {
            id = newNodeId,
            nodeType = CAMERA_GET_PLAYER_IN_CAR_MODE_NODE_TYPE,
            inputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.flow",
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
                    type = "core.number",
                }

            }
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCameraGetPlayerInCarModeNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "mode");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCameraGetPlayerInCarModeNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local mode = CameraOp.getPlayerInCarMode()
        return {1, mode}
    end
}

return cameraGetPlayerInCarModeNode
