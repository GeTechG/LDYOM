local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCarExplodeInCutsceneShakeAndBitsNode : LDNodeEditorNode
---@field self_ integer
---@field shake boolean
---@field effect boolean
---@field sound boolean

local CAR_EXPLODE_IN_CUTSCENE_SHAKE_AND_BITS_NODE_TYPE = "core.car_explode_in_cutscene_shake_and_bits"

---@type LDNodeEditorNodeType
local carExplodeInCutsceneShakeAndBitsNode = {
    typeName = CAR_EXPLODE_IN_CUTSCENE_SHAKE_AND_BITS_NODE_TYPE,
    category = "opcode.car",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCarExplodeInCutsceneShakeAndBitsNode
        local newNode = {
            id = newNodeId,
            nodeType = CAR_EXPLODE_IN_CUTSCENE_SHAKE_AND_BITS_NODE_TYPE,
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
			shake = false,
			effect = false,
			sound = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCarExplodeInCutsceneShakeAndBitsNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[2].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "shake", function ()
            node.shake = editor.dataTypes[node.inputs[3].type].drawEditValue(node.shake, "##shakeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "effect", function ()
            node.effect = editor.dataTypes[node.inputs[4].type].drawEditValue(node.effect, "##effectEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "sound", function ()
            node.sound = editor.dataTypes[node.inputs[5].type].drawEditValue(node.sound, "##soundEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCarExplodeInCutsceneShakeAndBitsNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local self_ = inputValues[2] or node.self_
		local shake = inputValues[3] or node.shake
		local effect = inputValues[4] or node.effect
		local sound = inputValues[5] or node.sound
		CarOp.explodeInCutsceneShakeAndBits(self_, shake, effect, sound)
        return {1}
    end
}

return carExplodeInCutsceneShakeAndBitsNode
