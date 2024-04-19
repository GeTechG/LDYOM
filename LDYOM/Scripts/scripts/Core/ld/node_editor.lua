local LDNodeEditorContext = require("ld.context")
require("libs.table_utils")

local config = NodeEditorConfig.new();
config.SettingsFile = ""

---@class NodeTypeInfo
---@field name string
---@field category string

---@class LDNodeEditor
---@field contexts LDNodeEditorContext[]
---@field currentIndexContext number
---@field dataTypes { [string]: LDNodeEditorDataType }
---@field __nodesFactories { [string]: LDNodeEditorNodeType }
---@field variables { [string]: LDNodeEditorVariable }
---@field nodeTypesInfo { [string]: NodeTypeInfo }
---@field callbacksProjectClear fun()[]

local LDNodeEditor = {}

LDNodeEditor.new = function()
    ---@type LDNodeEditor
    local newEditor = {
        contexts = {
            LDNodeEditorContext.new(NodeEditor.CreateEditor(config), "Workspace 1")
        },
        currentIndexContext = 1,
        dataTypes = {
            ["core.flow"] = {
                isMakeable = false,
                iconType = NodeEditorIconType.Flow,
                colorGetter = function()
                    return ImGui.GetStyleColorVec4(ImGuiCol.Text)
                end,
                makeNew = function()
                    return nil
                end,
                drawEditValue = function(value, name)
                    return ImGui.TextUnformatted("<NONE>")
                end,
                equals = function(a, b)
                    return a == b
                end
            },
            ["core.number"] = {
                isMakeable = true,
                iconType = NodeEditorIconType.Circle,
                colorGetter = function()
                    return ImVec4.new(0.35686274509, 0.77254901961, 0.49803921568, 1)
                end,
                makeNew = function()
                    return 0
                end,
                drawEditValue = function(value, name, availContent)
                    newValue = value
                    local titleWidth = ImVec2.new();
                    ImGui.CalcTextSize(titleWidth, name, nil, true, -1);
                    local width = math.max(availContent - titleWidth.x - ImGui.GetStyle().ItemSpacing.x * 2, 0)
                    ImGui.SetNextItemWidth(width);
                    _, newValue = ImGui.InputFloat(name, newValue, 0, 0, "%.3f", 0)
                    return newValue
                end,
                equals = function(a, b)
                    return a == b
                end
            },
            ["core.bool"] = {
                isMakeable = true,
                iconType = NodeEditorIconType.Circle,
                colorGetter = function()
                    return ImVec4.new(0.90980392156, 0, 0, 1)
                end,
                drawEditValue = function(value, name, availContent)
                    newValue = value
                    local titleWidth = ImVec2.new();
                    ImGui.CalcTextSize(titleWidth, name, nil, true, -1);
                    local width = math.max(availContent - titleWidth.x - ImGui.GetStyle().ItemSpacing.x * 2, 0)
                    ImGui.SetNextItemWidth(width);
                    _, newValue = ImGui.Checkbox(name, newValue)
                    return newValue
                end,
                makeNew = function()
                    return false
                end,
                equals = function(a, b)
                    return a == b
                end
            },
            ["core.string"] = {
                isMakeable = true,
                iconType = NodeEditorIconType.Circle,
                colorGetter = function()
                    return ImVec4.new(0.98039215686, 0.54901960784, 0.86666666666, 1)
                end,
                drawEditValue = function(value, name, availContent)
                    newValue = value
                    local titleWidth = ImVec2.new();
                    ImGui.CalcTextSize(titleWidth, name, nil, true, -1);
                    local width = math.max(availContent - titleWidth.x - ImGui.GetStyle().ItemSpacing.x * 2, 0)
                    ImGui.SetNextItemWidth(width);
                    _, newValue = ImGui.InputText(name, newValue, 0, nil, nil)
                    return newValue
                end,
                makeNew = function()
                    return ""
                end,
                equals = function(a, b)
                    return a == b
                end
            },
            ["core.Char"] = {
                isMakeable = true,
                iconType = NodeEditorIconType.Circle,
                colorGetter = function()
                    return ImVec4.new(0.74901960784, 0.24705882352, 0.29019607843, 1)
                end,
                drawEditValue = function(value, name, availContent)
                    return value
                end,
                makeNew = function()
                    return nil
                end,
                equals = function(a, b)
                    return a == b
                end
            },
            ["core.Object"] = {
                isMakeable = true,
                iconType = NodeEditorIconType.Circle,
                colorGetter = function()
                    return ImVec4.new(0.94901960784, 0.67450980392, 0.34117647058, 1)
                end,
                drawEditValue = function(value, name, availContent)
                    return value
                end,
                makeNew = function()
                    return nil
                end,
                equals = function(a, b)
                    return a == b
                end
            },
            ["core.Checkpoint"] = {
                isMakeable = true,
                iconType = NodeEditorIconType.Circle,
                colorGetter = function()
                    return ImVec4.new(0.57647058823, 0.61960784313, 0.74901960784, 1)
                end,
                drawEditValue = function(value, name, availContent)
                    return value
                end,
                makeNew = function()
                    return nil
                end,
                equals = function(a, b)
                    return a == b
                end
            },
            ["core.DecisionMakerChar"] = {
                isMakeable = true,
                iconType = NodeEditorIconType.Circle,
                colorGetter = function()
                    return ImVec4.new(0.74901960784, 0.45490196078, 0.2862745098, 1)
                end,
                drawEditValue = function(value, name, availContent)
                    return value
                end,
                makeNew = function()
                    return nil
                end,
                equals = function(a, b)
                    return a == b
                end
            },
            ["core.Searchlight"] = {
                isMakeable = true,
                iconType = NodeEditorIconType.Circle,
                colorGetter = function()
                    return ImVec4.new(0.94901960784, 0.67450980392, 0.44705882352, 1)
                end,
                drawEditValue = function(value, name, availContent)
                    return value
                end,
                makeNew = function()
                    return nil
                end,
                equals = function(a, b)
                    return a == b
                end
            },
            ["core.AudioStream3D"] = {
                isMakeable = true,
                iconType = NodeEditorIconType.Circle,
                colorGetter = function()
                    return ImVec4.new(0.06666666666, 0.21176470588, 0.33725490196, 1)
                end,
                drawEditValue = function(value, name, availContent)
                    return value
                end,
                makeNew = function()
                    return nil
                end,
                equals = function(a, b)
                    return a == b
                end
            },
            ["core.AudioStream"] = {
                isMakeable = true,
                iconType = NodeEditorIconType.Circle,
                colorGetter = function()
                    return ImVec4.new(0.16470588235, 0.19215686274, 0.34901960784, 1)
                end,
                drawEditValue = function(value, name, availContent)
                    return value
                end,
                makeNew = function()
                    return nil
                end,
                equals = function(a, b)
                    return a == b
                end
            },
            ["core.Vehicle"] = {
                isMakeable = true,
                iconType = NodeEditorIconType.Circle,
                colorGetter = function()
                    return ImVec4.new(0.05882352941, 0.31372549019, 0.65098039215, 1)
                end,
                drawEditValue = function(value, name, availContent)
                    return value
                end,
                makeNew = function()
                    return nil
                end,
                equals = function(a, b)
                    return a == b
                end
            },
            ["core.ScriptFire"] = {
                isMakeable = true,
                iconType = NodeEditorIconType.Circle,
                colorGetter = function()
                    return ImVec4.new(0.94901960784, 0.54901960784, 0.34117647058, 1)
                end,
                drawEditValue = function(value, name, availContent)
                    return value
                end,
                makeNew = function()
                    return nil
                end,
                equals = function(a, b)
                    return a == b
                end
            },
            ["core.Particle"] = {
                isMakeable = true,
                iconType = NodeEditorIconType.Circle,
                colorGetter = function()
                    return ImVec4.new(0.16078431372, 0.46274509803, 0.65098039215, 1)
                end,
                drawEditValue = function(value, name, availContent)
                    return value
                end,
                makeNew = function()
                    return nil
                end,
                equals = function(a, b)
                    return a == b
                end
            },
            ["core.Train"] = {
                isMakeable = true,
                iconType = NodeEditorIconType.Circle,
                colorGetter = function()
                    return ImVec4.new(0.2431372549, 0.31764705882, 0.34901960784, 1)
                end,
                drawEditValue = function(value, name, availContent)
                    return value
                end,
                makeNew = function()
                    return nil
                end,
                equals = function(a, b)
                    return a == b
                end
            }
        },
        __nodesFactories = {},
        variables = {},
        nodeTypesInfo = {},
        callbacksProjectClear = {}
    }
    return newEditor
