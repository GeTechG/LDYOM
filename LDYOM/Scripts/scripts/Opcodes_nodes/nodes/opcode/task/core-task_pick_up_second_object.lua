local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTaskPickUpSecondObjectNode : LDNodeEditorNode
---@field char integer
---@field object integer
---@field xOffset number
---@field yOffset number
---@field zOffset number
---@field boneId integer
---@field _p7 integer
---@field animationName string
---@field animationFile string
---@field time integer

local TASK_PICK_UP_SECOND_OBJECT_NODE_TYPE = "core.task_pick_up_second_object"

---@type LDNodeEditorNodeType
local taskPickUpSecondObjectNode = {
    typeName = TASK_PICK_UP_SECOND_OBJECT_NODE_TYPE,
    category = "opcode.task",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTaskPickUpSecondObjectNode
        local newNode = {
            id = newNodeId,
            nodeType = TASK_PICK_UP_SECOND_OBJECT_NODE_TYPE,
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
                    type = "core.Object",
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

            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.flow",
                }

            },
			char = 0,
			object = 0,
			xOffset = 0,
			yOffset = 0,
			zOffset = 0,
			boneId = 0,
			_p7 = 0,
			animationName = "",
			animationFile = "",
			time = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTaskPickUpSecondObjectNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "char", function ()
            node.char = editor.dataTypes[node.inputs[2].type].drawEditValue(node.char, "##charEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "object", function ()
            node.object = editor.dataTypes[node.inputs[3].type].drawEditValue(node.object, "##objectEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "xOffset", function ()
            node.xOffset = editor.dataTypes[node.inputs[4].type].drawEditValue(node.xOffset, "##xOffsetEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "yOffset", function ()
            node.yOffset = editor.dataTypes[node.inputs[5].type].drawEditValue(node.yOffset, "##yOffsetEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "zOffset", function ()
            node.zOffset = editor.dataTypes[node.inputs[6].type].drawEditValue(node.zOffset, "##zOffsetEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "boneId", function ()
            node.boneId = editor.dataTypes[node.inputs[7].type].drawEditValue(node.boneId, "##boneIdEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "_p7", function ()
            node._p7 = editor.dataTypes[node.inputs[8].type].drawEditValue(node._p7, "##_p7Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[9], "animationName", function ()
            node.animationName = editor.dataTypes[node.inputs[9].type].drawEditValue(node.animationName, "##animationNameEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[10], "animationFile", function ()
            node.animationFile = editor.dataTypes[node.inputs[10].type].drawEditValue(node.animationFile, "##animationFileEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[11], "time", function ()
            node.time = editor.dataTypes[node.inputs[11].type].drawEditValue(node.time, "##timeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorTaskPickUpSecondObjectNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local char = inputValues[2] or node.char
		if char == nil or char == 0 then
            error("Input char is not set")
        end
		local object = inputValues[3] or node.object
		if object == nil or object == 0 then
            error("Input object is not set")
        end
		local xOffset = inputValues[4] or node.xOffset
		local yOffset = inputValues[5] or node.yOffset
		local zOffset = inputValues[6] or node.zOffset
		local boneId = inputValues[7] or node.boneId
		local _p7 = inputValues[8] or node._p7
		local animationName = inputValues[9] or node.animationName
		local animationFile = inputValues[10] or node.animationFile
		local time = inputValues[11] or node.time
		TaskOp.pickUpSecondObject(char, object, xOffset, yOffset, zOffset, boneId, _p7, animationName, animationFile, time)
        return {1}
    end
}

return taskPickUpSecondObjectNode
