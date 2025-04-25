---@meta

---@alias EditorContext userdata

NodeEditor = {
    ---void SetCurrentEditor(EditorContext* ctx);
    ---@param ctx EditorContext | nil
    SetCurrentEditor = function(ctx) end,
    ---EditorContext* GetCurrentEditor();
    ---@return EditorContext
    GetCurrentEditor = function() end,

    ---EditorContext* CreateEditor(const Config* config = nullptr);
    ---@param config NodeEditorConfig
    ---@return EditorContext
    CreateEditor = function(config) end,

    ---void DestroyEditor(EditorContext* ctx);
    ---@param ctx EditorContext
    DestroyEditor = function(ctx) end,

    ---const Config& GetConfig(EditorContext* ctx = nullptr);
    ---@param ctx EditorContext
    ---@return NodeEditorConfig
    GetConfig = function(ctx) end,

    ---Style& GetStyle();
    ---@return NodeEditorStyle
    GetStyle = function() end,

    ---const char* GetStyleColorName(StyleColor colorIndex);
    ---@param colorIndex NodeEditorStyleColor
    ---@return string
    GetStyleColorName = function(colorIndex) end,

    ---void PushStyleColor(StyleColor colorIndex, const ImVec4& color);
    ---@param colorIndex NodeEditorStyleColor
    ---@param color ImVec4
    PushStyleColor = function(colorIndex, color) end,

    ---void PopStyleColor(int count = 1);
    ---@param count number
    PopStyleColor = function(count) end,

    ---void PushStyleVar(StyleVar varIndex, float value);
    ---@param varIndex NodeEditorStyleVar
    ---@param value number
    PushStyleVar = function(varIndex, value) end,

    ---void PushStyleVar(StyleVar varIndex, const ImVec2& value);
    ---@param varIndex NodeEditorStyleVar
    ---@param value ImVec2
    PushStyleVar = function(varIndex, value) end,

    ---void PushStyleVar(StyleVar varIndex, const ImVec4& value);
    ---@param varIndex NodeEditorStyleVar
    ---@param value ImVec4
    PushStyleVar = function(varIndex, value) end,

    ---void PopStyleVar(int count = 1);
    ---@param count number
    PopStyleVar = function(count) end,

    ---void Begin(const char* id, const ImVec2& size = ImVec2(0, 0));
    ---@param id string
    ---@param size ImVec2
    Begin = function(id, size) end,

    ---void End();
    End = function() end,

    ---void BeginNode(NodeId id);
    ---@param id NodeEditorNodeId
    BeginNode = function(id) end,

    ---void BeginPin(PinId id, PinKind kind);
    ---@param id NodeEditorPinId
    ---@param kind NodeEditorPinKind
    BeginPin = function(id, kind) end,

    ---void PinRect(const ImVec2& a, const ImVec2& b);
    ---@param a ImVec2
    ---@param b ImVec2
    PinRect = function(a, b) end,

    ---void PinPivotRect(const ImVec2& a, const ImVec2& b);
    ---@param a ImVec2
    ---@param b ImVec2
    PinPivotRect = function(a, b) end,

    ---void PinPivotSize(const ImVec2& size);
    ---@param size ImVec2
    PinPivotSize = function(size) end,

    ---void PinPivotScale(const ImVec2& scale);
    ---@param scale ImVec2
    PinPivotScale = function(scale) end,

    ---void PinPivotAlignment(const ImVec2& alignment);
    ---@param alignment ImVec2
    PinPivotAlignment = function(alignment) end,

    ---void EndPin();
    EndPin = function() end,

    ---void Group(const ImVec2& size);
    ---@param size ImVec2
    Group = function(size) end,

    ---void EndNode();
    EndNode = function() end,

    ---bool BeginGroupHint(NodeId nodeId);
    ---@param nodeId NodeEditorNodeId
    ---@return boolean
    BeginGroupHint = function(nodeId) end,

    ---ImVec2 GetGroupMin();
    ---@return ImVec2
    GetGroupMin = function() end,

    ---ImVec2 GetGroupMax();
    ---@return ImVec2
    GetGroupMax = function() end,

    ---ImDrawList* GetHintForegroundDrawList();
    ---@return ImDrawList
    GetHintForegroundDrawList = function() end,

    ---ImDrawList* GetHintBackgroundDrawList();
    ---@return ImDrawList
    GetHintBackgroundDrawList = function() end,

    ---void EndGroupHint();
    EndGroupHint = function() end,

    ---ImDrawList* GetNodeBackgroundDrawList(NodeId nodeId);
    ---@param nodeId NodeEditorNodeId
    ---@return ImDrawList
    GetNodeBackgroundDrawList = function(nodeId) end,

    ---bool Link(LinkId id, PinId startPinId, PinId endPinId, const ImVec4& color = ImVec4(1, 1, 1, 1), float thickness = 1.0f);
    ---@param id NodeEditorLinkId
    ---@param startPinId NodeEditorPinId
    ---@param endPinId NodeEditorPinId
    ---@param color ImVec4
    ---@param thickness number
    ---@return boolean
    Link = function(id, startPinId, endPinId, color, thickness) end,

    ---void Flow(LinkId linkId, FlowDirection direction = FlowDirection::Forward);
    ---@param linkId NodeEditorLinkId
    ---@param direction NodeEditorFlowDirection
    Flow = function(linkId, direction) end,

    ---bool BeginCreate(const ImVec4& color = ImVec4(1, 1, 1, 1), float thickness = 1.0f);
    ---@param color ImVec4
    ---@param thickness number
    ---@return boolean
    BeginCreate = function(color, thickness) end,

    ---bool QueryNewLink(PinId* startId, PinId* endId);
    ---@param startId NodeEditorPinId
    ---@param endId NodeEditorPinId
    ---@return boolean
    QueryNewLink = function(startId, endId) end,

    ---bool QueryNewLink(PinId* startId, PinId* endId, const ImVec4& color, float thickness = 1.0f);
    ---@param startId NodeEditorPinId
    ---@param endId NodeEditorPinId
    ---@param color ImVec4
    ---@param thickness number
    ---@return boolean
    QueryNewLink = function(startId, endId, color, thickness) end,

    ---bool QueryNewNode(PinId* pinId);
    ---@param pinId NodeEditorPinId
    ---@return boolean
    QueryNewNode = function(pinId) end,

    ---bool QueryNewNode(PinId* pinId, const ImVec4& color, float thickness = 1.0f);
    ---@param pinId NodeEditorPinId
    ---@param color ImVec4
    ---@param thickness number
    ---@return boolean
    QueryNewNode = function(pinId, color, thickness) end,

    ---bool AcceptNewItem();
    ---@return boolean
    AcceptNewItem = function() end,

    ---bool AcceptNewItem(const ImVec4& color, float thickness = 1.0f);
    ---@param color ImVec4
    ---@param thickness number
    ---@return boolean
    AcceptNewItem = function(color, thickness) end,

    ---void RejectNewItem();
    RejectNewItem = function() end,

    ---void RejectNewItem(const ImVec4& color, float thickness = 1.0f);
    ---@param color ImVec4
    ---@param thickness number
    RejectNewItem = function(color, thickness) end,

    ---void EndCreate();
    EndCreate = function() end,

    ---bool BeginDelete();
    ---@return boolean
    BeginDelete = function() end,

    ---bool QueryDeletedLink(LinkId* linkId, PinId* startId = nullptr, PinId* endId = nullptr);
    ---@param linkId NodeEditorLinkId
    ---@param startId NodeEditorPinId | nil
    ---@param endId NodeEditorPinId | nil
    ---@return boolean
    QueryDeletedLink = function(linkId, startId, endId) end,

    ---bool QueryDeletedNode(NodeId* nodeId);
    ---@param nodeId NodeEditorNodeId
    ---@return boolean
    QueryDeletedNode = function(nodeId) end,

    ---bool AcceptDeletedItem(bool deleteDependencies = true);
    ---@param deleteDependencies boolean
    ---@return boolean
    AcceptDeletedItem = function(deleteDependencies) end,

    ---void RejectDeletedItem();
    RejectDeletedItem = function() end,

    ---void EndDelete();
    EndDelete = function() end,

    ---void SetNodePosition(NodeId nodeId, const ImVec2& editorPosition);
    ---@param nodeId NodeEditorNodeId
    ---@param editorPosition ImVec2
    SetNodePosition = function(nodeId, editorPosition) end,

    ---void SetGroupSize(NodeId nodeId, const ImVec2& size);
    ---@param nodeId NodeEditorNodeId
    ---@param size ImVec2
    SetGroupSize = function(nodeId, size) end,

    ---ImVec2 GetNodePosition(NodeId nodeId);
    ---@param nodeId NodeEditorNodeId
    ---@param out ImVec2
    GetNodePosition = function(nodeId, out) end,

    ---ImVec2 GetNodeSize(NodeId nodeId);
    ---@param nodeId NodeEditorNodeId
    ---@return ImVec2
    GetNodeSize = function(nodeId) end,

    ---void CenterNodeOnScreen(NodeId nodeId);
    ---@param nodeId NodeEditorNodeId
    CenterNodeOnScreen = function(nodeId) end,

    ---void SetNodeZPosition(NodeId nodeId, float z); // Sets node z position, nodes with higher value are drawn over nodes with lower value
    ---@param nodeId NodeEditorNodeId
    ---@param z number
    SetNodeZPosition = function(nodeId, z) end,

    ---float GetNodeZPosition(NodeId nodeId); // Returns node z position, defaults is 0.0f
    ---@param nodeId NodeEditorNodeId
    ---@return number
    GetNodeZPosition = function(nodeId) end,

    ---void RestoreNodeState(NodeId nodeId);
    ---@param nodeId NodeEditorNodeId
    RestoreNodeState = function(nodeId) end,

    ---void Suspend();
    Suspend = function() end,

    ---void Resume();
    Resume = function() end,

    ---bool IsSuspended();
    ---@return boolean
    IsSuspended = function() end,

    ---bool IsActive();
    ---@return boolean
    IsActive = function() end,

    ---bool HasSelectionChanged();
    ---@return boolean
    HasSelectionChanged = function() end,

    ---int  GetSelectedObjectCount();
    ---@return number
    GetSelectedObjectCount = function() end,

    ---int  GetSelectedNodes(NodeId* nodes, int size);
    ---@param nodes NodeEditorNodeId
    ---@param size number
    ---@return number
    GetSelectedNodes = function(nodes, size) end,

    ---int  GetSelectedLinks(LinkId* links, int size);
    ---@param links NodeEditorLinkId
    ---@param size number
    ---@return number
    GetSelectedLinks = function(links, size) end,

    ---bool IsNodeSelected(NodeId nodeId);
    ---@param nodeId NodeEditorNodeId
    ---@return boolean
    IsNodeSelected = function(nodeId) end,

    ---bool IsLinkSelected(LinkId linkId);
    ---@param linkId NodeEditorLinkId
    ---@return boolean
    IsLinkSelected = function(linkId) end,

    ---void ClearSelection();
    ClearSelection = function() end,

    ---void SelectNode(NodeId nodeId, bool append = false);
    ---@param nodeId NodeEditorNodeId
    ---@param append boolean
    SelectNode = function(nodeId, append) end,

    ---void SelectLink(LinkId linkId, bool append = false);
    ---@param linkId NodeEditorLinkId
    ---@param append boolean
    SelectLink = function(linkId, append) end,

    ---void DeselectNode(NodeId nodeId);
    ---@param nodeId NodeEditorNodeId
    DeselectNode = function(nodeId) end,

    ---void DeselectLink(LinkId linkId);
    ---@param linkId NodeEditorLinkId
    DeselectLink = function(linkId) end,

    ---bool DeleteNode(NodeId nodeId);
    ---@param nodeId NodeEditorNodeId
    ---@return boolean
    DeleteNode = function(nodeId) end,

    ---bool DeleteLink(LinkId linkId);
    ---@param linkId NodeEditorLinkId
    ---@return boolean
    DeleteLink = function(linkId) end,

    ---bool HasAnyLinks(NodeId nodeId); // Returns true if node has any link connected
    ---@param nodeId NodeEditorNodeId
    ---@return boolean
    HasAnyLinks = function(nodeId) end,

    ---bool HasAnyLinks(PinId pinId); // Return true if pin has any link connected
    ---@param pinId NodeEditorPinId
    ---@return boolean
    HasAnyLinks = function(pinId) end,

    ---int BreakLinks(NodeId nodeId); // Break all links connected to this node
    ---@param nodeId NodeEditorNodeId
    ---@return number
    BreakLinks = function(nodeId) end,

    ---int BreakLinks(PinId pinId); // Break all links connected to this pin
    ---@param pinId NodeEditorPinId
    ---@return number
    BreakLinks = function(pinId) end,

    ---void NavigateToContent(float duration = -1);
    ---@param duration number
    NavigateToContent = function(duration) end,

    ---void NavigateToSelection(bool zoomIn = false, float duration = -1);
    ---@param zoomIn boolean
    ---@param duration number
    NavigateToSelection = function(zoomIn, duration) end,

    ---bool ShowNodeContextMenu(NodeId* nodeId);
    ---@param nodeId NodeEditorNodeId
    ---@return boolean
    ShowNodeContextMenu = function(nodeId) end,

    ---bool ShowPinContextMenu(PinId* pinId);
    ---@param pinId NodeEditorPinId
    ---@return boolean
    ShowPinContextMenu = function(pinId) end,

    ---bool ShowLinkContextMenu(LinkId* linkId);
    ---@param linkId NodeEditorLinkId
    ---@return boolean
    ShowLinkContextMenu = function(linkId) end,

    ---bool ShowBackgroundContextMenu();
    ---@return boolean
    ShowBackgroundContextMenu = function() end,

    ---void EnableShortcuts(bool enable);
    ---@param enable boolean
    EnableShortcuts = function(enable) end,

    ---bool AreShortcutsEnabled();
    ---@return boolean
    AreShortcutsEnabled = function() end,

    ---bool BeginShortcut();
    ---@return boolean
    BeginShortcut = function() end,

    ---bool AcceptCut();
    ---@return boolean
    AcceptCut = function() end,

    ---bool AcceptCopy();
    ---@return boolean
    AcceptCopy = function() end,

    ---bool AcceptPaste();
    ---@return boolean
    AcceptPaste = function() end,

    ---bool AcceptDuplicate();
    ---@return boolean
    AcceptDuplicate = function() end,

    ---bool AcceptCreateNode();
    ---@return boolean
    AcceptCreateNode = function() end,

    ---int  GetActionContextSize();
    ---@return number
    GetActionContextSize = function() end,

    ---int  GetActionContextNodes(NodeId* nodes, int size);
    ---@param nodes NodeEditorNodeId
    ---@param size number
    ---@return number
    GetActionContextNodes = function(nodes, size) end,

    ---int  GetActionContextLinks(LinkId* links, int size);
    ---@param links NodeEditorLinkId
    ---@param size number
    ---@return number
    GetActionContextLinks = function(links, size) end,

    ---void EndShortcut();
    EndShortcut = function() end,

    ---float GetCurrentZoom();
    ---@return number
    GetCurrentZoom = function() end,

    ---NodeId GetHoveredNode();
    ---@return NodeEditorNodeId
    GetHoveredNode = function() end,

    ---PinId GetHoveredPin();
    ---@return NodeEditorPinId
    GetHoveredPin = function() end,

    ---LinkId GetHoveredLink();
    ---@return NodeEditorLinkId
    GetHoveredLink = function() end,

    ---NodeId GetDoubleClickedNode();
    ---@return NodeEditorNodeId
    GetDoubleClickedNode = function() end,

    ---PinId GetDoubleClickedPin();
    ---@return NodeEditorPinId
    GetDoubleClickedPin = function() end,

    ---LinkId GetDoubleClickedLink();
    ---@return NodeEditorLinkId
    GetDoubleClickedLink = function() end,

    ---bool IsBackgroundClicked();
    ---@return boolean
    IsBackgroundClicked = function() end,

    ---bool IsBackgroundDoubleClicked();
    ---@return boolean
    IsBackgroundDoubleClicked = function() end,

    ---ImGuiMouseButton GetBackgroundClickButtonIndex(); // -1 if none
    ---@return ImGuiMouseButton
    GetBackgroundClickButtonIndex = function() end,

    ---ImGuiMouseButton GetBackgroundDoubleClickButtonIndex(); // -1 if none
    ---@return ImGuiMouseButton
    GetBackgroundDoubleClickButtonIndex = function() end,

    ---bool GetLinkPins(LinkId linkId, PinId* startPinId, PinId* endPinId); // pass nullptr if particular pin do not interest you
    ---@param linkId NodeEditorLinkId
    ---@param startPinId NodeEditorPinId
    ---@param endPinId NodeEditorPinId
    ---@return boolean
    GetLinkPins = function(linkId, startPinId, endPinId) end,

    ---bool PinHadAnyLinks(PinId pinId);
    ---@param pinId NodeEditorPinId
    ---@return boolean
    PinHadAnyLinks = function(pinId) end,

    ---ImVec2 GetScreenSize();
    ---@return ImVec2
    GetScreenSize = function() end,

    ---ImVec2 ScreenToCanvas(const ImVec2& pos);
    ---@param pos ImVec2
    ---@return ImVec2
    ScreenToCanvas = function(pos) end,

    ---ImVec2 CanvasToScreen(const ImVec2& pos);
    ---@param pos ImVec2
    ---@return ImVec2
    CanvasToScreen = function(pos) end,

    ---int GetNodeCount();                                // Returns number of submitted nodes since Begin() call
    ---@return number
    GetNodeCount = function() end,

    ---int GetOrderedNodeIds(NodeId* nodes, int size);    // Fills an array with node id's in order they're drawn; up to 'size` elements are set. Returns actual size of filled id's.
    ---@param nodes NodeEditorNodeId
    ---@param size number
    ---@return number
    GetOrderedNodeIds = function(nodes, size) end,
}

