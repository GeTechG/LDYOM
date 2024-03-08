local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorObjectPlayAnimNode : LDNodeEditorNode
---@field self_ integer
---@field animationName string
---@field animationFile string
---@field frameDelta number
---@field lockF boolean
---@field loop boolean

local OBJECT_PLAY_ANIM_NODE_TYPE = "core.object_play_anim"

---@type LDNodeEditorNodeType
local objectPlayAnimNode = {
    typeName = OBJECT_PLAY_ANIM_NODE_TYPE,
    category = "opcode.object",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorObjectPlayAnimNode
        local newNode = {
            id = newNodeId,
            nodeType = OBJECT_PLAY_ANIM_NODE_TYPE,
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
                    type = "core.Object",
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

            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.flow",
                }

            },
			self_ = 0,
			animationName = "",
			animationFile = "",
			frameDelta = 0,
			lockF = false,
			loop = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorObjectPlayAnimNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[2].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "lockF", function ()
            node.lockF = editor.dataTypes[node.inputs[6].type].drawEditValue(node.lockF, "##lockFEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "loop", function ()
            node.loop = editor.dataTypes[node.inputs[7].type].drawEditValue(node.loop, "##loopEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorObjectPlayAnimNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local self_ = inputValues[2] or node.self_
		local animationName = inputValues[3] or node.animationName
		local animationFile = inputValues[4] or node.animationFile
		local frameDelta = inputValues[5] or node.frameDelta
		local lockF = inputValues[6] or node.lockF
		local loop = inputValues[7] or node.loop
		ObjectOp.playAnim(self_, animationName, animationFile, frameDelta, lockF, loop)
        return {1}
    end
}

return objectPlayAnimNode
