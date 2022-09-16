local NodeEditorUtils = {}

---@enum ImNodesCol
ImNodesCol = {
    NodeBackground = 0,
    NodeBackgroundHovered = 1,
    NodeBackgroundSelected = 2,
    NodeOutline = 3,
    TitleBar = 4,
    TitleBarHovered = 5,
    TitleBarSelected = 6,
    Link = 7,
    LinkHovered = 8,
    LinkSelected = 9,
    Pin = 10,
    PinHovered = 11,
    BoxSelector = 12,
    BoxSelectorOutline = 13,
    GridBackground = 14,
    GridLine = 15,
    GridLinePrimary = 16,
    MiniMapBackground = 17,
    MiniMapBackgroundHovered = 18,
    MiniMapOutline = 19,
    MiniMapOutlineHovered = 20,
    MiniMapNodeBackground = 21,
    MiniMapNodeBackgroundHovered = 22,
    MiniMapNodeBackgroundSelected = 23,
    MiniMapNodeOutline = 24,
    MiniMapLink = 25,
    MiniMapLinkSelected = 26,
    MiniMapCanvas = 27,
    MiniMapCanvasOutline = 28,
    COUNT = 29
}

---@enum ImNodesStyleVar
ImNodesStyleVar = {
    GridSpacing = 0,
    NodeCornerRounding = 1,
    NodePadding =  2,
    NodeBorderThickness = 3,
    LinkThickness = 4,
    LinkLineSegmentsPerLength = 5,
    LinkHoverDistance = 6,
    PinCircleRadius = 7,
    PinQuadSideLength = 8,
    PinTriangleSideLength = 9,
    PinLineThickness = 10,
    PinHoverRadius = 11,
    PinOffset = 12,
    MiniMapPadding = 13,
    MiniMapOffset = 14,
    COUNT = 15
}

---@enum ImNodesPinShape
ImNodesPinShape = {
    Circle = 0,
    CircleFilled = 1,
    Triangle = 2,
    TriangleFilled = 3,
    Quad = 4,
    QuadFilled = 5
}

ICON_FA_FUNCTION = "\xef\x99\xa1";
ICON_FA_ARROW_ALT_FROM_LEFT = "\xef\x8d\x87"
ICON_FA_LOCK = "\xef\x80\xa3"

ImGuiCol_Text = 0;

---@param id integer
---@param name string
---@param type NodeType
NodeEditorUtils.beginNode = function(id, name, type)
    local icon = "";
	local colorTitlebar = 0;
    if type == NodeType.Function then
        icon = ICON_FA_FUNCTION;
		colorTitlebar = IM_COL32(119, 13, 255, 255);
    elseif type == NodeType.Event then
        icon = ICON_FA_ARROW_ALT_FROM_LEFT;
		colorTitlebar = IM_COL32(11, 115, 36, 255);
    elseif type == NodeType.Constant then
        icon = ICON_FA_LOCK;
		colorTitlebar = IM_COL32(176, 83, 2, 255);
    end

    local blend = ImGui.getStyleColorVec4(ImGuiCol_Text) * ImVec4:new(255, 255, 255, 255);
	local selectedColor = ImAlphaBlendColors(colorTitlebar, IM_COL32(blend.x, blend.y, blend.z, 20));
end

return NodeEditorUtils