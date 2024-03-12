require("libs.string_utils")
require("libs.table_utils")

local LDNodeEditor = require("ld.node_editor")
local LDNodeEditorContext = require("ld.context")
local imguiOperator = require("libs.imgui_operators")
local fa = require("libs.fa")
local bit = require("bit")

local createNewNode = false;
---@type LDNodeEditorPin | nil
local newNodeLinkPin = nil;

local contextNodeId = NodeEditor.NodeId();
local contextLinkId = NodeEditor.LinkId()
local contextPinId = NodeEditor.PinId();

local init = false;
local leftPaneWidth  = 400.0;
local leftPanelMax = 400.0;

local rightPaneWidth  = 400.0;
local rightPanelMax = 400.0;

local lastFrameWindowSize = 0.0;

local showRightMenu = false;
local searchBuffer = "";
---@type integer | nil
local selectedContext = nil;

local openPopupPosition = ImVec2.new();

---@alias CategoryNodeTable table<string, CategoryNodeTable | NodeTypeInfo>

---@type CategoryNodeTable
local categoriesNodes = {}

---@enum DetailType
local DetailType = {
    None = 0,
    Variable = 1,
    Function = 2,
}

local selectedDetail = {
    ---@type DetailType
    type = DetailType.None,
    ---@type integer | string
    id = 0,
}

---@param type DetailType
---@param id integer | string
function selectDetail(type, id)
    selectedDetail.type = type;
    selectedDetail.id = id;
    showRightMenu = true;
end

---@param id string
---@param split_vertically boolean
---@param thickness number
---@param size1 number
---@param size2 number
---@param min_size1 number
---@param min_size2 number
---@param splitter_long_axis_size number
---@return boolean
---@return number size1
---@return number size2
function Splitter(id, split_vertically, thickness, size1, size2, min_size1, min_size2, splitter_long_axis_size)
    local window = getCurrentWindowGImgui()
    local id = window:GetID(toVoidPtr(id))
    local bb = ImRect.new()
    bb.Min = imguiOperator.ImVec2Add(window.DC.CursorPos, split_vertically and ImVec2.new(size1, 0.0) or ImVec2.new(0.0, size1))
    local itemSize = ImVec2.new();
    ImGui.CalcItemSize(itemSize, split_vertically and ImVec2.new(thickness, splitter_long_axis_size) or ImVec2.new(splitter_long_axis_size, thickness), 0.0, 0.0)
    bb.Max = imguiOperator.ImVec2Add(bb.Min, itemSize)
    return ImGui.SplitterBehavior(bb, id, split_vertically and ImGuiAxis.X or ImGuiAxis.Y, size1, size2, min_size1, min_size2, 0.0, 0.0, 0)
end

