local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")
local enums = require("libs.enums")

---@class LDNodeEditorWorldIsExplosionInAreaNode : LDNodeEditorNode
---@field explosionType integer
---@field leftBottomX number
---@field leftBottomY number
---@field leftBottomZ number
---@field rightTopX number
---@field rightTopY number
---@field rightTopZ number

local WORLD_IS_EXPLOSION_IN_AREA_NODE_TYPE = "core.world_is_explosion_in_area"

---@type LDNodeEditorNodeType
local worldIsExplosionInAreaNode = {
    typeName = WORLD_IS_EXPLOSION_IN_AREA_NODE_TYPE,
    category = "opcode.world",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
	description = true,
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        ---@type LDNodeEditorWorldIsExplosionInAreaNode
        local newNode = {
            id = newNodeId,
            nodeType = WORLD_IS_EXPLOSION_IN_AREA_NODE_TYPE,
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
                    type = "core.bool",
                }

            },
			explosionType = 0,
			leftBottomX = 0,
			leftBottomY = 0,
			leftBottomZ = 0,
			rightTopX = 0,
			rightTopY = 0,
			rightTopZ = 0
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorWorldIsExplosionInAreaNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");
		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], "explosionType", function ()
            local name = "<Not set>";
            for key, value in pairs(enums.ExplosionType) do
                if value == node.explosionType then
                    name = key;
                    break;
                end
            end
            if ImGui.Button(name, ImVec2.new()) then
                NodeEditor.Suspend();
                ImGui.OpenPopup("##explosionTypeSelect", 0);
                NodeEditor.Resume();
            end

            if ImGui.IsPopupOpen("##explosionTypeSelect", 0) then
                NodeEditor.Suspend();
                if ImGui.BeginPopup("##explosionTypeSelect", 0) then
                    if ImGui.BeginListBox("##explosionTypeSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                        for key, value in pairs(enums.ExplosionType) do
                            if ImGui.Selectable(key, value == node.explosionType, 0, ImVec2.new()) then
                                node.explosionType = value;
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

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[3], "leftBottomX", function ()
            node.leftBottomX = editor.dataTypes[node.inputs[3].type].drawEditValue(node.leftBottomX, "##leftBottomXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[4], "leftBottomY", function ()
            node.leftBottomY = editor.dataTypes[node.inputs[4].type].drawEditValue(node.leftBottomY, "##leftBottomYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[5], "leftBottomZ", function ()
            node.leftBottomZ = editor.dataTypes[node.inputs[5].type].drawEditValue(node.leftBottomZ, "##leftBottomZEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[6], "rightTopX", function ()
            node.rightTopX = editor.dataTypes[node.inputs[6].type].drawEditValue(node.rightTopX, "##rightTopXEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[7], "rightTopY", function ()
            node.rightTopY = editor.dataTypes[node.inputs[7].type].drawEditValue(node.rightTopY, "##rightTopYEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[8], "rightTopZ", function ()
            node.rightTopZ = editor.dataTypes[node.inputs[8].type].drawEditValue(node.rightTopZ, "##rightTopZEdit", fontScale * 100)
        end);

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

		LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[2], "");


        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorWorldIsExplosionInAreaNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local explosionType = inputValues[2] or node.explosionType
		local leftBottomX = inputValues[3] or node.leftBottomX
		local leftBottomY = inputValues[4] or node.leftBottomY
		local leftBottomZ = inputValues[5] or node.leftBottomZ
		local rightTopX = inputValues[6] or node.rightTopX
		local rightTopY = inputValues[7] or node.rightTopY
		local rightTopZ = inputValues[8] or node.rightTopZ
		local result = WorldOp.isExplosionInArea(explosionType, leftBottomX, leftBottomY, leftBottomZ, rightTopX, rightTopY, rightTopZ)
        return {1, result}
    end
}

return worldIsExplosionInAreaNode