---@param id integer | nil
---@return NodeEditorLinkId
NodeEditor.LinkId = function (id)
end

---@param id integer | nil
---@return NodeEditorNodeId
NodeEditor.NodeId = function (id)
end

---@param id integer | nil
---@return NodeEditorPinId
NodeEditor.PinId = function (id)
end

---@param drawList ImDrawList
---@param a ImVec2
---@param b ImVec2
---@param type integer
---@param filled boolean
---@param color integer
---@param innerColor integer
NodeEditor.DrawIcon = function (drawList, a, b, type, filled, color, innerColor) end

-- void ax::Widgets::Icon(const ImVec2& size, IconType type, bool filled, const ImVec4& color/* = ImVec4(1, 1, 1, 1)*/, const ImVec4& innerColor/* = ImVec4(0, 0, 0, 0)*/)

---@param size ImVec2
---@param type NodeEditorIconType
---@param filled boolean
---@param color ImVec4
---@param innerColor ImVec4
NodeEditor.Icon = function (size, type, filled, color, innerColor) end

---@enum NodeEditorPinKind
NodeEditorPinKind = {
    Input = 0,
    Output = 1,
}

---@enum NodeEditorFlowDirection
NodeEditorFlowDirection = {
    Forward = 0,
    Backward = 1,
}
---@enum NodeEditorIconType
NodeEditorIconType = {
    Flow = 0,
    Circle = 1,
    Square = 2,
    Grid = 3,
    RoundSquare = 4,
    Diamond = 5,
}


