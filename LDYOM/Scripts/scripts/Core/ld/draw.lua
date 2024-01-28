local LDNodeEditor = require("ld.node_editor")
local LDNodeEditorContext = require("ld.context")
local imguiOperator = require("libs.imgui_operators")

local createNewNode = false;
---@type LDNodeEditorPin | nil
local newNodeLinkPin = nil;

local contextNodeId = NodeEditor.NodeId();
local contextLinkId = NodeEditor.LinkId()
local contextPinId = NodeEditor.PinId();

---@param ed LDNodeEditor
function draw(ed)
    local fontScale = ImGui.GetFontSize() / 16;
    ImGui.SetNextWindowSize(ImVec2.new(800 * fontScale, 600 * fontScale), ImGuiCond.Appearing);
    local isOpen, newState = ImGui.Begin("Node Editor", ld.window.luaWindowState, 0);
    ld.window.luaWindowState = newState;
    if isOpen then
        local context = ed.contexts[ed.currentIndexContext];
        NodeEditor.SetCurrentEditor(context.context);

        NodeEditor.Begin("My Editor", ImVec2.new(0, 0));

        ---///////////////////////////

        local cursorTopLeft = ImVec2.new();
        ImGui.GetCursorScreenPos(cursorTopLeft)

        local builder = BlueprintNodeBuilder.new()

        ImGui.PushStyleColor(ImGuiCol.Text, ImVec4.new(1, 1, 1, 1));
        for _, node in ipairs(context.nodes) do
            local nodeType = ed.__nodesFactories[node.nodeType];
            nodeType.draw(ed, context, node, builder);
        end
        ImGui.PopStyleColor(1);

        for _, link in ipairs(context.__links) do
            local inputPin = nil;
            for _, node in ipairs(context.nodes) do
                for _, pin in ipairs(node.inputs) do
                    if pin.id == link.inputId then
                        inputPin = pin;
                    end
                end
            end
            if inputPin then
                local color = ed.pinTypes[inputPin.type].colorGetter()
                NodeEditor.Link(NodeEditor.LinkId(link.id), NodeEditor.PinId(link.inputId), NodeEditor.PinId(link.outputId), color, 2);
            end
        end

        if not createNewNode then
            if NodeEditor.BeginCreate(ImVec4.new(1, 1, 1, 1), 2) then
                ---@param label string
                ---@param color ImColor
                local showLabel = function(label, color)
                    ImGui.SetCursorPosY(ImGui.GetCursorPosY() - ImGui.GetTextLineHeight());
                    local size = ImVec2.new();
                    ImGui.CalcTextSize(size, label, nil, false, -1)

                    local padding = ImGui.GetStyle().FramePadding;
                    local spacing = ImGui.GetStyle().ItemSpacing;

                    local cursorPos = ImVec2.new();
                    ImGui.GetCursorPos(cursorPos);
                    ImGui.SetCursorPos(imguiOperator.ImVec2Add(cursorPos, ImVec2.new(spacing.x, -spacing.y)));

                    local cursorScreenPos = ImVec2.new();
                    ImGui.GetCursorScreenPos(cursorScreenPos);
                    local rectMin = imguiOperator.ImVec2Subtract(cursorScreenPos, padding);
                    local rectMax = imguiOperator.ImVec2Add(imguiOperator.ImVec2Add(cursorScreenPos,size), padding);

                    local drawList = ImGui.GetWindowDrawList();
                    drawList:AddRectFilled(rectMin, rectMax, ImGui.ColorConvertFloat4ToU32(color.Value), size.y * 0.15, 0);
                    ImGui.PushStyleColor(ImGuiCol.Text, ImVec4.new(1, 1, 1, 1));
                    ImGui.TextUnformatted(label);
                    ImGui.PopStyleColor(1);
                end

                local outputPinId, inputPinId = NodeEditor.PinId(), NodeEditor.PinId();
                if NodeEditor.QueryNewLink(outputPinId, inputPinId) then
                    local outputPin = LDNodeEditorContext.findPin(context, outputPinId:Get());
                    local inputPin = LDNodeEditorContext.findPin(context, inputPinId:Get());

                    context.newLinkPin = outputPin or inputPin;

                    if outputPin and outputPin.kind == NodeEditorPinKind.Input then
                        outputPin, inputPin = inputPin, outputPin;
                        outputPinId, inputPinId = inputPinId, outputPinId;
                    end

                    if outputPin and inputPin then
                        if inputPin == outputPin then
                            NodeEditor.RejectNewItem(ImVec4.new(1, 0, 0, 1), 2);
                        elseif inputPin.kind == outputPin.kind then
                            showLabel("x Incompatible Pin Kind", ImColor.new(45, 32, 32, 180));
                            NodeEditor.RejectNewItem(ImVec4.new(1, 0, 0, 1), 2);
                        elseif inputPin.type ~= outputPin.type then
                            showLabel("x Incompatible Pin Type", ImColor.new(45, 32, 32, 180));
                            NodeEditor.RejectNewItem(ImVec4.new(1, 128 / 255, 128 / 255, 1), 1);
                        else
                            showLabel("+ Create Link", ImColor.new(32, 45, 32, 180));
                            if NodeEditor.AcceptNewItem(ImColor.new(128, 255, 128, 255).Value, 4.0) then
                                LDNodeEditorContext.addLink(context, inputPinId:Get(), outputPinId:Get());
                            end
                        end
                    end
                end

                local pinId = NodeEditor.PinId();
                if NodeEditor.QueryNewNode(pinId) then
                    context.newLinkPin = LDNodeEditorContext.findPin(context, pinId:Get());
                    if context.newLinkPin then
                        showLabel("+ Create Node", ImColor.new(32, 45, 32, 180));
                    end

                    if NodeEditor.AcceptNewItem() then
                        createNewNode = true;
                        newNodeLinkPin = LDNodeEditorContext.findPin(context, pinId:Get());
                        context.newLinkPin = nil;
                        NodeEditor.Suspend();
                        ImGui.OpenPopup("Create New Node", 0);
                        NodeEditor.Resume();
                    end
                end
            else
                context.newLinkPin = nil;
            end

            ImGui.SetCursorScreenPos(cursorTopLeft);

            NodeEditor.EndCreate();

            if NodeEditor.BeginDelete() then
                local nodeId = NodeEditor.NodeId();
                while NodeEditor.QueryDeletedNode(nodeId) do
                    if NodeEditor.AcceptDeletedItem(true) then
                        table.erase_if(context.nodes, function(node)
                            return node.id == nodeId:Get();
                        end);
                    end
                end

                local linkId = NodeEditor.LinkId();
                while NodeEditor.QueryDeletedLink(linkId) do
                    if NodeEditor.AcceptDeletedItem(true) then
                        table.erase_if(context.__links, function(link)
                            return link.id == linkId:Get();
                        end);
                    end
                end
            end
            NodeEditor.EndDelete();
        end

        ImGui.SetCursorScreenPos(cursorTopLeft);

        ---///////////////////////////

        local openPopupPosition = ImVec2.new();
        ImGui.GetMousePos(openPopupPosition);
        NodeEditor.Suspend();
        if NodeEditor.ShowNodeContextMenu(contextNodeId) then
            ImGui.OpenPopup("Node Context Menu", 0);
        elseif NodeEditor.ShowPinContextMenu(contextPinId) then
            ImGui.OpenPopup("Pin Context Menu", 0);
        elseif NodeEditor.ShowLinkContextMenu(contextLinkId) then
            ImGui.OpenPopup("Link Context Menu", 0);
        elseif NodeEditor.ShowBackgroundContextMenu() then
            ImGui.OpenPopup("Create New Node", 0);
            newNodeLinkPin = nil;
        end
        NodeEditor.Resume();

        NodeEditor.Suspend();
        ImGui.PushStyleVar(ImGuiStyleVar.WindowPadding, ImVec2.new(8, 8));
        if ImGui.BeginPopup("Node Context Menu", 0) then
            local node = LDNodeEditorContext.findNode(context, contextNodeId:Get());
            ImGui.TextUnformatted("Node Context Menu");
            ImGui.Separator();
            if node then
                ImGui.Text("ID: %p", node.id);
                ImGui.Text("Type: %s", node.nodeType);
                ImGui.Text("Inputs: %d", #node.inputs);
                ImGui.Text("Outputs: %d", #node.outputs);
            else
                ImGui.Text("Unknown node: %p", contextNodeId:Get());
            end
            ImGui.Separator();
            if ImGui.MenuItem("Delete","", false, true) then
                NodeEditor.DeleteNode(contextNodeId);
            end
            ImGui.EndPopup();
        end

        if ImGui.BeginPopup("Pin Context Menu", 0) then
            local pin = LDNodeEditorContext.findPin(context, contextPinId:Get());
            ImGui.TextUnformatted("Pin Context Menu");
            ImGui.Separator();
            if pin then
                ImGui.Text("ID: %p", pin.id);
                if pin.node then
                    ImGui.Text("Node: %p", pin.node);
                else
                    ImGui.Text("Node: %s", "<none>");
                end
            else
                ImGui.Text("Unknown pin: %p", contextPinId:Get());
            end
            ImGui.EndPopup();
        end

        if ImGui.BeginPopup("Link Context Menu", 0) then
            local link = LDNodeEditorContext.findLink(context, contextLinkId:Get());
            ImGui.TextUnformatted("Link Context Menu");
            ImGui.Separator();
            if link then
                ImGui.Text("ID: %p", link.id);
                ImGui.Text("From: %p", link.outputId);
                ImGui.Text("To: %p", link.inputId);
            else
                ImGui.Text("Unknown link: %p", contextLinkId:Get());
            end
            ImGui.Separator();
            if ImGui.MenuItem("Delete", "", false, true) then
                NodeEditor.DeleteLink(contextLinkId);
            end
            ImGui.EndPopup();
        end

        if ImGui.BeginPopup("Create New Node", 0) then
            local newNodePositon = ImVec2.new(openPopupPosition.x, openPopupPosition.y);

            local node = nil;
            for _, nodeType in pairs(ed.__nodesFactories) do
                if ImGui.MenuItem(ld.loc.get("nodes." .. nodeType.typeName .. ".title"), "", false, true) then
                    LDNodeEditor.addNewNode(ed, ed.currentIndexContext, nodeType);
                    node = context.nodes[#context.nodes];
                end
            end

            if node then
                createNewNode = false;

                NodeEditor.SetNodePosition(NodeEditor.NodeId(node.id), newNodePositon);

                local startPin = newNodeLinkPin;
                if startPin then
                    local pins = startPin.kind == NodeEditorPinKind.Input and node.outputs or node.inputs;

                    for _, pin in ipairs(pins) do
                        if LDNodeEditor.canCreateLink(startPin, pin) then
                            local endPin = pin;
                            if startPin.kind == NodeEditorPinKind.Input then
                                startPin, endPin = endPin, startPin;
                            end
                            LDNodeEditorContext.addLink(context, endPin.id, startPin.id);
                            break;
                        end
                    end
                end
            end

            ImGui.EndPopup();
        else
            createNewNode = false;
        end
        ImGui.PopStyleVar(1);
        NodeEditor.Resume();

        NodeEditor.End();

        NodeEditor.SetCurrentEditor(nil);
    end
    ImGui.End();
end

return draw