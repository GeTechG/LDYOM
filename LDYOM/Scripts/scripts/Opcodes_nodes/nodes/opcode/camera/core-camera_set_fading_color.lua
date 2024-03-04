local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCameraSetFadingColorNode : LDNodeEditorNode
---@field r integer
---@field g integer
---@field b integer

local CAMERA_SET_FADING_COLOR_NODE_TYPE = "core.camera_set_fading_color"

---@type LDNodeEditorNodeType
local cameraSetFadingColorNode = {
    typeName = CAMERA_SET_FADING_COLOR_NODE_TYPE,
    category = "opcode.camera",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCameraSetFadingColorNode
        local newNode = {
            id = newNodeId,
            nodeType = CAMERA_SET_FADING_COLOR_NODE_TYPE,
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
			r = 0,
			g = 0,
			b = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCameraSetFadingColorNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "r", function ()
            node.r = editor.dataTypes[node.inputs[2].type].drawEditValue(node.r, "##rEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "g", function ()
            node.g = editor.dataTypes[node.inputs[3].type].drawEditValue(node.g, "##gEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "b", function ()
            node.b = editor.dataTypes[node.inputs[4].type].drawEditValue(node.b, "##bEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorCameraSetFadingColorNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local r = inputValues[2] or node.r
		local g = inputValues[3] or node.g
		local b = inputValues[4] or node.b
		CameraOp.setFadingColor(r, g, b)
        return {1}
    end
}

return cameraSetFadingColorNode
