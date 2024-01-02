global record ImBitVector
    userdata
    Storage: ImVector_ImU32
    Clear: function(self: ImBitVector)
	ClearBit: function(self: ImBitVector, n: integer)
	Create: function(self: ImBitVector, sz: integer)
	SetBit: function(self: ImBitVector, n: integer)
	TestBit: function(self: ImBitVector, n: integer): boolean
end

global record ImColor
    userdata
    Value: ImVec4
    HSV: function(pOut: ImColor, h: number, s: number, v: number, a: number)
	new: function(): ImColor
	new: function(r: number, g: number, b: number, a: number): ImColor
	new: function(col: ImVec4): ImColor
	new: function(r: integer, g: integer, b: integer, a: integer): ImColor
	new: function(rgba: integer): ImColor
	SetHSV: function(self: ImColor, h: number, s: number, v: number, a: number)
	__gc: function(self: ImColor)
end

global record ImDrawChannel
    userdata
    _CmdBuffer: ImVector_ImDrawCmd
	_IdxBuffer: ImVector_ImDrawIdx
    
end

global record ImDrawCmd
    userdata
    ClipRect: ImVec4
	TextureId: any
	VtxOffset: integer
	IdxOffset: integer
	ElemCount: integer
	UserCallback: any
	UserCallbackData: any
    GetTexID: function(self: ImDrawCmd): any
	new: function(): ImDrawCmd
	__gc: function(self: ImDrawCmd)
end

global record ImDrawCmdHeader
    userdata
    ClipRect: ImVec4
	TextureId: any
	VtxOffset: integer
    
end

global record ImDrawData
    userdata
    Valid: boolean
	CmdListsCount: integer
	TotalIdxCount: integer
	TotalVtxCount: integer
	CmdLists: ImVector_ImDrawListPtr
	DisplayPos: ImVec2
	DisplaySize: ImVec2
	FramebufferScale: ImVec2
	OwnerViewport: ImGuiViewport
    AddDrawList: function(self: ImDrawData, draw_list: ImDrawList)
	Clear: function(self: ImDrawData)
	DeIndexAllBuffers: function(self: ImDrawData)
	new: function(): ImDrawData
	ScaleClipRects: function(self: ImDrawData, fb_scale: ImVec2)
	__gc: function(self: ImDrawData)
end

global record ImDrawDataBuilder
    userdata
    Layers: ImVector_ImDrawListPtr
	LayerData1: ImVector_ImDrawListPtr
    new: function(): ImDrawDataBuilder
	__gc: function(self: ImDrawDataBuilder)
end

global record ImDrawList
    userdata
    CmdBuffer: ImVector_ImDrawCmd
	IdxBuffer: ImVector_ImDrawIdx
	VtxBuffer: ImVector_ImDrawVert
	Flags: integer
	_VtxCurrentIdx: integer
	_Data: ImDrawListSharedData
	_OwnerName: string
	_VtxWritePtr: ImDrawVert
	_IdxWritePtr: any
	_ClipRectStack: ImVector_ImVec4
	_TextureIdStack: ImVector_ImTextureID
	_Path: ImVector_ImVec2
	_CmdHeader: ImDrawCmdHeader
	_Splitter: ImDrawListSplitter
	_FringeScale: number
    AddBezierCubic: function(self: ImDrawList, p1: ImVec2, p2: ImVec2, p3: ImVec2, p4: ImVec2, col: integer, thickness: number, num_segments: integer)
	AddBezierQuadratic: function(self: ImDrawList, p1: ImVec2, p2: ImVec2, p3: ImVec2, col: integer, thickness: number, num_segments: integer)
	AddCallback: function(self: ImDrawList, callback: any, callback_data: any)
	AddCircle: function(self: ImDrawList, center: ImVec2, radius: number, col: integer, num_segments: integer, thickness: number)
	AddCircleFilled: function(self: ImDrawList, center: ImVec2, radius: number, col: integer, num_segments: integer)
	AddConvexPolyFilled: function(self: ImDrawList, points: ImVec2, num_points: integer, col: integer)
	AddDrawCmd: function(self: ImDrawList)
	AddImage: function(self: ImDrawList, user_texture_id: any, p_min: ImVec2, p_max: ImVec2, uv_min: ImVec2, uv_max: ImVec2, col: integer)
	AddImageQuad: function(self: ImDrawList, user_texture_id: any, p1: ImVec2, p2: ImVec2, p3: ImVec2, p4: ImVec2, uv1: ImVec2, uv2: ImVec2, uv3: ImVec2, uv4: ImVec2, col: integer)
	AddImageRounded: function(self: ImDrawList, user_texture_id: any, p_min: ImVec2, p_max: ImVec2, uv_min: ImVec2, uv_max: ImVec2, col: integer, rounding: number, flags: integer)
	AddLine: function(self: ImDrawList, p1: ImVec2, p2: ImVec2, col: integer, thickness: number)
	AddNgon: function(self: ImDrawList, center: ImVec2, radius: number, col: integer, num_segments: integer, thickness: number)
	AddNgonFilled: function(self: ImDrawList, center: ImVec2, radius: number, col: integer, num_segments: integer)
	AddPolyline: function(self: ImDrawList, points: ImVec2, num_points: integer, col: integer, flags: integer, thickness: number)
	AddQuad: function(self: ImDrawList, p1: ImVec2, p2: ImVec2, p3: ImVec2, p4: ImVec2, col: integer, thickness: number)
	AddQuadFilled: function(self: ImDrawList, p1: ImVec2, p2: ImVec2, p3: ImVec2, p4: ImVec2, col: integer)
	AddRect: function(self: ImDrawList, p_min: ImVec2, p_max: ImVec2, col: integer, rounding: number, flags: integer, thickness: number)
	AddRectFilled: function(self: ImDrawList, p_min: ImVec2, p_max: ImVec2, col: integer, rounding: number, flags: integer)
	AddRectFilledMultiColor: function(self: ImDrawList, p_min: ImVec2, p_max: ImVec2, col_upr_left: integer, col_upr_right: integer, col_bot_right: integer, col_bot_left: integer)
	AddText: function(self: ImDrawList, pos: ImVec2, col: integer, text_begin: string, text_end: string)
	AddText: function(self: ImDrawList, font: ImFont, font_size: number, pos: ImVec2, col: integer, text_begin: string, text_end: string, wrap_width: number, cpu_fine_clip_rect: ImVec4)
	AddTriangle: function(self: ImDrawList, p1: ImVec2, p2: ImVec2, p3: ImVec2, col: integer, thickness: number)
	AddTriangleFilled: function(self: ImDrawList, p1: ImVec2, p2: ImVec2, p3: ImVec2, col: integer)
	ChannelsMerge: function(self: ImDrawList)
	ChannelsSetCurrent: function(self: ImDrawList, n: integer)
	ChannelsSplit: function(self: ImDrawList, count: integer)
	CloneOutput: function(self: ImDrawList): ImDrawList
	GetClipRectMax: function(pOut: ImVec2, self: ImDrawList)
	GetClipRectMin: function(pOut: ImVec2, self: ImDrawList)
	new: function(shared_data: ImDrawListSharedData): ImDrawList
	PathArcTo: function(self: ImDrawList, center: ImVec2, radius: number, a_min: number, a_max: number, num_segments: integer)
	PathArcToFast: function(self: ImDrawList, center: ImVec2, radius: number, a_min_of_12: integer, a_max_of_12: integer)
	PathBezierCubicCurveTo: function(self: ImDrawList, p2: ImVec2, p3: ImVec2, p4: ImVec2, num_segments: integer)
	PathBezierQuadraticCurveTo: function(self: ImDrawList, p2: ImVec2, p3: ImVec2, num_segments: integer)
	PathClear: function(self: ImDrawList)
	PathFillConvex: function(self: ImDrawList, col: integer)
	PathLineTo: function(self: ImDrawList, pos: ImVec2)
	PathLineToMergeDuplicate: function(self: ImDrawList, pos: ImVec2)
	PathRect: function(self: ImDrawList, rect_min: ImVec2, rect_max: ImVec2, rounding: number, flags: integer)
	PathStroke: function(self: ImDrawList, col: integer, flags: integer, thickness: number)
	PopClipRect: function(self: ImDrawList)
	PopTextureID: function(self: ImDrawList)
	PrimQuadUV: function(self: ImDrawList, a: ImVec2, b: ImVec2, c: ImVec2, d: ImVec2, uv_a: ImVec2, uv_b: ImVec2, uv_c: ImVec2, uv_d: ImVec2, col: integer)
	PrimRect: function(self: ImDrawList, a: ImVec2, b: ImVec2, col: integer)
	PrimRectUV: function(self: ImDrawList, a: ImVec2, b: ImVec2, uv_a: ImVec2, uv_b: ImVec2, col: integer)
	PrimReserve: function(self: ImDrawList, idx_count: integer, vtx_count: integer)
	PrimUnreserve: function(self: ImDrawList, idx_count: integer, vtx_count: integer)
	PrimVtx: function(self: ImDrawList, pos: ImVec2, uv: ImVec2, col: integer)
	PrimWriteIdx: function(self: ImDrawList, idx: any)
	PrimWriteVtx: function(self: ImDrawList, pos: ImVec2, uv: ImVec2, col: integer)
	PushClipRect: function(self: ImDrawList, clip_rect_min: ImVec2, clip_rect_max: ImVec2, intersect_with_current_clip_rect: boolean)
	PushClipRectFullScreen: function(self: ImDrawList)
	PushTextureID: function(self: ImDrawList, texture_id: any)
	_CalcCircleAutoSegmentCount: function(self: ImDrawList, radius: number): integer
	_ClearFreeMemory: function(self: ImDrawList)
	_OnChangedClipRect: function(self: ImDrawList)
	_OnChangedTextureID: function(self: ImDrawList)
	_OnChangedVtxOffset: function(self: ImDrawList)
	_PathArcToFastEx: function(self: ImDrawList, center: ImVec2, radius: number, a_min_sample: integer, a_max_sample: integer, a_step: integer)
	_PathArcToN: function(self: ImDrawList, center: ImVec2, radius: number, a_min: number, a_max: number, num_segments: integer)
	_PopUnusedDrawCmd: function(self: ImDrawList)
	_ResetForNewFrame: function(self: ImDrawList)
	_TryMergeDrawCmds: function(self: ImDrawList)
	__gc: function(self: ImDrawList)
end

global record ImDrawListSharedData
    userdata
    TexUvWhitePixel: ImVec2
	Font: ImFont
	FontSize: number
	CurveTessellationTol: number
	CircleSegmentMaxError: number
	ClipRectFullscreen: ImVec4
	InitialFlags: integer
	TempBuffer: ImVector_ImVec2
	ArcFastVtx: ImVec2
	ArcFastRadiusCutoff: number
	CircleSegmentCounts: any
	TexUvLines: ImVec4
    new: function(): ImDrawListSharedData
	SetCircleTessellationMaxError: function(self: ImDrawListSharedData, max_error: number)
	__gc: function(self: ImDrawListSharedData)
end

global record ImDrawListSplitter
    userdata
    _Current: integer
	_Count: integer
	_Channels: ImVector_ImDrawChannel
    Clear: function(self: ImDrawListSplitter)
	ClearFreeMemory: function(self: ImDrawListSplitter)
	new: function(): ImDrawListSplitter
	Merge: function(self: ImDrawListSplitter, draw_list: ImDrawList)
	SetCurrentChannel: function(self: ImDrawListSplitter, draw_list: ImDrawList, channel_idx: integer)
	Split: function(self: ImDrawListSplitter, draw_list: ImDrawList, count: integer)
	__gc: function(self: ImDrawListSplitter)
end

global record ImDrawVert
    userdata
    pos: ImVec2
	uv: ImVec2
	col: integer
    
end

global record ImFont
    userdata
    IndexAdvanceX: ImVector_float
	FallbackAdvanceX: number
	FontSize: number
	IndexLookup: ImVector_ImWchar
	Glyphs: ImVector_ImFontGlyph
	FallbackGlyph: ImFontGlyph
	ContainerAtlas: ImFontAtlas
	ConfigData: ImFontConfig
	ConfigDataCount: integer
	FallbackChar: any
	EllipsisChar: any
	EllipsisCharCount: integer
	EllipsisWidth: number
	EllipsisCharStep: number
	DirtyLookupTables: boolean
	Scale: number
	Ascent: number
	Descent: number
	MetricsTotalSurface: integer
	Used4kPagesMap: any
    AddGlyph: function(self: ImFont, src_cfg: ImFontConfig, c: any, x0: number, y0: number, x1: number, y1: number, u0: number, v0: number, u1: number, v1: number, advance_x: number)
	AddRemapChar: function(self: ImFont, dst: any, src: any, overwrite_dst: boolean)
	BuildLookupTable: function(self: ImFont)
	CalcTextSizeA: function(pOut: ImVec2, self: ImFont, size: number, max_width: number, wrap_width: number, text_begin: string, text_end: string, remaining: any)
	CalcWordWrapPositionA: function(self: ImFont, scale: number, text: string, text_end: string, wrap_width: number): string
	ClearOutputData: function(self: ImFont)
	FindGlyph: function(self: ImFont, c: any): ImFontGlyph
	FindGlyphNoFallback: function(self: ImFont, c: any): ImFontGlyph
	GetCharAdvance: function(self: ImFont, c: any): number
	GetDebugName: function(self: ImFont): string
	GrowIndex: function(self: ImFont, new_size: integer)
	new: function(): ImFont
	IsGlyphRangeUnused: function(self: ImFont, c_begin: integer, c_last: integer): boolean
	IsLoaded: function(self: ImFont): boolean
	RenderChar: function(self: ImFont, draw_list: ImDrawList, size: number, pos: ImVec2, col: integer, c: any)
	RenderText: function(self: ImFont, draw_list: ImDrawList, size: number, pos: ImVec2, col: integer, clip_rect: ImVec4, text_begin: string, text_end: string, wrap_width: number, cpu_fine_clip: boolean)
	SetGlyphVisible: function(self: ImFont, c: any, visible: boolean)
	__gc: function(self: ImFont)
end

global record ImFontAtlas
    userdata
    Flags: integer
	TexID: any
	TexDesiredWidth: integer
	TexGlyphPadding: integer
	Locked: boolean
	UserData: any
	TexReady: boolean
	TexPixelsUseColors: boolean
	TexPixelsAlpha8: any
	TexPixelsRGBA32: any
	TexWidth: integer
	TexHeight: integer
	TexUvScale: ImVec2
	TexUvWhitePixel: ImVec2
	Fonts: ImVector_ImFontPtr
	CustomRects: ImVector_ImFontAtlasCustomRect
	ConfigData: ImVector_ImFontConfig
	TexUvLines: ImVec4
	FontBuilderIO: ImFontBuilderIO
	FontBuilderFlags: integer
	PackIdMouseCursors: integer
	PackIdLines: integer
    AddCustomRectFontGlyph: function(self: ImFontAtlas, font: ImFont, id: any, width: integer, height: integer, advance_x: number, offset: ImVec2): integer
	AddCustomRectRegular: function(self: ImFontAtlas, width: integer, height: integer): integer
	AddFont: function(self: ImFontAtlas, font_cfg: ImFontConfig): ImFont
	AddFontDefault: function(self: ImFontAtlas, font_cfg: ImFontConfig): ImFont
	AddFontFromFileTTF: function(self: ImFontAtlas, filename: string, size_pixels: number, font_cfg: ImFontConfig, glyph_ranges: any): ImFont
	AddFontFromMemoryCompressedBase85TTF: function(self: ImFontAtlas, compressed_font_data_base85: string, size_pixels: number, font_cfg: ImFontConfig, glyph_ranges: any): ImFont
	AddFontFromMemoryCompressedTTF: function(self: ImFontAtlas, compressed_font_data: any, compressed_font_size: integer, size_pixels: number, font_cfg: ImFontConfig, glyph_ranges: any): ImFont
	AddFontFromMemoryTTF: function(self: ImFontAtlas, font_data: any, font_size: integer, size_pixels: number, font_cfg: ImFontConfig, glyph_ranges: any): ImFont
	Build: function(self: ImFontAtlas): boolean
	CalcCustomRectUV: function(self: ImFontAtlas, rect: ImFontAtlasCustomRect, out_uv_min: ImVec2, out_uv_max: ImVec2)
	Clear: function(self: ImFontAtlas)
	ClearFonts: function(self: ImFontAtlas)
	ClearInputData: function(self: ImFontAtlas)
	ClearTexData: function(self: ImFontAtlas)
	GetCustomRectByIndex: function(self: ImFontAtlas, index: integer): ImFontAtlasCustomRect
	GetGlyphRangesChineseFull: function(self: ImFontAtlas): any
	GetGlyphRangesChineseSimplifiedCommon: function(self: ImFontAtlas): any
	GetGlyphRangesCyrillic: function(self: ImFontAtlas): any
	GetGlyphRangesDefault: function(self: ImFontAtlas): any
	GetGlyphRangesGreek: function(self: ImFontAtlas): any
	GetGlyphRangesJapanese: function(self: ImFontAtlas): any
	GetGlyphRangesKorean: function(self: ImFontAtlas): any
	GetGlyphRangesThai: function(self: ImFontAtlas): any
	GetGlyphRangesVietnamese: function(self: ImFontAtlas): any
	GetMouseCursorTexData: function(self: ImFontAtlas, cursor: integer, out_offset: ImVec2, out_size: ImVec2, out_uv_border: ImVec2, out_uv_fill: ImVec2): boolean
	GetTexDataAsAlpha8: function(self: ImFontAtlas, out_width: integer, out_height: integer, out_bytes_per_pixel: integer): any, integer, integer, integer
	GetTexDataAsRGBA32: function(self: ImFontAtlas, out_width: integer, out_height: integer, out_bytes_per_pixel: integer): any, integer, integer, integer
	new: function(): ImFontAtlas
	IsBuilt: function(self: ImFontAtlas): boolean
	SetTexID: function(self: ImFontAtlas, id: any)
	__gc: function(self: ImFontAtlas)