---@class NodeEditorConfig
---@field SettingsFile string NodeEditor.json
---@field BeginSaveSession function
---@field EndSaveSession function
---@field SaveSettings function
---@field LoadSettings function
---@field SaveNodeSettings function
---@field LoadNodeSettings function
---@field UserPointer userdata nil
---@field CustomZoomLevels number[]
---@field CanvasSizeMode integer 0
---@field DragButtonIndex integer 0
---@field SelectButtonIndex integer 0
---@field NavigateButtonIndex integer 0
---@field ContextMenuButtonIndex integer 1
---@field EnableSmoothZoom boolean false
---@field SmoothZoomPower number 1.3

NodeEditorConfig = {
    ---@return NodeEditorConfig
    new = function() end,
}

---@class NodeEditorLinkId
---@field bool boolean

---@class NodeEditorLinkId
NodeEditorLinkId = {
    ---@return integer
    Get = function(self) end,
}

---@class NodeEditorNodeId
---@field bool boolean

---@class NodeEditorNodeId
NodeEditorNodeId = {
    ---@return integer
    Get = function(self) end,
}

---@class NodeEditorPinId
---@field bool boolean

---@class NodeEditorPinId
NodeEditorPinId = {
    ---@return integer
    Get = function(self) end,
}

---@class NodeEditorStyle
---@field NodePadding ImVec4
---@field NodeRounding number
---@field NodeBorderWidth number
---@field HoveredNodeBorderWidth number
---@field HoverNodeBorderOffset number
---@field SelectedNodeBorderWidth number
---@field SelectedNodeBorderOffset number
---@field PinRounding number
---@field PinBorderWidth number
---@field LinkStrength number
---@field SourceDirection ImVec2
---@field TargetDirection ImVec2
---@field ScrollDuration number
---@field FlowMarkerDistance number
---@field FlowSpeed number
---@field FlowDuration number
---@field PivotAlignment ImVec2
---@field PivotSize ImVec2
---@field PivotScale ImVec2
---@field PinCorners number
---@field PinRadius number
---@field PinArrowSize number
---@field PinArrowWidth number
---@field GroupRounding number
---@field GroupBorderWidth number
---@field HighlightConnectedLinks number
---@field SnapLinkToPinDir number
---@field Colors ImVec4[]

