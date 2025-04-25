local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorWorldGetRandomCarInSphereNoSaveRecursiveNode : LDNodeEditorNode
---@field x number
---@field y number
---@field z number
---@field radius number
---@field findNext boolean
---@field skipWrecked boolean

local WORLD_GET_RANDOM_CAR_IN_SPHERE_NO_SAVE_RECURSIVE_NODE_TYPE = "core.world_get_random_car_in_sphere_no_save_recursive"

---@type LDNodeEditorNodeType
local worldGetRandomCarInSphereNoSaveRecursiveNode = {
    typeName = WORLD_GET_RANDOM_CAR_IN_SPHERE_NO_SAVE_RECURSIVE_NODE_TYPE,
    category = "opcode.world",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorWorldGetRandomCarInSphereNoSaveRecursiveNode
        local newNode = {
            id = newNodeId,
            nodeType = WORLD_GET_RANDOM_CAR_IN_SPHERE_NO_SAVE_RECURSIVE_NODE_TYPE,
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
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.Vehicle",
                }

            },
			x = 0,
			y = 0,
			z = 0,
			radius = 0,
			findNext = false,
			skipWrecked = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorWorldGetRandomCarInSphereNoSaveRecursiveNode
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "findNext", function ()
            node.findNext = editor.dataTypes[node.inputs[5].type].drawEditValue(node.findNext, "##findNextEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "skipWrecked", function ()
            node.skipWrecked = editor.dataTypes[node.inputs[6].type].drawEditValue(node.skipWrecked, "##skipWreckedEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "handle");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorWorldGetRandomCarInSphereNoSaveRecursiveNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local x = inputValues[1] or node.x
		local y = inputValues[2] or node.y
		local z = inputValues[3] or node.z
		local radius = inputValues[4] or node.radius
		local findNext = inputValues[5] or node.findNext
		local skipWrecked = inputValues[6] or node.skipWrecked
		local result, handle = WorldOp.getRandomCarInSphereNoSaveRecursive(x, y, z, radius, findNext, skipWrecked)
        return {result, handle}
    end
}

return worldGetRandomCarInSphereNoSaveRecursiveNode
