local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorWorldGetWaterHeightAtCoordsNode : LDNodeEditorNode
---@field x number
---@field y number
---@field ignoreWaves boolean

local WORLD_GET_WATER_HEIGHT_AT_COORDS_NODE_TYPE = "core.world_get_water_height_at_coords"

---@type LDNodeEditorNodeType
local worldGetWaterHeightAtCoordsNode = {
    typeName = WORLD_GET_WATER_HEIGHT_AT_COORDS_NODE_TYPE,
    category = "opcode.world",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorWorldGetWaterHeightAtCoordsNode
        local newNode = {
            id = newNodeId,
            nodeType = WORLD_GET_WATER_HEIGHT_AT_COORDS_NODE_TYPE,
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
                    type = "core.number",
                }

            },
			x = 0,
			y = 0,
			ignoreWaves = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorWorldGetWaterHeightAtCoordsNode
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "ignoreWaves", function ()
            node.ignoreWaves = editor.dataTypes[node.inputs[4].type].drawEditValue(node.ignoreWaves, "##ignoreWavesEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "height");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorWorldGetWaterHeightAtCoordsNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local x = inputValues[2] or node.x
		local y = inputValues[3] or node.y
		local ignoreWaves = inputValues[4] or node.ignoreWaves
		local height = WorldOp.getWaterHeightAtCoords(x, y, ignoreWaves)
        return {1, height}
    end
}

return worldGetWaterHeightAtCoordsNode
