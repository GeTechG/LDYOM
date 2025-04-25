local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorWorldCreateScriptRoadblockNode : LDNodeEditorNode
---@field leftBottomX number
---@field leftBottomY number
---@field leftBottomZ number
---@field rightTopX number
---@field rightTopY number
---@field rightTopZ number
---@field type integer

local WORLD_CREATE_SCRIPT_ROADBLOCK_NODE_TYPE = "core.world_create_script_roadblock"

---@type LDNodeEditorNodeType
local worldCreateScriptRoadblockNode = {
    typeName = WORLD_CREATE_SCRIPT_ROADBLOCK_NODE_TYPE,
    category = "opcode.world",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorWorldCreateScriptRoadblockNode
        local newNode = {
            id = newNodeId,
            nodeType = WORLD_CREATE_SCRIPT_ROADBLOCK_NODE_TYPE,
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
                    type = "core.flow",
                }

            },
			leftBottomX = 0,
			leftBottomY = 0,
			leftBottomZ = 0,
			rightTopX = 0,
			rightTopY = 0,
			rightTopZ = 0,
			type = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorWorldCreateScriptRoadblockNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "leftBottomX", function ()
            node.leftBottomX = editor.dataTypes[node.inputs[2].type].drawEditValue(node.leftBottomX, "##leftBottomXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "leftBottomY", function ()
            node.leftBottomY = editor.dataTypes[node.inputs[3].type].drawEditValue(node.leftBottomY, "##leftBottomYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "leftBottomZ", function ()
            node.leftBottomZ = editor.dataTypes[node.inputs[4].type].drawEditValue(node.leftBottomZ, "##leftBottomZEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "rightTopX", function ()
            node.rightTopX = editor.dataTypes[node.inputs[5].type].drawEditValue(node.rightTopX, "##rightTopXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "rightTopY", function ()
            node.rightTopY = editor.dataTypes[node.inputs[6].type].drawEditValue(node.rightTopY, "##rightTopYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "rightTopZ", function ()
            node.rightTopZ = editor.dataTypes[node.inputs[7].type].drawEditValue(node.rightTopZ, "##rightTopZEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "type", function ()
            node.type = editor.dataTypes[node.inputs[8].type].drawEditValue(node.type, "##typeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorWorldCreateScriptRoadblockNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local leftBottomX = inputValues[2] or node.leftBottomX
		local leftBottomY = inputValues[3] or node.leftBottomY
		local leftBottomZ = inputValues[4] or node.leftBottomZ
		local rightTopX = inputValues[5] or node.rightTopX
		local rightTopY = inputValues[6] or node.rightTopY
		local rightTopZ = inputValues[7] or node.rightTopZ
		local type = inputValues[8] or node.type
		WorldOp.createScriptRoadblock(leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ, math.floor(type))
        return {1}
    end
}

return worldCreateScriptRoadblockNode
