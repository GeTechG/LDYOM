local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCameraSetCinemaNode : LDNodeEditorNode
---@field state boolean

local CAMERA_SET_CINEMA_NODE_TYPE = "core.camera_set_cinema"

---@type LDNodeEditorNodeType
local cameraSetCinemaNode = {
    typeName = CAMERA_SET_CINEMA_NODE_TYPE,
    category = "opcode.camera",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCameraSetCinemaNode
        local newNode = {
            id = newNodeId,
            nodeType = CAMERA_SET_CINEMA_NODE_TYPE,
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
			state = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCameraSetCinemaNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "state", function ()
            node.state = editor.dataTypes[node.inputs[2].type].drawEditValue(node.state, "##stateEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorCameraSetCinemaNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local state = inputValues[2] or node.state
		CameraOp.setCinema(state)
        return {1}
    end
}

return cameraSetCinemaNode
