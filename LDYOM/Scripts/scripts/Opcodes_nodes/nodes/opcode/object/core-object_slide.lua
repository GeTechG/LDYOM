local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorObjectSlideNode : LDNodeEditorNode
---@field self_ integer
---@field fromX number
---@field fromY number
---@field fromZ number
---@field xSpeed number
---@field ySpeed number
---@field zSpeed number
---@field collisionCheck boolean

local OBJECT_SLIDE_NODE_TYPE = "core.object_slide"

---@type LDNodeEditorNodeType
local objectSlideNode = {
    typeName = OBJECT_SLIDE_NODE_TYPE,
    category = "opcode.object",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorObjectSlideNode
        local newNode = {
            id = newNodeId,
            nodeType = OBJECT_SLIDE_NODE_TYPE,
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
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.bool",
                }

            },
			self_ = 0,
			fromX = 0,
			fromY = 0,
			fromZ = 0,
			xSpeed = 0,
			ySpeed = 0,
			zSpeed = 0,
			collisionCheck = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorObjectSlideNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[2].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "fromX", function ()
            node.fromX = editor.dataTypes[node.inputs[3].type].drawEditValue(node.fromX, "##fromXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "fromY", function ()
            node.fromY = editor.dataTypes[node.inputs[4].type].drawEditValue(node.fromY, "##fromYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "fromZ", function ()
            node.fromZ = editor.dataTypes[node.inputs[5].type].drawEditValue(node.fromZ, "##fromZEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "xSpeed", function ()
            node.xSpeed = editor.dataTypes[node.inputs[6].type].drawEditValue(node.xSpeed, "##xSpeedEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "ySpeed", function ()
            node.ySpeed = editor.dataTypes[node.inputs[7].type].drawEditValue(node.ySpeed, "##ySpeedEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "zSpeed", function ()
            node.zSpeed = editor.dataTypes[node.inputs[8].type].drawEditValue(node.zSpeed, "##zSpeedEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[9], "collisionCheck", function ()
            node.collisionCheck = editor.dataTypes[node.inputs[9].type].drawEditValue(node.collisionCheck, "##collisionCheckEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorObjectSlideNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local self_ = inputValues[2] or node.self_
		local fromX = inputValues[3] or node.fromX
		local fromY = inputValues[4] or node.fromY
		local fromZ = inputValues[5] or node.fromZ
		local xSpeed = inputValues[6] or node.xSpeed
		local ySpeed = inputValues[7] or node.ySpeed
		local zSpeed = inputValues[8] or node.zSpeed
		local collisionCheck = inputValues[9] or node.collisionCheck
		local result = ObjectOp.slide(self_, fromX, fromY, fromZ, xSpeed, ySpeed, zSpeed, collisionCheck)
        return {1, result}
    end
}

return objectSlideNode