---@param paneWidth number
---@param ed LDNodeEditor
function ShowLeftPane(paneWidth, ed)
    local fontScale = ImGui.GetFontSize() / 16;

    ImGui.BeginChild("Selection", ImVec2.new(paneWidth, 0), 0, 0);

    local contentRegionAvail = ImVec2.new();
    ImGui.GetContentRegionAvail(contentRegionAvail);
    paneWidth = contentRegionAvail.x;

    local openRenamePopupContext = false;
    if ImGui.Button(fa.ICON_FA_PLUS_SQUARE .. "##context", ImVec2.new(21 * fontScale, 0)) then
        LDNodeEditor.addNewContext(ed);
        selectedContext = #ed.contexts;
        ed.currentIndexContext = selectedContext;
        openRenameVariablePopup = true;
    end
    ImGui.SameLine(0, -1);
    local isOpenListContexts = ImGui.CollapsingHeader(ld.loc.get("nodes.node_editor.contexts"), 0);
    if isOpenListContexts then
        if ImGui.BeginListBox("##listContexts", ImVec2.new(paneWidth,  0)) then

            for i, context in ipairs(ed.contexts) do
                local isSelected = ed.currentIndexContext == i;
                if ImGui.Selectable(context.name, isSelected, 0, ImVec2.new(0, 0)) then
                    ed.currentIndexContext = i;
                end
                if ImGui.BeginPopupContextItem("##edContextContextMenu" .. i, ImGuiPopupFlags.MouseButtonRight) then
                    ImGui.TextUnformatted(context.name);
                    ImGui.Separator();
                    if ImGui.MenuItem(ld.loc.get("nodes.node_editor.rename"), "", false, true) then
                        selectedContext = i;
                        openRenamePopupContext = true;
                    end
                    if ImGui.MenuItem(ld.loc.get("nodes.node_editor.delete"), "", false, #ed.contexts > 1) then
                        table.remove(ed.contexts, i);
                        ed.currentIndexContext = ed.currentIndexContext - 1;
                        ed.currentIndexContext = math.max(1, ed.currentIndexContext);
                    end
                    ImGui.EndPopup();
                end
            end

            ImGui.EndListBox();
        end
    end

    if openRenamePopupContext then
        ImGui.OpenPopup("renameContextPopup", 0);
    end

    if ImGui.BeginPopup("renameContextPopup", 0) then
        local context = ed.contexts[selectedContext];
        local newName = context.name;
        _, newName = ImGui.InputText("##inputNameRename", newName, ImGuiInputTextFlags.EnterReturnsTrue, nil, nil);
        if ImGui.IsItemDeactivatedAfterEdit() then
            context.name = newName;
            ImGui.CloseCurrentPopup();
        end
        ImGui.EndPopup();
    end

    local openRenameVariablePopup = false;
    if ImGui.Button(fa.ICON_FA_PLUS_SQUARE .. "##var", ImVec2.new(21 * fontScale, 0)) then
        local newVarUUID = LDNodeEditor.addNewVariable(ed, "core.number");
        selectDetail(DetailType.Variable, newVarUUID);
        openRenameVariablePopup = true;
    end
    ImGui.SameLine(0, -1);
    local isOpenListVariables = ImGui.CollapsingHeader(ld.loc.get("nodes.node_editor.variables"), 0);
    if isOpenListVariables then
        if ImGui.BeginListBox("##listVariables", ImVec2.new(paneWidth,  0)) then
            for uuid, variable in pairs(ed.variables) do
                NodeEditor.Icon(ImVec2.new(fontScale * 16, fontScale * 16), NodeEditorIconType.Circle, true, ed.dataTypes[variable.type].colorGetter(), ImVec4.new());
                ImGui.SameLine(0, -1);
                local isSelected = selectedDetail.type == DetailType.Variable and selectedDetail.id == uuid;
                if ImGui.Selectable(variable.name, isSelected, 0, ImVec2.new(0, 0)) then
                    selectDetail(DetailType.Variable, uuid);
                end
                if ImGui.BeginPopupContextItem("##edVariableContextMenu" .. uuid, ImGuiPopupFlags.MouseButtonRight) then
                    ImGui.TextUnformatted(variable.name);
                    ImGui.Separator();
                    if ImGui.MenuItem(ld.loc.get("nodes.node_editor.rename"), "", false, true) then
                        selectDetail(DetailType.Variable, uuid);
                        openRenameVariablePopup = true;
                    end
                    if ImGui.MenuItem(ld.loc.get("nodes.node_editor.delete"), "", false, true) then
                        ed.variables[uuid] = nil;
                        if selectedDetail.type == DetailType.Variable and selectedDetail.id == uuid then
                            selectDetail(DetailType.None, 0);
                        end
                        for _, ctx in ipairs(ed.contexts) do
                            for _, node in pairs(ctx.nodes) do
                                ---@cast node LDNodeEditorSetVariableNode
                                if node.nodeType == "core.set_variable" and node.varUuid == uuid then
                                    node.varUuid = "";
                                    table.erase_if(ctx.__links, function(link)
                                        return link.inputId == node.inputs[2].id;
                                    end);
                                end
                                if node.nodeType == "core.get_variable" and node.varUuid == uuid then
                                    node.varUuid = "";
                                    table.erase_if(ctx.__links, function(link)
                                        return link.outputId == node.outputs[1].id;
                                    end);
                                end
                            end
                        end
                    end
                    ImGui.EndPopup();
                end
            end
            ImGui.EndListBox();
        end
    end

    if openRenameVariablePopup then
        ImGui.OpenPopup("renameVariablePopup", 0);
    end

    if selectedDetail.type == DetailType.Variable and ImGui.BeginPopup("renameVariablePopup", 0) then
        local variable = ed.variables[selectedDetail.id];
        local newName = variable.name;
        _, newName = ImGui.InputText("##inputNameRename", newName, ImGuiInputTextFlags.EnterReturnsTrue, nil, nil);
        if ImGui.IsItemDeactivatedAfterEdit() then
            variable.name = newName;
            ImGui.CloseCurrentPopup();
        end
        ImGui.EndPopup();
    end

    ImGui.Text(ld.loc.get("nodes.node_editor.player_position"));
    local playerPed = PlayerOp.getChar(0)
    local px, py, pz = CharOp.getCoordinates(playerPed)
    if ImGui.Button("x: " .. px, ImVec2.new()) then
        ImGui.SetClipboardText(tostring(px));
    end
    if ImGui.Button("y: " .. py, ImVec2.new()) then
        ImGui.SetClipboardText(tostring(py));
    end
    if ImGui.Button("z: " .. pz, ImVec2.new()) then
        ImGui.SetClipboardText(tostring(pz));
    end

    if ImGui.Button(ld.loc.get("nodes.node_editor.clear_anim"), ImVec2.new()) then
        CharOp.clearTasksImmediately(PlayerOp.getChar(0));
        CharOp.clearTasks(PlayerOp.getChar(0));
    end

    ImGui.EndChild();

end

---@param paneWidth number
---@param ed LDNodeEditor
function ShowRightPane(paneWidth, ed)
    local fontScale = ImGui.GetFontSize() / 16;

    ImGui.BeginChild("Selection2", ImVec2.new(paneWidth, 0), 0, 0);

    local contentRegionAvail = ImVec2.new();
    ImGui.GetContentRegionAvail(contentRegionAvail);
    paneWidth = contentRegionAvail.x;

    ---@diagnostic disable-next-line: param-type-mismatch
    if ImGui.BeginTabBar("RightPaneTabs", bit.bor(ImGuiTabBarFlags.FittingPolicyScroll, ImGuiTabBarFlags.Reorderable)) then
        local tabIsOpen = false;
        tabIsOpen, showRightMenu  = ImGui.BeginTabItem(fa.ICON_FA_INFO_SQUARE .. " " .. ld.loc.get("nodes.node_editor.details.title"), showRightMenu, ImGuiTabItemFlags.None);
        if tabIsOpen then

            if selectedDetail.type == DetailType.Variable then
                local variable = ed.variables[selectedDetail.id];
                if variable then
                    if ImGui.CollapsingHeader(ld.loc.get("nodes.node_editor.variable"), 0) then

                        local availContent = ImVec2.new();
                        ImGui.GetContentRegionAvail(availContent);
    
                        local titleWidth = ImVec2.new(); 
                        ImGui.CalcTextSize(titleWidth, ld.loc.get("nodes.node_editor.details.name"), nil, false, -1);
                        ImGui.SetNextItemWidth(availContent.x - titleWidth.x - ImGui.GetStyle().ItemSpacing.x * 2);
                        local isEdit, newName = ImGui.InputText(ld.loc.get("nodes.node_editor.details.name"), variable.name, 0, nil, nil);
                        if isEdit then
                            variable.name = newName;
                        end
    
                        ImGui.CalcTextSize(titleWidth, ld.loc.get("nodes.node_editor.details.type"), nil, false, -1);
                        ImGui.SetNextItemWidth(availContent.x - titleWidth.x - ImGui.GetStyle().ItemSpacing.x * 2);
                        if ImGui.BeginCombo(ld.loc.get("nodes.node_editor.details.type"), ld.loc.get("nodes.types." .. variable.type), 0) then
                            for id, type in pairs(ed.dataTypes) do
                                if not type.isMakeable then
                                    goto continue;
                                end
                                NodeEditor.Icon(ImVec2.new(fontScale * 16, fontScale * 16), NodeEditorIconType.Circle, true, ed.dataTypes[id].colorGetter(), ImVec4.new());
                                ImGui.SameLine(0, -1);
                                if ImGui.Selectable(ld.loc.get("nodes.types." .. id), variable.type == id, 0, ImVec2.new(0, 0)) then
                                    variable.type = id;
                                    variable.defaultValue = type.makeNew();
                                    for _, ctx in ipairs(ed.contexts) do
                                        for _, node in pairs(ctx.nodes) do
                                            ---@cast node LDNodeEditorSetVariableNode
                                            if node.nodeType == "core.set_variable" and node.varUuid == selectedDetail.id then
                                                node.varValue = ed.dataTypes[id].makeNew();
                                                node.inputs[2].type = id;
                                                table.erase_if(ctx.__links, function(link)
                                                    return link.inputId == node.inputs[2].id;
                                                end);
                                            end
                                            if node.nodeType == "core.get_variable" and node.varUuid == selectedDetail.id then
                                                node.outputs[1].type = id;
                                                table.erase_if(ctx.__links, function(link)
                                                    return link.outputId == node.outputs[1].id;
                                                end);
                                            end
                                        end
                                    end
                                end
                                ::continue::
                            end
                            ImGui.EndCombo();
                        end
                    end
                    if ImGui.CollapsingHeader(ld.loc.get("nodes.node_editor.details.default_value"), 0) then
                        local type = ed.dataTypes[variable.type];
                        variable.defaultValue = type.drawEditValue(variable.defaultValue, ld.loc.get("nodes.node_editor.details.value"), contentRegionAvail.x);
                    end
                end
            end


            ImGui.EndTabItem();
        end

        ImGui.EndTabBar();
    end

    ImGui.EndChild();

end

---@param ed LDNodeEditor
function draw(ed)
    local fontScale = ImGui.GetFontSize() / 16;
    local displaySize = ImGui.GetIO().DisplaySize;
    ImGui.SetNextWindowPos(ImVec2.new(displaySize.x / 2, displaySize.y / 2), ImGuiCond.Appearing, ImVec2.new(0.5, 0.5));
    ImGui.SetNextWindowSize(ImVec2.new(800 * fontScale, 600 * fontScale), ImGuiCond.Appearing);
    local isOpen, newState = ImGui.Begin(ld.loc.get("nodes.node_editor.title"), ld.window.luaWindowState, 0);
    ld.window.luaWindowState = newState;
    if isOpen then
        local context = ed.contexts[ed.currentIndexContext];
        NodeEditor.SetCurrentEditor(context.context);

        if not init then
            init = true;
            leftPaneWidth = 200.0 * fontScale;
            leftPanelMax = 200.0 * fontScale;

            rightPaneWidth = 400 * fontScale;
            rightPanelMax = 100 * fontScale;

            for _, nodeTypeInfo in pairs(ed.nodeTypesInfo) do
                local caterogyParts = string.split(nodeTypeInfo.category, ".");
                ---@type CategoryNodeTable
                local currentPart = categoriesNodes;
                for _, part in ipairs(caterogyParts) do
                    if not currentPart[part] then
                        currentPart[part] = {};
                    end
                    currentPart = currentPart[part];
                end
                currentPart[nodeTypeInfo.name] = nodeTypeInfo;
            end
        end

        _, leftPaneWidth, leftPanelMax = Splitter("##SplitterLeft", true, 4.0, leftPaneWidth, leftPanelMax, 50.0, 50.0, -1);

        ShowLeftPane(leftPaneWidth - 4.0, ed);

        ImGui.SameLine(0.0, 12.0);

        local availContent = ImVec2.new();
        ImGui.GetContentRegionAvail(availContent)
        if lastFrameWindowSize ~= 0 then
            rightPanelMax = availContent.x - rightPaneWidth - 50;
            rightPaneWidth = rightPaneWidth + availContent.x - lastFrameWindowSize;
            rightPaneWidth = math.min(rightPaneWidth, availContent.x - 50);
            rightPaneWidth = math.max(rightPaneWidth, 50);
        end
        lastFrameWindowSize = availContent.x;

        if showRightMenu then
            _, rightPaneWidth, rightPanelMax = Splitter("##SplitterRight", true, 4.0, rightPaneWidth, rightPanelMax, 50.0, 50.0, -1);
        end

        NodeEditor.Begin(ld.loc.get("nodes.node_editor.title") .. "##editor", ImVec2.new(showRightMenu and rightPaneWidth or 0, 0));

        ---///////////////////////////

        local cursorTopLeft = ImVec2.new();
        ImGui.GetCursorScreenPos(cursorTopLeft)

        local builder = BlueprintNodeBuilder.new()

        ImGui.PushStyleColor(ImGuiCol.Text, ImVec4.new(1, 1, 1, 1));
        for _, node in pairs(context.nodes) do
            local nodeType = LDNodeEditor.getNodeType(ed, node.nodeType);
            nodeType.draw(ed, context, node, builder);
            
            if not context.__loaded then
                NodeEditor.SetNodePosition(NodeEditor.NodeId(node.id), ImVec2.new(node.position_x, node.position_y));
            else
                local nodePos = ImVec2.new();
                NodeEditor.GetNodePosition(NodeEditor.NodeId(node.id), nodePos);
                node.position_x = nodePos.x;
                node.position_y = nodePos.y;
            end
        end
        ImGui.PopStyleColor(1);

        for _, link in ipairs(context.__links) do
            local inputPin = nil;
            for _, node in pairs(context.nodes) do
                for _, pin in ipairs(node.inputs) do
                    if pin.id == link.inputId then
                        inputPin = pin;
                    end
                end
            end
            if inputPin then
                local color = ed.dataTypes[inputPin.type].colorGetter()
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
                            showLabel(ld.loc.get("nodes.node_editor.incorrect_pin_kind"), ImColor.new(45, 32, 32, 180));
                            NodeEditor.RejectNewItem(ImVec4.new(1, 0, 0, 1), 2);
                        elseif inputPin.type ~= outputPin.type then
                            showLabel(ld.loc.get("nodes.node_editor.incorrect_pin_type"), ImColor.new(45, 32, 32, 180));
                            NodeEditor.RejectNewItem(ImVec4.new(1, 128 / 255, 128 / 255, 1), 1);
                        else
                            showLabel(ld.loc.get("nodes.node_editor.create_link"), ImColor.new(32, 45, 32, 180));
                            if NodeEditor.AcceptNewItem(ImColor.new(128, 255, 128, 255).Value, 4.0) then
                                local pinLinkId = LDNodeEditorContext.getLinkByPin(context, inputPinId:Get());
                                if pinLinkId then
                                    table.erase_if(context.__links, function(link)
                                        return link.id == pinLinkId.id;
                                    end);
                                end
                                LDNodeEditorContext.addLink(context, inputPinId:Get(), outputPinId:Get());
                            end
                        end
                    end
                end

                local pinId = NodeEditor.PinId();
                if NodeEditor.QueryNewNode(pinId) then
                    context.newLinkPin = LDNodeEditorContext.findPin(context, pinId:Get());
                    if context.newLinkPin then
                        showLabel(ld.loc.get("nodes.node_editor.create_node"), ImColor.new(32, 45, 32, 180));
                    end

                    if NodeEditor.AcceptNewItem() then
                        createNewNode = true;
                        newNodeLinkPin = LDNodeEditorContext.findPin(context, pinId:Get());
                        context.newLinkPin = nil;
                        NodeEditor.Suspend();
                        ImGui.OpenPopup("createNodePopup", 0);
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
                        context.nodes[nodeId:Get()] = nil;
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

        
        NodeEditor.Suspend();
        if NodeEditor.ShowNodeContextMenu(contextNodeId) then
            ImGui.OpenPopup("nodeContextPopup", 0);
        elseif NodeEditor.ShowPinContextMenu(contextPinId) then
            ImGui.OpenPopup("pinContextPopup", 0);
        elseif NodeEditor.ShowLinkContextMenu(contextLinkId) then
            -- ImGui.OpenPopup("linkContextPopup", 0);
        elseif NodeEditor.ShowBackgroundContextMenu() then
            ImGui.OpenPopup("createNodePopup", 0);
            newNodeLinkPin = nil;
        end
        NodeEditor.Resume();

        NodeEditor.Suspend();
        ImGui.PushStyleVar(ImGuiStyleVar.WindowPadding, ImVec2.new(8, 8));
        if ImGui.BeginPopup("nodeContextPopup", 0) then
            local node = LDNodeEditorContext.findNode(context, contextNodeId:Get());
            if node then
                ImGui.TextUnformatted(ld.loc.get("nodes.node_editor.node_context_menu"));
                local short_desc = ld.loc.get("nodes." .. node.nodeType .. ".short_desc")
                if short_desc ~= ("nodes." .. node.nodeType .. ".short_desc") then
                    ImGui.PushTextWrapPos(ImGui.GetFontSize() * 20.0);
                    ImGui.TextUnformatted(short_desc);
                    ImGui.PopTextWrapPos();
                end
                ImGui.Separator();
                if ImGui.MenuItem(ld.loc.get("nodes.node_editor.delete"),"", false, true) then
                    NodeEditor.DeleteNode(contextNodeId);
                end
            end
            ImGui.EndPopup();
        end

        if ImGui.BeginPopup("pinContextPopup", 0) then
            local pin = LDNodeEditorContext.findPin(context, contextPinId:Get());
            if pin then
                ImGui.TextUnformatted(ld.loc.get("nodes.node_editor.pin_context_menu"));
                ImGui.Separator();
                ImGui.Text(string.format(ld.loc.get("nodes.node_editor.type"), ld.loc.get("nodes.types." .. pin.type)));
            end
            ImGui.EndPopup();
        end

        if ImGui.BeginPopup("linkContextPopup", 0) then
            local link = LDNodeEditorContext.findLink(context, contextLinkId:Get());
            ImGui.TextUnformatted(ld.loc.get("nodes.node_editor.link_context_menu"));
            ImGui.Separator();
            if link then
                ImGui.Text("ID: %p", link.id);
                ImGui.Text("From: %p", link.outputId);
                ImGui.Text("To: %p", link.inputId);
            else
                ImGui.Text("Unknown link: %p", contextLinkId:Get());
            end
            ImGui.Separator();
            if ImGui.MenuItem(ld.loc.get("nodes.node_editor.delete"), "", false, true) then
                NodeEditor.DeleteLink(contextLinkId);
            end
            ImGui.EndPopup();
        end

        if ImGui.BeginPopup("createNodePopup", 0) then
            local newNodePositon = ImVec2.new(openPopupPosition.x, openPopupPosition.y);

            local node = nil;
            ---@param categories CategoryNodeTable
            function categoryRender(categories, depth)
                ImGui.BeginChild("##category" .. depth, ImVec2.new(fontScale * 200, fontScale * 300), 0, 0);
                for categoryName, category in pairs(categories) do
                    if category["name"] ~= nil and category["category"] ~= nil then
                        local nodeTypeT = ed.__nodesFactories[category["name"]];
                        if nodeTypeT and nodeTypeT.icon then
                            ImGui.TextColored(nodeTypeT.color, nodeTypeT.icon)
                            ImGui.SameLine(0, -1);
                        end
                        if ImGui.MenuItem(ld.loc.get("nodes." .. category["name"] .. ".title"), "", false, true) then
                            local nodeType = LDNodeEditor.getNodeType(ed, category["name"]);
                            local newNodeId = LDNodeEditor.addNewNode(ed, nodeType);
                            node = context.nodes[newNodeId];
                            ImGui.CloseCurrentPopup();
                        end
                        local short_desc = ld.loc.get("nodes." .. category["name"] .. ".short_desc")
                        if short_desc ~= ("nodes." .. category["name"] .. ".short_desc") then
                            if ImGui.BeginItemTooltip() then
                                ImGui.PushTextWrapPos(ImGui.GetFontSize() * 20.0);
                                ImGui.TextUnformatted(short_desc);
                                ImGui.PopTextWrapPos();
                                ImGui.EndTooltip();
                            end
                        end
                    else
                        if ImGui.BeginMenu(ld.loc.get("nodes.node_editor.categories." .. categoryName), true) then
                            categoryRender(category, depth + 1);
                            ImGui.EndMenu();
                        end
                    end
                end
                ImGui.EndChild();
            end

            local availContentSize = ImVec2.new();
            ImGui.GetContentRegionAvail(availContentSize);

            ImGui.SetNextItemWidth(availContentSize.x);
            local isEdit, newBuffer = ImGui.InputText("##searchNode", searchBuffer, ImGuiInputTextFlags.AutoSelectAll);
            if isEdit then
                searchBuffer = newBuffer;
            end

            if searchBuffer == "" then
                categoryRender(categoriesNodes, 0);
            else
                ImGui.BeginChild("##searchNodeChild", ImVec2.new(fontScale * 200, fontScale * 300), 0, 0);
                for _, nodeTypeInfo in pairs(ed.nodeTypesInfo) do
                    local short_desc = ld.loc.get("nodes." .. nodeTypeInfo.name .. ".short_desc")
                    local name = ld.loc.get("nodes." .. nodeTypeInfo.name .. ".title")

                    local isFindName = string.find(name:lower(), searchBuffer:lower()) ~= nil;
                    local isDescFind = string.find(short_desc:lower(), searchBuffer:lower()) ~= nil;
                    if isFindName or isDescFind then
                        if ImGui.MenuItem(name, "", false, true) then
                            local nodeType = LDNodeEditor.getNodeType(ed, nodeTypeInfo.name);
                            local newNodeId = LDNodeEditor.addNewNode(ed, nodeType);
                            node = context.nodes[newNodeId];
                            ImGui.CloseCurrentPopup();
                        end
                        if short_desc ~= ("nodes." .. nodeTypeInfo.name .. ".short_desc") then
                            if ImGui.BeginItemTooltip() then
                                ImGui.PushTextWrapPos(ImGui.GetFontSize() * 20.0);
                                ImGui.TextUnformatted(short_desc);
                                ImGui.PopTextWrapPos();
                                ImGui.EndTooltip();
                            end
                        end
                    end
                end
                ImGui.EndChild();
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
            NodeEditor.Resume();
            ImGui.GetMousePos(openPopupPosition);
            NodeEditor.Suspend();
            createNewNode = false;
        end
        ImGui.PopStyleVar(1);
        NodeEditor.Resume();

        NodeEditor.End();

        ImGui.SameLine(0.0, 12.0);

        if showRightMenu then
            ShowRightPane(0, ed);
        end

        context.__loaded = true;
        NodeEditor.SetCurrentEditor(nil);
    end
    ImGui.End();
end

return draw