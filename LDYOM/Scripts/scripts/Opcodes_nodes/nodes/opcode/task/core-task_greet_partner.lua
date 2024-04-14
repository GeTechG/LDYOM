local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTaskGreetPartnerNode : LDNodeEditorNode
---@field handle integer
---@field partner integer
---@field _p3 number
---@field _p4 integer

local TASK_GREET_PARTNER_NODE_TYPE = "core.task_greet_partner"

---@type LDNodeEditorNodeType
local taskGreetPartnerNode = {
    typeName = TASK_GREET_PARTNER_NODE_TYPE,
    category = "opcode.task",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTaskGreetPartnerNode
        local newNode = {
            id = newNodeId,
            nodeType = TASK_GREET_PARTNER_NODE_TYPE,
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
			handle = 0,
			partner = 0,
			_p3 = 0,
			_p4 = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTaskGreetPartnerNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "handle", function ()
            node.handle = editor.dataTypes[node.inputs[2].type].drawEditValue(node.handle, "##handleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "partner", function ()
            node.partner = editor.dataTypes[node.inputs[3].type].drawEditValue(node.partner, "##partnerEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "_p3", function ()
            node._p3 = editor.dataTypes[node.inputs[4].type].drawEditValue(node._p3, "##_p3Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "_p4", function ()
            node._p4 = editor.dataTypes[node.inputs[5].type].drawEditValue(node._p4, "##_p4Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorTaskGreetPartnerNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local handle = inputValues[2] or node.handle
		if handle == nil or handle == 0 then
            error("Input handle is not set")
        end
		local partner = inputValues[3] or node.partner
		if partner == nil or partner == 0 then
            error("Input partner is not set")
        end
		local _p3 = inputValues[4] or node._p3
		local _p4 = inputValues[5] or node._p4
		TaskOp.greetPartner(handle, partner, _p3, _p4)
        return {1}
    end
}

return taskGreetPartnerNode
