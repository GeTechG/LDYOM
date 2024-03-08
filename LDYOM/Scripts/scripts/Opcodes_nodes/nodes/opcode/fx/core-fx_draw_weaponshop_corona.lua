local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorFxDrawWeaponshopCoronaNode : LDNodeEditorNode
---@field x number
---@field y number
---@field z number
---@field size number
---@field coronaType integer
---@field flareType integer
---@field r integer
---@field g integer
---@field b integer

local FX_DRAW_WEAPONSHOP_CORONA_NODE_TYPE = "core.fx_draw_weaponshop_corona"

---@type LDNodeEditorNodeType
local fxDrawWeaponshopCoronaNode = {
    typeName = FX_DRAW_WEAPONSHOP_CORONA_NODE_TYPE,
    category = "opcode.fx",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorFxDrawWeaponshopCoronaNode
        local newNode = {
            id = newNodeId,
            nodeType = FX_DRAW_WEAPONSHOP_CORONA_NODE_TYPE,
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

            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.flow",
                }

            },
			x = 0,
			y = 0,
			z = 0,
			size = 0,
			coronaType = 0,
			flareType = 0,
			r = 0,
			g = 0,
			b = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorFxDrawWeaponshopCoronaNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "x", function ()
            node.x = editor.dataTypes[node.inputs[2].type].drawEditValue(node.x, "##xEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "y", function ()
            node.y = editor.dataTypes[node.inputs[3].type].drawEditValue(node.y, "##yEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "z", function ()
            node.z = editor.dataTypes[node.inputs[4].type].drawEditValue(node.z, "##zEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "size", function ()
            node.size = editor.dataTypes[node.inputs[5].type].drawEditValue(node.size, "##sizeEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "coronaType", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.CoronaType) do
                if value == node.coronaType then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##coronaTypeSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##coronaTypeSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##coronaTypeSelect", 0) then
                    if ImGui.BeginListBox("##coronaTypeSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.CoronaType) do
                            if ImGui.Selectable(key, value == node.coronaType, 0, ImVec2.new()) then
                                node.coronaType = value;
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "flareType", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.FlareType) do
                if value == node.flareType then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##flareTypeSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##flareTypeSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##flareTypeSelect", 0) then
                    if ImGui.BeginListBox("##flareTypeSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.FlareType) do
                            if ImGui.Selectable(key, value == node.flareType, 0, ImVec2.new()) then
                                node.flareType = value;
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "r", function ()
            node.r = editor.dataTypes[node.inputs[8].type].drawEditValue(node.r, "##rEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[9], "g", function ()
            node.g = editor.dataTypes[node.inputs[9].type].drawEditValue(node.g, "##gEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[10], "b", function ()
            node.b = editor.dataTypes[node.inputs[10].type].drawEditValue(node.b, "##bEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorFxDrawWeaponshopCoronaNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local x = inputValues[2] or node.x
		local y = inputValues[3] or node.y
		local z = inputValues[4] or node.z
		local size = inputValues[5] or node.size
		local coronaType = inputValues[6] or node.coronaType
		local flareType = inputValues[7] or node.flareType
		local r = inputValues[8] or node.r
		local g = inputValues[9] or node.g
		local b = inputValues[10] or node.b
		FxOp.drawWeaponshopCorona(x, y, z, size, coronaType, flareType, r, g, b)
        return {1}
    end
}

return fxDrawWeaponshopCoronaNode
