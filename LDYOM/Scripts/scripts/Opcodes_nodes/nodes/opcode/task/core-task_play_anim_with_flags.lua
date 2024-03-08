local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTaskPlayAnimWithFlagsNode : LDNodeEditorNode
---@field handle integer
---@field animationName string
---@field animationFile string
---@field frameDelta number
---@field loop boolean
---@field lockX boolean
---@field lockY boolean
---@field lockF boolean
---@field time integer
---@field disableForce boolean
---@field disableLockZ boolean

local TASK_PLAY_ANIM_WITH_FLAGS_NODE_TYPE = "core.task_play_anim_with_flags"

---@type LDNodeEditorNodeType
local taskPlayAnimWithFlagsNode = {
    typeName = TASK_PLAY_ANIM_WITH_FLAGS_NODE_TYPE,
    category = "opcode.task",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTaskPlayAnimWithFlagsNode
        local newNode = {
            id = newNodeId,
            nodeType = TASK_PLAY_ANIM_WITH_FLAGS_NODE_TYPE,
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
			animationName = "",
			animationFile = "",
			frameDelta = 0,
			loop = false,
			lockX = false,
			lockY = false,
			lockF = false,
			time = 0,
			disableForce = false,
			disableLockZ = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTaskPlayAnimWithFlagsNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "handle", function ()
            node.handle = editor.dataTypes[node.inputs[2].type].drawEditValue(node.handle, "##handleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "animationName", function ()
            node.animationName = editor.dataTypes[node.inputs[3].type].drawEditValue(node.animationName, "##animationNameEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "animationFile", function ()
            node.animationFile = editor.dataTypes[node.inputs[4].type].drawEditValue(node.animationFile, "##animationFileEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "frameDelta", function ()
            node.frameDelta = editor.dataTypes[node.inputs[5].type].drawEditValue(node.frameDelta, "##frameDeltaEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "loop", function ()
            node.loop = editor.dataTypes[node.inputs[6].type].drawEditValue(node.loop, "##loopEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "lockX", function ()
            node.lockX = editor.dataTypes[node.inputs[7].type].drawEditValue(node.lockX, "##lockXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "lockY", function ()
            node.lockY = editor.dataTypes[node.inputs[8].type].drawEditValue(node.lockY, "##lockYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[9], "lockF", function ()
            node.lockF = editor.dataTypes[node.inputs[9].type].drawEditValue(node.lockF, "##lockFEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[10], "time", function ()
            node.time = editor.dataTypes[node.inputs[10].type].drawEditValue(node.time, "##timeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[11], "disableForce", function ()
            node.disableForce = editor.dataTypes[node.inputs[11].type].drawEditValue(node.disableForce, "##disableForceEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[12], "disableLockZ", function ()
            node.disableLockZ = editor.dataTypes[node.inputs[12].type].drawEditValue(node.disableLockZ, "##disableLockZEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorTaskPlayAnimWithFlagsNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local handle = inputValues[2] or node.handle
		local animationName = inputValues[3] or node.animationName
		local animationFile = inputValues[4] or node.animationFile
		local frameDelta = inputValues[5] or node.frameDelta
		local loop = inputValues[6] or node.loop
		local lockX = inputValues[7] or node.lockX
		local lockY = inputValues[8] or node.lockY
		local lockF = inputValues[9] or node.lockF
		local time = inputValues[10] or node.time
		local disableForce = inputValues[11] or node.disableForce
		local disableLockZ = inputValues[12] or node.disableLockZ
		TaskOp.playAnimWithFlags(handle, animationName, animationFile, frameDelta, loop, lockX, lockY, lockF, time, disableForce, disableLockZ)
        return {1}
    end
}

return taskPlayAnimWithFlagsNode
