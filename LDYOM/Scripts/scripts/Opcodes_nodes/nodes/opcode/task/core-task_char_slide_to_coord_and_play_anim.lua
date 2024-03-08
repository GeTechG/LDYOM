local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTaskCharSlideToCoordAndPlayAnimNode : LDNodeEditorNode
---@field handle integer
---@field x number
---@field y number
---@field z number
---@field angle number
---@field radius number
---@field animationName string
---@field animationFile string
---@field frameDelta number
---@field loop boolean
---@field lockX boolean
---@field lockY boolean
---@field lockF boolean
---@field time integer

local TASK_CHAR_SLIDE_TO_COORD_AND_PLAY_ANIM_NODE_TYPE = "core.task_char_slide_to_coord_and_play_anim"

---@type LDNodeEditorNodeType
local taskCharSlideToCoordAndPlayAnimNode = {
    typeName = TASK_CHAR_SLIDE_TO_COORD_AND_PLAY_ANIM_NODE_TYPE,
    category = "opcode.task",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTaskCharSlideToCoordAndPlayAnimNode
        local newNode = {
            id = newNodeId,
            nodeType = TASK_CHAR_SLIDE_TO_COORD_AND_PLAY_ANIM_NODE_TYPE,
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
                    type = "core.Char",
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
                    type = "core.string",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.string",
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
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.bool",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.bool",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.bool",
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
			handle = 0,
			x = 0,
			y = 0,
			z = 0,
			angle = 0,
			radius = 0,
			animationName = "",
			animationFile = "",
			frameDelta = 0,
			loop = false,
			lockX = false,
			lockY = false,
			lockF = false,
			time = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTaskCharSlideToCoordAndPlayAnimNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "handle", function ()
            node.handle = editor.dataTypes[node.inputs[2].type].drawEditValue(node.handle, "##handleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "x", function ()
            node.x = editor.dataTypes[node.inputs[3].type].drawEditValue(node.x, "##xEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "y", function ()
            node.y = editor.dataTypes[node.inputs[4].type].drawEditValue(node.y, "##yEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "z", function ()
            node.z = editor.dataTypes[node.inputs[5].type].drawEditValue(node.z, "##zEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "angle", function ()
            node.angle = editor.dataTypes[node.inputs[6].type].drawEditValue(node.angle, "##angleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "radius", function ()
            node.radius = editor.dataTypes[node.inputs[7].type].drawEditValue(node.radius, "##radiusEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "animationName", function ()
            node.animationName = editor.dataTypes[node.inputs[8].type].drawEditValue(node.animationName, "##animationNameEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[9], "animationFile", function ()
            node.animationFile = editor.dataTypes[node.inputs[9].type].drawEditValue(node.animationFile, "##animationFileEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[10], "frameDelta", function ()
            node.frameDelta = editor.dataTypes[node.inputs[10].type].drawEditValue(node.frameDelta, "##frameDeltaEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[11], "loop", function ()
            node.loop = editor.dataTypes[node.inputs[11].type].drawEditValue(node.loop, "##loopEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[12], "lockX", function ()
            node.lockX = editor.dataTypes[node.inputs[12].type].drawEditValue(node.lockX, "##lockXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[13], "lockY", function ()
            node.lockY = editor.dataTypes[node.inputs[13].type].drawEditValue(node.lockY, "##lockYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[14], "lockF", function ()
            node.lockF = editor.dataTypes[node.inputs[14].type].drawEditValue(node.lockF, "##lockFEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[15], "time", function ()
            node.time = editor.dataTypes[node.inputs[15].type].drawEditValue(node.time, "##timeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorTaskCharSlideToCoordAndPlayAnimNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local handle = inputValues[2] or node.handle
		local x = inputValues[3] or node.x
		local y = inputValues[4] or node.y
		local z = inputValues[5] or node.z
		local angle = inputValues[6] or node.angle
		local radius = inputValues[7] or node.radius
		local animationName = inputValues[8] or node.animationName
		local animationFile = inputValues[9] or node.animationFile
		local frameDelta = inputValues[10] or node.frameDelta
		local loop = inputValues[11] or node.loop
		local lockX = inputValues[12] or node.lockX
		local lockY = inputValues[13] or node.lockY
		local lockF = inputValues[14] or node.lockF
		local time = inputValues[15] or node.time
		TaskOp.charSlideToCoordAndPlayAnim(handle, x, y, z, angle, radius, animationName, animationFile, frameDelta, loop, lockX, lockY, lockF, time)
        return {1}
    end
}

return taskCharSlideToCoordAndPlayAnimNode
