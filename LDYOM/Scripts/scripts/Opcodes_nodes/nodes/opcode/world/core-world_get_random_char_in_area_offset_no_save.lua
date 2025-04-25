local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorWorldGetRandomCharInAreaOffsetNoSaveNode : LDNodeEditorNode
---@field x number
---@field y number
---@field z number
---@field radiusX number
---@field radiusY number
---@field radiusZ number

local WORLD_GET_RANDOM_CHAR_IN_AREA_OFFSET_NO_SAVE_NODE_TYPE = "core.world_get_random_char_in_area_offset_no_save"

---@type LDNodeEditorNodeType
local worldGetRandomCharInAreaOffsetNoSaveNode = {
    typeName = WORLD_GET_RANDOM_CHAR_IN_AREA_OFFSET_NO_SAVE_NODE_TYPE,
    category = "opcode.world",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorWorldGetRandomCharInAreaOffsetNoSaveNode
        local newNode = {
            id = newNodeId,
            nodeType = WORLD_GET_RANDOM_CHAR_IN_AREA_OFFSET_NO_SAVE_NODE_TYPE,
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
                    type = "core.Char",
                }

            },
			x = 0,
			y = 0,
			z = 0,
			radiusX = 0,
			radiusY = 0,
			radiusZ = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorWorldGetRandomCharInAreaOffsetNoSaveNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "x", function ()
            node.x = editor.dataTypes[node.inputs[2].type].drawEditValue(node.x, "##xEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "y", function ()
            node.y = editor.dataTypes[node.inputs[3].type].drawEditValue(node.y, "##yEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "z", function ()
            node.z = editor.dataTypes[node.inputs[4].type].drawEditValue(node.z, "##zEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "radiusX", function ()
            node.radiusX = editor.dataTypes[node.inputs[5].type].drawEditValue(node.radiusX, "##radiusXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "radiusY", function ()
            node.radiusY = editor.dataTypes[node.inputs[6].type].drawEditValue(node.radiusY, "##radiusYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "radiusZ", function ()
            node.radiusZ = editor.dataTypes[node.inputs[7].type].drawEditValue(node.radiusZ, "##radiusZEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "handle");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorWorldGetRandomCharInAreaOffsetNoSaveNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local x = inputValues[2] or node.x
		local y = inputValues[3] or node.y
		local z = inputValues[4] or node.z
		local radiusX = inputValues[5] or node.radiusX
		local radiusY = inputValues[6] or node.radiusY
		local radiusZ = inputValues[7] or node.radiusZ
		local handle = WorldOp.getRandomCharInAreaOffsetNoSave(x, y, z, radiusX, radiusY, radiusZ)
        return {1, handle}
    end
}

return worldGetRandomCharInAreaOffsetNoSaveNode
