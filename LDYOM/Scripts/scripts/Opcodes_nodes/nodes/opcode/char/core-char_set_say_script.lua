local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCharSetSayScriptNode : LDNodeEditorNode
---@field self_ integer
---@field _p2 integer
---@field _p3 boolean
---@field _p4 boolean
---@field _p5 boolean

local CHAR_SET_SAY_SCRIPT_NODE_TYPE = "core.char_set_say_script"

---@type LDNodeEditorNodeType
local charSetSayScriptNode = {
    typeName = CHAR_SET_SAY_SCRIPT_NODE_TYPE,
    category = "opcode.char",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCharSetSayScriptNode
        local newNode = {
            id = newNodeId,
            nodeType = CHAR_SET_SAY_SCRIPT_NODE_TYPE,
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

            },
			self_ = 0,
			_p2 = 0,
			_p3 = false,
			_p4 = false,
			_p5 = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCharSetSayScriptNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[2].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "_p2", function ()
            node._p2 = editor.dataTypes[node.inputs[3].type].drawEditValue(node._p2, "##_p2Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "_p3", function ()
            node._p3 = editor.dataTypes[node.inputs[4].type].drawEditValue(node._p3, "##_p3Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "_p4", function ()
            node._p4 = editor.dataTypes[node.inputs[5].type].drawEditValue(node._p4, "##_p4Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "_p5", function ()
            node._p5 = editor.dataTypes[node.inputs[6].type].drawEditValue(node._p5, "##_p5Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCharSetSayScriptNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local self_ = inputValues[2] or node.self_
		if self_ == nil or self_ == 0 then
            error("Input self is not set")
        end
		local _p2 = inputValues[3] or node._p2
		local _p3 = inputValues[4] or node._p3
		local _p4 = inputValues[5] or node._p4
		local _p5 = inputValues[6] or node._p5
		CharOp.setSayScript(self_, _p2, _p3, _p4, _p5)
        return {1}
    end
}

return charSetSayScriptNode
