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

---@class LDNodeEditorSetGlobalVariableNode : LDNodeEditorNode
---@field varUuid string
---@field varValue any

local SET_GLOBAL_VARIABLE_TYPE = "core.set_global_variable"

---@type LDNodeEditorNodeType
local setVariableNode = {
    typeName = SET_GLOBAL_VARIABLE_TYPE,
    category = "global_variable",
    icon = nodesIcons["function"],
    color = nodesColors["function"],
    isCallable = true,
    ---@param ctx LDNodeEditorContext
    ---@param newNodeId integer
    ---@param getPinId fun():integer
    ---@param editor LDNodeEditor
    new = function(ctx, newNodeId, getPinId, editor)
        local varUuid = ""
        if ld.data == nil then
            ld.data = {}
        end
        if ld.data.globalVariables == nil then
            ld.data.globalVariables = {}
        end
        local var = nil
        for _, currVar in ipairs(ld.data.globalVariables) do
            var = currVar
            break
        end
        ---@type LDNodeEditorSetGlobalVariableNode
        local newNode = {
            id = newNodeId,
            nodeType = SET_GLOBAL_VARIABLE_TYPE,
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
                    type = (var ~= nil and globalVariableTypes[var.type]) or "core.number",
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
                    return editor.dataTypes[globalVariableTypes[var.type]].makeNew()
                end
                return 0
            end)(),
        }
        return newNode
    end,
    ---@param editor LDNodeEditor
    ---@param ctx LDNodeEditorContext
    ---@param node LDNodeEditorSetGlobalVariableNode
    ---@param builder BlueprintNodeBuilder
    draw = function(editor, ctx, node, builder)
        local fontScale = ImGui.GetFontSize() / 16;

        builder:Begin(NodeEditor.NodeId(node.id));
        LDNodeEditor.defaultHeader(editor, builder, node);

        LDNodeEditor.defaultInput(editor, ctx, builder, node.inputs[1], "");

        ---@type GlobalVariable | nil
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
                return link.inputId == node.inputs[2].id
            end)
        end

        if var ~= nil and globalVariableTypes[var.type] ~= node.inputs[2].type then
            node.inputs[2].type = globalVariableTypes[var.type]
            node.varValue = editor.dataTypes[globalVariableTypes[var.type]].makeNew()
            table.erase_if(ctx.__links, function(link)
                return link.inputId == node.inputs[2].id
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
                            node.inputs[2].type = globalVariableTypes[var.type]
                            node.varValue = editor.dataTypes[globalVariableTypes[var.type]].makeNew()
                            table.erase_if(ctx.__links, function(link)
                                return link.inputId == node.inputs[2].id
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

        if var then
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
    ---@param node LDNodeEditorSetGlobalVariableNode
    ---@param inputValues any[]
    run = function(editor, context, node, inputValues)
        local newValue = inputValues[2] or node.varValue
        ---@type GlobalVariable | nil
        local var = nil
        for _, currVar in ipairs(ld.data.globalVariables) do
            if currVar.uuid == node.varUuid then
                var = currVar
                break
            end
        end
        if var == nil then
            error("Variable not found")
        end
        if var.type == GlobalVariableType.Float then
            data.playing.globalVariables[node.varUuid] = newValue;
        elseif var.type == GlobalVariableType.Int then
            data.playing.globalVariables[node.varUuid] = math.floor(newValue);
        elseif var.type == GlobalVariableType.String then
            data.playing.globalVariables[node.varUuid] = tostring(newValue);
        elseif var.type == GlobalVariableType.Bool then
            data.playing.globalVariables[node.varUuid] = newValue;
        end
        return {1}
    end
}

return setVariableNode