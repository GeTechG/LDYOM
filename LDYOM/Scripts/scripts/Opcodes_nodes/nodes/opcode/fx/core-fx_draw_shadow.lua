local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorFxDrawShadowNode : LDNodeEditorNode
---@field textureType integer
---@field x number
---@field y number
---@field z number
---@field angle number
---@field length number
---@field intensity integer
---@field r integer
---@field g integer
---@field b integer

local FX_DRAW_SHADOW_NODE_TYPE = "core.fx_draw_shadow"

---@type LDNodeEditorNodeType
local fxDrawShadowNode = {
    typeName = FX_DRAW_SHADOW_NODE_TYPE,
    category = "opcode.fx",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorFxDrawShadowNode
        local newNode = {
            id = newNodeId,
            nodeType = FX_DRAW_SHADOW_NODE_TYPE,
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
			textureType = 0,
			x = 0,
			y = 0,
			z = 0,
			angle = 0,
			length = 0,
			intensity = 0,
			r = 0,
			g = 0,
			b = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorFxDrawShadowNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "textureType", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.ShadowTextureType) do
                if value == node.textureType then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##textureTypeSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##textureTypeSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##textureTypeSelect", 0) then
                    if ImGui.BeginListBox("##textureTypeSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.ShadowTextureType) do
                            if ImGui.Selectable(key, value == node.textureType, 0, ImVec2.new()) then
                                node.textureType = value;
                                ImGui.CloseCurrentPopup();
                            end
                        end
                        ImGui.EndListBox();
                    end
                    ImGui.EndPopup();
                end
                NodeEditor.Resume();
            end
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "x", function ()
            node.x = editor.dataTypes[node.inputs[3].type].drawEditValue(node.x, "##xEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "y", function ()
            node.y = editor.dataTypes[node.inputs[4].type].drawEditValue(node.y, "##yEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "z", function ()
            node.z = editor.dataTypes[node.inputs[5].type].drawEditValue(node.z, "##zEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "angle", function ()
            node.angle = editor.dataTypes[node.inputs[6].type].drawEditValue(node.angle, "##angleEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "length", function ()
            node.length = editor.dataTypes[node.inputs[7].type].drawEditValue(node.length, "##lengthEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "intensity", function ()
            node.intensity = editor.dataTypes[node.inputs[8].type].drawEditValue(node.intensity, "##intensityEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[9], "r", function ()
            node.r = editor.dataTypes[node.inputs[9].type].drawEditValue(node.r, "##rEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[10], "g", function ()
            node.g = editor.dataTypes[node.inputs[10].type].drawEditValue(node.g, "##gEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[11], "b", function ()
            node.b = editor.dataTypes[node.inputs[11].type].drawEditValue(node.b, "##bEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorFxDrawShadowNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local textureType = inputValues[2] or node.textureType
		local x = inputValues[3] or node.x
		local y = inputValues[4] or node.y
		local z = inputValues[5] or node.z
		local angle = inputValues[6] or node.angle
		local length = inputValues[7] or node.length
		local intensity = inputValues[8] or node.intensity
		local r = inputValues[9] or node.r
		local g = inputValues[10] or node.g
		local b = inputValues[11] or node.b
		FxOp.drawShadow(textureType, x, y, z, angle, length, math.floor(intensity), math.floor(r), math.floor(g), math.floor(b))
        return {1}
    end
}

return fxDrawShadowNode
