local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorWorldCreateBirdsNode : LDNodeEditorNode
---@field xFrom number
---@field yFrom number
---@field zFrom number
---@field xTo number
---@field yTo number
---@field zTo number
---@field quantity integer
---@field type integer

local WORLD_CREATE_BIRDS_NODE_TYPE = "core.world_create_birds"

---@type LDNodeEditorNodeType
local worldCreateBirdsNode = {
    typeName = WORLD_CREATE_BIRDS_NODE_TYPE,
    category = "opcode.world",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorWorldCreateBirdsNode
        local newNode = {
            id = newNodeId,
            nodeType = WORLD_CREATE_BIRDS_NODE_TYPE,
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

            },
			xFrom = 0,
			yFrom = 0,
			zFrom = 0,
			xTo = 0,
			yTo = 0,
			zTo = 0,
			quantity = 0,
			type = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorWorldCreateBirdsNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "xFrom", function ()
            node.xFrom = editor.dataTypes[node.inputs[2].type].drawEditValue(node.xFrom, "##xFromEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "yFrom", function ()
            node.yFrom = editor.dataTypes[node.inputs[3].type].drawEditValue(node.yFrom, "##yFromEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "zFrom", function ()
            node.zFrom = editor.dataTypes[node.inputs[4].type].drawEditValue(node.zFrom, "##zFromEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "xTo", function ()
            node.xTo = editor.dataTypes[node.inputs[5].type].drawEditValue(node.xTo, "##xToEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "yTo", function ()
            node.yTo = editor.dataTypes[node.inputs[6].type].drawEditValue(node.yTo, "##yToEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "zTo", function ()
            node.zTo = editor.dataTypes[node.inputs[7].type].drawEditValue(node.zTo, "##zToEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "quantity", function ()
            node.quantity = editor.dataTypes[node.inputs[8].type].drawEditValue(node.quantity, "##quantityEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[9], "type", function ()
            node.type = editor.dataTypes[node.inputs[9].type].drawEditValue(node.type, "##typeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorWorldCreateBirdsNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local xFrom = inputValues[2] or node.xFrom
		local yFrom = inputValues[3] or node.yFrom
		local zFrom = inputValues[4] or node.zFrom
		local xTo = inputValues[5] or node.xTo
		local yTo = inputValues[6] or node.yTo
		local zTo = inputValues[7] or node.zTo
		local quantity = inputValues[8] or node.quantity
		local type = inputValues[9] or node.type
		WorldOp.createBirds(xFrom, yFrom, zFrom, xTo, yTo, zTo, quantity, type)
        return {1}
    end
}

return worldCreateBirdsNode