---@enum NodeEditorStyleColor
NodeEditorStyleColor = {
    Bg = 0,
    Grid = 1,
    NodeBg = 2,
    NodeBorder = 3,
    HovNodeBorder = 4,
    SelNodeBorder = 5,
    NodeSelRect = 6,
    NodeSelRectBorder = 7,
    HovLinkBorder = 8,
    SelLinkBorder = 9,
    HighlightLinkBorder = 10,
    LinkSelRect = 11,
    LinkSelRectBorder = 12,
    PinRect = 13,
    PinRectBorder = 14,
    Flow = 15,
    FlowMarker = 16,
    GroupBg = 17,
    GroupBorder = 18,

    Count = 19,
}

---@enum NodeEditorStyleVar
NodeEditorStyleVar = {
    NodePadding = 0,
    NodeRounding = 1,
    NodeBorderWidth = 2,
    HoveredNodeBorderWidth = 3,
    SelectedNodeBorderWidth = 4,
    PinRounding = 5,
    PinBorderWidth = 6,
    LinkStrength = 7,
    SourceDirection = 8,
    TargetDirection = 9,
    ScrollDuration = 10,
    FlowMarkerDistance = 11,
    FlowSpeed = 12,
    FlowDuration = 13,
    PivotAlignment = 14,
    PivotSize = 15,
    PivotScale = 16,
    PinCorners = 17,
    PinRadius = 18,
    PinArrowSize = 19,
    PinArrowWidth = 20,
    GroupRounding = 21,
    GroupBorderWidth = 22,
    HighlightConnectedLinks = 23,
    SnapLinkToPinDir = 24,
    HovNodeBorderOffset = 25,
    SelNodeBorderOffset = 26,

    Count = 27,
}

