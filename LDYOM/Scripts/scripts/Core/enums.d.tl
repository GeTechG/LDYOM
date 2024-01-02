global record ImDrawFlags
    None: integer
	Closed: integer
	RoundCornersTopLeft: integer
	RoundCornersTopRight: integer
	RoundCornersBottomLeft: integer
	RoundCornersBottomRight: integer
	RoundCornersNone: integer
	RoundCornersTop: integer
	RoundCornersBottom: integer
	RoundCornersLeft: integer
	RoundCornersRight: integer
	RoundCornersAll: integer
	RoundCornersDefault_: integer
	RoundCornersMask_: integer
end

global record ImDrawListFlags
    None: integer
	AntiAliasedLines: integer
	AntiAliasedLinesUseTex: integer
	AntiAliasedFill: integer
	AllowVtxOffset: integer
end

global record ImFontAtlasFlags
    None: integer
	NoPowerOfTwoHeight: integer
	NoMouseCursors: integer
	NoBakedLines: integer
end

global record ImGuiActivateFlags
    None: integer
	PreferInput: integer
	PreferTweak: integer
	TryToPreserveState: integer
end

global record ImGuiAxis
    None: integer
	X: integer
	Y: integer
end

global record ImGuiBackendFlags
    None: integer
	HasGamepad: integer
	HasMouseCursors: integer
	HasSetMousePos: integer
	RendererHasVtxOffset: integer
end

global record ImGuiButtonFlagsPrivate
    ImGuiButtonFlags_PressedOnClick: integer
	ImGuiButtonFlags_PressedOnClickRelease: integer
	ImGuiButtonFlags_PressedOnClickReleaseAnywhere: integer
	ImGuiButtonFlags_PressedOnRelease: integer
	ImGuiButtonFlags_PressedOnDoubleClick: integer
	ImGuiButtonFlags_PressedOnDragDropHold: integer
	ImGuiButtonFlags_Repeat: integer
	ImGuiButtonFlags_FlattenChildren: integer
	ImGuiButtonFlags_AllowOverlap: integer
	ImGuiButtonFlags_DontClosePopups: integer
	ImGuiButtonFlags_AlignTextBaseLine: integer
	ImGuiButtonFlags_NoKeyModifiers: integer
	ImGuiButtonFlags_NoHoldingActiveId: integer
	ImGuiButtonFlags_NoNavFocus: integer
	ImGuiButtonFlags_NoHoveredOnFocus: integer
	ImGuiButtonFlags_NoSetKeyOwner: integer
	ImGuiButtonFlags_NoTestKeyOwner: integer
	ImGuiButtonFlags_PressedOnMask_: integer
	ImGuiButtonFlags_PressedOnDefault_: integer
end

global record ImGuiButtonFlags
    None: integer
	MouseButtonLeft: integer
	MouseButtonRight: integer
	MouseButtonMiddle: integer
	MouseButtonMask_: integer
	MouseButtonDefault_: integer
end

global record ImGuiCol
    Text: integer
	TextDisabled: integer
	WindowBg: integer
	ChildBg: integer
	PopupBg: integer
	Border: integer
	BorderShadow: integer
	FrameBg: integer
	FrameBgHovered: integer
	FrameBgActive: integer
	TitleBg: integer
	TitleBgActive: integer
	TitleBgCollapsed: integer
	MenuBarBg: integer
	ScrollbarBg: integer
	ScrollbarGrab: integer
	ScrollbarGrabHovered: integer
	ScrollbarGrabActive: integer
	CheckMark: integer
	SliderGrab: integer
	SliderGrabActive: integer
	Button: integer
	ButtonHovered: integer
	ButtonActive: integer
	Header: integer
	HeaderHovered: integer
	HeaderActive: integer
	Separator: integer
	SeparatorHovered: integer
	SeparatorActive: integer
	ResizeGrip: integer
	ResizeGripHovered: integer
	ResizeGripActive: integer
	Tab: integer
	TabHovered: integer
	TabActive: integer
	TabUnfocused: integer
	TabUnfocusedActive: integer
	PlotLines: integer
	PlotLinesHovered: integer
	PlotHistogram: integer
	PlotHistogramHovered: integer
	TableHeaderBg: integer
	TableBorderStrong: integer
	TableBorderLight: integer
	TableRowBg: integer
	TableRowBgAlt: integer
	TextSelectedBg: integer
	DragDropTarget: integer
	NavHighlight: integer
	NavWindowingHighlight: integer
	NavWindowingDimBg: integer
	ModalWindowDimBg: integer
	COUNT: integer
