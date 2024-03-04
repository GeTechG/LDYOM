local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCameraSetPositionUnfixedNode : LDNodeEditorNode
---@field xOffset number
---@field yOffset number

local CAMERA_SET_POSITION_UNFIXED_NODE_TYPE = "core.camera_set_position_unfixed"

---@type LDNodeEditorNodeType
local cameraSetPositionUnfixedNode = {
    typeName = CAMERA_SET_POSITION_UNFIXED_NODE_TYPE,
    category = "opcode.camera",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCameraSetPositionUnfixedNode
        local newNode = {
            id = newNodeId,
            nodeType = CAMERA_SET_POSITION_UNFIXED_NODE_TYPE,
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
			xOffset = 0,
			yOffset = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCameraSetPositionUnfixedNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "xOffset", function ()
            node.xOffset = editor.dataTypes[node.inputs[2].type].drawEditValue(node.xOffset, "##xOffsetEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "yOffset", function ()
            node.yOffset = editor.dataTypes[node.inputs[3].type].drawEditValue(node.yOffset, "##yOffsetEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorCameraSetPositionUnfixedNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local xOffset = inputValues[2] or node.xOffset
		local yOffset = inputValues[3] or node.yOffset
		CameraOp.setPositionUnfixed(xOffset, yOffset)
        return {1}
    end
}

return cameraSetPositionUnfixedNode