end

global record ImFontAtlasCustomRect
    userdata
    Width: integer
	Height: integer
	X: integer
	Y: integer
	GlyphID: integer
	GlyphAdvanceX: number
	GlyphOffset: ImVec2
	Font: ImFont
    new: function(): ImFontAtlasCustomRect
	IsPacked: function(self: ImFontAtlasCustomRect): boolean
	__gc: function(self: ImFontAtlasCustomRect)
end

global record ImFontBuilderIO
    userdata
    FontBuilder_Build: any
    
end

global record ImFontConfig
    userdata
    FontData: any
	FontDataSize: integer
	FontDataOwnedByAtlas: boolean
	FontNo: integer
	SizePixels: number
	OversampleH: integer
	OversampleV: integer
	PixelSnapH: boolean
	GlyphExtraSpacing: ImVec2
	GlyphOffset: ImVec2
	GlyphRanges: any
	GlyphMinAdvanceX: number
	GlyphMaxAdvanceX: number
	MergeMode: boolean
	FontBuilderFlags: integer
	RasterizerMultiply: number
	EllipsisChar: any
	Name: any
	DstFont: ImFont
    new: function(): ImFontConfig
	__gc: function(self: ImFontConfig)
end

global record ImFontGlyph
    userdata
    Colored: integer
	Visible: integer
	Codepoint: integer
	AdvanceX: number
	X0: number
	Y0: number
	X1: number
	Y1: number
	U0: number
	V0: number
	U1: number
	V1: number
    
end

global record ImFontGlyphRangesBuilder
    userdata
    UsedChars: ImVector_ImU32
    AddChar: function(self: ImFontGlyphRangesBuilder, c: any)
	AddRanges: function(self: ImFontGlyphRangesBuilder, ranges: any)
	AddText: function(self: ImFontGlyphRangesBuilder, text: string, text_end: string)
	BuildRanges: function(self: ImFontGlyphRangesBuilder, out_ranges: ImVector_ImWchar)
	Clear: function(self: ImFontGlyphRangesBuilder)
	GetBit: function(self: ImFontGlyphRangesBuilder, n: any): boolean
	new: function(): ImFontGlyphRangesBuilder
	SetBit: function(self: ImFontGlyphRangesBuilder, n: any)
	__gc: function(self: ImFontGlyphRangesBuilder)
end

global record ImGuiColorMod
    userdata
    Col: integer
	BackupValue: ImVec4
    
end

global record ImGuiComboPreviewData
    userdata
    PreviewRect: ImRect
	BackupCursorPos: ImVec2
	BackupCursorMaxPos: ImVec2
	BackupCursorPosPrevLine: ImVec2
	BackupPrevLineTextBaseOffset: number
	BackupLayout: integer
    new: function(): ImGuiComboPreviewData
	__gc: function(self: ImGuiComboPreviewData)
end

global record ImGuiContext
    userdata
    Initialized: boolean
	FontAtlasOwnedByContext: boolean
	IO: ImGuiIO
	Style: ImGuiStyle
	Font: ImFont
	FontSize: number
	FontBaseSize: number
	DrawListSharedData: ImDrawListSharedData
	Time: number
	FrameCount: integer
	FrameCountEnded: integer
	FrameCountRendered: integer
	WithinFrameScope: boolean
	WithinFrameScopeWithImplicitWindow: boolean
	WithinEndChild: boolean
	GcCompactAll: boolean
	TestEngineHookItems: boolean
	TestEngine: any
	InputEventsQueue: ImVector_ImGuiInputEvent
	InputEventsTrail: ImVector_ImGuiInputEvent
	InputEventsNextMouseSource: any
	InputEventsNextEventId: integer
	Windows: ImVector_ImGuiWindowPtr
	WindowsFocusOrder: ImVector_ImGuiWindowPtr
	WindowsTempSortBuffer: ImVector_ImGuiWindowPtr
	CurrentWindowStack: ImVector_ImGuiWindowStackData
	WindowsById: ImGuiStorage
	WindowsActiveCount: integer
	WindowsHoverPadding: ImVec2
	CurrentWindow: ImGuiWindow
	HoveredWindow: ImGuiWindow
	HoveredWindowUnderMovingWindow: ImGuiWindow
	MovingWindow: ImGuiWindow
	WheelingWindow: ImGuiWindow
	WheelingWindowRefMousePos: ImVec2
	WheelingWindowStartFrame: integer
	WheelingWindowReleaseTimer: number
	WheelingWindowWheelRemainder: ImVec2
	WheelingAxisAvg: ImVec2
	DebugHookIdInfo: any
	HoveredId: any
	HoveredIdPreviousFrame: any
	HoveredIdAllowOverlap: boolean
	HoveredIdDisabled: boolean
	HoveredIdTimer: number
	HoveredIdNotActiveTimer: number
	ActiveId: any
	ActiveIdIsAlive: any
	ActiveIdTimer: number
	ActiveIdIsJustActivated: boolean
	ActiveIdAllowOverlap: boolean
	ActiveIdNoClearOnFocusLoss: boolean
	ActiveIdHasBeenPressedBefore: boolean
	ActiveIdHasBeenEditedBefore: boolean
	ActiveIdHasBeenEditedThisFrame: boolean
	ActiveIdClickOffset: ImVec2
	ActiveIdWindow: ImGuiWindow
	ActiveIdSource: any
	ActiveIdMouseButton: integer
	ActiveIdPreviousFrame: any
	ActiveIdPreviousFrameIsAlive: boolean
	ActiveIdPreviousFrameHasBeenEditedBefore: boolean
	ActiveIdPreviousFrameWindow: ImGuiWindow
	LastActiveId: any
	LastActiveIdTimer: number
	KeysOwnerData: ImGuiKeyOwnerData
	KeysRoutingTable: ImGuiKeyRoutingTable
	ActiveIdUsingNavDirMask: integer
	ActiveIdUsingAllKeyboardKeys: boolean
	ActiveIdUsingNavInputMask: integer
	CurrentFocusScopeId: any
	CurrentItemFlags: integer
	DebugLocateId: any
	NextItemData: ImGuiNextItemData
	LastItemData: ImGuiLastItemData
	NextWindowData: ImGuiNextWindowData
	ColorStack: ImVector_ImGuiColorMod
	StyleVarStack: ImVector_ImGuiStyleMod
	FontStack: ImVector_ImFontPtr
	FocusScopeStack: ImVector_ImGuiID
	ItemFlagsStack: ImVector_ImGuiItemFlags
	GroupStack: ImVector_ImGuiGroupData
	OpenPopupStack: ImVector_ImGuiPopupData
	BeginPopupStack: ImVector_ImGuiPopupData
	NavTreeNodeStack: ImVector_ImGuiNavTreeNodeData
	BeginMenuCount: integer
	Viewports: ImVector_ImGuiViewportPPtr
	NavWindow: ImGuiWindow
	NavId: any
	NavFocusScopeId: any
	NavActivateId: any
	NavActivateDownId: any
	NavActivatePressedId: any
	NavActivateFlags: integer
	NavJustMovedToId: any
	NavJustMovedToFocusScopeId: any
	NavJustMovedToKeyMods: any
	NavNextActivateId: any
	NavNextActivateFlags: integer
	NavInputSource: any
	NavLayer: any
	NavIdIsAlive: boolean
	NavMousePosDirty: boolean
	NavDisableHighlight: boolean
	NavDisableMouseHover: boolean
	NavAnyRequest: boolean
	NavInitRequest: boolean
	NavInitRequestFromMove: boolean
	NavInitResult: ImGuiNavItemData
	NavMoveSubmitted: boolean
	NavMoveScoringItems: boolean
	NavMoveForwardToNextFrame: boolean
	NavMoveFlags: integer
	NavMoveScrollFlags: integer
	NavMoveKeyMods: any
	NavMoveDir: integer
	NavMoveDirForDebug: integer
	NavMoveClipDir: integer
	NavScoringRect: ImRect
	NavScoringNoClipRect: ImRect
	NavScoringDebugCount: integer
	NavTabbingDir: integer
	NavTabbingCounter: integer
	NavMoveResultLocal: ImGuiNavItemData
	NavMoveResultLocalVisible: ImGuiNavItemData
	NavMoveResultOther: ImGuiNavItemData
	NavTabbingResultFirst: ImGuiNavItemData
	ConfigNavWindowingKeyNext: any
	ConfigNavWindowingKeyPrev: any
	NavWindowingTarget: ImGuiWindow
	NavWindowingTargetAnim: ImGuiWindow
	NavWindowingListWindow: ImGuiWindow
	NavWindowingTimer: number
	NavWindowingHighlightAlpha: number
	NavWindowingToggleLayer: boolean
	NavWindowingAccumDeltaPos: ImVec2
	NavWindowingAccumDeltaSize: ImVec2
	DimBgRatio: number
	DragDropActive: boolean
	DragDropWithinSource: boolean
	DragDropWithinTarget: boolean
	DragDropSourceFlags: integer
	DragDropSourceFrameCount: integer
	DragDropMouseButton: integer
	DragDropPayload: ImGuiPayload
	DragDropTargetRect: ImRect
	DragDropTargetId: any
	DragDropAcceptFlags: integer
	DragDropAcceptIdCurrRectSurface: number
	DragDropAcceptIdCurr: any
	DragDropAcceptIdPrev: any
	DragDropAcceptFrameCount: integer
	DragDropHoldJustPressedId: any
	DragDropPayloadBufHeap: ImVector_unsigned_char
	DragDropPayloadBufLocal: integer
	ClipperTempDataStacked: integer
	ClipperTempData: ImVector_ImGuiListClipperData
	CurrentTable: ImGuiTable
	TablesTempDataStacked: integer
	TablesTempData: ImVector_ImGuiTableTempData
	Tables: any
	TablesLastTimeActive: ImVector_float
	DrawChannelsTempMergeBuffer: ImVector_ImDrawChannel
	CurrentTabBar: ImGuiTabBar
	TabBars: any
	CurrentTabBarStack: ImVector_ImGuiPtrOrIndex
	ShrinkWidthBuffer: ImVector_ImGuiShrinkWidthItem
	HoverItemDelayId: any
	HoverItemDelayIdPreviousFrame: any
	HoverItemDelayTimer: number
	HoverItemDelayClearTimer: number
	HoverItemUnlockedStationaryId: any
	HoverWindowUnlockedStationaryId: any
	MouseCursor: integer
	MouseStationaryTimer: number
	MouseLastValidPos: ImVec2
	InputTextState: ImGuiInputTextState
	InputTextDeactivatedState: ImGuiInputTextDeactivatedState
	InputTextPasswordFont: ImFont
	TempInputId: any
	ColorEditOptions: integer
	ColorEditCurrentID: any
	ColorEditSavedID: any
	ColorEditSavedHue: number
	ColorEditSavedSat: number
	ColorEditSavedColor: integer
	ColorPickerRef: ImVec4
	ComboPreviewData: ImGuiComboPreviewData
	SliderGrabClickOffset: number
	SliderCurrentAccum: number
	SliderCurrentAccumDirty: boolean
	DragCurrentAccumDirty: boolean
	DragCurrentAccum: number
	DragSpeedDefaultRatio: number
	ScrollbarClickDeltaToGrabCenter: number
	DisabledAlphaBackup: number
	DisabledStackSize: integer
	LockMarkEdited: integer
	TooltipOverrideCount: integer
	ClipboardHandlerData: ImVector_char
	MenusIdSubmittedThisFrame: ImVector_ImGuiID
	PlatformImeData: ImGuiPlatformImeData
	PlatformImeDataPrev: ImGuiPlatformImeData
	SettingsLoaded: boolean
	SettingsDirtyTimer: number
	SettingsIniData: ImGuiTextBuffer
	SettingsHandlers: ImVector_ImGuiSettingsHandler
	SettingsWindows: any
	SettingsTables: any
	Hooks: ImVector_ImGuiContextHook
	HookIdNext: any
	LocalizationTable: string
	LogEnabled: boolean
	LogType: any
	LogFile: any
	LogBuffer: ImGuiTextBuffer
	LogNextPrefix: string
	LogNextSuffix: string
	LogLinePosY: number
	LogLineFirstItem: boolean
	LogDepthRef: integer
	LogDepthToExpand: integer
	LogDepthToExpandDefault: integer
	DebugLogFlags: integer
	DebugLogBuf: ImGuiTextBuffer
	DebugLogIndex: ImGuiTextIndex
	DebugLogClipperAutoDisableFrames: any
	DebugLocateFrames: any
	DebugBeginReturnValueCullDepth: any
	DebugItemPickerActive: boolean
	DebugItemPickerMouseButton: any
	DebugItemPickerBreakId: any
	DebugMetricsConfig: ImGuiMetricsConfig
	DebugStackTool: ImGuiStackTool
	FramerateSecPerFrame: number
	FramerateSecPerFrameIdx: integer
	FramerateSecPerFrameCount: integer
	FramerateSecPerFrameAccum: number
	WantCaptureMouseNextFrame: integer
	WantCaptureKeyboardNextFrame: integer
	WantTextInputNextFrame: integer
	TempBuffer: ImVector_char
    new: function(shared_font_atlas: ImFontAtlas): ImGuiContext
	__gc: function(self: ImGuiContext)
end

global record ImGuiContextHook
    userdata
    HookId: any
	Type: any
	Owner: any
	Callback: any
	UserData: any
    new: function(): ImGuiContextHook
	__gc: function(self: ImGuiContextHook)
end

global record ImGuiDataTypeInfo
    userdata
    Size: any
	Name: string
	PrintFmt: string
	ScanFmt: string
    
end

global record ImGuiDataTypeTempStorage
    userdata
    Data: any
    
end

global record ImGuiDataVarInfo
    userdata
    Type: integer
	Count: integer
	Offset: integer
    GetVarPtr: function(self: ImGuiDataVarInfo, parent: any): any
end

global record ImGuiGroupData
    userdata
    WindowID: any
	BackupCursorPos: ImVec2
	BackupCursorMaxPos: ImVec2
	BackupIndent: ImVec1
	BackupGroupOffset: ImVec1
	BackupCurrLineSize: ImVec2
	BackupCurrLineTextBaseOffset: number
	BackupActiveIdIsAlive: any
	BackupActiveIdPreviousFrameIsAlive: boolean
	BackupHoveredIdIsAlive: boolean
	EmitItem: boolean
    
end

