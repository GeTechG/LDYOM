local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCharLocateOnFootCar3DNode : LDNodeEditorNode
---@field self_ integer
---@field vehicle integer
---@field xRadius number
---@field yRadius number
---@field zRadius number
---@field drawSphere boolean

local CHAR_LOCATE_ON_FOOT_CAR3_D_NODE_TYPE = "core.char_locate_on_foot_car3_d"

---@type LDNodeEditorNodeType
local charLocateOnFootCar3DNode = {
    typeName = CHAR_LOCATE_ON_FOOT_CAR3_D_NODE_TYPE,
    category = "opcode.char",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCharLocateOnFootCar3DNode
        local newNode = {
            id = newNodeId,
            nodeType = CHAR_LOCATE_ON_FOOT_CAR3_D_NODE_TYPE,
            inputs = {
                {
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
                    type = "core.bool",
                }

            },
			self_ = 0,
			vehicle = 0,
			xRadius = 0,
			yRadius = 0,
			zRadius = 0,
			drawSphere = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCharLocateOnFootCar3DNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[1].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "vehicle", function ()
            node.vehicle = editor.dataTypes[node.inputs[2].type].drawEditValue(node.vehicle, "##vehicleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "xRadius", function ()
            node.xRadius = editor.dataTypes[node.inputs[3].type].drawEditValue(node.xRadius, "##xRadiusEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "yRadius", function ()
            node.yRadius = editor.dataTypes[node.inputs[4].type].drawEditValue(node.yRadius, "##yRadiusEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "zRadius", function ()
            node.zRadius = editor.dataTypes[node.inputs[5].type].drawEditValue(node.zRadius, "##zRadiusEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "drawSphere", function ()
            node.drawSphere = editor.dataTypes[node.inputs[6].type].drawEditValue(node.drawSphere, "##drawSphereEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCharLocateOnFootCar3DNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local self_ = inputValues[1] or node.self_
		local vehicle = inputValues[2] or node.vehicle
		local xRadius = inputValues[3] or node.xRadius
		local yRadius = inputValues[4] or node.yRadius
		local zRadius = inputValues[5] or node.zRadius
		local drawSphere = inputValues[6] or node.drawSphere
		local result = CharOp.locateOnFootCar3D(self_, vehicle, xRadius, yRadius, zRadius, drawSphere)
        return {result}
    end
}

return charLocateOnFootCar3DNode