local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")

---@class LDNodeEditorGetVariableNode : LDNodeEditorNode
---@field varUuid string

local GET_VARIABLE_TYPE = "core.get_variable"

---@type LDNodeEditorNodeType
local getVariableNode = {
    typeName = GET_VARIABLE_TYPE,
    category = "variable",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = false,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    ---@param editor LDNodeEditor
    new = function(ctx, newNodeId, getPinId, editor)
        local varUuid = ""
        for uuid, var in pairs(editor.variables) do
            varUuid = uuid
            break
        end
        local var = editor.variables[varUuid]
        ---@type LDNodeEditorGetVariableNode
        local newNode = {
            id = newNodeId,
            nodeType = GET_VARIABLE_TYPE,
            inputs = {
            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = (var ~= nil and var.type) or "core.number",
                }
            },
            varUuid = varUuid,
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorGetVariableNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        if node.varUuid == "" then
            for uuid, var in pairs(editor.variables) do
                node.varUuid = uuid
                break
            end
        end

        local varName = (editor.variables[node.varUuid] ~= nil and editor.variables[node.varUuid].name) or "None"
        if ImGui.Button(varName, ImVec2.new()) then
            NodeEditor.Suspend();
            ImGui.OpenPopup("##varSelectSelect", 0);
            NodeEditor.Resume();
        end

        if ImGui.IsPopupOpen("##varSelectSelect", 0) then
            NodeEditor.Suspend();
            if ImGui.BeginPopup("##varSelectSelect", 0) then
                if ImGui.BeginListBox("##varSelectSelectChild", ImVec2.new(200 * fontScale, 100 * fontScale)) then
                    for uuid, var in pairs(editor.variables) do
                        if ImGui.Selectable(var.name, node.varUuid == uuid, 0, ImVec2.new()) then
                            node.varUuid = uuid;
                            node.outputs[1].type = var.type;
                            table.erase_if(ctx.__links, function(link)
                                return link.outputId == node.outputs[1].id
                            end);
                            ImGui.CloseCurrentPopup();
                        end
                    end
                    ImGui.EndListBox();
                end
                ImGui.EndPopup();
            end
            NodeEditor.Resume();
        end

        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], ld.loc.get("general.value"));

        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorGetVariableNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local var = editor.variables[node.varUuid]
        if var == nil then
            error("Variable not found")
        end
        return {var.value}
    end
}

return getVariableNode