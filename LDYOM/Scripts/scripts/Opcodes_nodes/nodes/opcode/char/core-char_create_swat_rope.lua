local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorCharCreateSwatRopeNode : LDNodeEditorNode
---@field pedType integer
---@field modelId integer
---@field x number
---@field y number
---@field z number

local CHAR_CREATE_SWAT_ROPE_NODE_TYPE = "core.char_create_swat_rope"

---@type LDNodeEditorNodeType
local charCreateSwatRopeNode = {
    typeName = CHAR_CREATE_SWAT_ROPE_NODE_TYPE,
    category = "opcode.char",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorCharCreateSwatRopeNode
        local newNode = {
            id = newNodeId,
            nodeType = CHAR_CREATE_SWAT_ROPE_NODE_TYPE,
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
			pedType = 0,
			modelId = 0,
			x = 0,
			y = 0,
			z = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorCharCreateSwatRopeNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "pedType", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.PedType) do
                if value == node.pedType then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##pedTypeSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##pedTypeSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##pedTypeSelect", 0) then
                    if ImGui.BeginListBox("##pedTypeSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.PedType) do
                            if ImGui.Selectable(key, value == node.pedType, 0, ImVec2.new()) then
                                node.pedType = value;
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "modelId", function ()
            node.modelId = editor.dataTypes[node.inputs[3].type].drawEditValue(node.modelId, "##modelIdEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "x", function ()
            node.x = editor.dataTypes[node.inputs[4].type].drawEditValue(node.x, "##xEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "y", function ()
            node.y = editor.dataTypes[node.inputs[5].type].drawEditValue(node.y, "##yEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "z", function ()
            node.z = editor.dataTypes[node.inputs[6].type].drawEditValue(node.z, "##zEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "handle");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param node LDNodeEditorCharCreateSwatRopeNode
    ---@param inputValues any[]
    run = function(editor, node, inputValues)
        local pedType = inputValues[2] or node.pedType
		local modelId = inputValues[3] or node.modelId
		local x = inputValues[4] or node.x
		local y = inputValues[5] or node.y
		local z = inputValues[6] or node.z
		local handle = CharOp.createSwatRope(pedType, modelId, x, y, z)
        return {1, handle}
    end
}

return charCreateSwatRopeNode
