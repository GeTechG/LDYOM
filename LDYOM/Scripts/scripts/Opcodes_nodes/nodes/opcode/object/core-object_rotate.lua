local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorObjectRotateNode : LDNodeEditorNode
---@field self_ integer
---@field fromAngle number
---@field toAngle number
---@field collisionCheck boolean

local OBJECT_ROTATE_NODE_TYPE = "core.object_rotate"

---@type LDNodeEditorNodeType
local objectRotateNode = {
    typeName = OBJECT_ROTATE_NODE_TYPE,
    category = "opcode.object",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorObjectRotateNode
        local newNode = {
            id = newNodeId,
            nodeType = OBJECT_ROTATE_NODE_TYPE,
            inputs = {
                {
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
                    type = "core.bool",
                }

            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.bool",
                }

            },
			self_ = 0,
			fromAngle = 0,
			toAngle = 0,
			collisionCheck = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorObjectRotateNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[1].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "fromAngle", function ()
            node.fromAngle = editor.dataTypes[node.inputs[2].type].drawEditValue(node.fromAngle, "##fromAngleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "toAngle", function ()
            node.toAngle = editor.dataTypes[node.inputs[3].type].drawEditValue(node.toAngle, "##toAngleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "collisionCheck", function ()
            node.collisionCheck = editor.dataTypes[node.inputs[4].type].drawEditValue(node.collisionCheck, "##collisionCheckEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorObjectRotateNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local self_ = inputValues[1] or node.self_
		if self_ == nil or self_ == 0 then
            error("Input self is not set")
        end
		local fromAngle = inputValues[2] or node.fromAngle
		local toAngle = inputValues[3] or node.toAngle
		local collisionCheck = inputValues[4] or node.collisionCheck
		local result = ObjectOp.rotate(self_, fromAngle, toAngle, collisionCheck)
        return {result}
    end
}

return objectRotateNode