---@class NodeEditorStyle
NodeEditorStyle = {
    ---@return NodeEditorStyle
    new = function() end,
}


---@enum BlueprintStage
BlueprintStage = {
    Invalid = 0,
    Begin = 1,
    Header = 2,
    Content = 3,
    Input = 4,
    Output = 5,
    Middle = 6,
    End = 7,
}

---@class NodeEditorContext
---@field Begin fun(self: NodeEditorContext)
---@field End fun(self: NodeEditorContext)
---@field BeginGroup fun(self: NodeEditorContext, id: NodeEditorNodeId)
---@field EndGroup fun(self: NodeEditorContext)
---@field BeginNode fun(self: NodeEditorContext, id: NodeEditorNodeId)
---@field EndNode fun(self: NodeEditorContext

---@class BlueprintNodeBuilder
---@field Begin fun(self: BlueprintNodeBuilder, id: NodeEditorNodeId)
---@field End fun(self: BlueprintNodeBuilder)
---@field Header fun(self: BlueprintNodeBuilder, color: ImVec4)
---@field EndHeader fun(self: BlueprintNodeBuilder)
---@field Input fun(self: BlueprintNodeBuilder, id: NodeEditorPinId)
---@field EndInput fun(self: BlueprintNodeBuilder)
---@field Middle fun(self: BlueprintNodeBuilder)
---@field Output fun(self: BlueprintNodeBuilder, id: NodeEditorPinId)
---@field EndOutput fun(self: BlueprintNodeBuilder)
---@field SetStage fun(self: BlueprintNodeBuilder, stage: BlueprintStage): boolean


BlueprintNodeBuilder = {
    ---@return BlueprintNodeBuilder
    new = function() end,
}