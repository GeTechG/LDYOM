local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCarIsStoppedInArea3DNode : LDNodeEditorNode
---@field self_ integer
---@field leftBottomX number
---@field leftBottomY number
---@field leftBottomZ number
---@field rightTopX number
---@field rightTopY number
---@field rightTopZ number
---@field drawSphere boolean

local CAR_IS_STOPPED_IN_AREA3_D_NODE_TYPE = "core.car_is_stopped_in_area3_d"

---@type LDNodeEditorNodeType
local carIsStoppedInArea3DNode = {
    typeName = CAR_IS_STOPPED_IN_AREA3_D_NODE_TYPE,
    category = "opcode.car",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCarIsStoppedInArea3DNode
        local newNode = {
            id = newNodeId,
            nodeType = CAR_IS_STOPPED_IN_AREA3_D_NODE_TYPE,
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
                    type = "core.bool",
                }

            },
			self_ = 0,
			leftBottomX = 0,
			leftBottomY = 0,
			leftBottomZ = 0,
			rightTopX = 0,
			rightTopY = 0,
			rightTopZ = 0,
			drawSphere = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCarIsStoppedInArea3DNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[2].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "leftBottomX", function ()
            node.leftBottomX = editor.dataTypes[node.inputs[3].type].drawEditValue(node.leftBottomX, "##leftBottomXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "leftBottomY", function ()
            node.leftBottomY = editor.dataTypes[node.inputs[4].type].drawEditValue(node.leftBottomY, "##leftBottomYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "leftBottomZ", function ()
            node.leftBottomZ = editor.dataTypes[node.inputs[5].type].drawEditValue(node.leftBottomZ, "##leftBottomZEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "rightTopX", function ()
            node.rightTopX = editor.dataTypes[node.inputs[6].type].drawEditValue(node.rightTopX, "##rightTopXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "rightTopY", function ()
            node.rightTopY = editor.dataTypes[node.inputs[7].type].drawEditValue(node.rightTopY, "##rightTopYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "rightTopZ", function ()
            node.rightTopZ = editor.dataTypes[node.inputs[8].type].drawEditValue(node.rightTopZ, "##rightTopZEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[9], "drawSphere", function ()
            node.drawSphere = editor.dataTypes[node.inputs[9].type].drawEditValue(node.drawSphere, "##drawSphereEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCarIsStoppedInArea3DNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local self_ = inputValues[2] or node.self_
		local leftBottomX = inputValues[3] or node.leftBottomX
		local leftBottomY = inputValues[4] or node.leftBottomY
		local leftBottomZ = inputValues[5] or node.leftBottomZ
		local rightTopX = inputValues[6] or node.rightTopX
		local rightTopY = inputValues[7] or node.rightTopY
		local rightTopZ = inputValues[8] or node.rightTopZ
		local drawSphere = inputValues[9] or node.drawSphere
		local result = CarOp.isStoppedInArea3D(self_, leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ, drawSphere)
        return {1, result}
    end
}

return carIsStoppedInArea3DNode