end

global record ImGuiColorEditFlags
    None: integer
	NoAlpha: integer
	NoPicker: integer
	NoOptions: integer
	NoSmallPreview: integer
	NoInputs: integer
	NoTooltip: integer
	NoLabel: integer
	NoSidePreview: integer
	NoDragDrop: integer
	NoBorder: integer
	AlphaBar: integer
	AlphaPreview: integer
	AlphaPreviewHalf: integer
	HDR: integer
	DisplayRGB: integer
	DisplayHSV: integer
	DisplayHex: integer
	Uint8: integer
	Float: integer
	PickerHueBar: integer
	PickerHueWheel: integer
	InputRGB: integer
	InputHSV: integer
	DefaultOptions_: integer
	DisplayMask_: integer
	DataTypeMask_: integer
	PickerMask_: integer
	InputMask_: integer
end

global record ImGuiComboFlagsPrivate
    ImGuiComboFlags_CustomPreview: integer
end

global record ImGuiComboFlags
    None: integer
	PopupAlignLeft: integer
	HeightSmall: integer
	HeightRegular: integer
	HeightLarge: integer
	HeightLargest: integer
	NoArrowButton: integer
	NoPreview: integer
	HeightMask_: integer
end

global record ImGuiCond
    None: integer
	Always: integer
	Once: integer
	FirstUseEver: integer
	Appearing: integer
end

global record ImGuiConfigFlags
    None: integer
	NavEnableKeyboard: integer
	NavEnableGamepad: integer
	NavEnableSetMousePos: integer
	NavNoCaptureKeyboard: integer
	NoMouse: integer
	NoMouseCursorChange: integer
	IsSRGB: integer
	IsTouchScreen: integer
end

global record ImGuiContextHookType
    NewFramePre: integer
	NewFramePost: integer
	EndFramePre: integer
	EndFramePost: integer
	RenderPre: integer
	RenderPost: integer
	Shutdown: integer
	PendingRemoval_: integer
end

global record ImGuiDataTypePrivate
    ImGuiDataType_String: integer
	ImGuiDataType_Pointer: integer
	ImGuiDataType_ID: integer
end

global record ImGuiDataType
    S8: integer
	U8: integer
	S16: integer
	U16: integer
	S32: integer
	U32: integer
	S64: integer
	U64: integer
	Float: integer
	Double: integer
	COUNT: integer
end

global record ImGuiDebugLogFlags
    None: integer
	EventActiveId: integer
	EventFocus: integer
	EventPopup: integer
	EventNav: integer
	EventClipper: integer
	EventSelection: integer
	EventIO: integer
	EventMask_: integer
	OutputToTTY: integer
end

global record ImGuiDir
    None: integer
	Left: integer
	Right: integer
	Up: integer
	Down: integer
	COUNT: integer
end

global record ImGuiDragDropFlags
    None: integer
	SourceNoPreviewTooltip: integer
	SourceNoDisableHover: integer
	SourceNoHoldToOpenOthers: integer
	SourceAllowNullID: integer
	SourceExtern: integer
	SourceAutoExpirePayload: integer
	AcceptBeforeDelivery: integer
	AcceptNoDrawDefaultRect: integer
	AcceptNoPreviewTooltip: integer
	AcceptPeekOnly: integer
end