end

---@param editor LDNodeEditor
LDNodeEditor.addNewContext = function(editor)
    ---@type LDNodeEditorContext
    local newContext = LDNodeEditorContext.new(NodeEditor.CreateEditor(config), "Workspace " .. (#editor.contexts + 1))
    table.insert(editor.contexts, newContext)
end

---@param editor LDNodeEditor
---@param id string
---@param type LDNodeEditorDataType
LDNodeEditor.addType = function(editor, id, type)
    editor.dataTypes[id] = type
end

---@param editor LDNodeEditor
---@param nodeType LDNodeEditorNodeType
LDNodeEditor.addNodeType = function(editor, nodeType)
    ---@diagnostic disable-next-line: inject-field
    nodeType.__class = "LDNodeEditorNodeType"
    editor.__nodesFactories[nodeType.typeName] = nodeType
end

---@param editor LDNodeEditor
---@param info NodeTypeInfo
LDNodeEditor.addNodeTypeInfo = function(editor, info)
    editor.nodeTypesInfo[info.name] = info
end

---@param editor LDNodeEditor
---@param typeName string
LDNodeEditor.getNodeType = function(editor, typeName)
    if not editor.__nodesFactories[typeName] then
        local info = editor.nodeTypesInfo[typeName]
        if info then
            local node = require("nodes\\" .. string.gsub(info.category, "%.", "\\") .. "\\" .. typeName:gsub("%.", "-"))
            LDNodeEditor.addNodeType(editor, node)
        end
    end
    return editor.__nodesFactories[typeName]
end

---@param editor LDNodeEditor
---@param nodeType LDNodeEditorNodeType
---@return integer newNodeId
LDNodeEditor.addNewNode = function(editor, nodeType)
    local context = editor.contexts[editor.currentIndexContext]
    local newNodeId = LDNodeEditorContext.getNextUniqueId(context);
    local newPinId = newNodeId;
    local getPinId = function()
        newPinId = newPinId + 1
        return newPinId
    end
    local node = nodeType.new(context, newNodeId, getPinId, editor)
    LDNodeEditorContext.addNode(context, node)

    return newNodeId
end

---@param a LDNodeEditorPin
---@param b LDNodeEditorPin
LDNodeEditor.canCreateLink = function(a, b)
    if not a or not b or a == b or a.kind == b.kind or a.type ~= b.type or a.node == b.node then
        return false
    end
    return true
end

---@param editor LDNodeEditor
---@param builder BlueprintNodeBuilder
---@param node LDNodeEditorNode
LDNodeEditor.defaultHeader = function (editor, builder, node)
    local nodeType = LDNodeEditor.getNodeType(editor, node.nodeType);
    local icon = nodeType.icon;
    if icon then
        icon = icon .. " ";
    else
        icon = "";
    end
    builder:Header(nodeType.color);
    ImGui.Spring(0, -1);
    if NodeEditor.GetCurrentZoom() < 3 then
        ImGui.TextUnformatted(icon .. ld.loc.get("nodes." .. node.nodeType .. ".title")); 
    end
    ImGui.Spring(1, -1);
    ImGui.Dummy(ImVec2.new(0, 28));
    ImGui.Spring(0, -1);
    builder:EndHeader();
end

---@param editor LDNodeEditor
---@param context LDNodeEditorContext
---@param builder BlueprintNodeBuilder
---@param pin LDNodeEditorPin
---@param name string | nil
---@param bodyCallback function | nil
LDNodeEditor.defaultInput = function (editor, context, builder, pin, name, bodyCallback)
    local alpha = ImGui.GetStyle().Alpha;
    if context.newLinkPin and not LDNodeEditor.canCreateLink(context.newLinkPin, pin) and context.newLinkPin ~= pin then
        alpha = alpha * (48.0 / 255.0);
    end
    builder:Input(NodeEditor.PinId(pin.id));
    ImGui.PushStyleVar(ImGuiStyleVar.Alpha, alpha);
    if NodeEditor.GetCurrentZoom() < 3 then
        LDNodeEditor.DrawPinIcon(editor, pin, LDNodeEditorContext.isPinLinked(context, pin.id), math.floor(alpha * 255));
    end
    ImGui.Spring(0, -1);
    if NodeEditor.GetCurrentZoom() < 3 then
        if name then
            ImGui.TextUnformatted(name);
            ImGui.Spring(0, -1);
        end
    else
        ImGui.Dummy(ImVec2.new(0, 28));
    end
    if NodeEditor.GetCurrentZoom() < 5 then
        if bodyCallback and not LDNodeEditorContext.isPinLinked(context, pin.id) then
            bodyCallback();
        end
    else
        ImGui.Dummy(ImVec2.new(ImGui.GetFontSize() * 7, 28));
    end
    ImGui.PopStyleVar(1);
    builder:EndInput();
end

LDNodeEditor.defaultOutput = function (editor, context, builder, pin, name, bodyCallback)
    local alpha = ImGui.GetStyle().Alpha;
    if context.newLinkPin and not LDNodeEditor.canCreateLink(context.newLinkPin, pin) and context.newLinkPin ~= pin then
        alpha = alpha * (48.0 / 255.0);
    end
    builder:Output(NodeEditor.PinId(pin.id));
    ImGui.PushStyleVar(ImGuiStyleVar.Alpha, alpha);
    if bodyCallback then
        bodyCallback();
        ImGui.Spring(0, -1);
    end
    if NodeEditor.GetCurrentZoom() < 3 then
        if name then
            ImGui.Spring(0, -1);
            ImGui.TextUnformatted(name);
        end
    else
        ImGui.Dummy(ImVec2.new(ImGui.GetFontSize() * 3, 28));
    end
    ImGui.Spring(0, -1);
    if NodeEditor.GetCurrentZoom() < 3 then
        LDNodeEditor.DrawPinIcon(editor, pin, LDNodeEditorContext.isPinLinked(context, pin.id), math.floor(alpha * 255));
    else
        ImGui.Dummy(ImVec2.new(ImGui.GetFontSize() * 3, 28));
    end
    ImGui.PopStyleVar(1);
    builder:EndOutput();
end

---@param editor LDNodeEditor
---@param pin LDNodeEditorPin
---@param isLinked boolean
---@param alpha number
LDNodeEditor.DrawPinIcon = function(editor, pin, isLinked, alpha)
    local fontScale = ImGui.GetFontSize() / 16;
    local pinType = editor.dataTypes[pin.type];
    local color = pinType.colorGetter();
    color.w = alpha / 255.0;
    NodeEditor.Icon(ImVec2.new(fontScale * 16, fontScale * 16), pinType.iconType, isLinked, color, ImColor.new(32, 32, 32, alpha).Value);
end

---@param editor LDNodeEditor
---@param type string
LDNodeEditor.addNewVariable = function(editor, type)
    local newVarUUID = uuid.new()
    while editor.variables[newVarUUID] ~= nil do
        newVarUUID = uuid.new()
    end
    editor.variables[newVarUUID] = {
        name = ld.loc.get("nodes.node_editor.variable") .. " " .. string.sub(newVarUUID, 1, 4),
        type = type,
        defaultValue = editor.dataTypes[type].makeNew(),
        value = nil
    }
    return newVarUUID
end

---@param ed LDNodeEditor
---@param context LDNodeEditorContext
---@param node LDNodeEditorNode
LDNodeEditor.getInputValues = function(ed, context, node)
    local inputValues = {}
    local calledNodes = {}
    for i, input in ipairs(node.inputs) do
        table.insert(inputValues, nil)
        for _, link in ipairs(context.__links) do
            if link.inputId == input.id then
                local outNodeId = math.floor(link.outputId / 100) * 100;
                local outNodeType = LDNodeEditor.getNodeType(ed, context.nodes[outNodeId].nodeType);
                if outNodeType.isCallable == false and not table.contains(calledNodes, outNodeId) then
                    LDNodeEditor.runNode(ed, context, context.nodes[outNodeId])
                    table.insert(calledNodes, outNodeId)
                end

                inputValues[i] = context.__pinsValues[link.outputId]
                break;
            end
        end
    end
    return inputValues
end

---@param ed LDNodeEditor
---@param context LDNodeEditorContext
---@param node LDNodeEditorNode
LDNodeEditor.runNode = function(ed, context, node)
    local nodeType = LDNodeEditor.getNodeType(ed, node.nodeType)
    local inputValues = LDNodeEditor.getInputValues(ed, context, node)

    -- print("Running node " .. node.nodeType)
    local outputValues = nodeType.run(ed, context, node, inputValues)
    if #outputValues ~= #node.outputs then
        error("The number of outputs of the node " .. node.nodeType .. " is different from the number of returned values")
    end

    local callNum = 1
    ---@type LDNodeEditorNode[]
    local flowCalls = {}
    for i, output in ipairs(node.outputs) do
        context.__pinsValues[output.id] = outputValues[i]

        if output.type == "core.flow" and outputValues[i] > 0 then
            callNum = outputValues[i]
            for _, link in ipairs(context.__links) do
                if link.outputId == output.id then
                    local nodeId = math.floor(link.inputId / 100) * 100
                    local nextNode = LDNodeEditorContext.findNode(context, nodeId)
                    if nextNode then
                        table.insert(flowCalls, nextNode)
                    end
                end
            end
        end
    end

    for _, nextNode in ipairs(flowCalls) do
        for i = 1, callNum do
            LDNodeEditor.runNode(ed, context, nextNode)
        end
    end
end

---@param editor LDNodeEditor
---@param callback fun()
function LDNodeEditor.registerCallbackProjectClear(editor, callback)
    table.insert(editor.callbacksProjectClear, callback)
end

---@param editor LDNodeEditor
function LDNodeEditor.clearProject(editor)
    for _, callback in ipairs(editor.callbacksProjectClear) do
        callback()
    end
end

return LDNodeEditor