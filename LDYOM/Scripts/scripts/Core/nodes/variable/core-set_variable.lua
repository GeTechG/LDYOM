local LDNodeEditorContext = require("ld.context")
local LDNodeEditor = require("ld.node_editor")
local nodesColors = require("ld.node_colors")
local nodesIcons = require("ld.node_icons")
local fa = require("libs.fa")

---@class LDNodeEditorSetVariableNode : LDNodeEditorNode
---@field varUuid string
---@field varValue any

local SET_VARIABLE_TYPE = "core.set_variable"

---@type LDNodeEditorNodeType
local setVariableNode = {
    typeName = SET_VARIABLE_TYPE,
    category = "variable",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
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
        ---@type LDNodeEditorSetVariableNode
        local newNode = {
            id = newNodeId,
            nodeType = SET_VARIABLE_TYPE,
            inputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = "core.flow",
                },
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Input,
                    type = (var ~= nil and var.type) or "core.number",
                }
            },
            outputs = {
                {
                    id = getPinId(),
                    node = newNodeId,
                    kind = NodeEditorPinKind.Output,
                    type = "core.flow",
                },
            },
            varUuid = varUuid,
            varValue = (function ()
                if var ~= nil then
                    return editor.dataTypes[var.type].makeNew()
                end
                return 0
            end)(),
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorSetVariableNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");

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
                            node.varValue = editor.dataTypes[var.type].makeNew()
                            node.inputs[2].type = var.type
                            table.erase_if(ctx.__links, function(link)
                                return link.inputId == node.inputs[2].id;
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

        if node.varUuid ~= "" then
            LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[2], ld.loc.get("general.value"), function ()
                local type = editor.dataTypes[node.inputs[2].type];
                node.varValue = type.drawEditValue(node.varValue, "##varValue", fontScale * 100);
            end);
        end

        LDNodeEditor.defaultOutput(editor, ctx, builder, node.outputs[1], "");

        builder:End();
    end,
    ---@param editor LDNodeEditor
    ---@param context LDNodeEditorContext
    ---@param node LDNodeEditorSetVariableNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local var = editor.variables[node.varUuid]
        if var then
            var.value = inputValues[2] or node.varValue
        end
        return {1}
    end
}

return setVariableNode