global record ImGuiFocusRequestFlags
    None: integer
	RestoreFocusedChild: integer
	UnlessBelowModal: integer
end

global record ImGuiFocusedFlags
    None: integer
	ChildWindows: integer
	RootWindow: integer
	AnyWindow: integer
	NoPopupHierarchy: integer
	RootAndChildWindows: integer
end

global record ImGuiHoveredFlagsPrivate
    ImGuiHoveredFlags_DelayMask_: integer
	ImGuiHoveredFlags_AllowedMaskForIsWindowHovered: integer
	ImGuiHoveredFlags_AllowedMaskForIsItemHovered: integer
end

global record ImGuiHoveredFlags
    None: integer
	ChildWindows: integer
	RootWindow: integer
	AnyWindow: integer
	NoPopupHierarchy: integer
	AllowWhenBlockedByPopup: integer
	AllowWhenBlockedByActiveItem: integer
	AllowWhenOverlappedByItem: integer
	AllowWhenOverlappedByWindow: integer
	AllowWhenDisabled: integer
	NoNavOverride: integer
	AllowWhenOverlapped: integer
	RectOnly: integer
	RootAndChildWindows: integer
	ForTooltip: integer
	Stationary: integer
	DelayNone: integer
	DelayShort: integer
	DelayNormal: integer
	NoSharedDelay: integer
end

global record ImGuiInputEventType
    None: integer
	MousePos: integer
	MouseWheel: integer
	MouseButton: integer
	Key: integer
	Text: integer
	Focus: integer
	COUNT: integer
end

global record ImGuiInputFlags
    None: integer
	Repeat: integer
	RepeatRateDefault: integer
	RepeatRateNavMove: integer
	RepeatRateNavTweak: integer
	RepeatRateMask_: integer
	CondHovered: integer
	CondActive: integer
	CondDefault_: integer
	CondMask_: integer
	LockThisFrame: integer
	LockUntilRelease: integer
	RouteFocused: integer
	RouteGlobalLow: integer
	RouteGlobal: integer
	RouteGlobalHigh: integer
	RouteMask_: integer
	RouteAlways: integer
	RouteUnlessBgFocused: integer
	RouteExtraMask_: integer
	SupportedByIsKeyPressed: integer
	SupportedByShortcut: integer
	SupportedBySetKeyOwner: integer
	SupportedBySetItemKeyOwner: integer
end

global record ImGuiInputSource
    None: integer
	Mouse: integer
	Keyboard: integer
	Gamepad: integer
	Clipboard: integer
	COUNT: integer
end

global record ImGuiInputTextFlagsPrivate
    ImGuiInputTextFlags_Multiline: integer
	ImGuiInputTextFlags_NoMarkEdited: integer
	ImGuiInputTextFlags_MergedItem: integer
end

global record ImGuiInputTextFlags
    None: integer
	CharsDecimal: integer
	CharsHexadecimal: integer
	CharsUppercase: integer
	CharsNoBlank: integer
	AutoSelectAll: integer
	EnterReturnsTrue: integer
	CallbackCompletion: integer
	CallbackHistory: integer
	CallbackAlways: integer
	CallbackCharFilter: integer
	AllowTabInput: integer
	CtrlEnterForNewLine: integer
	NoHorizontalScroll: integer
	AlwaysOverwrite: integer
	ReadOnly: integer
	Password: integer
	NoUndoRedo: integer
	CharsScientific: integer
	CallbackResize: integer
	CallbackEdit: integer
	EscapeClearsAll: integer
end

global record ImGuiItemFlags
    None: integer
	NoTabStop: integer
	ButtonRepeat: integer
	Disabled: integer
	NoNav: integer
	NoNavDefaultFocus: integer
	SelectableDontClosePopup: integer
	MixedValue: integer
	ReadOnly: integer
	NoWindowHoverableCheck: integer
	AllowOverlap: integer
	Inputable: integer
end

