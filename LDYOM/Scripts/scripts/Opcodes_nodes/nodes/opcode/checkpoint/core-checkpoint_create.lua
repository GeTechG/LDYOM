local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCheckpointCreateNode : LDNodeEditorNode
---@field type integer
---@field x number
---@field y number
---@field z number
---@field pointX number
---@field pointY number
---@field pointZ number
---@field radius number

local CHECKPOINT_CREATE_NODE_TYPE = "core.checkpoint_create"

---@type LDNodeEditorNodeType
local checkpointCreateNode = {
    typeName = CHECKPOINT_CREATE_NODE_TYPE,
    category = "opcode.checkpoint",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCheckpointCreateNode
        local newNode = {
            id = newNodeId,
            nodeType = CHECKPOINT_CREATE_NODE_TYPE,
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
                    type = "core.Checkpoint",
                }

            },
			type = 0,
			x = 0,
			y = 0,
			z = 0,
			pointX = 0,
			pointY = 0,
			pointZ = 0,
			radius = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCheckpointCreateNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "type", function ()
            node.type = editor.dataTypes[node.inputs[2].type].drawEditValue(node.type, "##typeEdit", fontScale * 100)
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "pointX", function ()
            node.pointX = editor.dataTypes[node.inputs[6].type].drawEditValue(node.pointX, "##pointXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "pointY", function ()
            node.pointY = editor.dataTypes[node.inputs[7].type].drawEditValue(node.pointY, "##pointYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "pointZ", function ()
            node.pointZ = editor.dataTypes[node.inputs[8].type].drawEditValue(node.pointZ, "##pointZEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[9], "radius", function ()
            node.radius = editor.dataTypes[node.inputs[9].type].drawEditValue(node.radius, "##radiusEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "handle");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCheckpointCreateNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local type = inputValues[2] or node.type
		local x = inputValues[3] or node.x
		local y = inputValues[4] or node.y
		local z = inputValues[5] or node.z
		local pointX = inputValues[6] or node.pointX
		local pointY = inputValues[7] or node.pointY
		local pointZ = inputValues[8] or node.pointZ
		local radius = inputValues[9] or node.radius
		local handle = CheckpointOp.create(math.floor(type), x, y, z, pointX, pointY, pointZ, radius)
        return {1, handle}
    end
}

return checkpointCreateNode
