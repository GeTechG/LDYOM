local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorPlaneFollowEntityNode : LDNodeEditorNode
---@field self_ integer
---@field char integer
---@field vehicle integer
---@field radius number

local PLANE_FOLLOW_ENTITY_NODE_TYPE = "core.plane_follow_entity"

---@type LDNodeEditorNodeType
local planeFollowEntityNode = {
    typeName = PLANE_FOLLOW_ENTITY_NODE_TYPE,
    category = "opcode.plane",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorPlaneFollowEntityNode
        local newNode = {
            id = newNodeId,
            nodeType = PLANE_FOLLOW_ENTITY_NODE_TYPE,
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
                    type = "core.Vehicle",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.Char",
                }
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.Vehicle",
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
			self_ = 0,
			char = 0,
			vehicle = 0,
			radius = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorPlaneFollowEntityNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[2].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "char", function ()
            node.char = editor.dataTypes[node.inputs[3].type].drawEditValue(node.char, "##charEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "vehicle", function ()
            node.vehicle = editor.dataTypes[node.inputs[4].type].drawEditValue(node.vehicle, "##vehicleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "radius", function ()
            node.radius = editor.dataTypes[node.inputs[5].type].drawEditValue(node.radius, "##radiusEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorPlaneFollowEntityNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local self_ = inputValues[2] or node.self_
		local char = inputValues[3] or node.char
		local vehicle = inputValues[4] or node.vehicle
		local radius = inputValues[5] or node.radius
		PlaneOp.followEntity(self_, char, vehicle, radius)
        return {1}
    end
}

return planeFollowEntityNode