global record ImGuiItemStatusFlags
    None: integer
	HoveredRect: integer
	HasDisplayRect: integer
	Edited: integer
	ToggledSelection: integer
	ToggledOpen: integer
	HasDeactivated: integer
	Deactivated: integer
	HoveredWindow: integer
	FocusedByTabbing: integer
	Visible: integer
end

global record ImGuiKey
    None: integer
	Tab: integer
	LeftArrow: integer
	RightArrow: integer
	UpArrow: integer
	DownArrow: integer
	PageUp: integer
	PageDown: integer
	Home: integer
	End: integer
	Insert: integer
	Delete: integer
	Backspace: integer
	Space: integer
	Enter: integer
	Escape: integer
	LeftCtrl: integer
	LeftShift: integer
	LeftAlt: integer
	LeftSuper: integer
	RightCtrl: integer
	RightShift: integer
	RightAlt: integer
	RightSuper: integer
	Menu: integer
	["0"]: integer
	["1"]: integer
	["2"]: integer
	["3"]: integer
	["4"]: integer
	["5"]: integer
	["6"]: integer
	["7"]: integer
	["8"]: integer
	["9"]: integer
	A: integer
	B: integer
	C: integer
	D: integer
	E: integer
	F: integer
	G: integer
	H: integer
	I: integer
	J: integer
	K: integer
	L: integer
	M: integer
	N: integer
	O: integer
	P: integer
	Q: integer
	R: integer
	S: integer
	T: integer
	U: integer
	V: integer
	W: integer
	X: integer
	Y: integer
	Z: integer
	F1: integer
	F2: integer
	F3: integer
	F4: integer
	F5: integer
	F6: integer
	F7: integer
	F8: integer
	F9: integer
	F10: integer
	F11: integer
	F12: integer
	Apostrophe: integer
	Comma: integer
	Minus: integer
	Period: integer
	Slash: integer
	Semicolon: integer
	Equal: integer
	LeftBracket: integer
	Backslash: integer
	RightBracket: integer
	GraveAccent: integer
	CapsLock: integer
	ScrollLock: integer
	NumLock: integer
	PrintScreen: integer
	Pause: integer
	Keypad0: integer
	Keypad1: integer
	Keypad2: integer
	Keypad3: integer
	Keypad4: integer
	Keypad5: integer
	Keypad6: integer
	Keypad7: integer
	Keypad8: integer
	Keypad9: integer
	KeypadDecimal: integer
	KeypadDivide: integer
	KeypadMultiply: integer
	KeypadSubtract: integer
	KeypadAdd: integer
	KeypadEnter: integer
	KeypadEqual: integer
	GamepadStart: integer
	GamepadBack: integer
	GamepadFaceLeft: integer
	GamepadFaceRight: integer
	GamepadFaceUp: integer
	GamepadFaceDown: integer
	GamepadDpadLeft: integer
	GamepadDpadRight: integer
	GamepadDpadUp: integer
	GamepadDpadDown: integer
	GamepadL1: integer
	GamepadR1: integer
	GamepadL2: integer
	GamepadR2: integer
	GamepadL3: integer
	GamepadR3: integer
	GamepadLStickLeft: integer
	GamepadLStickRight: integer
	GamepadLStickUp: integer
	GamepadLStickDown: integer
	GamepadRStickLeft: integer
	GamepadRStickRight: integer
	GamepadRStickUp: integer
	GamepadRStickDown: integer
	MouseLeft: integer
	MouseRight: integer
	MouseMiddle: integer
	MouseX1: integer
	MouseX2: integer
	MouseWheelX: integer
	MouseWheelY: integer
	ReservedForModCtrl: integer
	ReservedForModShift: integer
	ReservedForModAlt: integer
	ReservedForModSuper: integer
	COUNT: integer
	ImGuiMod_None: integer
	ImGuiMod_Ctrl: integer
	ImGuiMod_Shift: integer
	ImGuiMod_Alt: integer
	ImGuiMod_Super: integer
	ImGuiMod_Shortcut: integer
	ImGuiMod_Mask_: integer
	NamedKey_BEGIN: integer
	NamedKey_END: integer
	NamedKey_COUNT: integer
	KeysData_SIZE: integer
	KeysData_OFFSET: integer