global record ImGuiIO
    userdata
    ConfigFlags: integer
	BackendFlags: integer
	DisplaySize: ImVec2
	DeltaTime: number
	IniSavingRate: number
	IniFilename: string
	LogFilename: string
	UserData: any
	Fonts: ImFontAtlas
	FontGlobalScale: number
	FontAllowUserScaling: boolean
	FontDefault: ImFont
	DisplayFramebufferScale: ImVec2
	MouseDrawCursor: boolean
	ConfigMacOSXBehaviors: boolean
	ConfigInputTrickleEventQueue: boolean
	ConfigInputTextCursorBlink: boolean
	ConfigInputTextEnterKeepActive: boolean
	ConfigDragClickToInputText: boolean
	ConfigWindowsResizeFromEdges: boolean
	ConfigWindowsMoveFromTitleBarOnly: boolean
	ConfigMemoryCompactTimer: number
	MouseDoubleClickTime: number
	MouseDoubleClickMaxDist: number
	MouseDragThreshold: number
	KeyRepeatDelay: number
	KeyRepeatRate: number
	ConfigDebugBeginReturnValueOnce: boolean
	ConfigDebugBeginReturnValueLoop: boolean
	ConfigDebugIgnoreFocusLoss: boolean
	ConfigDebugIniSettings: boolean
	BackendPlatformName: string
	BackendRendererName: string
	BackendPlatformUserData: any
	BackendRendererUserData: any
	BackendLanguageUserData: any
	GetClipboardTextFn: any
	SetClipboardTextFn: any
	ClipboardUserData: any
	SetPlatformImeDataFn: any
	_UnusedPadding: any
	PlatformLocaleDecimalPoint: any
	WantCaptureMouse: boolean
	WantCaptureKeyboard: boolean
	WantTextInput: boolean
	WantSetMousePos: boolean
	WantSaveIniSettings: boolean
	NavActive: boolean
	NavVisible: boolean
	Framerate: number
	MetricsRenderVertices: integer
	MetricsRenderIndices: integer
	MetricsRenderWindows: integer
	MetricsActiveWindows: integer
	MetricsActiveAllocations: integer
	MouseDelta: ImVec2
	KeyMap: integer
	KeysDown: boolean
	NavInputs: number
	Ctx: ImGuiContext
	MousePos: ImVec2
	MouseDown: boolean
	MouseWheel: number
	MouseWheelH: number
	MouseSource: any
	KeyCtrl: boolean
	KeyShift: boolean
	KeyAlt: boolean
	KeySuper: boolean
	KeyMods: any
	KeysData: ImGuiKeyData
	WantCaptureMouseUnlessPopupClose: boolean
	MousePosPrev: ImVec2
	MouseClickedPos: ImVec2
	MouseClickedTime: number
	MouseClicked: boolean
	MouseDoubleClicked: boolean
	MouseClickedCount: any
	MouseClickedLastCount: any
	MouseReleased: boolean
	MouseDownOwned: boolean
	MouseDownOwnedUnlessPopupClose: boolean
	MouseWheelRequestAxisSwap: boolean
	MouseDownDuration: number
	MouseDownDurationPrev: number
	MouseDragMaxDistanceSqr: number
	PenPressure: number
	AppFocusLost: boolean
	AppAcceptingEvents: boolean
	BackendUsingLegacyKeyArrays: any
	BackendUsingLegacyNavInputArray: boolean
	InputQueueSurrogate: any
	InputQueueCharacters: ImVector_ImWchar
    AddFocusEvent: function(self: ImGuiIO, focused: boolean)
	AddInputCharacter: function(self: ImGuiIO, c: integer)
	AddInputCharacterUTF16: function(self: ImGuiIO, c: any)
	AddInputCharactersUTF8: function(self: ImGuiIO, str: string)
	AddKeyAnalogEvent: function(self: ImGuiIO, key: any, down: boolean, v: number)
	AddKeyEvent: function(self: ImGuiIO, key: any, down: boolean)
	AddMouseButtonEvent: function(self: ImGuiIO, button: integer, down: boolean)
	AddMousePosEvent: function(self: ImGuiIO, x: number, y: number)
	AddMouseSourceEvent: function(self: ImGuiIO, source: any)
	AddMouseWheelEvent: function(self: ImGuiIO, wheel_x: number, wheel_y: number)
	ClearEventsQueue: function(self: ImGuiIO)
	ClearInputKeys: function(self: ImGuiIO)
	new: function(): ImGuiIO
	SetAppAcceptingEvents: function(self: ImGuiIO, accepting_events: boolean)
	SetKeyEventNativeData: function(self: ImGuiIO, key: any, native_keycode: integer, native_scancode: integer, native_legacy_index: integer)
	__gc: function(self: ImGuiIO)
end

global record ImGuiInputEvent
    userdata
    Type: any
	Source: any
	EventId: integer
	MousePos: ImGuiInputEventMousePos
	MouseWheel: ImGuiInputEventMouseWheel
	MouseButton: ImGuiInputEventMouseButton
	Key: ImGuiInputEventKey
	Text: ImGuiInputEventText
	AppFocused: ImGuiInputEventAppFocused
	AddedByTestEngine: boolean
    new: function(): ImGuiInputEvent
	__gc: function(self: ImGuiInputEvent)
end

global record ImGuiInputEventAppFocused
    userdata
    Focused: boolean
    
end

global record ImGuiInputEventKey
    userdata
    Key: any
	Down: boolean
	AnalogValue: number
    
end

global record ImGuiInputEventMouseButton
    userdata
    Button: integer
	Down: boolean
	MouseSource: any
    
end

global record ImGuiInputEventMousePos
    userdata
    PosX: number
	PosY: number
	MouseSource: any
    
end

global record ImGuiInputEventMouseWheel
    userdata
    WheelX: number
	WheelY: number
	MouseSource: any
    
end

global record ImGuiInputEventText
    userdata
    Char: integer
    
end

global record ImGuiInputTextCallbackData
    userdata
    Ctx: ImGuiContext
	EventFlag: integer
	Flags: integer
	UserData: any
	EventChar: any
	EventKey: any
	Buf: any
	BufTextLen: integer
	BufSize: integer
	BufDirty: boolean
	CursorPos: integer
	SelectionStart: integer
	SelectionEnd: integer
    ClearSelection: function(self: ImGuiInputTextCallbackData)
	DeleteChars: function(self: ImGuiInputTextCallbackData, pos: integer, bytes_count: integer)
	HasSelection: function(self: ImGuiInputTextCallbackData): boolean
	new: function(): ImGuiInputTextCallbackData
	InsertChars: function(self: ImGuiInputTextCallbackData, pos: integer, text: string, text_end: string)
	SelectAll: function(self: ImGuiInputTextCallbackData)
	__gc: function(self: ImGuiInputTextCallbackData)
end

global record ImGuiInputTextDeactivatedState
    userdata
    ID: any
	TextA: ImVector_char
    ClearFreeMemory: function(self: ImGuiInputTextDeactivatedState)
	new: function(): ImGuiInputTextDeactivatedState
	__gc: function(self: ImGuiInputTextDeactivatedState)
end

global record ImGuiInputTextState
    userdata
    Ctx: ImGuiContext
	ID: any
	CurLenW: integer
	CurLenA: integer
	TextW: ImVector_ImWchar
	TextA: ImVector_char
	InitialTextA: ImVector_char
	TextAIsValid: boolean
	BufCapacityA: integer
	ScrollX: number
	Stb: STB_TexteditState
	CursorAnim: number
	CursorFollow: boolean
	SelectedAllMouseLock: boolean
	Edited: boolean
	Flags: integer
    ClearFreeMemory: function(self: ImGuiInputTextState)
	ClearSelection: function(self: ImGuiInputTextState)
	ClearText: function(self: ImGuiInputTextState)
	CursorAnimReset: function(self: ImGuiInputTextState)
	CursorClamp: function(self: ImGuiInputTextState)
	GetCursorPos: function(self: ImGuiInputTextState): integer
	GetRedoAvailCount: function(self: ImGuiInputTextState): integer
	GetSelectionEnd: function(self: ImGuiInputTextState): integer
	GetSelectionStart: function(self: ImGuiInputTextState): integer
	GetUndoAvailCount: function(self: ImGuiInputTextState): integer
	HasSelection: function(self: ImGuiInputTextState): boolean
	new: function(): ImGuiInputTextState
	OnKeyPressed: function(self: ImGuiInputTextState, key: integer)
	SelectAll: function(self: ImGuiInputTextState)
	__gc: function(self: ImGuiInputTextState)
end

global record ImGuiKeyData
    userdata
    Down: boolean
	DownDuration: number
	DownDurationPrev: number
	AnalogValue: number
    
end

global record ImGuiKeyOwnerData
    userdata
    OwnerCurr: any
	OwnerNext: any
	LockThisFrame: boolean
	LockUntilRelease: boolean
    new: function(): ImGuiKeyOwnerData
	__gc: function(self: ImGuiKeyOwnerData)
end

global record ImGuiKeyRoutingData
    userdata
    NextEntryIndex: any
	Mods: any
	RoutingNextScore: any
	RoutingCurr: any
	RoutingNext: any
    new: function(): ImGuiKeyRoutingData
	__gc: function(self: ImGuiKeyRoutingData)
end

global record ImGuiKeyRoutingTable
    userdata
    Index: any
	Entries: ImVector_ImGuiKeyRoutingData
	EntriesNext: ImVector_ImGuiKeyRoutingData
    Clear: function(self: ImGuiKeyRoutingTable)
	new: function(): ImGuiKeyRoutingTable
	__gc: function(self: ImGuiKeyRoutingTable)
end

global record ImGuiLastItemData
    userdata
    ID: any
	InFlags: integer
	StatusFlags: integer
	Rect: ImRect
	NavRect: ImRect
	DisplayRect: ImRect
    new: function(): ImGuiLastItemData
	__gc: function(self: ImGuiLastItemData)
end

global record ImGuiListClipper
    userdata
    Ctx: ImGuiContext
	DisplayStart: integer
	DisplayEnd: integer
	ItemsCount: integer
	ItemsHeight: number
	StartPosY: number
	TempData: any
    Begin: function(self: ImGuiListClipper, items_count: integer, items_height: number)
	End: function(self: ImGuiListClipper)
	new: function(): ImGuiListClipper
	IncludeItemByIndex: function(self: ImGuiListClipper, item_index: integer)
	IncludeItemsByIndex: function(self: ImGuiListClipper, item_begin: integer, item_end: integer)
	Step: function(self: ImGuiListClipper): boolean
	__gc: function(self: ImGuiListClipper)
end

global record ImGuiListClipperData
    userdata
    ListClipper: ImGuiListClipper
	LossynessOffset: number
	StepNo: integer
	ItemsFrozen: integer
	Ranges: ImVector_ImGuiListClipperRange
    new: function(): ImGuiListClipperData
	Reset: function(self: ImGuiListClipperData, clipper: ImGuiListClipper)
	__gc: function(self: ImGuiListClipperData)
end

global record ImGuiListClipperRange
    userdata
    Min: integer
	Max: integer
	PosToIndexConvert: boolean
	PosToIndexOffsetMin: any
	PosToIndexOffsetMax: any
    FromIndices: function(min: integer, max: integer): ImGuiListClipperRange
	FromPositions: function(y1: number, y2: number, off_min: integer, off_max: integer): ImGuiListClipperRange
end

global record ImGuiLocEntry
    userdata
    Key: any
	Text: string
    
end

global record ImGuiMenuColumns
    userdata
    TotalWidth: integer
	NextTotalWidth: integer
	Spacing: any
	OffsetIcon: any
	OffsetLabel: any
	OffsetShortcut: any
	OffsetMark: any
	Widths: any
    CalcNextTotalWidth: function(self: ImGuiMenuColumns, update_offsets: boolean)
	DeclColumns: function(self: ImGuiMenuColumns, w_icon: number, w_label: number, w_shortcut: number, w_mark: number): number
	new: function(): ImGuiMenuColumns
	Update: function(self: ImGuiMenuColumns, spacing: number, window_reappearing: boolean)
	__gc: function(self: ImGuiMenuColumns)
end

global record ImGuiMetricsConfig
    userdata
    ShowDebugLog: boolean
	ShowStackTool: boolean
	ShowWindowsRects: boolean
	ShowWindowsBeginOrder: boolean
	ShowTablesRects: boolean
	ShowDrawCmdMesh: boolean
	ShowDrawCmdBoundingBoxes: boolean
	ShowAtlasTintedWithTextColor: boolean
	ShowWindowsRectsType: integer
	ShowTablesRectsType: integer
    
end

global record ImGuiNavItemData
    userdata
    Window: ImGuiWindow
	ID: any
	FocusScopeId: any
	RectRel: ImRect
	InFlags: integer
	DistBox: number
	DistCenter: number
	DistAxial: number
    Clear: function(self: ImGuiNavItemData)
	new: function(): ImGuiNavItemData
	__gc: function(self: ImGuiNavItemData)
end

global record ImGuiNavTreeNodeData
    userdata
    ID: any
	InFlags: integer
	NavRect: ImRect
    
end

global record ImGuiNextItemData
    userdata
    Flags: integer
	ItemFlags: integer
	Width: number
	FocusScopeId: any
	OpenCond: integer
	OpenVal: boolean
    ClearFlags: function(self: ImGuiNextItemData)
	new: function(): ImGuiNextItemData
	__gc: function(self: ImGuiNextItemData)
end

global record ImGuiNextWindowData
    userdata
    Flags: integer
	PosCond: integer
	SizeCond: integer
	CollapsedCond: integer
	PosVal: ImVec2
	PosPivotVal: ImVec2
	SizeVal: ImVec2
	ContentSizeVal: ImVec2
	ScrollVal: ImVec2
	CollapsedVal: boolean
	SizeConstraintRect: ImRect
	SizeCallback: any
	SizeCallbackUserData: any
	BgAlphaVal: number
	MenuBarOffsetMinVal: ImVec2
    ClearFlags: function(self: ImGuiNextWindowData)
	new: function(): ImGuiNextWindowData
	__gc: function(self: ImGuiNextWindowData)
end

global record ImGuiOldColumnData
    userdata
    OffsetNorm: number
	OffsetNormBeforeResize: number
	Flags: integer
	ClipRect: ImRect
    new: function(): ImGuiOldColumnData
	__gc: function(self: ImGuiOldColumnData)
end

global record ImGuiOldColumns
    userdata
    ID: any
	Flags: integer
	IsFirstFrame: boolean
	IsBeingResized: boolean
	Current: integer
	Count: integer
	OffMinX: number
	OffMaxX: number
	LineMinY: number
	LineMaxY: number
	HostCursorPosY: number
	HostCursorMaxPosX: number
	HostInitialClipRect: ImRect
	HostBackupClipRect: ImRect
	HostBackupParentWorkRect: ImRect
	Columns: ImVector_ImGuiOldColumnData
	Splitter: ImDrawListSplitter
    new: function(): ImGuiOldColumns
	__gc: function(self: ImGuiOldColumns)
end

global record ImGuiOnceUponAFrame
    userdata
    RefFrame: integer
    new: function(): ImGuiOnceUponAFrame
	__gc: function(self: ImGuiOnceUponAFrame)
end

global record ImGuiPayload
    userdata
    Data: any
	DataSize: integer
	SourceId: any
	SourceParentId: any
	DataFrameCount: integer
	DataType: any
	Preview: boolean
	Delivery: boolean
    Clear: function(self: ImGuiPayload)
	new: function(): ImGuiPayload
	IsDataType: function(self: ImGuiPayload, type: string): boolean
	IsDelivery: function(self: ImGuiPayload): boolean
	IsPreview: function(self: ImGuiPayload): boolean
	__gc: function(self: ImGuiPayload)
end

global record ImGuiPlatformImeData
    userdata
    WantVisible: boolean
	InputPos: ImVec2
	InputLineHeight: number
    new: function(): ImGuiPlatformImeData
	__gc: function(self: ImGuiPlatformImeData)
end

global record ImGuiPopupData
    userdata
    PopupId: any
	Window: ImGuiWindow
	BackupNavWindow: ImGuiWindow
	ParentNavLayer: integer
	OpenFrameCount: integer
	OpenParentId: any
	OpenPopupPos: ImVec2
	OpenMousePos: ImVec2
    new: function(): ImGuiPopupData
	__gc: function(self: ImGuiPopupData)
end

global record ImGuiPtrOrIndex
    userdata
    Ptr: any
	Index: integer
    new: function(ptr: any): ImGuiPtrOrIndex
	new: function(index: integer): ImGuiPtrOrIndex
	__gc: function(self: ImGuiPtrOrIndex)
end

global record ImGuiSettingsHandler
    userdata
    TypeName: string
	TypeHash: any
	ClearAllFn: any
	ReadInitFn: any
	ReadOpenFn: any
	ReadLineFn: any
	ApplyAllFn: any
	WriteAllFn: any
	UserData: any
    new: function(): ImGuiSettingsHandler
	__gc: function(self: ImGuiSettingsHandler)
end

global record ImGuiShrinkWidthItem
    userdata
    Index: integer
	Width: number
	InitialWidth: number
    
end

global record ImGuiSizeCallbackData
    userdata
    UserData: any
	Pos: ImVec2
	CurrentSize: ImVec2
	DesiredSize: ImVec2
    
end

global record ImGuiStackLevelInfo
    userdata
    ID: any
	QueryFrameCount: any
	QuerySuccess: boolean
	DataType: integer
	Desc: any
    new: function(): ImGuiStackLevelInfo
	__gc: function(self: ImGuiStackLevelInfo)
end

global record ImGuiStackSizes
    userdata
    SizeOfIDStack: integer
	SizeOfColorStack: integer
	SizeOfStyleVarStack: integer
	SizeOfFontStack: integer
	SizeOfFocusScopeStack: integer
	SizeOfGroupStack: integer
	SizeOfItemFlagsStack: integer
	SizeOfBeginPopupStack: integer
	SizeOfDisabledStack: integer
    CompareWithContextState: function(self: ImGuiStackSizes, ctx: ImGuiContext)
	new: function(): ImGuiStackSizes
	SetToContextState: function(self: ImGuiStackSizes, ctx: ImGuiContext)
	__gc: function(self: ImGuiStackSizes)
end

global record ImGuiStackTool
    userdata
    LastActiveFrame: integer
	StackLevel: integer
	QueryId: any
	Results: ImVector_ImGuiStackLevelInfo
	CopyToClipboardOnCtrlC: boolean
	CopyToClipboardLastTime: number
    new: function(): ImGuiStackTool
	__gc: function(self: ImGuiStackTool)
end

global record ImGuiStorage
    userdata
    Data: ImVector_ImGuiStoragePair
    BuildSortByKey: function(self: ImGuiStorage)
	Clear: function(self: ImGuiStorage)
	GetBool: function(self: ImGuiStorage, key: any, default_val: boolean): boolean
	GetBoolRef: function(self: ImGuiStorage, key: any, default_val: boolean): any
	GetFloat: function(self: ImGuiStorage, key: any, default_val: number): number
	GetFloatRef: function(self: ImGuiStorage, key: any, default_val: number): any
	GetInt: function(self: ImGuiStorage, key: any, default_val: integer): integer
	GetIntRef: function(self: ImGuiStorage, key: any, default_val: integer): any
	GetVoidPtr: function(self: ImGuiStorage, key: any): any
	GetVoidPtrRef: function(self: ImGuiStorage, key: any, default_val: any): any
	SetAllInt: function(self: ImGuiStorage, val: integer)
	SetBool: function(self: ImGuiStorage, key: any, val: boolean)
	SetFloat: function(self: ImGuiStorage, key: any, val: number)
	SetInt: function(self: ImGuiStorage, key: any, val: integer)
	SetVoidPtr: function(self: ImGuiStorage, key: any, val: any)
