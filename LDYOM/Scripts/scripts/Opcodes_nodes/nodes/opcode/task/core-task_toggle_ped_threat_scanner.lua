local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTaskTogglePedThreatScannerNode : LDNodeEditorNode
---@field handle integer
---@field _p2 boolean
---@field _p3 boolean
---@field _p4 boolean

local TASK_TOGGLE_PED_THREAT_SCANNER_NODE_TYPE = "core.task_toggle_ped_threat_scanner"

---@type LDNodeEditorNodeType
local taskTogglePedThreatScannerNode = {
    typeName = TASK_TOGGLE_PED_THREAT_SCANNER_NODE_TYPE,
    category = "opcode.task",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTaskTogglePedThreatScannerNode
        local newNode = {
            id = newNodeId,
            nodeType = TASK_TOGGLE_PED_THREAT_SCANNER_NODE_TYPE,
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
			handle = 0,
			_p2 = false,
			_p3 = false,
			_p4 = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTaskTogglePedThreatScannerNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "handle", function ()
            node.handle = editor.dataTypes[node.inputs[2].type].drawEditValue(node.handle, "##handleEdit", fontScale * 100)
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

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorTaskTogglePedThreatScannerNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local handle = inputValues[2] or node.handle
		local _p2 = inputValues[3] or node._p2
		local _p3 = inputValues[4] or node._p3
		local _p4 = inputValues[5] or node._p4
		TaskOp.togglePedThreatScanner(handle, _p2, _p3, _p4)
        return {1}
    end
}

return taskTogglePedThreatScannerNode
