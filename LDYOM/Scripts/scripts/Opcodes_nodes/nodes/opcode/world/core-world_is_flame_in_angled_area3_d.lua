local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorWorldIsFlameInAngledArea3DNode : LDNodeEditorNode
---@field leftBottomX number
---@field leftBottomY number
---@field leftBottomZ number
---@field rightTopX number
---@field rightTopY number
---@field rightTopZ number
---@field angle number
---@field drawSphere boolean

local WORLD_IS_FLAME_IN_ANGLED_AREA3_D_NODE_TYPE = "core.world_is_flame_in_angled_area3_d"

---@type LDNodeEditorNodeType
local worldIsFlameInAngledArea3DNode = {
    typeName = WORLD_IS_FLAME_IN_ANGLED_AREA3_D_NODE_TYPE,
    category = "opcode.world",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorWorldIsFlameInAngledArea3DNode
        local newNode = {
            id = newNodeId,
            nodeType = WORLD_IS_FLAME_IN_ANGLED_AREA3_D_NODE_TYPE,
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
			leftBottomX = 0,
			leftBottomY = 0,
			leftBottomZ = 0,
			rightTopX = 0,
			rightTopY = 0,
			rightTopZ = 0,
			angle = 0,
			drawSphere = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorWorldIsFlameInAngledArea3DNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "leftBottomX", function ()
            node.leftBottomX = editor.dataTypes[node.inputs[1].type].drawEditValue(node.leftBottomX, "##leftBottomXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "leftBottomY", function ()
            node.leftBottomY = editor.dataTypes[node.inputs[2].type].drawEditValue(node.leftBottomY, "##leftBottomYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "leftBottomZ", function ()
            node.leftBottomZ = editor.dataTypes[node.inputs[3].type].drawEditValue(node.leftBottomZ, "##leftBottomZEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "rightTopX", function ()
            node.rightTopX = editor.dataTypes[node.inputs[4].type].drawEditValue(node.rightTopX, "##rightTopXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "rightTopY", function ()
            node.rightTopY = editor.dataTypes[node.inputs[5].type].drawEditValue(node.rightTopY, "##rightTopYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "rightTopZ", function ()
            node.rightTopZ = editor.dataTypes[node.inputs[6].type].drawEditValue(node.rightTopZ, "##rightTopZEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "angle", function ()
            node.angle = editor.dataTypes[node.inputs[7].type].drawEditValue(node.angle, "##angleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "drawSphere", function ()
            node.drawSphere = editor.dataTypes[node.inputs[8].type].drawEditValue(node.drawSphere, "##drawSphereEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorWorldIsFlameInAngledArea3DNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local leftBottomX = inputValues[1] or node.leftBottomX
		local leftBottomY = inputValues[2] or node.leftBottomY
		local leftBottomZ = inputValues[3] or node.leftBottomZ
		local rightTopX = inputValues[4] or node.rightTopX
		local rightTopY = inputValues[5] or node.rightTopY
		local rightTopZ = inputValues[6] or node.rightTopZ
		local angle = inputValues[7] or node.angle
		local drawSphere = inputValues[8] or node.drawSphere
		local result = WorldOp.isFlameInAngledArea3D(leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ, angle, drawSphere)
        return {result}
    end
}

return worldIsFlameInAngledArea3DNode