end

global record ImGuiLayoutType
    Horizontal: integer
	Vertical: integer
end

global record ImGuiLocKey
    VersionStr: integer
	TableSizeOne: integer
	TableSizeAllFit: integer
	TableSizeAllDefault: integer
	TableResetOrder: integer
	WindowingMainMenuBar: integer
	WindowingPopup: integer
	WindowingUntitled: integer
	COUNT: integer
end

global record ImGuiLogType
    None: integer
	TTY: integer
	File: integer
	Buffer: integer
	Clipboard: integer
end

global record ImGuiMouseButton
    Left: integer
	Right: integer
	Middle: integer
	COUNT: integer
end

global record ImGuiMouseCursor
    None: integer
	Arrow: integer
	TextInput: integer
	ResizeAll: integer
	ResizeNS: integer
	ResizeEW: integer
	ResizeNESW: integer
	ResizeNWSE: integer
	Hand: integer
	NotAllowed: integer
	COUNT: integer
end

global record ImGuiMouseSource
    Mouse: integer
	TouchScreen: integer
	Pen: integer
	COUNT: integer
end

global record ImGuiNavHighlightFlags
    None: integer
	TypeDefault: integer
	TypeThin: integer
	AlwaysDraw: integer
	NoRounding: integer
end

global record ImGuiNavInput
    Activate: integer
	Cancel: integer
	Input: integer
	Menu: integer
	DpadLeft: integer
	DpadRight: integer
	DpadUp: integer
	DpadDown: integer
	LStickLeft: integer
	LStickRight: integer
	LStickUp: integer
	LStickDown: integer
	FocusPrev: integer
	FocusNext: integer
	TweakSlow: integer
	TweakFast: integer
	COUNT: integer
end

global record ImGuiNavLayer
    Main: integer
	Menu: integer
	COUNT: integer
end

global record ImGuiNavMoveFlags
    None: integer
	LoopX: integer
	LoopY: integer
	WrapX: integer
	WrapY: integer
	WrapMask_: integer
	AllowCurrentNavId: integer
	AlsoScoreVisibleSet: integer
	ScrollToEdgeY: integer
	Forwarded: integer
	DebugNoResult: integer
	FocusApi: integer
	IsTabbing: integer
	IsPageMove: integer
	Activate: integer
	NoSelect: integer
	NoSetNavHighlight: integer
end

global record ImGuiNextItemDataFlags
    None: integer
	HasWidth: integer
	HasOpen: integer
end

global record ImGuiNextWindowDataFlags
    None: integer
	HasPos: integer
	HasSize: integer
	HasContentSize: integer
	HasCollapsed: integer
	HasSizeConstraint: integer
	HasFocus: integer
	HasBgAlpha: integer
	HasScroll: integer
end

global record ImGuiOldColumnFlags
    None: integer
	NoBorder: integer
	NoResize: integer
	NoPreserveWidths: integer
	NoForceWithinWindow: integer
	GrowParentContentsSize: integer
end

global record ImGuiPlotType
    Lines: integer
	Histogram: integer
end

global record ImGuiPopupFlags
    None: integer
	MouseButtonLeft: integer
	MouseButtonRight: integer
	MouseButtonMiddle: integer
	MouseButtonMask_: integer
	MouseButtonDefault_: integer
	NoOpenOverExistingPopup: integer
	NoOpenOverItems: integer
	AnyPopupId: integer
	AnyPopupLevel: integer
	AnyPopup: integer
end

global record ImGuiPopupPositionPolicy
    Default: integer
	ComboBox: integer
	Tooltip: integer
end

