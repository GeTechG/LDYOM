local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorWorldGetNearestTagPositionNode : LDNodeEditorNode
---@field xCoord number
---@field yCoord number
---@field zCoord number

local WORLD_GET_NEAREST_TAG_POSITION_NODE_TYPE = "core.world_get_nearest_tag_position"

---@type LDNodeEditorNodeType
local worldGetNearestTagPositionNode = {
    typeName = WORLD_GET_NEAREST_TAG_POSITION_NODE_TYPE,
    category = "opcode.world",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorWorldGetNearestTagPositionNode
        local newNode = {
            id = newNodeId,
            nodeType = WORLD_GET_NEAREST_TAG_POSITION_NODE_TYPE,
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
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.number",
                }

            },
			xCoord = 0,
			yCoord = 0,
			zCoord = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorWorldGetNearestTagPositionNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "xCoord", function ()
            node.xCoord = editor.dataTypes[node.inputs[2].type].drawEditValue(node.xCoord, "##xCoordEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "yCoord", function ()
            node.yCoord = editor.dataTypes[node.inputs[3].type].drawEditValue(node.yCoord, "##yCoordEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "zCoord", function ()
            node.zCoord = editor.dataTypes[node.inputs[4].type].drawEditValue(node.zCoord, "##zCoordEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "x");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[3], "y");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[4], "z");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorWorldGetNearestTagPositionNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local xCoord = inputValues[2] or node.xCoord
		local yCoord = inputValues[3] or node.yCoord
		local zCoord = inputValues[4] or node.zCoord
		local x, y, z = WorldOp.getNearestTagPosition(xCoord, yCoord, zCoord)
        return {1, x, y, z}
    end
}

return worldGetNearestTagPositionNode
