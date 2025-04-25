local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorWorldIsClosestObjectOfTypeSmashedOrDamagedNode : LDNodeEditorNode
---@field x number
---@field y number
---@field z number
---@field radius number
---@field modelId integer
---@field smashed boolean
---@field damaged boolean

local WORLD_IS_CLOSEST_OBJECT_OF_TYPE_SMASHED_OR_DAMAGED_NODE_TYPE = "core.world_is_closest_object_of_type_smashed_or_damaged"

---@type LDNodeEditorNodeType
local worldIsClosestObjectOfTypeSmashedOrDamagedNode = {
    typeName = WORLD_IS_CLOSEST_OBJECT_OF_TYPE_SMASHED_OR_DAMAGED_NODE_TYPE,
    category = "opcode.world",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorWorldIsClosestObjectOfTypeSmashedOrDamagedNode
        local newNode = {
            id = newNodeId,
            nodeType = WORLD_IS_CLOSEST_OBJECT_OF_TYPE_SMASHED_OR_DAMAGED_NODE_TYPE,
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
                    type = "core.bool",
                }

            },
			x = 0,
			y = 0,
			z = 0,
			radius = 0,
			modelId = 0,
			smashed = false,
			damaged = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorWorldIsClosestObjectOfTypeSmashedOrDamagedNode
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "radius", function ()
            node.radius = editor.dataTypes[node.inputs[4].type].drawEditValue(node.radius, "##radiusEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "modelId", function ()
            node.modelId = editor.dataTypes[node.inputs[5].type].drawEditValue(node.modelId, "##modelIdEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "smashed", function ()
            node.smashed = editor.dataTypes[node.inputs[6].type].drawEditValue(node.smashed, "##smashedEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "damaged", function ()
            node.damaged = editor.dataTypes[node.inputs[7].type].drawEditValue(node.damaged, "##damagedEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorWorldIsClosestObjectOfTypeSmashedOrDamagedNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local x = inputValues[1] or node.x
		local y = inputValues[2] or node.y
		local z = inputValues[3] or node.z
		local radius = inputValues[4] or node.radius
		local modelId = inputValues[5] or node.modelId
		local smashed = inputValues[6] or node.smashed
		local damaged = inputValues[7] or node.damaged
		local result = WorldOp.isClosestObjectOfTypeSmashedOrDamaged(x, y, z, radius, modelId, smashed, damaged)
        return {result}
    end
}

return worldIsClosestObjectOfTypeSmashedOrDamagedNode
