local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCarSetProofsNode : LDNodeEditorNode
---@field self_ integer
---@field bulletProof boolean
---@field fireProof boolean
---@field explosionProof boolean
---@field collisionProof boolean
---@field meleeProof boolean

local CAR_SET_PROOFS_NODE_TYPE = "core.car_set_proofs"

---@type LDNodeEditorNodeType
local carSetProofsNode = {
    typeName = CAR_SET_PROOFS_NODE_TYPE,
    category = "opcode.car",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCarSetProofsNode
        local newNode = {
            id = newNodeId,
            nodeType = CAR_SET_PROOFS_NODE_TYPE,
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
			bulletProof = false,
			fireProof = false,
			explosionProof = false,
			collisionProof = false,
			meleeProof = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCarSetProofsNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[2].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "bulletProof", function ()
            node.bulletProof = editor.dataTypes[node.inputs[3].type].drawEditValue(node.bulletProof, "##bulletProofEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "fireProof", function ()
            node.fireProof = editor.dataTypes[node.inputs[4].type].drawEditValue(node.fireProof, "##fireProofEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "explosionProof", function ()
            node.explosionProof = editor.dataTypes[node.inputs[5].type].drawEditValue(node.explosionProof, "##explosionProofEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "collisionProof", function ()
            node.collisionProof = editor.dataTypes[node.inputs[6].type].drawEditValue(node.collisionProof, "##collisionProofEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "meleeProof", function ()
            node.meleeProof = editor.dataTypes[node.inputs[7].type].drawEditValue(node.meleeProof, "##meleeProofEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorCarSetProofsNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local self_ = inputValues[2] or node.self_
		local bulletProof = inputValues[3] or node.bulletProof
		local fireProof = inputValues[4] or node.fireProof
		local explosionProof = inputValues[5] or node.explosionProof
		local collisionProof = inputValues[6] or node.collisionProof
		local meleeProof = inputValues[7] or node.meleeProof
		CarOp.setProofs(self_, bulletProof, fireProof, explosionProof, collisionProof, meleeProof)
        return {1}
    end
}

return carSetProofsNode
