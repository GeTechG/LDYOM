local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorWorldIsPointObscuredByAMissionEntityNode : LDNodeEditorNode
---@field x number
---@field y number
---@field z number
---@field radiusX number
---@field radiusY number
---@field radiusZ number

local WORLD_IS_POINT_OBSCURED_BY_A_MISSION_ENTITY_NODE_TYPE = "core.world_is_point_obscured_by_a_mission_entity"

---@type LDNodeEditorNodeType
local worldIsPointObscuredByAMissionEntityNode = {
    typeName = WORLD_IS_POINT_OBSCURED_BY_A_MISSION_ENTITY_NODE_TYPE,
    category = "opcode.world",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorWorldIsPointObscuredByAMissionEntityNode
        local newNode = {
            id = newNodeId,
            nodeType = WORLD_IS_POINT_OBSCURED_BY_A_MISSION_ENTITY_NODE_TYPE,
            inputs = {
                {
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
                    type = "core.bool",
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
    ---@param node LDNodeEditorWorldIsPointObscuredByAMissionEntityNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "x", function ()
            node.x = editor.dataTypes[node.inputs[1].type].drawEditValue(node.x, "##xEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "y", function ()
            node.y = editor.dataTypes[node.inputs[2].type].drawEditValue(node.y, "##yEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "z", function ()
            node.z = editor.dataTypes[node.inputs[3].type].drawEditValue(node.z, "##zEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "radiusX", function ()
            node.radiusX = editor.dataTypes[node.inputs[4].type].drawEditValue(node.radiusX, "##radiusXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "radiusY", function ()
            node.radiusY = editor.dataTypes[node.inputs[5].type].drawEditValue(node.radiusY, "##radiusYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "radiusZ", function ()
            node.radiusZ = editor.dataTypes[node.inputs[6].type].drawEditValue(node.radiusZ, "##radiusZEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorWorldIsPointObscuredByAMissionEntityNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local x = inputValues[1] or node.x
		local y = inputValues[2] or node.y
		local z = inputValues[3] or node.z
		local radiusX = inputValues[4] or node.radiusX
		local radiusY = inputValues[5] or node.radiusY
		local radiusZ = inputValues[6] or node.radiusZ
		local result = WorldOp.isPointObscuredByAMissionEntity(x, y, z, radiusX, radiusY, radiusZ)
        return {result}
    end
}

return worldIsPointObscuredByAMissionEntityNode
