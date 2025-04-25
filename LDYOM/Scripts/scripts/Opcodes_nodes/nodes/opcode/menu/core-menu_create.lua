local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorMenuCreateNode : LDNodeEditorNode
---@field header string
---@field topLeftX number
---@field topLeftY number
---@field width number
---@field numColumns integer
---@field interactive boolean
---@field background boolean
---@field alignment integer

local MENU_CREATE_NODE_TYPE = "core.menu_create"

---@type LDNodeEditorNodeType
local menuCreateNode = {
    typeName = MENU_CREATE_NODE_TYPE,
    category = "opcode.menu",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorMenuCreateNode
        local newNode = {
            id = newNodeId,
            nodeType = MENU_CREATE_NODE_TYPE,
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
                    type = "core.string",
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
			header = "",
			topLeftX = 0,
			topLeftY = 0,
			width = 0,
			numColumns = 0,
			interactive = false,
			background = false,
			alignment = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorMenuCreateNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "header", function ()
            node.header = editor.dataTypes[node.inputs[2].type].drawEditValue(node.header, "##headerEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "topLeftX", function ()
            node.topLeftX = editor.dataTypes[node.inputs[3].type].drawEditValue(node.topLeftX, "##topLeftXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "topLeftY", function ()
            node.topLeftY = editor.dataTypes[node.inputs[4].type].drawEditValue(node.topLeftY, "##topLeftYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "width", function ()
            node.width = editor.dataTypes[node.inputs[5].type].drawEditValue(node.width, "##widthEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "numColumns", function ()
            node.numColumns = editor.dataTypes[node.inputs[6].type].drawEditValue(node.numColumns, "##numColumnsEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "interactive", function ()
            node.interactive = editor.dataTypes[node.inputs[7].type].drawEditValue(node.interactive, "##interactiveEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "background", function ()
            node.background = editor.dataTypes[node.inputs[8].type].drawEditValue(node.background, "##backgroundEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[9], "alignment", function ()
            node.alignment = editor.dataTypes[node.inputs[9].type].drawEditValue(node.alignment, "##alignmentEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "handle");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorMenuCreateNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local header = inputValues[2] or node.header
		local topLeftX = inputValues[3] or node.topLeftX
		local topLeftY = inputValues[4] or node.topLeftY
		local width = inputValues[5] or node.width
		local numColumns = inputValues[6] or node.numColumns
		local interactive = inputValues[7] or node.interactive
		local background = inputValues[8] or node.background
		local alignment = inputValues[9] or node.alignment
		local handle = MenuOp.create(header, topLeftX, topLeftY, width, math.floor(numColumns), interactive, background, math.floor(alignment))
        return {1, handle}
    end
}

return menuCreateNode
