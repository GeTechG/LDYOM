local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorHeliKeepEntityInViewNode : LDNodeEditorNode
---@field self_ integer
---@field char integer
---@field vehicle integer
---@field minAltitude number
---@field maxAltitude integer

local HELI_KEEP_ENTITY_IN_VIEW_NODE_TYPE = "core.heli_keep_entity_in_view"

---@type LDNodeEditorNodeType
local heliKeepEntityInViewNode = {
    typeName = HELI_KEEP_ENTITY_IN_VIEW_NODE_TYPE,
    category = "opcode.heli",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorHeliKeepEntityInViewNode
        local newNode = {
            id = newNodeId,
            nodeType = HELI_KEEP_ENTITY_IN_VIEW_NODE_TYPE,
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
                    type = "core.Char",
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
			char = 0,
			vehicle = 0,
			minAltitude = 0,
			maxAltitude = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorHeliKeepEntityInViewNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "self", function ()
            node.self_ = editor.dataTypes[node.inputs[2].type].drawEditValue(node.self_, "##self_Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "char", function ()
            node.char = editor.dataTypes[node.inputs[3].type].drawEditValue(node.char, "##charEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "vehicle", function ()
            node.vehicle = editor.dataTypes[node.inputs[4].type].drawEditValue(node.vehicle, "##vehicleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "minAltitude", function ()
            node.minAltitude = editor.dataTypes[node.inputs[5].type].drawEditValue(node.minAltitude, "##minAltitudeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "maxAltitude", function ()
            node.maxAltitude = editor.dataTypes[node.inputs[6].type].drawEditValue(node.maxAltitude, "##maxAltitudeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorHeliKeepEntityInViewNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local self_ = inputValues[2] or node.self_
		local char = inputValues[3] or node.char
		local vehicle = inputValues[4] or node.vehicle
		local minAltitude = inputValues[5] or node.minAltitude
		local maxAltitude = inputValues[6] or node.maxAltitude
		HeliOp.keepEntityInView(self_, char, vehicle, minAltitude, maxAltitude)
        return {1}
    end
}

return heliKeepEntityInViewNode
