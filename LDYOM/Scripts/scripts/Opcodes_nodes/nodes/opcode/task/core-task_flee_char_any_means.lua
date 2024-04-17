local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorTaskFleeCharAnyMeansNode : LDNodeEditorNode
---@field handle integer
---@field threat integer
---@field runDistance number
---@field time integer
---@field changeCourse boolean
---@field _p6 integer
---@field _p7 integer
---@field radius number

local TASK_FLEE_CHAR_ANY_MEANS_NODE_TYPE = "core.task_flee_char_any_means"

---@type LDNodeEditorNodeType
local taskFleeCharAnyMeansNode = {
    typeName = TASK_FLEE_CHAR_ANY_MEANS_NODE_TYPE,
    category = "opcode.task",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorTaskFleeCharAnyMeansNode
        local newNode = {
            id = newNodeId,
            nodeType = TASK_FLEE_CHAR_ANY_MEANS_NODE_TYPE,
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
			threat = 0,
			runDistance = 0,
			time = 0,
			changeCourse = false,
			_p6 = 0,
			_p7 = 0,
			radius = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorTaskFleeCharAnyMeansNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "handle", function ()
            node.handle = editor.dataTypes[node.inputs[2].type].drawEditValue(node.handle, "##handleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "threat", function ()
            node.threat = editor.dataTypes[node.inputs[3].type].drawEditValue(node.threat, "##threatEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "runDistance", function ()
            node.runDistance = editor.dataTypes[node.inputs[4].type].drawEditValue(node.runDistance, "##runDistanceEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "time", function ()
            node.time = editor.dataTypes[node.inputs[5].type].drawEditValue(node.time, "##timeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "changeCourse", function ()
            node.changeCourse = editor.dataTypes[node.inputs[6].type].drawEditValue(node.changeCourse, "##changeCourseEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "_p6", function ()
            node._p6 = editor.dataTypes[node.inputs[7].type].drawEditValue(node._p6, "##_p6Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "_p7", function ()
            node._p7 = editor.dataTypes[node.inputs[8].type].drawEditValue(node._p7, "##_p7Edit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[9], "radius", function ()
            node.radius = editor.dataTypes[node.inputs[9].type].drawEditValue(node.radius, "##radiusEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorTaskFleeCharAnyMeansNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local handle = inputValues[2] or node.handle
		if handle == nil or handle == 0 then
            error("Input handle is not set")
        end
		local threat = inputValues[3] or node.threat
		if threat == nil or threat == 0 then
            error("Input threat is not set")
        end
		local runDistance = inputValues[4] or node.runDistance
		local time = inputValues[5] or node.time
		local changeCourse = inputValues[6] or node.changeCourse
		local _p6 = inputValues[7] or node._p6
		local _p7 = inputValues[8] or node._p7
		local radius = inputValues[9] or node.radius
		TaskOp.fleeCharAnyMeans(handle, threat, runDistance, math.floor(time), changeCourse, math.floor(_p6), math.floor(_p7), radius)
        return {1}
    end
}

return taskFleeCharAnyMeansNode
