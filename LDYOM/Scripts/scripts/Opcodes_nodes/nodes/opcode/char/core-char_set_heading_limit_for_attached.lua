local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCharSetHeadingLimitForAttachedNode : LDNodeEditorNode
---@field self_ integer
---@field orientation integer
---@field headingLimit number

local CHAR_SET_HEADING_LIMIT_FOR_ATTACHED_NODE_TYPE = "core.char_set_heading_limit_for_attached"

---@type LDNodeEditorNodeType
local charSetHeadingLimitForAttachedNode = {
    typeName = CHAR_SET_HEADING_LIMIT_FOR_ATTACHED_NODE_TYPE,
    category = "opcode.char",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCharSetHeadingLimitForAttachedNode
        local newNode = {
            id = newNodeId,
            nodeType = CHAR_SET_HEADING_LIMIT_FOR_ATTACHED_NODE_TYPE,
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
                    type = "core.Char",
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
			self_ = 0,
			orientation = 0,
			headingLimit = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCharSetHeadingLimitForAttachedNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[2].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "orientation", function ()
            node.orientation = editor.dataTypes[node.inputs[3].type].drawEditValue(node.orientation, "##orientationEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "headingLimit", function ()
            node.headingLimit = editor.dataTypes[node.inputs[4].type].drawEditValue(node.headingLimit, "##headingLimitEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCharSetHeadingLimitForAttachedNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local self_ = inputValues[2] or node.self_
		if self_ == nil or self_ == 0 then
            error("Input self is not set")
        end
		local orientation = inputValues[3] or node.orientation
		local headingLimit = inputValues[4] or node.headingLimit
		CharOp.setHeadingLimitForAttached(self_, orientation, headingLimit)
        return {1}
    end
}

return charSetHeadingLimitForAttachedNode