end

global record ImGuiStoragePair
    userdata
    key: any
	val_i: integer
	val_f: number
	val_p: any
    new: function(_key: any, _val_i: integer): ImGuiStoragePair
	new: function(_key: any, _val_f: number): ImGuiStoragePair
	new: function(_key: any, _val_p: any): ImGuiStoragePair
	__gc: function(self: ImGuiStoragePair)
end

global record ImGuiStyle
    userdata
    Alpha: number
	DisabledAlpha: number
	WindowPadding: ImVec2
	WindowRounding: number
	WindowBorderSize: number
	WindowMinSize: ImVec2
	WindowTitleAlign: ImVec2
	WindowMenuButtonPosition: integer
	ChildRounding: number
	ChildBorderSize: number
	PopupRounding: number
	PopupBorderSize: number
	FramePadding: ImVec2
	FrameRounding: number
	FrameBorderSize: number
	ItemSpacing: ImVec2
	ItemInnerSpacing: ImVec2
	CellPadding: ImVec2
	TouchExtraPadding: ImVec2
	IndentSpacing: number
	ColumnsMinSpacing: number
	ScrollbarSize: number
	ScrollbarRounding: number
	GrabMinSize: number
	GrabRounding: number
	LogSliderDeadzone: number
	TabRounding: number
	TabBorderSize: number
	TabMinWidthForCloseButton: number
	ColorButtonPosition: integer
	ButtonTextAlign: ImVec2
	SelectableTextAlign: ImVec2
	SeparatorTextBorderSize: number
	SeparatorTextAlign: ImVec2
	SeparatorTextPadding: ImVec2
	DisplayWindowPadding: ImVec2
	DisplaySafeAreaPadding: ImVec2
	MouseCursorScale: number
	AntiAliasedLines: boolean
	AntiAliasedLinesUseTex: boolean
	AntiAliasedFill: boolean
	CurveTessellationTol: number
	CircleTessellationMaxError: number
	Colors: ImVec4
	HoverStationaryDelay: number
	HoverDelayShort: number
	HoverDelayNormal: number
	HoverFlagsForTooltipMouse: integer
	HoverFlagsForTooltipNav: integer
    new: function(): ImGuiStyle
	ScaleAllSizes: function(self: ImGuiStyle, scale_factor: number)
	__gc: function(self: ImGuiStyle)
end

global record ImGuiStyleMod
    userdata
    VarIdx: integer
	BackupInt: integer
	BackupFloat: number
    new: function(idx: integer, v: integer): ImGuiStyleMod
	new: function(idx: integer, v: number): ImGuiStyleMod
	new: function(idx: integer, v: ImVec2): ImGuiStyleMod
	__gc: function(self: ImGuiStyleMod)
end

global record ImGuiTabBar
    userdata
    Tabs: ImVector_ImGuiTabItem
	Flags: integer
	ID: any
	SelectedTabId: any
	NextSelectedTabId: any
	VisibleTabId: any
	CurrFrameVisible: integer
	PrevFrameVisible: integer
	BarRect: ImRect
	CurrTabsContentsHeight: number
	PrevTabsContentsHeight: number
	WidthAllTabs: number
	WidthAllTabsIdeal: number
	ScrollingAnim: number
	ScrollingTarget: number
	ScrollingTargetDistToVisibility: number
	ScrollingSpeed: number
	ScrollingRectMinX: number
	ScrollingRectMaxX: number
	ReorderRequestTabId: any
	ReorderRequestOffset: any
	BeginCount: any
	WantLayout: boolean
	VisibleTabWasSubmitted: boolean
	TabsAddedNew: boolean
	TabsActiveCount: any
	LastTabItemIdx: any
	ItemSpacingY: number
	FramePadding: ImVec2
	BackupCursorPos: ImVec2
	TabsNames: ImGuiTextBuffer
    new: function(): ImGuiTabBar
	__gc: function(self: ImGuiTabBar)
end

global record ImGuiTabItem
    userdata
    ID: any
	Flags: integer
	LastFrameVisible: integer
	LastFrameSelected: integer
	Offset: number
	Width: number
	ContentWidth: number
	RequestedWidth: number
	NameOffset: any
	BeginOrder: any
	IndexDuringLayout: any
	WantClose: boolean
    new: function(): ImGuiTabItem
	__gc: function(self: ImGuiTabItem)
end

global record ImGuiTable
    userdata
    ID: any
	Flags: integer
	RawData: any
	TempData: ImGuiTableTempData
	Columns: any
	DisplayOrderToIndex: any
	RowCellData: any
	EnabledMaskByDisplayOrder: any
	EnabledMaskByIndex: any
	VisibleMaskByIndex: any
	SettingsLoadedFlags: integer
	SettingsOffset: integer
	LastFrameActive: integer
	ColumnsCount: integer
	CurrentRow: integer
	CurrentColumn: integer
	InstanceCurrent: any
	InstanceInteracted: any
	RowPosY1: number
	RowPosY2: number
	RowMinHeight: number
	RowCellPaddingY: number
	RowTextBaseline: number
	RowIndentOffsetX: number
	RowFlags: integer
	LastRowFlags: integer
	RowBgColorCounter: integer
	RowBgColor: integer
	BorderColorStrong: integer
	BorderColorLight: integer
	BorderX1: number
	BorderX2: number
	HostIndentX: number
	MinColumnWidth: number
	OuterPaddingX: number
	CellPaddingX: number
	CellSpacingX1: number
	CellSpacingX2: number
	InnerWidth: number
	ColumnsGivenWidth: number
	ColumnsAutoFitWidth: number
	ColumnsStretchSumWeights: number
	ResizedColumnNextWidth: number
	ResizeLockMinContentsX2: number
	RefScale: number
	OuterRect: ImRect
	InnerRect: ImRect
	WorkRect: ImRect
	InnerClipRect: ImRect
	BgClipRect: ImRect
	Bg0ClipRectForDrawCmd: ImRect
	Bg2ClipRectForDrawCmd: ImRect
	HostClipRect: ImRect
	HostBackupInnerClipRect: ImRect
	OuterWindow: ImGuiWindow
	InnerWindow: ImGuiWindow
	ColumnsNames: ImGuiTextBuffer
	DrawSplitter: ImDrawListSplitter
	InstanceDataFirst: ImGuiTableInstanceData
	InstanceDataExtra: ImVector_ImGuiTableInstanceData
	SortSpecsSingle: ImGuiTableColumnSortSpecs
	SortSpecsMulti: ImVector_ImGuiTableColumnSortSpecs
	SortSpecs: ImGuiTableSortSpecs
	SortSpecsCount: any
	ColumnsEnabledCount: any
	ColumnsEnabledFixedCount: any
	DeclColumnsCount: any
	HoveredColumnBody: any
	HoveredColumnBorder: any
	AutoFitSingleColumn: any
	ResizedColumn: any
	LastResizedColumn: any
	HeldHeaderColumn: any
	ReorderColumn: any
	ReorderColumnDir: any
	LeftMostEnabledColumn: any
	RightMostEnabledColumn: any
	LeftMostStretchedColumn: any
	RightMostStretchedColumn: any
	ContextPopupColumn: any
	FreezeRowsRequest: any
	FreezeRowsCount: any
	FreezeColumnsRequest: any
	FreezeColumnsCount: any
	RowCellDataCurrent: any
	DummyDrawChannel: any
	Bg2DrawChannelCurrent: any
	Bg2DrawChannelUnfrozen: any
	IsLayoutLocked: boolean
	IsInsideRow: boolean
	IsInitializing: boolean
	IsSortSpecsDirty: boolean
	IsUsingHeaders: boolean
	IsContextPopupOpen: boolean
	IsSettingsRequestLoad: boolean
	IsSettingsDirty: boolean
	IsDefaultDisplayOrder: boolean
	IsResetAllRequest: boolean
	IsResetDisplayOrderRequest: boolean
	IsUnfrozenRows: boolean
	IsDefaultSizingPolicy: boolean
	HasScrollbarYCurr: boolean
	HasScrollbarYPrev: boolean
	MemoryCompacted: boolean
	HostSkipItems: boolean
    new: function(): ImGuiTable
	__gc: function(self: ImGuiTable)
end

global record ImGuiTableCellData
    userdata
    BgColor: integer
	Column: any
    
end

global record ImGuiTableColumn
    userdata
    Flags: integer
	WidthGiven: number
	MinX: number
	MaxX: number
	WidthRequest: number
	WidthAuto: number
	StretchWeight: number
	InitStretchWeightOrWidth: number
	ClipRect: ImRect
	UserID: any
	WorkMinX: number
	WorkMaxX: number
	ItemWidth: number
	ContentMaxXFrozen: number
	ContentMaxXUnfrozen: number
	ContentMaxXHeadersUsed: number
	ContentMaxXHeadersIdeal: number
	NameOffset: any
	DisplayOrder: any
	IndexWithinEnabledSet: any
	PrevEnabledColumn: any
	NextEnabledColumn: any
	SortOrder: any
	DrawChannelCurrent: any
	DrawChannelFrozen: any
	DrawChannelUnfrozen: any
	IsEnabled: boolean
	IsUserEnabled: boolean
	IsUserEnabledNextFrame: boolean
	IsVisibleX: boolean
	IsVisibleY: boolean
	IsRequestOutput: boolean
	IsSkipItems: boolean
	IsPreserveWidthAuto: boolean
	NavLayerCurrent: any
	AutoFitQueue: any
	CannotSkipItemsQueue: any
	SortDirection: any
	SortDirectionsAvailCount: any
	SortDirectionsAvailMask: any
	SortDirectionsAvailList: any
    new: function(): ImGuiTableColumn
	__gc: function(self: ImGuiTableColumn)
end

global record ImGuiTableColumnSettings
    userdata
    WidthOrWeight: number
	UserID: any
	Index: any
	DisplayOrder: any
	SortOrder: any
	SortDirection: any
	IsEnabled: any
	IsStretch: any
    new: function(): ImGuiTableColumnSettings
	__gc: function(self: ImGuiTableColumnSettings)
end

global record ImGuiTableColumnSortSpecs
    userdata
    ColumnUserID: any
	ColumnIndex: any
	SortOrder: any
	SortDirection: integer
    new: function(): ImGuiTableColumnSortSpecs
	__gc: function(self: ImGuiTableColumnSortSpecs)
end

global record ImGuiTableInstanceData
    userdata
    TableInstanceID: any
	LastOuterHeight: number
	LastFirstRowHeight: number
	LastFrozenHeight: number
	HoveredRowLast: integer
	HoveredRowNext: integer
    new: function(): ImGuiTableInstanceData
	__gc: function(self: ImGuiTableInstanceData)
end

global record ImGuiTableSettings
    userdata
    ID: any
	SaveFlags: integer
	RefScale: number
	ColumnsCount: any
	ColumnsCountMax: any
	WantApply: boolean
    GetColumnSettings: function(self: ImGuiTableSettings): ImGuiTableColumnSettings
	new: function(): ImGuiTableSettings
	__gc: function(self: ImGuiTableSettings)
end

global record ImGuiTableSortSpecs
    userdata
    Specs: ImGuiTableColumnSortSpecs
	SpecsCount: integer
	SpecsDirty: boolean
    new: function(): ImGuiTableSortSpecs
	__gc: function(self: ImGuiTableSortSpecs)
end

global record ImGuiTableTempData
    userdata
    TableIndex: integer
	LastTimeActive: number
	UserOuterSize: ImVec2
	DrawSplitter: ImDrawListSplitter
	HostBackupWorkRect: ImRect
	HostBackupParentWorkRect: ImRect
	HostBackupPrevLineSize: ImVec2
	HostBackupCurrLineSize: ImVec2
	HostBackupCursorMaxPos: ImVec2
	HostBackupColumnsOffset: ImVec1
	HostBackupItemWidth: number
	HostBackupItemWidthStackSize: integer
    new: function(): ImGuiTableTempData
	__gc: function(self: ImGuiTableTempData)
end

global record ImGuiTextBuffer
    userdata
    Buf: ImVector_char
    new: function(): ImGuiTextBuffer
	append: function(self: ImGuiTextBuffer, str: string, str_end: string)
	appendf: function(self: ImGuiTextBuffer, fmt: string, ...: any)
	appendfv: function(self: ImGuiTextBuffer, fmt: string, args: any)
	begin: function(self: ImGuiTextBuffer): string
	c_str: function(self: ImGuiTextBuffer): string
	clear: function(self: ImGuiTextBuffer)
	__gc: function(self: ImGuiTextBuffer)
	empty: function(self: ImGuiTextBuffer): boolean
	end_: function(self: ImGuiTextBuffer): string
	reserve: function(self: ImGuiTextBuffer, capacity: integer)
	size: function(self: ImGuiTextBuffer): integer
end

global record ImGuiTextFilter
    userdata
    InputBuf: any
	Filters: ImVector_ImGuiTextRange
	CountGrep: integer
    Build: function(self: ImGuiTextFilter)
	Clear: function(self: ImGuiTextFilter)
	Draw: function(self: ImGuiTextFilter, label: string, width: number): boolean
	new: function(default_filter: string): ImGuiTextFilter
	IsActive: function(self: ImGuiTextFilter): boolean
	PassFilter: function(self: ImGuiTextFilter, text: string, text_end: string): boolean
	__gc: function(self: ImGuiTextFilter)
end

global record ImGuiTextIndex
    userdata
    LineOffsets: ImVector_int
	EndOffset: integer
    append: function(self: ImGuiTextIndex, base: string, old_size: integer, new_size: integer)
	clear: function(self: ImGuiTextIndex)
	get_line_begin: function(self: ImGuiTextIndex, base: string, n: integer): string
	get_line_end: function(self: ImGuiTextIndex, base: string, n: integer): string
	size: function(self: ImGuiTextIndex): integer
end

global record ImGuiTextRange
    userdata
    b: string
	e: string
    new: function(): ImGuiTextRange
	new: function(_b: string, _e: string): ImGuiTextRange
	__gc: function(self: ImGuiTextRange)
	empty: function(self: ImGuiTextRange): boolean
	split: function(self: ImGuiTextRange, separator: any, out: ImVector_ImGuiTextRange)
end

global record ImGuiViewport
    userdata
    Flags: integer
	Pos: ImVec2
	Size: ImVec2
	WorkPos: ImVec2
	WorkSize: ImVec2
	PlatformHandleRaw: any
    GetCenter: function(pOut: ImVec2, self: ImGuiViewport)
	GetWorkCenter: function(pOut: ImVec2, self: ImGuiViewport)
	new: function(): ImGuiViewport
	__gc: function(self: ImGuiViewport)
end

global record ImGuiViewportP
    userdata
    _ImGuiViewport: ImGuiViewport
	BgFgDrawListsLastFrame: integer
	BgFgDrawLists: ImDrawList
	DrawDataP: ImDrawData
	DrawDataBuilder: ImDrawDataBuilder
	WorkOffsetMin: ImVec2
	WorkOffsetMax: ImVec2
	BuildWorkOffsetMin: ImVec2
	BuildWorkOffsetMax: ImVec2
    CalcWorkRectPos: function(pOut: ImVec2, self: ImGuiViewportP, off_min: ImVec2)
	CalcWorkRectSize: function(pOut: ImVec2, self: ImGuiViewportP, off_min: ImVec2, off_max: ImVec2)
	GetBuildWorkRect: function(pOut: ImRect, self: ImGuiViewportP)
	GetMainRect: function(pOut: ImRect, self: ImGuiViewportP)
	GetWorkRect: function(pOut: ImRect, self: ImGuiViewportP)
	new: function(): ImGuiViewportP
	UpdateWorkRect: function(self: ImGuiViewportP)
	__gc: function(self: ImGuiViewportP)
end

