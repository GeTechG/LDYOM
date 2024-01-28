local LDNodeEditorContext = require("ld.context")

local config = NodeEditorConfig.new();
config.SettingsFile = ""

---@class LDNodeEditor
---@field contexts LDNodeEditorContext[]
---@field currentIndexContext number
---@field pinTypes { [string]: LDNodeEditorPinType }
---@field __nodesFactories { [string]: LDNodeEditorNodeType }
---@field createNewNode boolean

local LDNodeEditor = {}

LDNodeEditor.new = function()
    ---@type LDNodeEditor
    local newEditor = {
        contexts = {
            LDNodeEditorContext.new(NodeEditor.CreateEditor(config), "Workspace 1")
        },
        currentIndexContext = 1,
        pinTypes = {
            ["core.flow"] = {
                name = "flow",
                iconType = NodeEditorIconType.Flow,
                colorGetter = function()
                    return ImGui.GetStyleColorVec4(ImGuiCol.Text)
                end
            },
            ["core.number"] = {
                name = "number",
                iconType = NodeEditorIconType.Circle,
                colorGetter = function()
                    return ImVec4.new(0.14117647058, 0.62352941176, 0.87058823529, 1)
                end
            },
            ["core.bool"] = {
                name = "bool",
                iconType = NodeEditorIconType.Circle,
                colorGetter = function()
                    return ImVec4.new(0.14117647058, 0.62352941176, 0.87058823529, 1)
                end
            },
            ["core.string"] = {
                name = "string",
                iconType = NodeEditorIconType.Circle,
                colorGetter = function()
                    return ImVec4.new(0.47058823529, 0.69019607843, 0.41960784313, 1)
                end
            }
        },
        __nodesFactories = {},
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
---@param type LDNodeEditorPinType
LDNodeEditor.addType = function(editor, id, type)
    editor.pinTypes[id] = type
end

---@param editor LDNodeEditor
---@param id string
---@param nodeType LDNodeEditorNodeType
LDNodeEditor.addNodeType = function(editor, id, nodeType)
    editor.__nodesFactories[id] = nodeType
end

---@param editor LDNodeEditor
---@param contextIndex number
---@param nodeType LDNodeEditorNodeType
LDNodeEditor.addNewNode = function(editor, contextIndex, nodeType)
    local context = editor.contexts[contextIndex]
    local node = nodeType.new(context)
    table.insert(context.nodes, node)
end

---@param a LDNodeEditorPin
---@param b LDNodeEditorPin
LDNodeEditor.canCreateLink = function(a, b)
    if not a or not b or a == b or a.kind == b.kind or a.type ~= b.type or a.node == b.node then
        return false
    end
    return true
end

---@param builder BlueprintNodeBuilder
---@param node LDNodeEditorNode
---@param color ImVec4
LDNodeEditor.defaultHeader = function (builder, node, color)
    builder:Header(color);
    ImGui.Spring(0, -1);
    ImGui.TextUnformatted(ld.loc.get("nodes." .. node.nodeType .. ".title"));
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
    LDNodeEditor.DrawPinIcon(editor, pin, LDNodeEditorContext.isPinLinked(context, pin.id), math.floor(alpha * 255));
    ImGui.Spring(0, -1);
    if name then
        ImGui.TextUnformatted(name);
        ImGui.Spring(0, -1);
    end
    if bodyCallback then
        bodyCallback();
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
    if name then
        ImGui.Spring(0, -1);
        ImGui.TextUnformatted(name);
    end
    ImGui.Spring(0, -1);
    LDNodeEditor.DrawPinIcon(editor, pin, LDNodeEditorContext.isPinLinked(context, pin.id), math.floor(alpha * 255));
    ImGui.PopStyleVar(1);
    builder:EndOutput();
end

---@param editor LDNodeEditor
---@param pin LDNodeEditorPin
---@param isLinked boolean
---@param alpha number
LDNodeEditor.DrawPinIcon = function(editor, pin, isLinked, alpha)
    local pinType = editor.pinTypes[pin.type];
    local color = pinType.colorGetter();
    color.w = alpha / 255.0;
    NodeEditor.Icon(ImVec2.new(24, 24), pinType.iconType, isLinked, color, ImColor.new(32, 32, 32, alpha).Value);
end

return LDNodeEditor