local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorWorldGetCityFromCoordsNode : LDNodeEditorNode
---@field x number
---@field y number
---@field z number

local WORLD_GET_CITY_FROM_COORDS_NODE_TYPE = "core.world_get_city_from_coords"

---@type LDNodeEditorNodeType
local worldGetCityFromCoordsNode = {
    typeName = WORLD_GET_CITY_FROM_COORDS_NODE_TYPE,
    category = "opcode.world",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorWorldGetCityFromCoordsNode
        local newNode = {
            id = newNodeId,
            nodeType = WORLD_GET_CITY_FROM_COORDS_NODE_TYPE,
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
			z = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorWorldGetCityFromCoordsNode
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

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "townId");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorWorldGetCityFromCoordsNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local x = inputValues[2] or node.x
		local y = inputValues[3] or node.y
		local z = inputValues[4] or node.z
		local townId = WorldOp.getCityFromCoords(x, y, z)
        return {1, townId}
    end
}

return worldGetCityFromCoordsNode
