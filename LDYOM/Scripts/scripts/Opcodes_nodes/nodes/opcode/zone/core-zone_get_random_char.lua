local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorZoneGetRandomCharNode : LDNodeEditorNode
---@field zone string
---@field civilian boolean
---@field gang boolean
---@field criminalOrProstitute boolean

local ZONE_GET_RANDOM_CHAR_NODE_TYPE = "core.zone_get_random_char"

---@type LDNodeEditorNodeType
local zoneGetRandomCharNode = {
    typeName = ZONE_GET_RANDOM_CHAR_NODE_TYPE,
    category = "opcode.zone",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorZoneGetRandomCharNode
        local newNode = {
            id = newNodeId,
            nodeType = ZONE_GET_RANDOM_CHAR_NODE_TYPE,
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
				,{
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.Char",
                }

            },
			zone = "",
			civilian = false,
			gang = false,
			criminalOrProstitute = false
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorZoneGetRandomCharNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "zone", function ()
            node.zone = editor.dataTypes[node.inputs[2].type].drawEditValue(node.zone, "##zoneEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "civilian", function ()
            node.civilian = editor.dataTypes[node.inputs[3].type].drawEditValue(node.civilian, "##civilianEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "gang", function ()
            node.gang = editor.dataTypes[node.inputs[4].type].drawEditValue(node.gang, "##gangEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "criminalOrProstitute", function ()
            node.criminalOrProstitute = editor.dataTypes[node.inputs[5].type].drawEditValue(node.criminalOrProstitute, "##criminalOrProstituteEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "handle");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorZoneGetRandomCharNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local zone = inputValues[2] or node.zone
		local civilian = inputValues[3] or node.civilian
		local gang = inputValues[4] or node.gang
		local criminalOrProstitute = inputValues[5] or node.criminalOrProstitute
		local handle = ZoneOp.getRandomChar(zone, civilian, gang, criminalOrProstitute)
        return {1, handle}
    end
}

return zoneGetRandomCharNode