global record ImGuiScrollFlags
    None: integer
	KeepVisibleEdgeX: integer
	KeepVisibleEdgeY: integer
	KeepVisibleCenterX: integer
	KeepVisibleCenterY: integer
	AlwaysCenterX: integer
	AlwaysCenterY: integer
	NoScrollParent: integer
	MaskX_: integer
	MaskY_: integer
end

global record ImGuiSelectableFlagsPrivate
    ImGuiSelectableFlags_NoHoldingActiveID: integer
	ImGuiSelectableFlags_SelectOnNav: integer
	ImGuiSelectableFlags_SelectOnClick: integer
	ImGuiSelectableFlags_SelectOnRelease: integer
	ImGuiSelectableFlags_SpanAvailWidth: integer
	ImGuiSelectableFlags_SetNavIdOnHover: integer
	ImGuiSelectableFlags_NoPadWithHalfSpacing: integer
	ImGuiSelectableFlags_NoSetKeyOwner: integer
end

global record ImGuiSelectableFlags
    None: integer
	DontClosePopups: integer
	SpanAllColumns: integer
	AllowDoubleClick: integer
	Disabled: integer
	AllowOverlap: integer
end

global record ImGuiSeparatorFlags
    None: integer
	Horizontal: integer
	Vertical: integer
	SpanAllColumns: integer
end

global record ImGuiSliderFlagsPrivate
    ImGuiSliderFlags_Vertical: integer
	ImGuiSliderFlags_ReadOnly: integer
end

global record ImGuiSliderFlags
    None: integer
	AlwaysClamp: integer
	Logarithmic: integer
	NoRoundToFormat: integer
	NoInput: integer
	InvalidMask_: integer
end

global record ImGuiSortDirection
    None: integer
	Ascending: integer
	Descending: integer
end

global record ImGuiStyleVar
    Alpha: integer
	DisabledAlpha: integer
	WindowPadding: integer
	WindowRounding: integer
	WindowBorderSize: integer
	WindowMinSize: integer
	WindowTitleAlign: integer
	ChildRounding: integer
	ChildBorderSize: integer
	PopupRounding: integer
	PopupBorderSize: integer
	FramePadding: integer
	FrameRounding: integer
	FrameBorderSize: integer
	ItemSpacing: integer
	ItemInnerSpacing: integer
	IndentSpacing: integer
	CellPadding: integer
	ScrollbarSize: integer
	ScrollbarRounding: integer
	GrabMinSize: integer
	GrabRounding: integer
	TabRounding: integer
	ButtonTextAlign: integer
	SelectableTextAlign: integer
	SeparatorTextBorderSize: integer
	SeparatorTextAlign: integer
	SeparatorTextPadding: integer
	COUNT: integer
end

global record ImGuiTabBarFlagsPrivate
    ImGuiTabBarFlags_DockNode: integer
	ImGuiTabBarFlags_IsFocused: integer
	ImGuiTabBarFlags_SaveSettings: integer
end

global record ImGuiTabBarFlags
    None: integer
	Reorderable: integer
	AutoSelectNewTabs: integer
	TabListPopupButton: integer
	NoCloseWithMiddleMouseButton: integer
	NoTabListScrollingButtons: integer
	NoTooltip: integer
	FittingPolicyResizeDown: integer
	FittingPolicyScroll: integer
	FittingPolicyMask_: integer
	FittingPolicyDefault_: integer
end

global record ImGuiTabItemFlagsPrivate
    ImGuiTabItemFlags_SectionMask_: integer
	ImGuiTabItemFlags_NoCloseButton: integer
	ImGuiTabItemFlags_Button: integer
end

global record ImGuiTabItemFlags
    None: integer
	UnsavedDocument: integer
	SetSelected: integer
	NoCloseWithMiddleMouseButton: integer
	NoPushId: integer
	NoTooltip: integer
	NoReorder: integer
	Leading: integer
	Trailing: integer
end

global record ImGuiTableBgTarget
    None: integer
	RowBg0: integer
	RowBg1: integer
	CellBg: integer
end

