local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")

local globalVariableTypes = {
    [GlobalVariableType.Float] = "core.number",
    [GlobalVariableType.Int] = "core.number",
    [GlobalVariableType.String] = "core.string",
    [GlobalVariableType.Bool] = "core.bool",
}

---@class LDNodeEditorGetGlobalVariableNode : LDNodeEditorNode
---@field varUuid string

local GET_GLOBAL_VARIABLE_TYPE = "core.get_global_variable"

---@type LDNodeEditorNodeType
local getVariableNode = {
    typeName = GET_GLOBAL_VARIABLE_TYPE,
    category = "global_variable",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    new = function(ctx, newNodeId, getPinId)
        local varUuid = ""
        if ld.data == nil then
            ld.data = {}
        end
        if ld.data.globalVariables == nil then
            ld.data.globalVariables = {}
        end
        for _, var in ipairs(ld.data.globalVariables) do
            varUuid = var.uuid
            break
        end
        local var = ld.data.globalVariables[varUuid]
        ---@type LDNodeEditorGetGlobalVariableNode
        local newNode = {
            id = newNodeId,
            nodeType = GET_GLOBAL_VARIABLE_TYPE,
            inputs = {
            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = (var ~= nil and globalVariableTypes[var.type]) or "core.number",
                }
            },
            varUuid = varUuid,
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorGetGlobalVariableNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        local var = nil
        for _, currVar in ipairs(ld.data.globalVariables) do
            if currVar.uuid == node.varUuid then
                var = currVar
                break
            end
        end

        if var == nil then
            for _, currVar in ipairs(ld.data.globalVariables) do
                node.varUuid = currVar.uuid
                break
            end
            table.erase_if(ctx.__links, function(link)
                return link.outputId == node.outputs[1].id
            end)
        end

        if var ~= nil and globalVariableTypes[var.type] ~= node.outputs[1].type then
            node.outputs[1].type = globalVariableTypes[var.type]
            table.erase_if(ctx.__links, function(link)
                return link.outputId == node.outputs[1].id
            end)
        end

        local varName = (var ~= nil and var.name) or "None"
        if ImGui.Button(varName, ImVec2.new()) then
            NodeEditor.Suspend();
            ImGui.OpenPopup("##varSelectSelect", 0);
            NodeEditor.Resume();
        end

        if ImGui.IsPopupOpen("##varSelectSelect", 0) then
            NodeEditor.Suspend();
            if ImGui.BeginPopup("##varSelectSelect", 0) then
                if ImGui.BeginListBox("##varSelectSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                    for _, var in ipairs(ld.data.globalVariables) do
                        if ImGui.Selectable(var.name, node.varUuid == var.uuid, 0, ImVec2.new()) then
                            node.varUuid = var.uuid
                            node.outputs[1].type = globalVariableTypes[var.type]
                            table.erase_if(ctx.__links, function(link)
                                return link.outputId == node.outputs[1].id
                            end)
                            ImGui.CloseCurrentPopup();
                        end
                    end
                    ImGui.EndListBox();
                end
                ImGui.EndPopup();
            end
            NodeEditor.Resume();
        end

        if var ~= nil then
            LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], ld.loc.get("general.value"));
        end

        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorGetGlobalVariableNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local value = data.playing.globalVariables[node.varUuid]
        return {value}
    end
}

return getVariableNode