global record ImGuiWindow
    userdata
    Ctx: ImGuiContext
	Name: any
	ID: any
	Flags: integer
	Viewport: ImGuiViewportP
	Pos: ImVec2
	Size: ImVec2
	SizeFull: ImVec2
	ContentSize: ImVec2
	ContentSizeIdeal: ImVec2
	ContentSizeExplicit: ImVec2
	WindowPadding: ImVec2
	WindowRounding: number
	WindowBorderSize: number
	DecoOuterSizeX1: number
	DecoOuterSizeY1: number
	DecoOuterSizeX2: number
	DecoOuterSizeY2: number
	DecoInnerSizeX1: number
	DecoInnerSizeY1: number
	NameBufLen: integer
	MoveId: any
	ChildId: any
	Scroll: ImVec2
	ScrollMax: ImVec2
	ScrollTarget: ImVec2
	ScrollTargetCenterRatio: ImVec2
	ScrollTargetEdgeSnapDist: ImVec2
	ScrollbarSizes: ImVec2
	ScrollbarX: boolean
	ScrollbarY: boolean
	Active: boolean
	WasActive: boolean
	WriteAccessed: boolean
	Collapsed: boolean
	WantCollapseToggle: boolean
	SkipItems: boolean
	Appearing: boolean
	Hidden: boolean
	IsFallbackWindow: boolean
	IsExplicitChild: boolean
	HasCloseButton: boolean
	ResizeBorderHeld: any
	BeginCount: integer
	BeginCountPreviousFrame: integer
	BeginOrderWithinParent: integer
	BeginOrderWithinContext: integer
	FocusOrder: integer
	PopupId: any
	AutoFitFramesX: any
	AutoFitFramesY: any
	AutoFitChildAxises: any
	AutoFitOnlyGrows: boolean
	AutoPosLastDirection: integer
	HiddenFramesCanSkipItems: any
	HiddenFramesCannotSkipItems: any
	HiddenFramesForRenderOnly: any
	DisableInputsFrames: any
	SetWindowPosAllowFlags: integer
	SetWindowSizeAllowFlags: integer
	SetWindowCollapsedAllowFlags: integer
	SetWindowPosVal: ImVec2
	SetWindowPosPivot: ImVec2
	IDStack: ImVector_ImGuiID
	DC: ImGuiWindowTempData
	OuterRectClipped: ImRect
	InnerRect: ImRect
	InnerClipRect: ImRect
	WorkRect: ImRect
	ParentWorkRect: ImRect
	ClipRect: ImRect
	ContentRegionRect: ImRect
	HitTestHoleSize: ImVec2ih
	HitTestHoleOffset: ImVec2ih
	LastFrameActive: integer
	LastTimeActive: number
	ItemWidthDefault: number
	StateStorage: ImGuiStorage
	ColumnsStorage: ImVector_ImGuiOldColumns
	FontWindowScale: number
	SettingsOffset: integer
	DrawList: ImDrawList
	DrawListInst: ImDrawList
	ParentWindow: ImGuiWindow
	ParentWindowInBeginStack: ImGuiWindow
	RootWindow: ImGuiWindow
	RootWindowPopupTree: ImGuiWindow
	RootWindowForTitleBarHighlight: ImGuiWindow
	RootWindowForNav: ImGuiWindow
	NavLastChildNavWindow: ImGuiWindow
	NavLastIds: any
	NavRectRel: ImRect
	NavPreferredScoringPosRel: ImVec2
	NavRootFocusScopeId: any
	MemoryDrawListIdxCapacity: integer
	MemoryDrawListVtxCapacity: integer
	MemoryCompacted: boolean
    CalcFontSize: function(self: ImGuiWindow): number
	GetID: function(self: ImGuiWindow, str: string, str_end: string): any
	GetID: function(self: ImGuiWindow, ptr: any): any
	GetID: function(self: ImGuiWindow, n: integer): any
	GetIDFromRectangle: function(self: ImGuiWindow, r_abs: ImRect): any
	new: function(context: ImGuiContext, name: string): ImGuiWindow
	MenuBarHeight: function(self: ImGuiWindow): number
	MenuBarRect: function(pOut: ImRect, self: ImGuiWindow)
	Rect: function(pOut: ImRect, self: ImGuiWindow)
	TitleBarHeight: function(self: ImGuiWindow): number
	TitleBarRect: function(pOut: ImRect, self: ImGuiWindow)
	__gc: function(self: ImGuiWindow)
end

global record ImGuiWindowSettings
    userdata
    ID: any
	Pos: ImVec2ih
	Size: ImVec2ih
	Collapsed: boolean
	WantApply: boolean
	WantDelete: boolean
    GetName: function(self: ImGuiWindowSettings): any
	new: function(): ImGuiWindowSettings
	__gc: function(self: ImGuiWindowSettings)
end

global record ImGuiWindowStackData
    userdata
    Window: ImGuiWindow
	ParentLastItemDataBackup: ImGuiLastItemData
	StackSizesOnBegin: ImGuiStackSizes
    
end

global record ImGuiWindowTempData
    userdata
    CursorPos: ImVec2
	CursorPosPrevLine: ImVec2
	CursorStartPos: ImVec2
	CursorMaxPos: ImVec2
	IdealMaxPos: ImVec2
	CurrLineSize: ImVec2
	PrevLineSize: ImVec2
	CurrLineTextBaseOffset: number
	PrevLineTextBaseOffset: number
	IsSameLine: boolean
	IsSetPos: boolean
	Indent: ImVec1
	ColumnsOffset: ImVec1
	GroupOffset: ImVec1
	CursorStartPosLossyness: ImVec2
	NavLayerCurrent: any
	NavLayersActiveMask: integer
	NavLayersActiveMaskNext: integer
	NavIsScrollPushableX: boolean
	NavHideHighlightOneFrame: boolean
	NavWindowHasScrollY: boolean
	MenuBarAppending: boolean
	MenuBarOffset: ImVec2
	MenuColumns: ImGuiMenuColumns
	TreeDepth: integer
	TreeJumpToParentOnPopMask: integer
	ChildWindows: ImVector_ImGuiWindowPtr
	StateStorage: ImGuiStorage
	CurrentColumns: ImGuiOldColumns
	CurrentTableIdx: integer
	LayoutType: integer
	ParentLayoutType: integer
	ItemWidth: number
	TextWrapPos: number
	ItemWidthStack: ImVector_float
	TextWrapPosStack: ImVector_float
    
end

global record ImRect
    userdata
    Min: ImVec2
	Max: ImVec2
    Add: function(self: ImRect, p: ImVec2)
	Add: function(self: ImRect, r: ImRect)
	ClipWith: function(self: ImRect, r: ImRect)
	ClipWithFull: function(self: ImRect, r: ImRect)
	Contains: function(self: ImRect, p: ImVec2): boolean
	Contains: function(self: ImRect, r: ImRect): boolean
	Expand: function(self: ImRect, amount: any)
	Expand: function(self: ImRect, amount: ImVec2)
	Floor: function(self: ImRect)
	GetArea: function(self: ImRect): number
	GetBL: function(pOut: ImVec2, self: ImRect)
	GetBR: function(pOut: ImVec2, self: ImRect)
	GetCenter: function(pOut: ImVec2, self: ImRect)
	GetHeight: function(self: ImRect): number
	GetSize: function(pOut: ImVec2, self: ImRect)
	GetTL: function(pOut: ImVec2, self: ImRect)
	GetTR: function(pOut: ImVec2, self: ImRect)
	GetWidth: function(self: ImRect): number
	new: function(): ImRect
	new: function(min: ImVec2, max: ImVec2): ImRect
	new: function(v: ImVec4): ImRect
	new: function(x1: number, y1: number, x2: number, y2: number): ImRect
	IsInverted: function(self: ImRect): boolean
	Overlaps: function(self: ImRect, r: ImRect): boolean
	ToVec4: function(pOut: ImVec4, self: ImRect)
	Translate: function(self: ImRect, d: ImVec2)
	TranslateX: function(self: ImRect, dx: number)
	TranslateY: function(self: ImRect, dy: number)
	__gc: function(self: ImRect)
end

global record ImVec1
    userdata
    x: number
    new: function(): ImVec1
	new: function(_x: number): ImVec1
	__gc: function(self: ImVec1)
end

global record ImVec2
    userdata
    x: number
	y: number
    new: function(): ImVec2
	new: function(_x: number, _y: number): ImVec2
	__gc: function(self: ImVec2)
end

global record ImVec2ih
    userdata
    x: integer
	y: integer
    new: function(): ImVec2ih
	new: function(_x: integer, _y: integer): ImVec2ih
	new: function(rhs: ImVec2): ImVec2ih
	__gc: function(self: ImVec2ih)
end

global record ImVec4
    userdata
    x: number
	y: number
	z: number
	w: number
    new: function(): ImVec4
	new: function(_x: number, _y: number, _z: number, _w: number): ImVec4
	__gc: function(self: ImVec4)
end

global record STB_TexteditState
    userdata
    cursor: integer
	select_start: integer
	select_end: integer
	insert_mode: integer
	row_count_per_page: integer
	cursor_at_end_of_line: integer
	initialized: integer
	has_preferred_x: integer
	single_line: integer
	padding1: integer
	padding2: integer
	padding3: integer
	preferred_x: number
	undostate: StbUndoState
    
end

global record StbTexteditRow
    userdata
    x0: number
	x1: number
	baseline_y_delta: number
	ymin: number
	ymax: number
	num_chars: integer
    
end

global record StbUndoRecord
    userdata
    where: integer
	insert_length: integer
	delete_length: integer
	char_storage: integer
    
end

global record StbUndoState
    userdata
    undo_rec: StbUndoRecord
	undo_char: any
	undo_point: integer
	redo_point: integer
	undo_char_point: integer
	redo_char_point: integer
    
end

global record ImVector_ImGuiGroupData
    userdata
    Size: integer
    Capacity: integer
    Data: {ImGuiGroupData}
end

global record ImVector_ImGuiWindowStackData
    userdata
    Size: integer
    Capacity: integer
    Data: {ImGuiWindowStackData}
end

global record ImVector_ImWchar
    userdata
    Size: integer
    Capacity: integer
    Data: {any}
end

global record ImVector_ImU32
    userdata
    Size: integer
    Capacity: integer
    Data: {integer}
end

global record ImVector_ImTextureID
    userdata
    Size: integer
    Capacity: integer
    Data: {any}
end

global record ImVector_ImGuiTableColumnSortSpecs
    userdata
    Size: integer
    Capacity: integer
    Data: {ImGuiTableColumnSortSpecs}
end

global record ImVector_ImGuiTextRange
    userdata
    Size: integer
    Capacity: integer
    Data: {ImGuiTextRange}
end

global record ImVector_int
    userdata
    Size: integer
    Capacity: integer
    Data: {integer}
end

global record ImVector_ImFontPtr
    userdata
    Size: integer
    Capacity: integer
    Data: {ImFont}
end

global record ImVector_ImGuiPopupData
    userdata
    Size: integer
    Capacity: integer
    Data: {ImGuiPopupData}
end

global record ImVector_ImGuiTableInstanceData
    userdata
    Size: integer
    Capacity: integer
    Data: {ImGuiTableInstanceData}
end

global record ImVector_ImFontAtlasCustomRect
    userdata
    Size: integer
    Capacity: integer
    Data: {ImFontAtlasCustomRect}
end

global record ImVector_ImGuiID
    userdata
    Size: integer
    Capacity: integer
    Data: {any}
end

global record ImVector_ImGuiItemFlags
    userdata
    Size: integer
    Capacity: integer
    Data: {integer}
end

global record ImVector_ImDrawIdx
    userdata
    Size: integer
    Capacity: integer
    Data: {any}
end

global record ImVector_ImGuiShrinkWidthItem
    userdata
    Size: integer
    Capacity: integer
    Data: {ImGuiShrinkWidthItem}
end

global record ImVector_ImGuiViewportPPtr
    userdata
    Size: integer
    Capacity: integer
    Data: {ImGuiViewportP}
end

global record ImVector_ImGuiTabItem
    userdata
    Size: integer
    Capacity: integer
    Data: {ImGuiTabItem}
end

global record ImVector_ImDrawListPtr
    userdata
    Size: integer
    Capacity: integer
    Data: {ImDrawList}
end

global record ImVector_ImGuiKeyRoutingData
    userdata
    Size: integer
    Capacity: integer
    Data: {ImGuiKeyRoutingData}
end

global record ImVector_ImGuiInputEvent
    userdata
    Size: integer
    Capacity: integer
    Data: {ImGuiInputEvent}
end

global record ImVector_ImDrawChannel
    userdata
    Size: integer
    Capacity: integer
    Data: {ImDrawChannel}
end

global record ImVector_ImGuiStyleMod
    userdata
    Size: integer
    Capacity: integer
    Data: {ImGuiStyleMod}
end

global record ImVector_ImVec2
    userdata
    Size: integer
    Capacity: integer
    Data: {ImVec2}
end

global record ImVector_ImFontGlyph
    userdata
    Size: integer
    Capacity: integer
    Data: {ImFontGlyph}
end

global record ImVector_ImGuiColorMod
    userdata
    Size: integer
    Capacity: integer
    Data: {ImGuiColorMod}
end

global record ImVector_ImGuiContextHook
    userdata
    Size: integer
    Capacity: integer
    Data: {ImGuiContextHook}
end

global record ImVector_ImFontConfig
    userdata
    Size: integer
    Capacity: integer
    Data: {ImFontConfig}
end

global record ImVector_ImDrawVert
    userdata
    Size: integer
    Capacity: integer
    Data: {ImDrawVert}
end

global record ImVector_ImGuiOldColumns
    userdata
    Size: integer
    Capacity: integer
    Data: {ImGuiOldColumns}
end

global record ImVector_ImGuiOldColumnData
    userdata
    Size: integer
    Capacity: integer
    Data: {ImGuiOldColumnData}
end

global record ImVector_ImDrawCmd
    userdata
    Size: integer
    Capacity: integer
    Data: {ImDrawCmd}
end

global record ImVector_unsigned_char
    userdata
    Size: integer
    Capacity: integer
    Data: {integer}
end

global record ImVector_ImGuiWindowPtr
    userdata
    Size: integer
    Capacity: integer
    Data: {ImGuiWindow}
end

global record ImVector_ImGuiSettingsHandler
    userdata
    Size: integer
    Capacity: integer
    Data: {ImGuiSettingsHandler}
end

global record ImVector_ImGuiStoragePair
    userdata
    Size: integer
    Capacity: integer
    Data: {ImGuiStoragePair}
end

global record ImVector_ImGuiPtrOrIndex
    userdata
    Size: integer
    Capacity: integer
    Data: {ImGuiPtrOrIndex}
end

global record ImVector_ImGuiStackLevelInfo
    userdata
    Size: integer
    Capacity: integer
    Data: {ImGuiStackLevelInfo}
end

global record ImVector_char
    userdata
    Size: integer
    Capacity: integer
    Data: {any}
end

global record ImVector_ImGuiListClipperData
    userdata
    Size: integer
    Capacity: integer
    Data: {ImGuiListClipperData}
end

global record ImVector_ImGuiTableTempData
    userdata
    Size: integer
    Capacity: integer
    Data: {ImGuiTableTempData}
end

global record ImVector_ImGuiListClipperRange
    userdata
    Size: integer
    Capacity: integer
    Data: {ImGuiListClipperRange}
end

global record ImVector_ImGuiNavTreeNodeData
    userdata
    Size: integer
    Capacity: integer
    Data: {ImGuiNavTreeNodeData}
end

global record ImVector_float
    userdata
    Size: integer
    Capacity: integer
    Data: {number}
end

global record ImVector_ImVec4
    userdata
    Size: integer
    Capacity: integer
    Data: {ImVec4}
end