global record ImGuiTableColumnFlags
    None: integer
	Disabled: integer
	DefaultHide: integer
	DefaultSort: integer
	WidthStretch: integer
	WidthFixed: integer
	NoResize: integer
	NoReorder: integer
	NoHide: integer
	NoClip: integer
	NoSort: integer
	NoSortAscending: integer
	NoSortDescending: integer
	NoHeaderLabel: integer
	NoHeaderWidth: integer
	PreferSortAscending: integer
	PreferSortDescending: integer
	IndentEnable: integer
	IndentDisable: integer
	IsEnabled: integer
	IsVisible: integer
	IsSorted: integer
	IsHovered: integer
	WidthMask_: integer
	IndentMask_: integer
	StatusMask_: integer
	NoDirectResize_: integer
end

global record ImGuiTableFlags
    None: integer
	Resizable: integer
	Reorderable: integer
	Hideable: integer
	Sortable: integer
	NoSavedSettings: integer
	ContextMenuInBody: integer
	RowBg: integer
	BordersInnerH: integer
	BordersOuterH: integer
	BordersInnerV: integer
	BordersOuterV: integer
	BordersH: integer
	BordersV: integer
	BordersInner: integer
	BordersOuter: integer
	Borders: integer
	NoBordersInBody: integer
	NoBordersInBodyUntilResize: integer
	SizingFixedFit: integer
	SizingFixedSame: integer
	SizingStretchProp: integer
	SizingStretchSame: integer
	NoHostExtendX: integer
	NoHostExtendY: integer
	NoKeepColumnsVisible: integer
	PreciseWidths: integer
	NoClip: integer
	PadOuterX: integer
	NoPadOuterX: integer
	NoPadInnerX: integer
	ScrollX: integer
	ScrollY: integer
	SortMulti: integer
	SortTristate: integer
	SizingMask_: integer
end

global record ImGuiTableRowFlags
    None: integer
	Headers: integer
end

global record ImGuiTextFlags
    None: integer
	NoWidthForLargeClippedText: integer
end

global record ImGuiTooltipFlags
    None: integer
	OverridePrevious: integer
end

global record ImGuiTreeNodeFlagsPrivate
    ImGuiTreeNodeFlags_ClipLabelForTrailingButton: integer
	ImGuiTreeNodeFlags_UpsideDownArrow: integer
end

global record ImGuiTreeNodeFlags
    None: integer
	Selected: integer
	Framed: integer
	AllowOverlap: integer
	NoTreePushOnOpen: integer
	NoAutoOpenOnLog: integer
	DefaultOpen: integer
	OpenOnDoubleClick: integer
	OpenOnArrow: integer
	Leaf: integer
	Bullet: integer
	FramePadding: integer
	SpanAvailWidth: integer
	SpanFullWidth: integer
	NavLeftJumpsBackHere: integer
	CollapsingHeader: integer
end

global record ImGuiViewportFlags
    None: integer
	IsPlatformWindow: integer
	IsPlatformMonitor: integer
	OwnedByApp: integer
end

global record ImGuiWindowFlags
    None: integer
	NoTitleBar: integer
	NoResize: integer
	NoMove: integer
	NoScrollbar: integer
	NoScrollWithMouse: integer
	NoCollapse: integer
	AlwaysAutoResize: integer
	NoBackground: integer
	NoSavedSettings: integer
	NoMouseInputs: integer
	MenuBar: integer
	HorizontalScrollbar: integer
	NoFocusOnAppearing: integer
	NoBringToFrontOnFocus: integer
	AlwaysVerticalScrollbar: integer
	AlwaysHorizontalScrollbar: integer
	AlwaysUseWindowPadding: integer
	NoNavInputs: integer
	NoNavFocus: integer
	UnsavedDocument: integer
	NoNav: integer
	NoDecoration: integer
	NoInputs: integer
	NavFlattened: integer
	ChildWindow: integer
	Tooltip: integer
	Popup: integer
	Modal: integer
	ChildMenu: integer
end
