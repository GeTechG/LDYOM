local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCarSetModelComponentsNode : LDNodeEditorNode
---@field _unused integer
---@field component1 integer
---@field component2 integer

local CAR_SET_MODEL_COMPONENTS_NODE_TYPE = "core.car_set_model_components"

---@type LDNodeEditorNodeType
local carSetModelComponentsNode = {
    typeName = CAR_SET_MODEL_COMPONENTS_NODE_TYPE,
    category = "opcode.car",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCarSetModelComponentsNode
        local newNode = {
            id = newNodeId,
            nodeType = CAR_SET_MODEL_COMPONENTS_NODE_TYPE,
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

            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.flow",
                }

            },
			_unused = 0,
			component1 = 0,
			component2 = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCarSetModelComponentsNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "_unused", function ()
            node._unused = editor.dataTypes[node.inputs[2].type].drawEditValue(node._unused, "##_unusedEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "component1", function ()
            node.component1 = editor.dataTypes[node.inputs[3].type].drawEditValue(node.component1, "##component1Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "component2", function ()
            node.component2 = editor.dataTypes[node.inputs[4].type].drawEditValue(node.component2, "##component2Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorCarSetModelComponentsNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local _unused = inputValues[2] or node._unused
		local component1 = inputValues[3] or node.component1
		local component2 = inputValues[4] or node.component2
		CarOp.setModelComponents(_unused, math.floor(component1), math.floor(component2))
        return {1}
    end
}

return carSetModelComponentsNode
