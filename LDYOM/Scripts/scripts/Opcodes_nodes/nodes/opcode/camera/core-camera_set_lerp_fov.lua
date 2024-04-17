local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCameraSetLerpFovNode : LDNodeEditorNode
---@field from number
---@field to number
---@field time integer
---@field ease boolean

local CAMERA_SET_LERP_FOV_NODE_TYPE = "core.camera_set_lerp_fov"

---@type LDNodeEditorNodeType
local cameraSetLerpFovNode = {
    typeName = CAMERA_SET_LERP_FOV_NODE_TYPE,
    category = "opcode.camera",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCameraSetLerpFovNode
        local newNode = {
            id = newNodeId,
            nodeType = CAMERA_SET_LERP_FOV_NODE_TYPE,
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
			from = 0,
			to = 0,
			time = 0,
			ease = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCameraSetLerpFovNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "from", function ()
            node.from = editor.dataTypes[node.inputs[2].type].drawEditValue(node.from, "##fromEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "to", function ()
            node.to = editor.dataTypes[node.inputs[3].type].drawEditValue(node.to, "##toEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "time", function ()
            node.time = editor.dataTypes[node.inputs[4].type].drawEditValue(node.time, "##timeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "ease", function ()
            node.ease = editor.dataTypes[node.inputs[5].type].drawEditValue(node.ease, "##easeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCameraSetLerpFovNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local from = inputValues[2] or node.from
		local to = inputValues[3] or node.to
		local time = inputValues[4] or node.time
		local ease = inputValues[5] or node.ease
		CameraOp.setLerpFov(from, to, math.floor(time), ease)
        return {1}
    end
}

return cameraSetLerpFovNode