global record ImGui
    userdata
    
    AcceptDragDropPayload: function(type: string, flags: integer): ImGuiPayload
	ActivateItemByID: function(id: any)
	AddContextHook: function(context: ImGuiContext, hook: ImGuiContextHook): any
	AddDrawListToDrawDataEx: function(draw_data: ImDrawData, out_list: ImVector_ImDrawListPtr, draw_list: ImDrawList)
	AddSettingsHandler: function(handler: ImGuiSettingsHandler)
	AlignTextToFramePadding: function()
	ArrowButton: function(str_id: string, dir: integer): boolean
	ArrowButtonEx: function(str_id: string, dir: integer, size_arg: ImVec2, flags: integer): boolean
	Begin: function(name: string, p_open: boolean, flags: integer): boolean, boolean
	BeginChild: function(str_id: string, size: ImVec2, border: boolean, flags: integer): boolean
	BeginChild: function(id: any, size: ImVec2, border: boolean, flags: integer): boolean
	BeginChildEx: function(name: string, id: any, size_arg: ImVec2, border: boolean, flags: integer): boolean
	BeginChildFrame: function(id: any, size: ImVec2, flags: integer): boolean
	BeginColumns: function(str_id: string, count: integer, flags: integer)
	BeginCombo: function(label: string, preview_value: string, flags: integer): boolean
	BeginComboPopup: function(popup_id: any, bb: ImRect, flags: integer): boolean
	BeginComboPreview: function(): boolean
	BeginDisabled: function(disabled: boolean)
	BeginDragDropSource: function(flags: integer): boolean
	BeginDragDropTarget: function(): boolean
	BeginDragDropTargetCustom: function(bb: ImRect, id: any): boolean
	BeginGroup: function()
	BeginItemTooltip: function(): boolean
	BeginListBox: function(label: string, size: ImVec2): boolean
	BeginMainMenuBar: function(): boolean
	BeginMenu: function(label: string, enabled: boolean): boolean
	BeginMenuBar: function(): boolean
	BeginMenuEx: function(label: string, icon: string, enabled: boolean): boolean
	BeginPopup: function(str_id: string, flags: integer): boolean
	BeginPopupContextItem: function(str_id: string, popup_flags: integer): boolean
	BeginPopupContextVoid: function(str_id: string, popup_flags: integer): boolean
	BeginPopupContextWindow: function(str_id: string, popup_flags: integer): boolean
	BeginPopupEx: function(id: any, extra_flags: integer): boolean
	BeginPopupModal: function(name: string, p_open: boolean, flags: integer): boolean, boolean
	BeginTabBar: function(str_id: string, flags: integer): boolean
	BeginTabBarEx: function(tab_bar: ImGuiTabBar, bb: ImRect, flags: integer): boolean
	BeginTabItem: function(label: string, p_open: boolean, flags: integer): boolean, boolean
	BeginTable: function(str_id: string, column: integer, flags: integer, outer_size: ImVec2, inner_width: number): boolean
	BeginTableEx: function(name: string, id: any, columns_count: integer, flags: integer, outer_size: ImVec2, inner_width: number): boolean
	BeginTooltip: function(): boolean
	BeginTooltipEx: function(tooltip_flags: integer, extra_window_flags: integer): boolean
	BeginViewportSideBar: function(name: string, viewport: ImGuiViewport, dir: integer, size: number, window_flags: integer): boolean
	BringWindowToDisplayBack: function(window: ImGuiWindow)
	BringWindowToDisplayBehind: function(window: ImGuiWindow, above_window: ImGuiWindow)
	BringWindowToDisplayFront: function(window: ImGuiWindow)
	BringWindowToFocusFront: function(window: ImGuiWindow)
	Bullet: function()
	BulletText: function(fmt: string, ...: any)
	BulletTextV: function(fmt: string, args: any)
	Button: function(label: string, size: ImVec2): boolean
	ButtonBehavior: function(bb: ImRect, id: any, out_hovered: boolean, out_held: boolean, flags: integer): boolean, boolean, boolean
	ButtonEx: function(label: string, size_arg: ImVec2, flags: integer): boolean
	CalcItemSize: function(pOut: ImVec2, size: ImVec2, default_w: number, default_h: number)
	CalcItemWidth: function(): number
	CalcTextSize: function(pOut: ImVec2, text: string, text_end: string, hide_text_after_double_hash: boolean, wrap_width: number)
	CalcTypematicRepeatAmount: function(t0: number, t1: number, repeat_delay: number, repeat_rate: number): integer
	CalcWindowNextAutoFitSize: function(pOut: ImVec2, window: ImGuiWindow)
	CalcWrapWidthForPos: function(pos: ImVec2, wrap_pos_x: number): number
	CallContextHooks: function(context: ImGuiContext, type: any)
	Checkbox: function(label: string, v: boolean): boolean, boolean
	CheckboxFlags: function(label: string, flags: integer, flags_value: integer): boolean, integer
	CheckboxFlags: function(label: string, flags: integer, flags_value: integer): boolean, integer
	CheckboxFlags: function(label: string, flags: any, flags_value: any): boolean
	CheckboxFlags: function(label: string, flags: any, flags_value: any): boolean
	ClearActiveID: function()
	ClearDragDrop: function()
	ClearIniSettings: function()
	ClearWindowSettings: function(name: string)
	CloseButton: function(id: any, pos: ImVec2): boolean
	CloseCurrentPopup: function()
	ClosePopupToLevel: function(remaining: integer, restore_focus_to_window_under_popup: boolean)
	ClosePopupsExceptModals: function()
	ClosePopupsOverWindow: function(ref_window: ImGuiWindow, restore_focus_to_window_under_popup: boolean)
	CollapseButton: function(id: any, pos: ImVec2): boolean
	CollapsingHeader: function(label: string, flags: integer): boolean
	CollapsingHeader: function(label: string, p_visible: boolean, flags: integer): boolean, boolean
	ColorButton: function(desc_id: string, col: ImVec4, flags: integer, size: ImVec2): boolean
	ColorConvertFloat4ToU32: function(in_: ImVec4): integer
	ColorConvertHSVtoRGB: function(h: number, s: number, v: number, out_r: number, out_g: number, out_b: number): number, number, number
	ColorConvertRGBtoHSV: function(r: number, g: number, b: number, out_h: number, out_s: number, out_v: number): number, number, number
	ColorConvertU32ToFloat4: function(pOut: ImVec4, in_: integer)
	ColorEdit3: function(label: string, col: any, flags: integer): boolean
	ColorEdit4: function(label: string, col: any, flags: integer): boolean
	ColorEditOptionsPopup: function(col: any, flags: integer)
	ColorPicker3: function(label: string, col: any, flags: integer): boolean
	ColorPicker4: function(label: string, col: any, flags: integer, ref_col: any): boolean
	ColorPickerOptionsPopup: function(ref_col: any, flags: integer)
	ColorTooltip: function(text: string, col: any, flags: integer)
	Columns: function(count: integer, id: string, border: boolean)
	Combo: function(label: string, current_item: integer, items: {any}, items_count: integer, popup_max_height_in_items: integer): boolean, integer
	Combo: function(label: string, current_item: integer, items_separated_by_zeros: string, popup_max_height_in_items: integer): boolean, integer
	Combo: function(label: string, current_item: integer, items_getter: any, data: any, items_count: integer, popup_max_height_in_items: integer): boolean, integer
	ConvertShortcutMod: function(key_chord: any): any
	ConvertSingleModFlagToKey: function(ctx: ImGuiContext, key: any): any
	CreateContext: function(shared_font_atlas: ImFontAtlas): ImGuiContext
	CreateNewWindowSettings: function(name: string): ImGuiWindowSettings
	DataTypeApplyFromText: function(buf: string, data_type: integer, p_data: any, format: string): boolean
	DataTypeApplyOp: function(data_type: integer, op: integer, output: any, arg_1: any, arg_2: any)
	DataTypeClamp: function(data_type: integer, p_data: any, p_min: any, p_max: any): boolean
	DataTypeCompare: function(data_type: integer, arg_1: any, arg_2: any): integer
	DataTypeFormatString: function(buf: any, buf_size: integer, data_type: integer, p_data: any, format: string): integer
	DataTypeGetInfo: function(data_type: integer): ImGuiDataTypeInfo
	DebugCheckVersionAndDataLayout: function(version_str: string, sz_io: any, sz_style: any, sz_vec2: any, sz_vec4: any, sz_drawvert: any, sz_drawidx: any): boolean
	DebugDrawCursorPos: function(col: integer)
	DebugDrawItemRect: function(col: integer)
	DebugDrawLineExtents: function(col: integer)
	DebugHookIdInfo: function(id: any, data_type: integer, data_id: any, data_id_end: any)
	DebugLocateItem: function(target_id: any)
	DebugLocateItemOnHover: function(target_id: any)
	DebugLocateItemResolveWithLastItem: function()
	DebugLog: function(fmt: string, ...: any)
	DebugLogV: function(fmt: string, args: any)
	DebugNodeColumns: function(columns: ImGuiOldColumns)
	DebugNodeDrawCmdShowMeshAndBoundingBox: function(out_draw_list: ImDrawList, draw_list: ImDrawList, draw_cmd: ImDrawCmd, show_mesh: boolean, show_aabb: boolean)
	DebugNodeDrawList: function(window: ImGuiWindow, viewport: ImGuiViewportP, draw_list: ImDrawList, label: string)
	DebugNodeFont: function(font: ImFont)
	DebugNodeFontGlyph: function(font: ImFont, glyph: ImFontGlyph)
	DebugNodeInputTextState: function(state: ImGuiInputTextState)
	DebugNodeStorage: function(storage: ImGuiStorage, label: string)
	DebugNodeTabBar: function(tab_bar: ImGuiTabBar, label: string)
	DebugNodeTable: function(table: ImGuiTable)
	DebugNodeTableSettings: function(settings: ImGuiTableSettings)
	DebugNodeViewport: function(viewport: ImGuiViewportP)
	DebugNodeWindow: function(window: ImGuiWindow, label: string)
	DebugNodeWindowSettings: function(settings: ImGuiWindowSettings)
	DebugNodeWindowsList: function(windows: ImVector_ImGuiWindowPtr, label: string)
	DebugNodeWindowsListByBeginStackParent: function(windows: any, windows_size: integer, parent_in_begin_stack: ImGuiWindow)
	DebugRenderKeyboardPreview: function(draw_list: ImDrawList)
	DebugRenderViewportThumbnail: function(draw_list: ImDrawList, viewport: ImGuiViewportP, bb: ImRect)
	DebugStartItemPicker: function()
	DebugTextEncoding: function(text: string)
	DestroyContext: function(ctx: ImGuiContext)
	DragBehavior: function(id: any, data_type: integer, p_v: any, v_speed: number, p_min: any, p_max: any, format: string, flags: integer): boolean
	DragFloat: function(label: string, v: number, v_speed: number, v_min: number, v_max: number, format: string, flags: integer): boolean, number
	DragFloat2: function(label: string, v: any, v_speed: number, v_min: number, v_max: number, format: string, flags: integer): boolean
	DragFloat3: function(label: string, v: any, v_speed: number, v_min: number, v_max: number, format: string, flags: integer): boolean
	DragFloat4: function(label: string, v: any, v_speed: number, v_min: number, v_max: number, format: string, flags: integer): boolean
	DragFloatRange2: function(label: string, v_current_min: number, v_current_max: number, v_speed: number, v_min: number, v_max: number, format: string, format_max: string, flags: integer): boolean, number, number
	DragInt: function(label: string, v: integer, v_speed: number, v_min: integer, v_max: integer, format: string, flags: integer): boolean, integer
	DragInt2: function(label: string, v: any, v_speed: number, v_min: integer, v_max: integer, format: string, flags: integer): boolean
	DragInt3: function(label: string, v: any, v_speed: number, v_min: integer, v_max: integer, format: string, flags: integer): boolean
	DragInt4: function(label: string, v: any, v_speed: number, v_min: integer, v_max: integer, format: string, flags: integer): boolean
	DragIntRange2: function(label: string, v_current_min: integer, v_current_max: integer, v_speed: number, v_min: integer, v_max: integer, format: string, format_max: string, flags: integer): boolean, integer, integer
	DragScalar: function(label: string, data_type: integer, p_data: any, v_speed: number, p_min: any, p_max: any, format: string, flags: integer): boolean
	DragScalarN: function(label: string, data_type: integer, p_data: any, components: integer, v_speed: number, p_min: any, p_max: any, format: string, flags: integer): boolean
	Dummy: function(size: ImVec2)
	End: function()
	EndChild: function()
	EndChildFrame: function()
	EndColumns: function()
	EndCombo: function()
	EndComboPreview: function()
	EndDisabled: function()
	EndDragDropSource: function()
	EndDragDropTarget: function()
	EndFrame: function()
	EndGroup: function()
	EndListBox: function()
	EndMainMenuBar: function()
	EndMenu: function()
	EndMenuBar: function()
	EndPopup: function()
	EndTabBar: function()
	EndTabItem: function()
	EndTable: function()
	EndTooltip: function()
	ErrorCheckEndFrameRecover: function(log_callback: any, user_data: any)
	ErrorCheckEndWindowRecover: function(log_callback: any, user_data: any)
	ErrorCheckUsingSetCursorPosToExtendParentBoundaries: function()
	FindBestWindowPosForPopup: function(pOut: ImVec2, window: ImGuiWindow)
	FindBestWindowPosForPopupEx: function(pOut: ImVec2, ref_pos: ImVec2, size: ImVec2, last_dir: integer, r_outer: ImRect, r_avoid: ImRect, policy: any)
	FindBlockingModal: function(window: ImGuiWindow): ImGuiWindow
	FindBottomMostVisibleWindowWithinBeginStack: function(window: ImGuiWindow): ImGuiWindow
	FindOrCreateColumns: function(window: ImGuiWindow, id: any): ImGuiOldColumns
	FindRenderedTextEnd: function(text: string, text_end: string): string
	FindSettingsHandler: function(type_name: string): ImGuiSettingsHandler
	FindWindowByID: function(id: any): ImGuiWindow
	FindWindowByName: function(name: string): ImGuiWindow
	FindWindowDisplayIndex: function(window: ImGuiWindow): integer
	FindWindowSettingsByID: function(id: any): ImGuiWindowSettings
	FindWindowSettingsByWindow: function(window: ImGuiWindow): ImGuiWindowSettings
	FocusItem: function()
	FocusTopMostWindowUnderOne: function(under_this_window: ImGuiWindow, ignore_window: ImGuiWindow, filter_viewport: ImGuiViewport, flags: integer)
	FocusWindow: function(window: ImGuiWindow, flags: integer)
	GcAwakeTransientWindowBuffers: function(window: ImGuiWindow)
	GcCompactTransientMiscBuffers: function()
	GcCompactTransientWindowBuffers: function(window: ImGuiWindow)
	GetActiveID: function(): any
	GetAllocatorFunctions: function(p_alloc_func: any, p_free_func: any, p_user_data: any)
	GetBackgroundDrawList: function(): ImDrawList
	GetBackgroundDrawList: function(viewport: ImGuiViewport): ImDrawList
	GetClipboardText: function(): string
	GetColorU32: function(idx: integer, alpha_mul: number): integer
	GetColorU32: function(col: ImVec4): integer
	GetColorU32: function(col: integer): integer
	GetColumnIndex: function(): integer
	GetColumnNormFromOffset: function(columns: ImGuiOldColumns, offset: number): number
	GetColumnOffset: function(column_index: integer): number
	GetColumnOffsetFromNorm: function(columns: ImGuiOldColumns, offset_norm: number): number
	GetColumnWidth: function(column_index: integer): number
	GetColumnsCount: function(): integer
	GetColumnsID: function(str_id: string, count: integer): any
	GetContentRegionAvail: function(pOut: ImVec2)
	GetContentRegionMax: function(pOut: ImVec2)
	GetContentRegionMaxAbs: function(pOut: ImVec2)
	GetCurrentContext: function(): ImGuiContext
	GetCurrentFocusScope: function(): any
	GetCurrentTabBar: function(): ImGuiTabBar
	GetCurrentTable: function(): ImGuiTable
	GetCurrentWindow: function(): ImGuiWindow
	GetCurrentWindowRead: function(): ImGuiWindow
	GetCursorPos: function(pOut: ImVec2)
	GetCursorPosX: function(): number
	GetCursorPosY: function(): number
	GetCursorScreenPos: function(pOut: ImVec2)
	GetCursorStartPos: function(pOut: ImVec2)
	GetDefaultFont: function(): ImFont
	GetDragDropPayload: function(): ImGuiPayload
	GetDrawData: function(): ImDrawData
	GetDrawListSharedData: function(): ImDrawListSharedData
	GetFocusID: function(): any
	GetFont: function(): ImFont
	GetFontSize: function(): number
	GetFontTexUvWhitePixel: function(pOut: ImVec2)
	GetForegroundDrawList: function(): ImDrawList
	GetForegroundDrawList: function(window: ImGuiWindow): ImDrawList
	GetForegroundDrawList: function(viewport: ImGuiViewport): ImDrawList
	GetFrameCount: function(): integer
	GetFrameHeight: function(): number
	GetFrameHeightWithSpacing: function(): number
	GetHoveredID: function(): any
	GetID: function(str_id: string): any
	GetID: function(str_id_begin: string, str_id_end: string): any
	GetID: function(ptr_id: any): any
	GetIDWithSeed: function(str_id_begin: string, str_id_end: string, seed: any): any
	GetIDWithSeed: function(n: integer, seed: any): any
	GetIO: function(): ImGuiIO
	GetInputTextState: function(id: any): ImGuiInputTextState
	GetItemFlags: function(): integer
	GetItemID: function(): any
	GetItemRectMax: function(pOut: ImVec2)
	GetItemRectMin: function(pOut: ImVec2)
	GetItemRectSize: function(pOut: ImVec2)
	GetItemStatusFlags: function(): integer
	GetKeyChordName: function(key_chord: any, out_buf: any, out_buf_size: integer)
	GetKeyData: function(ctx: ImGuiContext, key: any): ImGuiKeyData
	GetKeyData: function(key: any): ImGuiKeyData
	GetKeyIndex: function(key: any): any
	GetKeyMagnitude2d: function(pOut: ImVec2, key_left: any, key_right: any, key_up: any, key_down: any)
	GetKeyName: function(key: any): string
	GetKeyOwner: function(key: any): any
	GetKeyOwnerData: function(ctx: ImGuiContext, key: any): ImGuiKeyOwnerData
	GetKeyPressedAmount: function(key: any, repeat_delay: number, rate: number): integer
	GetMainViewport: function(): ImGuiViewport
	GetMouseClickedCount: function(button: integer): integer
	GetMouseCursor: function(): integer
	GetMouseDragDelta: function(pOut: ImVec2, button: integer, lock_threshold: number)
	GetMousePos: function(pOut: ImVec2)
	GetMousePosOnOpeningCurrentPopup: function(pOut: ImVec2)
	GetNavTweakPressedAmount: function(axis: any): number
	GetPopupAllowedExtentRect: function(pOut: ImRect, window: ImGuiWindow)
	GetScrollMaxX: function(): number
	GetScrollMaxY: function(): number
	GetScrollX: function(): number
	GetScrollY: function(): number
	GetShortcutRoutingData: function(key_chord: any): ImGuiKeyRoutingData
	GetStateStorage: function(): ImGuiStorage
	GetStyle: function(): ImGuiStyle
	GetStyleColorName: function(idx: integer): string
	GetStyleColorVec4: function(idx: integer): ImVec4
	GetStyleVarInfo: function(idx: integer): ImGuiDataVarInfo
	GetTextLineHeight: function(): number
	GetTextLineHeightWithSpacing: function(): number
	GetTime: function(): number
	GetTopMostAndVisiblePopupModal: function(): ImGuiWindow
	GetTopMostPopupModal: function(): ImGuiWindow
	GetTreeNodeToLabelSpacing: function(): number
	GetTypematicRepeatRate: function(flags: integer, repeat_delay: number, repeat_rate: number): number, number
	GetVersion: function(): string
	GetWindowContentRegionMax: function(pOut: ImVec2)
	GetWindowContentRegionMin: function(pOut: ImVec2)
	GetWindowDrawList: function(): ImDrawList
	GetWindowHeight: function(): number
	GetWindowPos: function(pOut: ImVec2)
	GetWindowResizeBorderID: function(window: ImGuiWindow, dir: integer): any
	GetWindowResizeCornerID: function(window: ImGuiWindow, n: integer): any
	GetWindowScrollbarID: function(window: ImGuiWindow, axis: any): any
	GetWindowScrollbarRect: function(pOut: ImRect, window: ImGuiWindow, axis: any)
	GetWindowSize: function(pOut: ImVec2)
	GetWindowWidth: function(): number
	Image: function(user_texture_id: any, size: ImVec2, uv0: ImVec2, uv1: ImVec2, tint_col: ImVec4, border_col: ImVec4)
	ImageButton: function(str_id: string, user_texture_id: any, size: ImVec2, uv0: ImVec2, uv1: ImVec2, bg_col: ImVec4, tint_col: ImVec4): boolean
	ImageButtonEx: function(id: any, texture_id: any, size: ImVec2, uv0: ImVec2, uv1: ImVec2, bg_col: ImVec4, tint_col: ImVec4, flags: integer): boolean
	Indent: function(indent_w: number)
	Initialize: function()
	InputDouble: function(label: string, v: number, step: number, step_fast: number, format: string, flags: integer): boolean, number
	InputFloat: function(label: string, v: number, step: number, step_fast: number, format: string, flags: integer): boolean, number
	InputFloat2: function(label: string, v: any, format: string, flags: integer): boolean
	InputFloat3: function(label: string, v: any, format: string, flags: integer): boolean
	InputFloat4: function(label: string, v: any, format: string, flags: integer): boolean
	InputInt: function(label: string, v: integer, step: integer, step_fast: integer, flags: integer): boolean, integer
	InputInt2: function(label: string, v: any, flags: integer): boolean
	InputInt3: function(label: string, v: any, flags: integer): boolean
	InputInt4: function(label: string, v: any, flags: integer): boolean
	InputScalar: function(label: string, data_type: integer, p_data: any, p_step: any, p_step_fast: any, format: string, flags: integer): boolean
	InputScalarN: function(label: string, data_type: integer, p_data: any, components: integer, p_step: any, p_step_fast: any, format: string, flags: integer): boolean
	InputText: function(label: string, buf: any, buf_size: any, flags: integer, callback: any, user_data: any): boolean
	InputTextDeactivateHook: function(id: any)
	InputTextEx: function(label: string, hint: string, buf: any, buf_size: integer, size_arg: ImVec2, flags: integer, callback: any, user_data: any): boolean
	InputTextMultiline: function(label: string, buf: any, buf_size: any, size: ImVec2, flags: integer, callback: any, user_data: any): boolean
	InputTextWithHint: function(label: string, hint: string, buf: any, buf_size: any, flags: integer, callback: any, user_data: any): boolean
	InvisibleButton: function(str_id: string, size: ImVec2, flags: integer): boolean
	IsActiveIdUsingNavDir: function(dir: integer): boolean
	IsAliasKey: function(key: any): boolean
	IsAnyItemActive: function(): boolean
	IsAnyItemFocused: function(): boolean
	IsAnyItemHovered: function(): boolean
	IsAnyMouseDown: function(): boolean
	IsClippedEx: function(bb: ImRect, id: any): boolean
	IsDragDropActive: function(): boolean
	IsDragDropPayloadBeingAccepted: function(): boolean
	IsGamepadKey: function(key: any): boolean
	IsItemActivated: function(): boolean
	IsItemActive: function(): boolean
	IsItemClicked: function(mouse_button: integer): boolean
	IsItemDeactivated: function(): boolean
	IsItemDeactivatedAfterEdit: function(): boolean
	IsItemEdited: function(): boolean
	IsItemFocused: function(): boolean
	IsItemHovered: function(flags: integer): boolean
	IsItemToggledOpen: function(): boolean
	IsItemToggledSelection: function(): boolean
	IsItemVisible: function(): boolean
	IsKeyDown: function(key: any): boolean
	IsKeyDown: function(key: any, owner_id: any): boolean
	IsKeyPressed: function(key: any, repeat_: boolean): boolean
	IsKeyPressed: function(key: any, owner_id: any, flags: integer): boolean
	IsKeyPressedMap: function(key: any, repeat_: boolean): boolean
	IsKeyReleased: function(key: any): boolean
	IsKeyReleased: function(key: any, owner_id: any): boolean
	IsKeyboardKey: function(key: any): boolean
	IsLegacyKey: function(key: any): boolean
	IsMouseClicked: function(button: integer, repeat_: boolean): boolean
	IsMouseClicked: function(button: integer, owner_id: any, flags: integer): boolean
	IsMouseDoubleClicked: function(button: integer): boolean
	IsMouseDown: function(button: integer): boolean
	IsMouseDown: function(button: integer, owner_id: any): boolean
	IsMouseDragPastThreshold: function(button: integer, lock_threshold: number): boolean
	IsMouseDragging: function(button: integer, lock_threshold: number): boolean
	IsMouseHoveringRect: function(r_min: ImVec2, r_max: ImVec2, clip: boolean): boolean
	IsMouseKey: function(key: any): boolean
	IsMousePosValid: function(mouse_pos: ImVec2): boolean
	IsMouseReleased: function(button: integer): boolean
	IsMouseReleased: function(button: integer, owner_id: any): boolean
	IsNamedKey: function(key: any): boolean
	IsNamedKeyOrModKey: function(key: any): boolean
	IsPopupOpen: function(str_id: string, flags: integer): boolean
	IsPopupOpen: function(id: any, popup_flags: integer): boolean
	IsRectVisible: function(size: ImVec2): boolean
	IsRectVisible: function(rect_min: ImVec2, rect_max: ImVec2): boolean
	IsWindowAbove: function(potential_above: ImGuiWindow, potential_below: ImGuiWindow): boolean
	IsWindowAppearing: function(): boolean
	IsWindowChildOf: function(window: ImGuiWindow, potential_parent: ImGuiWindow, popup_hierarchy: boolean): boolean
	IsWindowCollapsed: function(): boolean
	IsWindowContentHoverable: function(window: ImGuiWindow, flags: integer): boolean
	IsWindowFocused: function(flags: integer): boolean
	IsWindowHovered: function(flags: integer): boolean
	IsWindowNavFocusable: function(window: ImGuiWindow): boolean
	IsWindowWithinBeginStackOf: function(window: ImGuiWindow, potential_parent: ImGuiWindow): boolean
	ItemAdd: function(bb: ImRect, id: any, nav_bb: ImRect, extra_flags: integer): boolean
	ItemHoverable: function(bb: ImRect, id: any, item_flags: integer): boolean
	ItemSize: function(size: ImVec2, text_baseline_y: number)
	ItemSize: function(bb: ImRect, text_baseline_y: number)
	KeepAliveID: function(id: any)
	LabelText: function(label: string, fmt: string, ...: any)
	LabelTextV: function(label: string, fmt: string, args: any)
	ListBox: function(label: string, current_item: integer, items: {any}, items_count: integer, height_in_items: integer): boolean, integer
	ListBox: function(label: string, current_item: integer, items_getter: any, data: any, items_count: integer, height_in_items: integer): boolean, integer
	LoadIniSettingsFromDisk: function(ini_filename: string)
	LoadIniSettingsFromMemory: function(ini_data: string, ini_size: any)
	LocalizeGetMsg: function(key: any): string
	LocalizeRegisterEntries: function(entries: ImGuiLocEntry, count: integer)
	LogBegin: function(type: any, auto_open_depth: integer)
	LogButtons: function()
	LogFinish: function()
	LogRenderedText: function(ref_pos: ImVec2, text: string, text_end: string)
	LogSetNextTextDecoration: function(prefix: string, suffix: string)
	LogText: function(fmt: string, ...: any)
	LogTextV: function(fmt: string, args: any)
	LogToBuffer: function(auto_open_depth: integer)
	LogToClipboard: function(auto_open_depth: integer)
	LogToFile: function(auto_open_depth: integer, filename: string)
	LogToTTY: function(auto_open_depth: integer)
	MarkIniSettingsDirty: function()
	MarkIniSettingsDirty: function(window: ImGuiWindow)
	MarkItemEdited: function(id: any)
	MemAlloc: function(size: any): any
	MemFree: function(ptr: any)
	MenuItem: function(label: string, shortcut: string, selected: boolean, enabled: boolean): boolean
	MenuItem: function(label: string, shortcut: string, p_selected: boolean, enabled: boolean): boolean, boolean
	MenuItemEx: function(label: string, icon: string, shortcut: string, selected: boolean, enabled: boolean): boolean
	MouseButtonToKey: function(button: integer): any
	NavClearPreferredPosForAxis: function(axis: any)
	NavInitRequestApplyResult: function()
	NavInitWindow: function(window: ImGuiWindow, force_reinit: boolean)
	NavMoveRequestApplyResult: function()
	NavMoveRequestButNoResultYet: function(): boolean
	NavMoveRequestCancel: function()
	NavMoveRequestForward: function(move_dir: integer, clip_dir: integer, move_flags: integer, scroll_flags: integer)
	NavMoveRequestResolveWithLastItem: function(result: ImGuiNavItemData)
	NavMoveRequestResolveWithPastTreeNode: function(result: ImGuiNavItemData, tree_node_data: ImGuiNavTreeNodeData)
	NavMoveRequestSubmit: function(move_dir: integer, clip_dir: integer, move_flags: integer, scroll_flags: integer)
	NavMoveRequestTryWrapping: function(window: ImGuiWindow, move_flags: integer)
	NavUpdateCurrentWindowIsScrollPushableX: function()
	NewFrame: function()
	NewLine: function()
	NextColumn: function()
	OpenPopup: function(str_id: string, popup_flags: integer)
	OpenPopup: function(id: any, popup_flags: integer)
	OpenPopupEx: function(id: any, popup_flags: integer)
	OpenPopupOnItemClick: function(str_id: string, popup_flags: integer)
	PlotEx: function(plot_type: any, label: string, values_getter: any, data: any, values_count: integer, values_offset: integer, overlay_text: string, scale_min: number, scale_max: number, size_arg: ImVec2): integer
	PlotHistogram: function(label: string, values: any, values_count: integer, values_offset: integer, overlay_text: string, scale_min: number, scale_max: number, graph_size: ImVec2, stride: integer)
	PlotHistogram: function(label: string, values_getter: any, data: any, values_count: integer, values_offset: integer, overlay_text: string, scale_min: number, scale_max: number, graph_size: ImVec2)
	PlotLines: function(label: string, values: any, values_count: integer, values_offset: integer, overlay_text: string, scale_min: number, scale_max: number, graph_size: ImVec2, stride: integer)
	PlotLines: function(label: string, values_getter: any, data: any, values_count: integer, values_offset: integer, overlay_text: string, scale_min: number, scale_max: number, graph_size: ImVec2)
	PopButtonRepeat: function()
	PopClipRect: function()
	PopColumnsBackground: function()
	PopFocusScope: function()
	PopFont: function()
	PopID: function()
	PopItemFlag: function()
	PopItemWidth: function()
	PopStyleColor: function(count: integer)
	PopStyleVar: function(count: integer)
	PopTabStop: function()
	PopTextWrapPos: function()
	ProgressBar: function(fraction: number, size_arg: ImVec2, overlay: string)
	PushButtonRepeat: function(repeat_: boolean)
	PushClipRect: function(clip_rect_min: ImVec2, clip_rect_max: ImVec2, intersect_with_current_clip_rect: boolean)
	PushColumnClipRect: function(column_index: integer)
	PushColumnsBackground: function()
	PushFocusScope: function(id: any)
	PushFont: function(font: ImFont)
	PushID: function(str_id: string)
	PushID: function(str_id_begin: string, str_id_end: string)
	PushID: function(ptr_id: any)
	PushID: function(int_id: integer)
	PushItemFlag: function(option: integer, enabled: boolean)
	PushItemWidth: function(item_width: number)
	PushMultiItemsWidths: function(components: integer, width_full: number)
	PushOverrideID: function(id: any)
	PushStyleColor: function(idx: integer, col: integer)
	PushStyleColor: function(idx: integer, col: ImVec4)
	PushStyleVar: function(idx: integer, val: number)
	PushStyleVar: function(idx: integer, val: ImVec2)
	PushTabStop: function(tab_stop: boolean)
	PushTextWrapPos: function(wrap_local_pos_x: number)
	RadioButton: function(label: string, active: boolean): boolean
	RadioButton: function(label: string, v: integer, v_button: integer): boolean, integer
	RemoveContextHook: function(context: ImGuiContext, hook_to_remove: any)
	RemoveSettingsHandler: function(type_name: string)
	Render: function()
	RenderArrow: function(draw_list: ImDrawList, pos: ImVec2, col: integer, dir: integer, scale: number)
	RenderArrowPointingAt: function(draw_list: ImDrawList, pos: ImVec2, half_sz: ImVec2, direction: integer, col: integer)
	RenderBullet: function(draw_list: ImDrawList, pos: ImVec2, col: integer)
	RenderCheckMark: function(draw_list: ImDrawList, pos: ImVec2, col: integer, sz: number)
	RenderColorRectWithAlphaCheckerboard: function(draw_list: ImDrawList, p_min: ImVec2, p_max: ImVec2, fill_col: integer, grid_step: number, grid_off: ImVec2, rounding: number, flags: integer)
	RenderDragDropTargetRect: function(bb: ImRect)
	RenderFrame: function(p_min: ImVec2, p_max: ImVec2, fill_col: integer, border: boolean, rounding: number)
	RenderFrameBorder: function(p_min: ImVec2, p_max: ImVec2, rounding: number)
	RenderMouseCursor: function(pos: ImVec2, scale: number, mouse_cursor: integer, col_fill: integer, col_border: integer, col_shadow: integer)
	RenderNavHighlight: function(bb: ImRect, id: any, flags: integer)
	RenderRectFilledRangeH: function(draw_list: ImDrawList, rect: ImRect, col: integer, x_start_norm: number, x_end_norm: number, rounding: number)
	RenderRectFilledWithHole: function(draw_list: ImDrawList, outer: ImRect, inner: ImRect, col: integer, rounding: number)
	RenderText: function(pos: ImVec2, text: string, text_end: string, hide_text_after_hash: boolean)
	RenderTextClipped: function(pos_min: ImVec2, pos_max: ImVec2, text: string, text_end: string, text_size_if_known: ImVec2, align: ImVec2, clip_rect: ImRect)
	RenderTextClippedEx: function(draw_list: ImDrawList, pos_min: ImVec2, pos_max: ImVec2, text: string, text_end: string, text_size_if_known: ImVec2, align: ImVec2, clip_rect: ImRect)
	RenderTextEllipsis: function(draw_list: ImDrawList, pos_min: ImVec2, pos_max: ImVec2, clip_max_x: number, ellipsis_max_x: number, text: string, text_end: string, text_size_if_known: ImVec2)
	RenderTextWrapped: function(pos: ImVec2, text: string, text_end: string, wrap_width: number)
	ResetMouseDragDelta: function(button: integer)
	SameLine: function(offset_from_start_x: number, spacing: number)
	SaveIniSettingsToDisk: function(ini_filename: string)
	SaveIniSettingsToMemory: function(out_ini_size: any): string
	ScrollToBringRectIntoView: function(window: ImGuiWindow, rect: ImRect)
	ScrollToItem: function(flags: integer)
	ScrollToRect: function(window: ImGuiWindow, rect: ImRect, flags: integer)
	ScrollToRectEx: function(pOut: ImVec2, window: ImGuiWindow, rect: ImRect, flags: integer)
	Scrollbar: function(axis: any)
	ScrollbarEx: function(bb: ImRect, id: any, axis: any, p_scroll_v: any, avail_v: any, contents_v: any, flags: integer): boolean
	Selectable: function(label: string, selected: boolean, flags: integer, size: ImVec2): boolean
	Selectable: function(label: string, p_selected: boolean, flags: integer, size: ImVec2): boolean, boolean
	Separator: function()
	SeparatorEx: function(flags: integer, thickness: number)
	SeparatorText: function(label: string)
	SeparatorTextEx: function(id: any, label: string, label_end: string, extra_width: number)
	SetActiveID: function(id: any, window: ImGuiWindow)
	SetActiveIdUsingAllKeyboardKeys: function()
	SetAllocatorFunctions: function(alloc_func: any, free_func: any, user_data: any)
	SetClipboardText: function(text: string)
	SetColorEditOptions: function(flags: integer)
	SetColumnOffset: function(column_index: integer, offset_x: number)
	SetColumnWidth: function(column_index: integer, width: number)
	SetCurrentContext: function(ctx: ImGuiContext)
	SetCurrentFont: function(font: ImFont)
	SetCursorPos: function(local_pos: ImVec2)
	SetCursorPosX: function(local_x: number)
	SetCursorPosY: function(local_y: number)
	SetCursorScreenPos: function(pos: ImVec2)
	SetDragDropPayload: function(type: string, data: any, sz: any, cond: integer): boolean
	SetFocusID: function(id: any, window: ImGuiWindow)
	SetHoveredID: function(id: any)
	SetItemDefaultFocus: function()
	SetItemKeyOwner: function(key: any, flags: integer)
	SetItemTooltip: function(fmt: string, ...: any)
	SetItemTooltipV: function(fmt: string, args: any)
	SetKeyOwner: function(key: any, owner_id: any, flags: integer)
	SetKeyOwnersForKeyChord: function(key: any, owner_id: any, flags: integer)
	SetKeyboardFocusHere: function(offset: integer)
	SetLastItemData: function(item_id: any, in_flags: integer, status_flags: integer, item_rect: ImRect)
	SetMouseCursor: function(cursor_type: integer)
	SetNavID: function(id: any, nav_layer: any, focus_scope_id: any, rect_rel: ImRect)
	SetNavWindow: function(window: ImGuiWindow)
	SetNextFrameWantCaptureKeyboard: function(want_capture_keyboard: boolean)
	SetNextFrameWantCaptureMouse: function(want_capture_mouse: boolean)
	SetNextItemAllowOverlap: function()
	SetNextItemOpen: function(is_open: boolean, cond: integer)
	SetNextItemWidth: function(item_width: number)
	SetNextWindowBgAlpha: function(alpha: number)
	SetNextWindowCollapsed: function(collapsed: boolean, cond: integer)
	SetNextWindowContentSize: function(size: ImVec2)
	SetNextWindowFocus: function()
	SetNextWindowPos: function(pos: ImVec2, cond: integer, pivot: ImVec2)
	SetNextWindowScroll: function(scroll: ImVec2)
	SetNextWindowSize: function(size: ImVec2, cond: integer)
	SetNextWindowSizeConstraints: function(size_min: ImVec2, size_max: ImVec2, custom_callback: any, custom_callback_data: any)
	SetScrollFromPosX: function(local_x: number, center_x_ratio: number)
	SetScrollFromPosX: function(window: ImGuiWindow, local_x: number, center_x_ratio: number)
	SetScrollFromPosY: function(local_y: number, center_y_ratio: number)
	SetScrollFromPosY: function(window: ImGuiWindow, local_y: number, center_y_ratio: number)
	SetScrollHereX: function(center_x_ratio: number)
	SetScrollHereY: function(center_y_ratio: number)
	SetScrollX: function(scroll_x: number)
	SetScrollX: function(window: ImGuiWindow, scroll_x: number)
	SetScrollY: function(scroll_y: number)
	SetScrollY: function(window: ImGuiWindow, scroll_y: number)
	SetShortcutRouting: function(key_chord: any, owner_id: any, flags: integer): boolean
	SetStateStorage: function(storage: ImGuiStorage)
	SetTabItemClosed: function(tab_or_docked_window_label: string)
	SetTooltip: function(fmt: string, ...: any)
	SetTooltipV: function(fmt: string, args: any)
	SetWindowClipRectBeforeSetChannel: function(window: ImGuiWindow, clip_rect: ImRect)
	SetWindowCollapsed: function(collapsed: boolean, cond: integer)
	SetWindowCollapsed: function(name: string, collapsed: boolean, cond: integer)
	SetWindowCollapsed: function(window: ImGuiWindow, collapsed: boolean, cond: integer)
	SetWindowFocus: function()
	SetWindowFocus: function(name: string)
	SetWindowFontScale: function(scale: number)
	SetWindowHiddendAndSkipItemsForCurrentFrame: function(window: ImGuiWindow)
	SetWindowHitTestHole: function(window: ImGuiWindow, pos: ImVec2, size: ImVec2)
	SetWindowPos: function(pos: ImVec2, cond: integer)
	SetWindowPos: function(name: string, pos: ImVec2, cond: integer)
	SetWindowPos: function(window: ImGuiWindow, pos: ImVec2, cond: integer)
	SetWindowSize: function(size: ImVec2, cond: integer)
	SetWindowSize: function(name: string, size: ImVec2, cond: integer)
	SetWindowSize: function(window: ImGuiWindow, size: ImVec2, cond: integer)
	SetWindowViewport: function(window: ImGuiWindow, viewport: ImGuiViewportP)
	ShadeVertsLinearColorGradientKeepAlpha: function(draw_list: ImDrawList, vert_start_idx: integer, vert_end_idx: integer, gradient_p0: ImVec2, gradient_p1: ImVec2, col0: integer, col1: integer)
	ShadeVertsLinearUV: function(draw_list: ImDrawList, vert_start_idx: integer, vert_end_idx: integer, a: ImVec2, b: ImVec2, uv_a: ImVec2, uv_b: ImVec2, clamp: boolean)
	Shortcut: function(key_chord: any, owner_id: any, flags: integer): boolean
	ShowAboutWindow: function(p_open: boolean): boolean
	ShowDebugLogWindow: function(p_open: boolean): boolean
	ShowDemoWindow: function(p_open: boolean): boolean
	ShowFontAtlas: function(atlas: ImFontAtlas)
	ShowFontSelector: function(label: string)
	ShowMetricsWindow: function(p_open: boolean): boolean
	ShowStackToolWindow: function(p_open: boolean): boolean
	ShowStyleEditor: function(ref: ImGuiStyle)
	ShowStyleSelector: function(label: string): boolean
	ShowUserGuide: function()
	ShrinkWidths: function(items: ImGuiShrinkWidthItem, count: integer, width_excess: number)
	Shutdown: function()
	SliderAngle: function(label: string, v_rad: number, v_degrees_min: number, v_degrees_max: number, format: string, flags: integer): boolean, number
	SliderBehavior: function(bb: ImRect, id: any, data_type: integer, p_v: any, p_min: any, p_max: any, format: string, flags: integer, out_grab_bb: ImRect): boolean
	SliderFloat: function(label: string, v: number, v_min: number, v_max: number, format: string, flags: integer): boolean, number
	SliderFloat2: function(label: string, v: any, v_min: number, v_max: number, format: string, flags: integer): boolean
	SliderFloat3: function(label: string, v: any, v_min: number, v_max: number, format: string, flags: integer): boolean
	SliderFloat4: function(label: string, v: any, v_min: number, v_max: number, format: string, flags: integer): boolean
	SliderInt: function(label: string, v: integer, v_min: integer, v_max: integer, format: string, flags: integer): boolean, integer
	SliderInt2: function(label: string, v: any, v_min: integer, v_max: integer, format: string, flags: integer): boolean
	SliderInt3: function(label: string, v: any, v_min: integer, v_max: integer, format: string, flags: integer): boolean
	SliderInt4: function(label: string, v: any, v_min: integer, v_max: integer, format: string, flags: integer): boolean
	SliderScalar: function(label: string, data_type: integer, p_data: any, p_min: any, p_max: any, format: string, flags: integer): boolean
	SliderScalarN: function(label: string, data_type: integer, p_data: any, components: integer, p_min: any, p_max: any, format: string, flags: integer): boolean
	SmallButton: function(label: string): boolean
	Spacing: function()
	SplitterBehavior: function(bb: ImRect, id: any, axis: any, size1: number, size2: number, min_size1: number, min_size2: number, hover_extend: number, hover_visibility_delay: number, bg_col: integer): boolean, number, number
	StartMouseMovingWindow: function(window: ImGuiWindow)
	StyleColorsClassic: function(dst: ImGuiStyle)
	StyleColorsDark: function(dst: ImGuiStyle)
	StyleColorsLight: function(dst: ImGuiStyle)
	TabBarCloseTab: function(tab_bar: ImGuiTabBar, tab: ImGuiTabItem)
	TabBarFindTabByID: function(tab_bar: ImGuiTabBar, tab_id: any): ImGuiTabItem
	TabBarFindTabByOrder: function(tab_bar: ImGuiTabBar, order: integer): ImGuiTabItem
	TabBarGetCurrentTab: function(tab_bar: ImGuiTabBar): ImGuiTabItem
	TabBarGetTabName: function(tab_bar: ImGuiTabBar, tab: ImGuiTabItem): string
	TabBarGetTabOrder: function(tab_bar: ImGuiTabBar, tab: ImGuiTabItem): integer
	TabBarProcessReorder: function(tab_bar: ImGuiTabBar): boolean
	TabBarQueueFocus: function(tab_bar: ImGuiTabBar, tab: ImGuiTabItem)
	TabBarQueueReorder: function(tab_bar: ImGuiTabBar, tab: ImGuiTabItem, offset: integer)
	TabBarQueueReorderFromMousePos: function(tab_bar: ImGuiTabBar, tab: ImGuiTabItem, mouse_pos: ImVec2)
	TabBarRemoveTab: function(tab_bar: ImGuiTabBar, tab_id: any)
	TabItemBackground: function(draw_list: ImDrawList, bb: ImRect, flags: integer, col: integer)
	TabItemButton: function(label: string, flags: integer): boolean
	TabItemCalcSize: function(pOut: ImVec2, label: string, has_close_button_or_unsaved_marker: boolean)
	TabItemCalcSize: function(pOut: ImVec2, window: ImGuiWindow)
	TabItemEx: function(tab_bar: ImGuiTabBar, label: string, p_open: boolean, flags: integer, docked_window: ImGuiWindow): boolean, boolean
	TabItemLabelAndCloseButton: function(draw_list: ImDrawList, bb: ImRect, flags: integer, frame_padding: ImVec2, label: string, tab_id: any, close_button_id: any, is_contents_visible: boolean, out_just_closed: boolean, out_text_clipped: boolean): boolean, boolean
	TableBeginApplyRequests: function(table: ImGuiTable)
	TableBeginCell: function(table: ImGuiTable, column_n: integer)
	TableBeginContextMenuPopup: function(table: ImGuiTable): boolean
	TableBeginInitMemory: function(table: ImGuiTable, columns_count: integer)
	TableBeginRow: function(table: ImGuiTable)
	TableDrawBorders: function(table: ImGuiTable)
	TableDrawContextMenu: function(table: ImGuiTable)
	TableEndCell: function(table: ImGuiTable)
	TableEndRow: function(table: ImGuiTable)
	TableFindByID: function(id: any): ImGuiTable
	TableFixColumnSortDirection: function(table: ImGuiTable, column: ImGuiTableColumn)
	TableGcCompactSettings: function()
	TableGcCompactTransientBuffers: function(table: ImGuiTable)
	TableGcCompactTransientBuffers: function(table: ImGuiTableTempData)
	TableGetBoundSettings: function(table: ImGuiTable): ImGuiTableSettings
	TableGetCellBgRect: function(pOut: ImRect, table: ImGuiTable, column_n: integer)
	TableGetColumnCount: function(): integer
	TableGetColumnFlags: function(column_n: integer): integer
	TableGetColumnIndex: function(): integer
	TableGetColumnName: function(column_n: integer): string
	TableGetColumnName: function(table: ImGuiTable, column_n: integer): string
	TableGetColumnNextSortDirection: function(column: ImGuiTableColumn): integer
	TableGetColumnResizeID: function(table: ImGuiTable, column_n: integer, instance_no: integer): any
	TableGetColumnWidthAuto: function(table: ImGuiTable, column: ImGuiTableColumn): number
	TableGetHeaderRowHeight: function(): number
	TableGetHoveredColumn: function(): integer
	TableGetHoveredRow: function(): integer
	TableGetInstanceData: function(table: ImGuiTable, instance_no: integer): ImGuiTableInstanceData
	TableGetInstanceID: function(table: ImGuiTable, instance_no: integer): any
	TableGetMaxColumnWidth: function(table: ImGuiTable, column_n: integer): number
	TableGetRowIndex: function(): integer
	TableGetSortSpecs: function(): ImGuiTableSortSpecs
	TableHeader: function(label: string)
	TableHeadersRow: function()
	TableLoadSettings: function(table: ImGuiTable)
	TableMergeDrawChannels: function(table: ImGuiTable)
	TableNextColumn: function(): boolean
	TableNextRow: function(row_flags: integer, min_row_height: number)
	TableOpenContextMenu: function(column_n: integer)
	TablePopBackgroundChannel: function()
	TablePushBackgroundChannel: function()
	TableRemove: function(table: ImGuiTable)
	TableResetSettings: function(table: ImGuiTable)
	TableSaveSettings: function(table: ImGuiTable)
	TableSetBgColor: function(target: integer, color: integer, column_n: integer)
	TableSetColumnEnabled: function(column_n: integer, v: boolean)
	TableSetColumnIndex: function(column_n: integer): boolean
	TableSetColumnSortDirection: function(column_n: integer, sort_direction: integer, append_to_sort_specs: boolean)
	TableSetColumnWidth: function(column_n: integer, width: number)
	TableSetColumnWidthAutoAll: function(table: ImGuiTable)
	TableSetColumnWidthAutoSingle: function(table: ImGuiTable, column_n: integer)
	TableSettingsAddSettingsHandler: function()
	TableSettingsCreate: function(id: any, columns_count: integer): ImGuiTableSettings
	TableSettingsFindByID: function(id: any): ImGuiTableSettings
	TableSetupColumn: function(label: string, flags: integer, init_width_or_weight: number, user_id: any)
	TableSetupDrawChannels: function(table: ImGuiTable)
	TableSetupScrollFreeze: function(cols: integer, rows: integer)
	TableSortSpecsBuild: function(table: ImGuiTable)
	TableSortSpecsSanitize: function(table: ImGuiTable)
	TableUpdateBorders: function(table: ImGuiTable)
	TableUpdateColumnsWeightFromWidth: function(table: ImGuiTable)
	TableUpdateLayout: function(table: ImGuiTable)
	TempInputIsActive: function(id: any): boolean
	TempInputScalar: function(bb: ImRect, id: any, label: string, data_type: integer, p_data: any, format: string, p_clamp_min: any, p_clamp_max: any): boolean
	TempInputText: function(bb: ImRect, id: any, label: string, buf: any, buf_size: integer, flags: integer): boolean
	TestKeyOwner: function(key: any, owner_id: any): boolean
	TestShortcutRouting: function(key_chord: any, owner_id: any): boolean
	Text: function(fmt: string, ...: any)
	TextColored: function(col: ImVec4, fmt: string, ...: any)
	TextColoredV: function(col: ImVec4, fmt: string, args: any)
	TextDisabled: function(fmt: string, ...: any)
	TextDisabledV: function(fmt: string, args: any)
	TextEx: function(text: string, text_end: string, flags: integer)
	TextUnformatted: function(text: string, text_end: string)
	TextV: function(fmt: string, args: any)
	TextWrapped: function(fmt: string, ...: any)
	TextWrappedV: function(fmt: string, args: any)
	TreeNode: function(label: string): boolean
	TreeNode: function(str_id: string, fmt: string, ...: any): boolean
	TreeNode: function(ptr_id: any, fmt: string, ...: any): boolean
	TreeNodeBehavior: function(id: any, flags: integer, label: string, label_end: string): boolean
	TreeNodeEx: function(label: string, flags: integer): boolean
	TreeNodeEx: function(str_id: string, flags: integer, fmt: string, ...: any): boolean
	TreeNodeEx: function(ptr_id: any, flags: integer, fmt: string, ...: any): boolean
	TreeNodeExV: function(str_id: string, flags: integer, fmt: string, args: any): boolean
	TreeNodeExV: function(ptr_id: any, flags: integer, fmt: string, args: any): boolean
	TreeNodeSetOpen: function(id: any, open: boolean)
	TreeNodeUpdateNextOpen: function(id: any, flags: integer): boolean
	TreeNodeV: function(str_id: string, fmt: string, args: any): boolean
	TreeNodeV: function(ptr_id: any, fmt: string, args: any): boolean
	TreePop: function()
	TreePush: function(str_id: string)
	TreePush: function(ptr_id: any)
	TreePushOverrideID: function(id: any)
	Unindent: function(indent_w: number)
	UpdateHoveredWindowAndCaptureFlags: function()
	UpdateInputEvents: function(trickle_fast_inputs: boolean)
	UpdateMouseMovingWindowEndFrame: function()
	UpdateMouseMovingWindowNewFrame: function()
	UpdateWindowParentAndRootLinks: function(window: ImGuiWindow, flags: integer, parent_window: ImGuiWindow)
	VSliderFloat: function(label: string, size: ImVec2, v: number, v_min: number, v_max: number, format: string, flags: integer): boolean, number
	VSliderInt: function(label: string, size: ImVec2, v: integer, v_min: integer, v_max: integer, format: string, flags: integer): boolean, integer
	VSliderScalar: function(label: string, size: ImVec2, data_type: integer, p_data: any, p_min: any, p_max: any, format: string, flags: integer): boolean
	Value: function(prefix: string, b: boolean)
	Value: function(prefix: string, v: integer)
	Value: function(prefix: string, v: integer)
	Value: function(prefix: string, v: number, float_format: string)
	WindowPosRelToAbs: function(pOut: ImVec2, window: ImGuiWindow, p: ImVec2)
	WindowRectAbsToRel: function(pOut: ImRect, window: ImGuiWindow, r: ImRect)
	WindowRectRelToAbs: function(pOut: ImRect, window: ImGuiWindow, r: ImRect)
end
