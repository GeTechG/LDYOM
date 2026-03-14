---@diagnostic disable: duplicate-index
---@meta
---@class ImBitVector
---@field Storage ImVector_ImU32
ImBitVector = {
    ---Clear()
	---@param self ImBitVector
    Clear = function (self) end,
	---ClearBit(int n)
	---@param self ImBitVector
	---@param n integer
    ClearBit = function (self, n) end,
	---Create(int sz)
	---@param self ImBitVector
	---@param sz integer
    Create = function (self, sz) end,
	---SetBit(int n)
	---@param self ImBitVector
	---@param n integer
    SetBit = function (self, n) end,
	---TestBit(int n)
	---@param self ImBitVector
	---@param n integer
    ---@return boolean
    TestBit = function (self, n) end
}

---@class ImColor
---@field Value ImVec4
ImColor = {
    ---HSV(float h,float s,float v,float a=1.0f)
	---@param h number
	---@param s number
	---@param v number
	---@param a number
    ---@return userdata
    HSV = function (h, s, v, a) end,
	---ImColor()
    ---@return ImColor
    new = function () end,
	---ImColor(float r,float g,float b,float a=1.0f)
	---@param r number
	---@param g number
	---@param b number
	---@param a number
    ---@return ImColor
    new = function (r, g, b, a) end,
	---ImColor(const ImVec4& col)
	---@param col ImVec4
    ---@return ImColor
    new = function (col) end,
	---ImColor(int r,int g,int b,int a=255)
	---@param r integer
	---@param g integer
	---@param b integer
	---@param a integer
    ---@return ImColor
    new = function (r, g, b, a) end,
	---ImColor(ImU32 rgba)
	---@param rgba integer
    ---@return ImColor
    new = function (rgba) end,
	---SetHSV(float h,float s,float v,float a=1.0f)
	---@param self ImColor
	---@param h number
	---@param s number
	---@param v number
	---@param a number
    SetHSV = function (self, h, s, v, a) end
}

---@class ImDrawChannel
---@field _CmdBuffer ImVector_ImDrawCmd
---@field _IdxBuffer ImVector_ImDrawIdx
ImDrawChannel = {
    
}

---@class ImDrawCmd
---@field ClipRect ImVec4
---@field TexRef ImTextureRef
---@field VtxOffset integer
---@field IdxOffset integer
---@field ElemCount integer
---@field UserCallback userdata
---@field UserCallbackData userdata
---@field UserCallbackDataSize integer
---@field UserCallbackDataOffset integer
ImDrawCmd = {
    ---GetTexID()
	---@param self ImDrawCmd
    ---@return userdata
    GetTexID = function (self) end,
	---ImDrawCmd()
    ---@return ImDrawCmd
    new = function () end
}

---@class ImDrawCmdHeader
---@field ClipRect ImVec4
---@field TexRef ImTextureRef
---@field VtxOffset integer
ImDrawCmdHeader = {
    
}

---@class ImDrawData
---@field Valid boolean
---@field CmdListsCount integer
---@field TotalIdxCount integer
---@field TotalVtxCount integer
---@field CmdLists ImVector_ImDrawListPtr
---@field DisplayPos ImVec2
---@field DisplaySize ImVec2
---@field FramebufferScale ImVec2
---@field OwnerViewport ImGuiViewport
---@field Textures ImVector_ImTextureDataPtr
ImDrawData = {
    ---AddDrawList(ImDrawList* draw_list)
	---@param self ImDrawData
	---@param draw_list ImDrawList
    AddDrawList = function (self, draw_list) end,
	---Clear()
	---@param self ImDrawData
    Clear = function (self) end,
	---DeIndexAllBuffers()
	---@param self ImDrawData
    DeIndexAllBuffers = function (self) end,
	---ImDrawData()
    ---@return ImDrawData
    new = function () end,
	---ScaleClipRects(const ImVec2& fb_scale)
	---@param self ImDrawData
	---@param fb_scale ImVec2
    ScaleClipRects = function (self, fb_scale) end
}

---@class ImDrawDataBuilder
---@field Layers ImVector_ImDrawListPtr
---@field LayerData1 ImVector_ImDrawListPtr
ImDrawDataBuilder = {
    ---ImDrawDataBuilder()
    ---@return ImDrawDataBuilder
    new = function () end
}

---@class ImDrawList
---@field CmdBuffer ImVector_ImDrawCmd
---@field IdxBuffer ImVector_ImDrawIdx
---@field VtxBuffer ImVector_ImDrawVert
---@field Flags ImDrawListFlags
---@field _VtxCurrentIdx integer
---@field _Data ImDrawListSharedData
---@field _VtxWritePtr ImDrawVert
---@field _IdxWritePtr userdata
---@field _Path ImVector_ImVec2
---@field _CmdHeader ImDrawCmdHeader
---@field _Splitter ImDrawListSplitter
---@field _ClipRectStack ImVector_ImVec4
---@field _TextureStack ImVector_ImTextureRef
---@field _CallbacksDataBuf ImVector_ImU8
---@field _FringeScale number
---@field _OwnerName string
ImDrawList = {
    ---AddBezierCubic(const ImVec2& p1,const ImVec2& p2,const ImVec2& p3,const ImVec2& p4,ImU32 col,float thickness,int num_segments=0)
	---@param self ImDrawList
	---@param p1 ImVec2
	---@param p2 ImVec2
	---@param p3 ImVec2
	---@param p4 ImVec2
	---@param col integer
	---@param thickness number
	---@param num_segments integer
    AddBezierCubic = function (self, p1, p2, p3, p4, col, thickness, num_segments) end,
	---AddBezierQuadratic(const ImVec2& p1,const ImVec2& p2,const ImVec2& p3,ImU32 col,float thickness,int num_segments=0)
	---@param self ImDrawList
	---@param p1 ImVec2
	---@param p2 ImVec2
	---@param p3 ImVec2
	---@param col integer
	---@param thickness number
	---@param num_segments integer
    AddBezierQuadratic = function (self, p1, p2, p3, col, thickness, num_segments) end,
	---AddCallback(ImDrawCallback callback,void* userdata,size_t userdata_size=0)
	---@param self ImDrawList
	---@param callback userdata
	---@param userdata userdata
	---@param userdata_size userdata
    AddCallback = function (self, callback, userdata, userdata_size) end,
	---AddCircle(const ImVec2& center,float radius,ImU32 col,int num_segments=0,float thickness=1.0f)
	---@param self ImDrawList
	---@param center ImVec2
	---@param radius number
	---@param col integer
	---@param num_segments integer
	---@param thickness number
    AddCircle = function (self, center, radius, col, num_segments, thickness) end,
	---AddCircleFilled(const ImVec2& center,float radius,ImU32 col,int num_segments=0)
	---@param self ImDrawList
	---@param center ImVec2
	---@param radius number
	---@param col integer
	---@param num_segments integer
    AddCircleFilled = function (self, center, radius, col, num_segments) end,
	---AddConcavePolyFilled(const ImVec2* points,int num_points,ImU32 col)
	---@param self ImDrawList
	---@param points ImVec2
	---@param num_points integer
	---@param col integer
    AddConcavePolyFilled = function (self, points, num_points, col) end,
	---AddConvexPolyFilled(const ImVec2* points,int num_points,ImU32 col)
	---@param self ImDrawList
	---@param points ImVec2
	---@param num_points integer
	---@param col integer
    AddConvexPolyFilled = function (self, points, num_points, col) end,
	---AddDrawCmd()
	---@param self ImDrawList
    AddDrawCmd = function (self) end,
	---AddEllipse(const ImVec2& center,const ImVec2& radius,ImU32 col,float rot=0.0f,int num_segments=0,float thickness=1.0f)
	---@param self ImDrawList
	---@param center ImVec2
	---@param radius ImVec2
	---@param col integer
	---@param rot number
	---@param num_segments integer
	---@param thickness number
    AddEllipse = function (self, center, radius, col, rot, num_segments, thickness) end,
	---AddEllipseFilled(const ImVec2& center,const ImVec2& radius,ImU32 col,float rot=0.0f,int num_segments=0)
	---@param self ImDrawList
	---@param center ImVec2
	---@param radius ImVec2
	---@param col integer
	---@param rot number
	---@param num_segments integer
    AddEllipseFilled = function (self, center, radius, col, rot, num_segments) end,
	---AddImage(ImTextureRef tex_ref,const ImVec2& p_min,const ImVec2& p_max,const ImVec2& uv_min=ImVec2(0,0),const ImVec2& uv_max=ImVec2(1,1),ImU32 col=(((ImU32)(255)<<24)|((ImU32)(255)<<16)|((ImU32)(255)<<8)|((ImU32)(255)<<0)))
	---@param self ImDrawList
	---@param tex_ref ImTextureRef
	---@param p_min ImVec2
	---@param p_max ImVec2
	---@param uv_min ImVec2
	---@param uv_max ImVec2
	---@param col integer
    AddImage = function (self, tex_ref, p_min, p_max, uv_min, uv_max, col) end,
	---AddImageQuad(ImTextureRef tex_ref,const ImVec2& p1,const ImVec2& p2,const ImVec2& p3,const ImVec2& p4,const ImVec2& uv1=ImVec2(0,0),const ImVec2& uv2=ImVec2(1,0),const ImVec2& uv3=ImVec2(1,1),const ImVec2& uv4=ImVec2(0,1),ImU32 col=(((ImU32)(255)<<24)|((ImU32)(255)<<16)|((ImU32)(255)<<8)|((ImU32)(255)<<0)))
	---@param self ImDrawList
	---@param tex_ref ImTextureRef
	---@param p1 ImVec2
	---@param p2 ImVec2
	---@param p3 ImVec2
	---@param p4 ImVec2
	---@param uv1 ImVec2
	---@param uv2 ImVec2
	---@param uv3 ImVec2
	---@param uv4 ImVec2
	---@param col integer
    AddImageQuad = function (self, tex_ref, p1, p2, p3, p4, uv1, uv2, uv3, uv4, col) end,
	---AddImageRounded(ImTextureRef tex_ref,const ImVec2& p_min,const ImVec2& p_max,const ImVec2& uv_min,const ImVec2& uv_max,ImU32 col,float rounding,ImDrawFlags flags=0)
	---@param self ImDrawList
	---@param tex_ref ImTextureRef
	---@param p_min ImVec2
	---@param p_max ImVec2
	---@param uv_min ImVec2
	---@param uv_max ImVec2
	---@param col integer
	---@param rounding number
	---@param flags ImDrawFlags
    AddImageRounded = function (self, tex_ref, p_min, p_max, uv_min, uv_max, col, rounding, flags) end,
	---AddLine(const ImVec2& p1,const ImVec2& p2,ImU32 col,float thickness=1.0f)
	---@param self ImDrawList
	---@param p1 ImVec2
	---@param p2 ImVec2
	---@param col integer
	---@param thickness number
    AddLine = function (self, p1, p2, col, thickness) end,
	---AddNgon(const ImVec2& center,float radius,ImU32 col,int num_segments,float thickness=1.0f)
	---@param self ImDrawList
	---@param center ImVec2
	---@param radius number
	---@param col integer
	---@param num_segments integer
	---@param thickness number
    AddNgon = function (self, center, radius, col, num_segments, thickness) end,
	---AddNgonFilled(const ImVec2& center,float radius,ImU32 col,int num_segments)
	---@param self ImDrawList
	---@param center ImVec2
	---@param radius number
	---@param col integer
	---@param num_segments integer
    AddNgonFilled = function (self, center, radius, col, num_segments) end,
	---AddPolyline(const ImVec2* points,int num_points,ImU32 col,ImDrawFlags flags,float thickness)
	---@param self ImDrawList
	---@param points ImVec2
	---@param num_points integer
	---@param col integer
	---@param flags ImDrawFlags
	---@param thickness number
    AddPolyline = function (self, points, num_points, col, flags, thickness) end,
	---AddQuad(const ImVec2& p1,const ImVec2& p2,const ImVec2& p3,const ImVec2& p4,ImU32 col,float thickness=1.0f)
	---@param self ImDrawList
	---@param p1 ImVec2
	---@param p2 ImVec2
	---@param p3 ImVec2
	---@param p4 ImVec2
	---@param col integer
	---@param thickness number
    AddQuad = function (self, p1, p2, p3, p4, col, thickness) end,
	---AddQuadFilled(const ImVec2& p1,const ImVec2& p2,const ImVec2& p3,const ImVec2& p4,ImU32 col)
	---@param self ImDrawList
	---@param p1 ImVec2
	---@param p2 ImVec2
	---@param p3 ImVec2
	---@param p4 ImVec2
	---@param col integer
    AddQuadFilled = function (self, p1, p2, p3, p4, col) end,
	---AddRect(const ImVec2& p_min,const ImVec2& p_max,ImU32 col,float rounding=0.0f,ImDrawFlags flags=0,float thickness=1.0f)
	---@param self ImDrawList
	---@param p_min ImVec2
	---@param p_max ImVec2
	---@param col integer
	---@param rounding number
	---@param flags ImDrawFlags
	---@param thickness number
    AddRect = function (self, p_min, p_max, col, rounding, flags, thickness) end,
	---AddRectFilled(const ImVec2& p_min,const ImVec2& p_max,ImU32 col,float rounding=0.0f,ImDrawFlags flags=0)
	---@param self ImDrawList
	---@param p_min ImVec2
	---@param p_max ImVec2
	---@param col integer
	---@param rounding number
	---@param flags ImDrawFlags
    AddRectFilled = function (self, p_min, p_max, col, rounding, flags) end,
	---AddRectFilledMultiColor(const ImVec2& p_min,const ImVec2& p_max,ImU32 col_upr_left,ImU32 col_upr_right,ImU32 col_bot_right,ImU32 col_bot_left)
	---@param self ImDrawList
	---@param p_min ImVec2
	---@param p_max ImVec2
	---@param col_upr_left integer
	---@param col_upr_right integer
	---@param col_bot_right integer
	---@param col_bot_left integer
    AddRectFilledMultiColor = function (self, p_min, p_max, col_upr_left, col_upr_right, col_bot_right, col_bot_left) end,
	---AddText(const ImVec2& pos,ImU32 col,const char* text_begin,const char* text_end=((void*)0))
	---@param self ImDrawList
	---@param pos ImVec2
	---@param col integer
	---@param text_begin string
	---@param text_end string
    AddText = function (self, pos, col, text_begin, text_end) end,
	---AddText(ImFont* font,float font_size,const ImVec2& pos,ImU32 col,const char* text_begin,const char* text_end=((void*)0),float wrap_width=0.0f,const ImVec4* cpu_fine_clip_rect=((void*)0))
	---@param self ImDrawList
	---@param font ImFont
	---@param font_size number
	---@param pos ImVec2
	---@param col integer
	---@param text_begin string
	---@param text_end string
	---@param wrap_width number
	---@param cpu_fine_clip_rect ImVec4
    AddText = function (self, font, font_size, pos, col, text_begin, text_end, wrap_width, cpu_fine_clip_rect) end,
	---AddTriangle(const ImVec2& p1,const ImVec2& p2,const ImVec2& p3,ImU32 col,float thickness=1.0f)
	---@param self ImDrawList
	---@param p1 ImVec2
	---@param p2 ImVec2
	---@param p3 ImVec2
	---@param col integer
	---@param thickness number
    AddTriangle = function (self, p1, p2, p3, col, thickness) end,
	---AddTriangleFilled(const ImVec2& p1,const ImVec2& p2,const ImVec2& p3,ImU32 col)
	---@param self ImDrawList
	---@param p1 ImVec2
	---@param p2 ImVec2
	---@param p3 ImVec2
	---@param col integer
    AddTriangleFilled = function (self, p1, p2, p3, col) end,
	---ChannelsMerge()
	---@param self ImDrawList
    ChannelsMerge = function (self) end,
	---ChannelsSetCurrent(int n)
	---@param self ImDrawList
	---@param n integer
    ChannelsSetCurrent = function (self, n) end,
	---ChannelsSplit(int count)
	---@param self ImDrawList
	---@param count integer
    ChannelsSplit = function (self, count) end,
	---CloneOutput()
	---@param self ImDrawList
    ---@return ImDrawList
    CloneOutput = function (self) end,
	---GetClipRectMax()
	---@param self ImDrawList
    ---@return userdata
    GetClipRectMax = function (self) end,
	---GetClipRectMin()
	---@param self ImDrawList
    ---@return userdata
    GetClipRectMin = function (self) end,
	---ImDrawList(ImDrawListSharedData* shared_data)
	---@param shared_data ImDrawListSharedData
    ---@return ImDrawList
    new = function (shared_data) end,
	---PathArcTo(const ImVec2& center,float radius,float a_min,float a_max,int num_segments=0)
	---@param self ImDrawList
	---@param center ImVec2
	---@param radius number
	---@param a_min number
	---@param a_max number
	---@param num_segments integer
    PathArcTo = function (self, center, radius, a_min, a_max, num_segments) end,
	---PathArcToFast(const ImVec2& center,float radius,int a_min_of_12,int a_max_of_12)
	---@param self ImDrawList
	---@param center ImVec2
	---@param radius number
	---@param a_min_of_12 integer
	---@param a_max_of_12 integer
    PathArcToFast = function (self, center, radius, a_min_of_12, a_max_of_12) end,
	---PathBezierCubicCurveTo(const ImVec2& p2,const ImVec2& p3,const ImVec2& p4,int num_segments=0)
	---@param self ImDrawList
	---@param p2 ImVec2
	---@param p3 ImVec2
	---@param p4 ImVec2
	---@param num_segments integer
    PathBezierCubicCurveTo = function (self, p2, p3, p4, num_segments) end,
	---PathBezierQuadraticCurveTo(const ImVec2& p2,const ImVec2& p3,int num_segments=0)
	---@param self ImDrawList
	---@param p2 ImVec2
	---@param p3 ImVec2
	---@param num_segments integer
    PathBezierQuadraticCurveTo = function (self, p2, p3, num_segments) end,
	---PathClear()
	---@param self ImDrawList
    PathClear = function (self) end,
	---PathEllipticalArcTo(const ImVec2& center,const ImVec2& radius,float rot,float a_min,float a_max,int num_segments=0)
	---@param self ImDrawList
	---@param center ImVec2
	---@param radius ImVec2
	---@param rot number
	---@param a_min number
	---@param a_max number
	---@param num_segments integer
    PathEllipticalArcTo = function (self, center, radius, rot, a_min, a_max, num_segments) end,
	---PathFillConcave(ImU32 col)
	---@param self ImDrawList
	---@param col integer
    PathFillConcave = function (self, col) end,
	---PathFillConvex(ImU32 col)
	---@param self ImDrawList
	---@param col integer
    PathFillConvex = function (self, col) end,
	---PathLineTo(const ImVec2& pos)
	---@param self ImDrawList
	---@param pos ImVec2
    PathLineTo = function (self, pos) end,
	---PathLineToMergeDuplicate(const ImVec2& pos)
	---@param self ImDrawList
	---@param pos ImVec2
    PathLineToMergeDuplicate = function (self, pos) end,
	---PathRect(const ImVec2& rect_min,const ImVec2& rect_max,float rounding=0.0f,ImDrawFlags flags=0)
	---@param self ImDrawList
	---@param rect_min ImVec2
	---@param rect_max ImVec2
	---@param rounding number
	---@param flags ImDrawFlags
    PathRect = function (self, rect_min, rect_max, rounding, flags) end,
	---PathStroke(ImU32 col,ImDrawFlags flags=0,float thickness=1.0f)
	---@param self ImDrawList
	---@param col integer
	---@param flags ImDrawFlags
	---@param thickness number
    PathStroke = function (self, col, flags, thickness) end,
	---PopClipRect()
	---@param self ImDrawList
    PopClipRect = function (self) end,
	---PopTexture()
	---@param self ImDrawList
    PopTexture = function (self) end,
	---PrimQuadUV(const ImVec2& a,const ImVec2& b,const ImVec2& c,const ImVec2& d,const ImVec2& uv_a,const ImVec2& uv_b,const ImVec2& uv_c,const ImVec2& uv_d,ImU32 col)
	---@param self ImDrawList
	---@param a ImVec2
	---@param b ImVec2
	---@param c ImVec2
	---@param d ImVec2
	---@param uv_a ImVec2
	---@param uv_b ImVec2
	---@param uv_c ImVec2
	---@param uv_d ImVec2
	---@param col integer
    PrimQuadUV = function (self, a, b, c, d, uv_a, uv_b, uv_c, uv_d, col) end,
	---PrimRect(const ImVec2& a,const ImVec2& b,ImU32 col)
	---@param self ImDrawList
	---@param a ImVec2
	---@param b ImVec2
	---@param col integer
    PrimRect = function (self, a, b, col) end,
	---PrimRectUV(const ImVec2& a,const ImVec2& b,const ImVec2& uv_a,const ImVec2& uv_b,ImU32 col)
	---@param self ImDrawList
	---@param a ImVec2
	---@param b ImVec2
	---@param uv_a ImVec2
	---@param uv_b ImVec2
	---@param col integer
    PrimRectUV = function (self, a, b, uv_a, uv_b, col) end,
	---PrimReserve(int idx_count,int vtx_count)
	---@param self ImDrawList
	---@param idx_count integer
	---@param vtx_count integer
    PrimReserve = function (self, idx_count, vtx_count) end,
	---PrimUnreserve(int idx_count,int vtx_count)
	---@param self ImDrawList
	---@param idx_count integer
	---@param vtx_count integer
    PrimUnreserve = function (self, idx_count, vtx_count) end,
	---PrimVtx(const ImVec2& pos,const ImVec2& uv,ImU32 col)
	---@param self ImDrawList
	---@param pos ImVec2
	---@param uv ImVec2
	---@param col integer
    PrimVtx = function (self, pos, uv, col) end,
	---PrimWriteIdx(ImDrawIdx idx)
	---@param self ImDrawList
	---@param idx userdata
    PrimWriteIdx = function (self, idx) end,
	---PrimWriteVtx(const ImVec2& pos,const ImVec2& uv,ImU32 col)
	---@param self ImDrawList
	---@param pos ImVec2
	---@param uv ImVec2
	---@param col integer
    PrimWriteVtx = function (self, pos, uv, col) end,
	---PushClipRect(const ImVec2& clip_rect_min,const ImVec2& clip_rect_max,bool intersect_with_current_clip_rect=false)
	---@param self ImDrawList
	---@param clip_rect_min ImVec2
	---@param clip_rect_max ImVec2
	---@param intersect_with_current_clip_rect boolean
    PushClipRect = function (self, clip_rect_min, clip_rect_max, intersect_with_current_clip_rect) end,
	---PushClipRectFullScreen()
	---@param self ImDrawList
    PushClipRectFullScreen = function (self) end,
	---PushTexture(ImTextureRef tex_ref)
	---@param self ImDrawList
	---@param tex_ref ImTextureRef
    PushTexture = function (self, tex_ref) end,
	---_CalcCircleAutoSegmentCount(float radius)
	---@param self ImDrawList
	---@param radius number
    ---@return integer
    _CalcCircleAutoSegmentCount = function (self, radius) end,
	---_ClearFreeMemory()
	---@param self ImDrawList
    _ClearFreeMemory = function (self) end,
	---_OnChangedClipRect()
	---@param self ImDrawList
    _OnChangedClipRect = function (self) end,
	---_OnChangedTexture()
	---@param self ImDrawList
    _OnChangedTexture = function (self) end,
	---_OnChangedVtxOffset()
	---@param self ImDrawList
    _OnChangedVtxOffset = function (self) end,
	---_PathArcToFastEx(const ImVec2& center,float radius,int a_min_sample,int a_max_sample,int a_step)
	---@param self ImDrawList
	---@param center ImVec2
	---@param radius number
	---@param a_min_sample integer
	---@param a_max_sample integer
	---@param a_step integer
    _PathArcToFastEx = function (self, center, radius, a_min_sample, a_max_sample, a_step) end,
	---_PathArcToN(const ImVec2& center,float radius,float a_min,float a_max,int num_segments)
	---@param self ImDrawList
	---@param center ImVec2
	---@param radius number
	---@param a_min number
	---@param a_max number
	---@param num_segments integer
    _PathArcToN = function (self, center, radius, a_min, a_max, num_segments) end,
	---_PopUnusedDrawCmd()
	---@param self ImDrawList
    _PopUnusedDrawCmd = function (self) end,
	---_ResetForNewFrame()
	---@param self ImDrawList
    _ResetForNewFrame = function (self) end,
	---_SetDrawListSharedData(ImDrawListSharedData* data)
	---@param self ImDrawList
	---@param data ImDrawListSharedData
    _SetDrawListSharedData = function (self, data) end,
	---_SetTexture(ImTextureRef tex_ref)
	---@param self ImDrawList
	---@param tex_ref ImTextureRef
    _SetTexture = function (self, tex_ref) end,
	---_TryMergeDrawCmds()
	---@param self ImDrawList
    _TryMergeDrawCmds = function (self) end
}

---@class ImDrawListSharedData
---@field TexUvWhitePixel ImVec2
---@field TexUvLines ImVec4
---@field FontAtlas ImFontAtlas
---@field Font ImFont
---@field FontSize number
---@field FontScale number
---@field CurveTessellationTol number
---@field CircleSegmentMaxError number
---@field InitialFringeScale number
---@field InitialFlags ImDrawListFlags
---@field ClipRectFullscreen ImVec4
---@field TempBuffer ImVector_ImVec2
---@field DrawLists ImVector_ImDrawListPtr
---@field Context ImGuiContext
---@field ArcFastVtx ImVec2
---@field ArcFastRadiusCutoff number
---@field CircleSegmentCounts userdata
ImDrawListSharedData = {
    ---ImDrawListSharedData()
    ---@return ImDrawListSharedData
    new = function () end,
	---SetCircleTessellationMaxError(float max_error)
	---@param self ImDrawListSharedData
	---@param max_error number
    SetCircleTessellationMaxError = function (self, max_error) end
}

---@class ImDrawListSplitter
---@field _Current integer
---@field _Count integer
---@field _Channels ImVector_ImDrawChannel
ImDrawListSplitter = {
    ---Clear()
	---@param self ImDrawListSplitter
    Clear = function (self) end,
	---ClearFreeMemory()
	---@param self ImDrawListSplitter
    ClearFreeMemory = function (self) end,
	---ImDrawListSplitter()
    ---@return ImDrawListSplitter
    new = function () end,
	---Merge(ImDrawList* draw_list)
	---@param self ImDrawListSplitter
	---@param draw_list ImDrawList
    Merge = function (self, draw_list) end,
	---SetCurrentChannel(ImDrawList* draw_list,int channel_idx)
	---@param self ImDrawListSplitter
	---@param draw_list ImDrawList
	---@param channel_idx integer
    SetCurrentChannel = function (self, draw_list, channel_idx) end,
	---Split(ImDrawList* draw_list,int count)
	---@param self ImDrawListSplitter
	---@param draw_list ImDrawList
	---@param count integer
    Split = function (self, draw_list, count) end
}

---@class ImDrawVert
---@field pos ImVec2
---@field uv ImVec2
---@field col integer
ImDrawVert = {
    
}

---@class ImFont
---@field LastBaked ImFontBaked
---@field OwnerAtlas ImFontAtlas
---@field Flags ImFontFlags
---@field CurrentRasterizerDensity number
---@field FontId userdata
---@field LegacySize number
---@field Sources ImVector_ImFontConfigPtr
---@field EllipsisChar userdata
---@field FallbackChar userdata
---@field Used8kPagesMap userdata
---@field EllipsisAutoBake boolean
---@field RemapPairs ImGuiStorage
ImFont = {
    ---AddRemapChar(ImWchar from_codepoint,ImWchar to_codepoint)
	---@param self ImFont
	---@param from_codepoint userdata
	---@param to_codepoint userdata
    AddRemapChar = function (self, from_codepoint, to_codepoint) end,
	---CalcTextSizeA(float size,float max_width,float wrap_width,const char* text_begin,const char* text_end=((void*)0),const char** out_remaining=((void*)0))
	---@param self ImFont
	---@param size number
	---@param max_width number
	---@param wrap_width number
	---@param text_begin string
	---@param text_end string
	---@param out_remaining userdata
    ---@return userdata
    CalcTextSizeA = function (self, size, max_width, wrap_width, text_begin, text_end, out_remaining) end,
	---CalcWordWrapPosition(float size,const char* text,const char* text_end,float wrap_width)
	---@param self ImFont
	---@param size number
	---@param text string
	---@param text_end string
	---@param wrap_width number
    ---@return string
    CalcWordWrapPosition = function (self, size, text, text_end, wrap_width) end,
	---ClearOutputData()
	---@param self ImFont
    ClearOutputData = function (self) end,
	---GetDebugName()
	---@param self ImFont
    ---@return string
    GetDebugName = function (self) end,
	---GetFontBaked(float font_size,float density=-1.0f)
	---@param self ImFont
	---@param font_size number
	---@param density number
    ---@return ImFontBaked
    GetFontBaked = function (self, font_size, density) end,
	---ImFont()
    ---@return ImFont
    new = function () end,
	---IsGlyphInFont(ImWchar c)
	---@param self ImFont
	---@param c userdata
    ---@return boolean
    IsGlyphInFont = function (self, c) end,
	---IsGlyphRangeUnused(unsigned int c_begin,unsigned int c_last)
	---@param self ImFont
	---@param c_begin integer
	---@param c_last integer
    ---@return boolean
    IsGlyphRangeUnused = function (self, c_begin, c_last) end,
	---IsLoaded()
	---@param self ImFont
    ---@return boolean
    IsLoaded = function (self) end,
	---RenderChar(ImDrawList* draw_list,float size,const ImVec2& pos,ImU32 col,ImWchar c,const ImVec4* cpu_fine_clip=((void*)0))
	---@param self ImFont
	---@param draw_list ImDrawList
	---@param size number
	---@param pos ImVec2
	---@param col integer
	---@param c userdata
	---@param cpu_fine_clip ImVec4
    RenderChar = function (self, draw_list, size, pos, col, c, cpu_fine_clip) end,
	---RenderText(ImDrawList* draw_list,float size,const ImVec2& pos,ImU32 col,const ImVec4& clip_rect,const char* text_begin,const char* text_end,float wrap_width=0.0f,ImDrawTextFlags flags=0)
	---@param self ImFont
	---@param draw_list ImDrawList
	---@param size number
	---@param pos ImVec2
	---@param col integer
	---@param clip_rect ImVec4
	---@param text_begin string
	---@param text_end string
	---@param wrap_width number
	---@param flags ImDrawTextFlags
    RenderText = function (self, draw_list, size, pos, col, clip_rect, text_begin, text_end, wrap_width, flags) end
}

---@class ImFontAtlas
---@field Flags ImFontAtlasFlags
---@field TexDesiredFormat ImTextureFormat
---@field TexGlyphPadding integer
---@field TexMinWidth integer
---@field TexMinHeight integer
---@field TexMaxWidth integer
---@field TexMaxHeight integer
---@field UserData userdata
---@field TexRef ImTextureRef
---@field TexData ImTextureData
---@field TexList ImVector_ImTextureDataPtr
---@field Locked boolean
---@field RendererHasTextures boolean
---@field TexIsBuilt boolean
---@field TexPixelsUseColors boolean
---@field TexUvScale ImVec2
---@field TexUvWhitePixel ImVec2
---@field Fonts ImVector_ImFontPtr
---@field Sources ImVector_ImFontConfig
---@field TexUvLines ImVec4
---@field TexNextUniqueID integer
---@field FontNextUniqueID integer
---@field DrawListSharedDatas ImVector_ImDrawListSharedDataPtr
---@field Builder ImFontAtlasBuilder
---@field FontLoader ImFontLoader
---@field FontLoaderName string
---@field FontLoaderData userdata
---@field FontLoaderFlags integer
---@field RefCount integer
---@field OwnerContext ImGuiContext
ImFontAtlas = {
    ---AddCustomRect(int width,int height,ImFontAtlasRect* out_r=((void*)0))
	---@param self ImFontAtlas
	---@param width integer
	---@param height integer
	---@param out_r ImFontAtlasRect
    ---@return userdata
    AddCustomRect = function (self, width, height, out_r) end,
	---AddFont(const ImFontConfig* font_cfg)
	---@param self ImFontAtlas
	---@param font_cfg ImFontConfig
    ---@return ImFont
    AddFont = function (self, font_cfg) end,
	---AddFontDefault(const ImFontConfig* font_cfg=((void*)0))
	---@param self ImFontAtlas
	---@param font_cfg ImFontConfig
    ---@return ImFont
    AddFontDefault = function (self, font_cfg) end,
	---AddFontFromFileTTF(const char* filename,float size_pixels=0.0f,const ImFontConfig* font_cfg=((void*)0),const ImWchar* glyph_ranges=((void*)0))
	---@param self ImFontAtlas
	---@param filename string
	---@param size_pixels number
	---@param font_cfg ImFontConfig
	---@param glyph_ranges userdata
    ---@return ImFont
    AddFontFromFileTTF = function (self, filename, size_pixels, font_cfg, glyph_ranges) end,
	---AddFontFromMemoryCompressedBase85TTF(const char* compressed_font_data_base85,float size_pixels=0.0f,const ImFontConfig* font_cfg=((void*)0),const ImWchar* glyph_ranges=((void*)0))
	---@param self ImFontAtlas
	---@param compressed_font_data_base85 string
	---@param size_pixels number
	---@param font_cfg ImFontConfig
	---@param glyph_ranges userdata
    ---@return ImFont
    AddFontFromMemoryCompressedBase85TTF = function (self, compressed_font_data_base85, size_pixels, font_cfg, glyph_ranges) end,
	---AddFontFromMemoryCompressedTTF(const void* compressed_font_data,int compressed_font_data_size,float size_pixels=0.0f,const ImFontConfig* font_cfg=((void*)0),const ImWchar* glyph_ranges=((void*)0))
	---@param self ImFontAtlas
	---@param compressed_font_data userdata
	---@param compressed_font_data_size integer
	---@param size_pixels number
	---@param font_cfg ImFontConfig
	---@param glyph_ranges userdata
    ---@return ImFont
    AddFontFromMemoryCompressedTTF = function (self, compressed_font_data, compressed_font_data_size, size_pixels, font_cfg, glyph_ranges) end,
	---AddFontFromMemoryTTF(void* font_data,int font_data_size,float size_pixels=0.0f,const ImFontConfig* font_cfg=((void*)0),const ImWchar* glyph_ranges=((void*)0))
	---@param self ImFontAtlas
	---@param font_data userdata
	---@param font_data_size integer
	---@param size_pixels number
	---@param font_cfg ImFontConfig
	---@param glyph_ranges userdata
    ---@return ImFont
    AddFontFromMemoryTTF = function (self, font_data, font_data_size, size_pixels, font_cfg, glyph_ranges) end,
	---Clear()
	---@param self ImFontAtlas
    Clear = function (self) end,
	---ClearFonts()
	---@param self ImFontAtlas
    ClearFonts = function (self) end,
	---ClearInputData()
	---@param self ImFontAtlas
    ClearInputData = function (self) end,
	---ClearTexData()
	---@param self ImFontAtlas
    ClearTexData = function (self) end,
	---CompactCache()
	---@param self ImFontAtlas
    CompactCache = function (self) end,
	---GetCustomRect(ImFontAtlasRectId id,ImFontAtlasRect* out_r)
	---@param self ImFontAtlas
	---@param id userdata
	---@param out_r ImFontAtlasRect
    ---@return boolean
    GetCustomRect = function (self, id, out_r) end,
	---GetGlyphRangesDefault()
	---@param self ImFontAtlas
    ---@return userdata
    GetGlyphRangesDefault = function (self) end,
	---ImFontAtlas()
    ---@return ImFontAtlas
    new = function () end,
	---RemoveCustomRect(ImFontAtlasRectId id)
	---@param self ImFontAtlas
	---@param id userdata
    RemoveCustomRect = function (self, id) end,
	---RemoveFont(ImFont* font)
	---@param self ImFontAtlas
	---@param font ImFont
    RemoveFont = function (self, font) end,
	---SetFontLoader(const ImFontLoader* font_loader)
	---@param self ImFontAtlas
	---@param font_loader ImFontLoader
    SetFontLoader = function (self, font_loader) end
}

---@class ImFontAtlasBuilder
---@field PackContext stbrp_context_opaque
---@field PackNodes ImVector_stbrp_node_im
---@field Rects ImVector_ImTextureRect
---@field RectsIndex ImVector_ImFontAtlasRectEntry
---@field TempBuffer ImVector_unsigned_char
---@field RectsIndexFreeListStart integer
---@field RectsPackedCount integer
---@field RectsPackedSurface integer
---@field RectsDiscardedCount integer
---@field RectsDiscardedSurface integer
---@field FrameCount integer
---@field MaxRectSize ImVec2i
---@field MaxRectBounds ImVec2i
---@field LockDisableResize boolean
---@field PreloadedAllGlyphsRanges boolean
---@field BakedPool userdata
---@field BakedMap ImGuiStorage
---@field BakedDiscardedCount integer
---@field PackIdMouseCursors userdata
---@field PackIdLinesTexData userdata
ImFontAtlasBuilder = {
    ---ImFontAtlasBuilder()
    ---@return ImFontAtlasBuilder
    new = function () end
}

---@class ImFontAtlasPostProcessData
---@field FontAtlas ImFontAtlas
---@field Font ImFont
---@field FontSrc ImFontConfig
---@field FontBaked ImFontBaked
---@field Glyph ImFontGlyph
---@field Pixels userdata
---@field Format ImTextureFormat
---@field Pitch integer
---@field Width integer
---@field Height integer
ImFontAtlasPostProcessData = {
    
}

---@class ImFontAtlasRect
---@field x integer
---@field y integer
---@field w integer
---@field h integer
---@field uv0 ImVec2
---@field uv1 ImVec2
ImFontAtlasRect = {
    ---ImFontAtlasRect()
    ---@return ImFontAtlasRect
    new = function () end
}

---@class ImFontAtlasRectEntry
---@field TargetIndex integer
---@field Generation integer
---@field IsUsed integer
ImFontAtlasRectEntry = {
    
}

---@class ImFontBaked
---@field IndexAdvanceX ImVector_float
---@field FallbackAdvanceX number
---@field Size number
---@field RasterizerDensity number
---@field IndexLookup ImVector_ImU16
---@field Glyphs ImVector_ImFontGlyph
---@field FallbackGlyphIndex integer
---@field Ascent number
---@field Descent number
---@field MetricsTotalSurface integer
---@field WantDestroy integer
---@field LoadNoFallback integer
---@field LoadNoRenderOnLayout integer
---@field LastUsedFrame integer
---@field BakedId userdata
---@field OwnerFont ImFont
---@field FontLoaderDatas userdata
ImFontBaked = {
    ---ClearOutputData()
	---@param self ImFontBaked
    ClearOutputData = function (self) end,
	---FindGlyph(ImWchar c)
	---@param self ImFontBaked
	---@param c userdata
    ---@return ImFontGlyph
    FindGlyph = function (self, c) end,
	---FindGlyphNoFallback(ImWchar c)
	---@param self ImFontBaked
	---@param c userdata
    ---@return ImFontGlyph
    FindGlyphNoFallback = function (self, c) end,
	---GetCharAdvance(ImWchar c)
	---@param self ImFontBaked
	---@param c userdata
    ---@return number
    GetCharAdvance = function (self, c) end,
	---ImFontBaked()
    ---@return ImFontBaked
    new = function () end,
	---IsGlyphLoaded(ImWchar c)
	---@param self ImFontBaked
	---@param c userdata
    ---@return boolean
    IsGlyphLoaded = function (self, c) end
}

---@class ImFontConfig
---@field Name userdata
---@field FontData userdata
---@field FontDataSize integer
---@field FontDataOwnedByAtlas boolean
---@field MergeMode boolean
---@field PixelSnapH boolean
---@field PixelSnapV boolean
---@field OversampleH userdata
---@field OversampleV userdata
---@field EllipsisChar userdata
---@field SizePixels number
---@field GlyphRanges userdata
---@field GlyphExcludeRanges userdata
---@field GlyphOffset ImVec2
---@field GlyphMinAdvanceX number
---@field GlyphMaxAdvanceX number
---@field GlyphExtraAdvanceX number
---@field FontNo integer
---@field FontLoaderFlags integer
---@field RasterizerMultiply number
---@field RasterizerDensity number
---@field Flags ImFontFlags
---@field DstFont ImFont
---@field FontLoader ImFontLoader
---@field FontLoaderData userdata
ImFontConfig = {
    ---ImFontConfig()
    ---@return ImFontConfig
    new = function () end
}

---@class ImFontGlyph
---@field Colored integer
---@field Visible integer
---@field SourceIdx integer
---@field Codepoint integer
---@field AdvanceX number
---@field X0 number
---@field Y0 number
---@field X1 number
---@field Y1 number
---@field U0 number
---@field V0 number
---@field U1 number
---@field V1 number
---@field PackId integer
ImFontGlyph = {
    ---ImFontGlyph()
    ---@return ImFontGlyph
    new = function () end
}

---@class ImFontGlyphRangesBuilder
---@field UsedChars ImVector_ImU32
ImFontGlyphRangesBuilder = {
    ---AddChar(ImWchar c)
	---@param self ImFontGlyphRangesBuilder
	---@param c userdata
    AddChar = function (self, c) end,
	---AddRanges(const ImWchar* ranges)
	---@param self ImFontGlyphRangesBuilder
	---@param ranges userdata
    AddRanges = function (self, ranges) end,
	---AddText(const char* text,const char* text_end=((void*)0))
	---@param self ImFontGlyphRangesBuilder
	---@param text string
	---@param text_end string
    AddText = function (self, text, text_end) end,
	---BuildRanges(ImVector<ImWchar>* out_ranges)
	---@param self ImFontGlyphRangesBuilder
	---@param out_ranges ImVector_ImWchar
    BuildRanges = function (self, out_ranges) end,
	---Clear()
	---@param self ImFontGlyphRangesBuilder
    Clear = function (self) end,
	---GetBit(size_t n)
	---@param self ImFontGlyphRangesBuilder
	---@param n userdata
    ---@return boolean
    GetBit = function (self, n) end,
	---ImFontGlyphRangesBuilder()
    ---@return ImFontGlyphRangesBuilder
    new = function () end,
	---SetBit(size_t n)
	---@param self ImFontGlyphRangesBuilder
	---@param n userdata
    SetBit = function (self, n) end
}

---@class ImFontLoader
---@field Name string
---@field LoaderInit userdata
---@field LoaderShutdown userdata
---@field FontSrcInit userdata
---@field FontSrcDestroy userdata
---@field FontSrcContainsGlyph userdata
---@field FontBakedInit userdata
---@field FontBakedDestroy userdata
---@field FontBakedLoadGlyph userdata
---@field FontBakedSrcLoaderDataSize userdata
ImFontLoader = {
    ---ImFontLoader()
    ---@return ImFontLoader
    new = function () end
}

---@class ImFontStackData
---@field Font ImFont
---@field FontSizeBeforeScaling number
---@field FontSizeAfterScaling number
ImFontStackData = {
    
}

---@class ImGuiBoxSelectState
---@field ID userdata
---@field IsActive boolean
---@field IsStarting boolean
---@field IsStartedFromVoid boolean
---@field IsStartedSetNavIdOnce boolean
---@field RequestClear boolean
---@field KeyMods userdata
---@field StartPosRel ImVec2
---@field EndPosRel ImVec2
---@field ScrollAccum ImVec2
---@field Window ImGuiWindow
---@field UnclipMode boolean
---@field UnclipRect ImRect
---@field BoxSelectRectPrev ImRect
---@field BoxSelectRectCurr ImRect
ImGuiBoxSelectState = {
    ---ImGuiBoxSelectState()
    ---@return ImGuiBoxSelectState
    new = function () end
}

---@class ImGuiColorMod
---@field Col ImGuiCol
---@field BackupValue ImVec4
ImGuiColorMod = {
    
}

---@class ImGuiComboPreviewData
---@field PreviewRect ImRect
---@field BackupCursorPos ImVec2
---@field BackupCursorMaxPos ImVec2
---@field BackupCursorPosPrevLine ImVec2
---@field BackupPrevLineTextBaseOffset number
---@field BackupLayout ImGuiLayoutType
ImGuiComboPreviewData = {
    ---ImGuiComboPreviewData()
    ---@return ImGuiComboPreviewData
    new = function () end
}

---@class ImGuiContext
---@field Initialized boolean
---@field WithinFrameScope boolean
---@field WithinFrameScopeWithImplicitWindow boolean
---@field TestEngineHookItems boolean
---@field FrameCount integer
---@field FrameCountEnded integer
---@field FrameCountRendered integer
---@field Time number
---@field ContextName userdata
---@field IO ImGuiIO
---@field PlatformIO ImGuiPlatformIO
---@field Style ImGuiStyle
---@field FontAtlases ImVector_ImFontAtlasPtr
---@field Font ImFont
---@field FontBaked ImFontBaked
---@field FontSize number
---@field FontSizeBase number
---@field FontBakedScale number
---@field FontRasterizerDensity number
---@field CurrentDpiScale number
---@field DrawListSharedData ImDrawListSharedData
---@field WithinEndChildID userdata
---@field TestEngine userdata
---@field InputEventsQueue ImVector_ImGuiInputEvent
---@field InputEventsTrail ImVector_ImGuiInputEvent
---@field InputEventsNextMouseSource ImGuiMouseSource
---@field InputEventsNextEventId integer
---@field Windows ImVector_ImGuiWindowPtr
---@field WindowsFocusOrder ImVector_ImGuiWindowPtr
---@field WindowsTempSortBuffer ImVector_ImGuiWindowPtr
---@field CurrentWindowStack ImVector_ImGuiWindowStackData
---@field WindowsById ImGuiStorage
---@field WindowsActiveCount integer
---@field WindowsBorderHoverPadding number
---@field DebugBreakInWindow userdata
---@field CurrentWindow ImGuiWindow
---@field HoveredWindow ImGuiWindow
---@field HoveredWindowUnderMovingWindow ImGuiWindow
---@field HoveredWindowBeforeClear ImGuiWindow
---@field MovingWindow ImGuiWindow
---@field WheelingWindow ImGuiWindow
---@field WheelingWindowRefMousePos ImVec2
---@field WheelingWindowStartFrame integer
---@field WheelingWindowScrolledFrame integer
---@field WheelingWindowReleaseTimer number
---@field WheelingWindowWheelRemainder ImVec2
---@field WheelingAxisAvg ImVec2
---@field DebugDrawIdConflictsId userdata
---@field DebugHookIdInfoId userdata
---@field HoveredId userdata
---@field HoveredIdPreviousFrame userdata
---@field HoveredIdPreviousFrameItemCount integer
---@field HoveredIdTimer number
---@field HoveredIdNotActiveTimer number
---@field HoveredIdAllowOverlap boolean
---@field HoveredIdIsDisabled boolean
---@field ItemUnclipByLog boolean
---@field ActiveId userdata
---@field ActiveIdIsAlive userdata
---@field ActiveIdTimer number
---@field ActiveIdIsJustActivated boolean
---@field ActiveIdAllowOverlap boolean
---@field ActiveIdNoClearOnFocusLoss boolean
---@field ActiveIdHasBeenPressedBefore boolean
---@field ActiveIdHasBeenEditedBefore boolean
---@field ActiveIdHasBeenEditedThisFrame boolean
---@field ActiveIdFromShortcut boolean
---@field ActiveIdMouseButton userdata
---@field ActiveIdDisabledId userdata
---@field ActiveIdClickOffset ImVec2
---@field ActiveIdSource ImGuiInputSource
---@field ActiveIdWindow ImGuiWindow
---@field ActiveIdPreviousFrame userdata
---@field DeactivatedItemData ImGuiDeactivatedItemData
---@field ActiveIdValueOnActivation ImGuiDataTypeStorage
---@field LastActiveId userdata
---@field LastActiveIdTimer number
---@field LastKeyModsChangeTime number
---@field LastKeyModsChangeFromNoneTime number
---@field LastKeyboardKeyPressTime number
---@field KeysMayBeCharInput userdata
---@field KeysOwnerData ImGuiKeyOwnerData
---@field KeysRoutingTable ImGuiKeyRoutingTable
---@field ActiveIdUsingNavDirMask integer
---@field ActiveIdUsingAllKeyboardKeys boolean
---@field DebugBreakInShortcutRouting userdata
---@field CurrentFocusScopeId userdata
---@field CurrentItemFlags ImGuiItemFlags
---@field DebugLocateId userdata
---@field NextItemData ImGuiNextItemData
---@field LastItemData ImGuiLastItemData
---@field NextWindowData ImGuiNextWindowData
---@field DebugShowGroupRects boolean
---@field GcCompactAll boolean
---@field DebugFlashStyleColorIdx ImGuiCol
---@field ColorStack ImVector_ImGuiColorMod
---@field StyleVarStack ImVector_ImGuiStyleMod
---@field FontStack ImVector_ImFontStackData
---@field FocusScopeStack ImVector_ImGuiFocusScopeData
---@field ItemFlagsStack ImVector_ImGuiItemFlags
---@field GroupStack ImVector_ImGuiGroupData
---@field OpenPopupStack ImVector_ImGuiPopupData
---@field BeginPopupStack ImVector_ImGuiPopupData
---@field TreeNodeStack ImVector_ImGuiTreeNodeStackData
---@field Viewports ImVector_ImGuiViewportPPtr
---@field NavCursorVisible boolean
---@field NavHighlightItemUnderNav boolean
---@field NavMousePosDirty boolean
---@field NavIdIsAlive boolean
---@field NavId userdata
---@field NavWindow ImGuiWindow
---@field NavFocusScopeId userdata
---@field NavLayer ImGuiNavLayer
---@field NavActivateId userdata
---@field NavActivateDownId userdata
---@field NavActivatePressedId userdata
---@field NavActivateFlags ImGuiActivateFlags
---@field NavFocusRoute ImVector_ImGuiFocusScopeData
---@field NavHighlightActivatedId userdata
---@field NavHighlightActivatedTimer number
---@field NavNextActivateId userdata
---@field NavNextActivateFlags ImGuiActivateFlags
---@field NavInputSource ImGuiInputSource
---@field NavLastValidSelectionUserData userdata
---@field NavCursorHideFrames userdata
---@field NavAnyRequest boolean
---@field NavInitRequest boolean
---@field NavInitRequestFromMove boolean
---@field NavInitResult ImGuiNavItemData
---@field NavMoveSubmitted boolean
---@field NavMoveScoringItems boolean
---@field NavMoveForwardToNextFrame boolean
---@field NavMoveFlags ImGuiNavMoveFlags
---@field NavMoveScrollFlags ImGuiScrollFlags
---@field NavMoveKeyMods userdata
---@field NavMoveDir ImGuiDir
---@field NavMoveDirForDebug ImGuiDir
---@field NavMoveClipDir ImGuiDir
---@field NavScoringRect ImRect
---@field NavScoringNoClipRect ImRect
---@field NavScoringDebugCount integer
---@field NavTabbingDir integer
---@field NavTabbingCounter integer
---@field NavMoveResultLocal ImGuiNavItemData
---@field NavMoveResultLocalVisible ImGuiNavItemData
---@field NavMoveResultOther ImGuiNavItemData
---@field NavTabbingResultFirst ImGuiNavItemData
---@field NavJustMovedFromFocusScopeId userdata
---@field NavJustMovedToId userdata
---@field NavJustMovedToFocusScopeId userdata
---@field NavJustMovedToKeyMods userdata
---@field NavJustMovedToIsTabbing boolean
---@field NavJustMovedToHasSelectionData boolean
---@field ConfigNavWindowingWithGamepad boolean
---@field ConfigNavWindowingKeyNext userdata
---@field ConfigNavWindowingKeyPrev userdata
---@field NavWindowingTarget ImGuiWindow
---@field NavWindowingTargetAnim ImGuiWindow
---@field NavWindowingListWindow ImGuiWindow
---@field NavWindowingTimer number
---@field NavWindowingHighlightAlpha number
---@field NavWindowingInputSource ImGuiInputSource
---@field NavWindowingToggleLayer boolean
---@field NavWindowingToggleKey ImGuiKey
---@field NavWindowingAccumDeltaPos ImVec2
---@field NavWindowingAccumDeltaSize ImVec2
---@field DimBgRatio number
---@field DragDropActive boolean
---@field DragDropWithinSource boolean
---@field DragDropWithinTarget boolean
---@field DragDropSourceFlags ImGuiDragDropFlags
---@field DragDropSourceFrameCount integer
---@field DragDropMouseButton integer
---@field DragDropPayload ImGuiPayload
---@field DragDropTargetRect ImRect
---@field DragDropTargetClipRect ImRect
---@field DragDropTargetId userdata
---@field DragDropTargetFullViewport userdata
---@field DragDropAcceptFlagsCurr ImGuiDragDropFlags
---@field DragDropAcceptFlagsPrev ImGuiDragDropFlags
---@field DragDropAcceptIdCurrRectSurface number
---@field DragDropAcceptIdCurr userdata
---@field DragDropAcceptIdPrev userdata
---@field DragDropAcceptFrameCount integer
---@field DragDropHoldJustPressedId userdata
---@field DragDropPayloadBufHeap ImVector_unsigned_char
---@field DragDropPayloadBufLocal integer
---@field ClipperTempDataStacked integer
---@field ClipperTempData ImVector_ImGuiListClipperData
---@field CurrentTable ImGuiTable
---@field DebugBreakInTable userdata
---@field TablesTempDataStacked integer
---@field TablesTempData ImVector_ImGuiTableTempData
---@field Tables userdata
---@field TablesLastTimeActive ImVector_float
---@field DrawChannelsTempMergeBuffer ImVector_ImDrawChannel
---@field CurrentTabBar ImGuiTabBar
---@field TabBars userdata
---@field CurrentTabBarStack ImVector_ImGuiPtrOrIndex
---@field ShrinkWidthBuffer ImVector_ImGuiShrinkWidthItem
---@field BoxSelectState ImGuiBoxSelectState
---@field CurrentMultiSelect ImGuiMultiSelectTempData
---@field MultiSelectTempDataStacked integer
---@field MultiSelectTempData ImVector_ImGuiMultiSelectTempData
---@field MultiSelectStorage userdata
---@field HoverItemDelayId userdata
---@field HoverItemDelayIdPreviousFrame userdata
---@field HoverItemDelayTimer number
---@field HoverItemDelayClearTimer number
---@field HoverItemUnlockedStationaryId userdata
---@field HoverWindowUnlockedStationaryId userdata
---@field MouseCursor ImGuiMouseCursor
---@field MouseStationaryTimer number
---@field MouseLastValidPos ImVec2
---@field InputTextState ImGuiInputTextState
---@field InputTextLineIndex ImGuiTextIndex
---@field InputTextDeactivatedState ImGuiInputTextDeactivatedState
---@field InputTextPasswordFontBackupBaked ImFontBaked
---@field InputTextPasswordFontBackupFlags ImFontFlags
---@field TempInputId userdata
---@field DataTypeZeroValue ImGuiDataTypeStorage
---@field BeginMenuDepth integer
---@field BeginComboDepth integer
---@field ColorEditOptions ImGuiColorEditFlags
---@field ColorEditCurrentID userdata
---@field ColorEditSavedID userdata
---@field ColorEditSavedHue number
---@field ColorEditSavedSat number
---@field ColorEditSavedColor integer
---@field ColorPickerRef ImVec4
---@field ComboPreviewData ImGuiComboPreviewData
---@field WindowResizeBorderExpectedRect ImRect
---@field WindowResizeRelativeMode boolean
---@field ScrollbarSeekMode integer
---@field ScrollbarClickDeltaToGrabCenter number
---@field SliderGrabClickOffset number
---@field SliderCurrentAccum number
---@field SliderCurrentAccumDirty boolean
---@field DragCurrentAccumDirty boolean
---@field DragCurrentAccum number
---@field DragSpeedDefaultRatio number
---@field DisabledAlphaBackup number
---@field DisabledStackSize integer
---@field TooltipOverrideCount integer
---@field TooltipPreviousWindow ImGuiWindow
---@field ClipboardHandlerData ImVector_char
---@field MenusIdSubmittedThisFrame ImVector_ImGuiID
---@field TypingSelectState ImGuiTypingSelectState
---@field PlatformImeData ImGuiPlatformImeData
---@field PlatformImeDataPrev ImGuiPlatformImeData
---@field UserTextures ImVector_ImTextureDataPtr
---@field SettingsLoaded boolean
---@field SettingsDirtyTimer number
---@field SettingsIniData ImGuiTextBuffer
---@field SettingsHandlers ImVector_ImGuiSettingsHandler
---@field SettingsWindows userdata
---@field SettingsTables userdata
---@field Hooks ImVector_ImGuiContextHook
---@field HookIdNext userdata
---@field LocalizationTable string
---@field LogEnabled boolean
---@field LogLineFirstItem boolean
---@field LogFlags ImGuiLogFlags
---@field LogWindow ImGuiWindow
---@field LogFile userdata
---@field LogBuffer ImGuiTextBuffer
---@field LogNextPrefix string
---@field LogNextSuffix string
---@field LogLinePosY number
---@field LogDepthRef integer
---@field LogDepthToExpand integer
---@field LogDepthToExpandDefault integer
---@field ErrorCallback userdata
---@field ErrorCallbackUserData userdata
---@field ErrorTooltipLockedPos ImVec2
---@field ErrorFirst boolean
---@field ErrorCountCurrentFrame integer
---@field StackSizesInNewFrame ImGuiErrorRecoveryState
---@field StackSizesInBeginForCurrentWindow ImGuiErrorRecoveryState
---@field DebugDrawIdConflictsCount integer
---@field DebugLogFlags ImGuiDebugLogFlags
---@field DebugLogBuf ImGuiTextBuffer
---@field DebugLogIndex ImGuiTextIndex
---@field DebugLogSkippedErrors integer
---@field DebugLogAutoDisableFlags ImGuiDebugLogFlags
---@field DebugLogAutoDisableFrames userdata
---@field DebugLocateFrames userdata
---@field DebugBreakInLocateId boolean
---@field DebugBreakKeyChord userdata
---@field DebugBeginReturnValueCullDepth userdata
---@field DebugItemPickerActive boolean
---@field DebugItemPickerMouseButton userdata
---@field DebugItemPickerBreakId userdata
---@field DebugFlashStyleColorTime number
---@field DebugFlashStyleColorBackup ImVec4
---@field DebugMetricsConfig ImGuiMetricsConfig
---@field DebugItemPathQuery ImGuiDebugItemPathQuery
---@field DebugIDStackTool ImGuiIDStackTool
---@field DebugAllocInfo ImGuiDebugAllocInfo
---@field FramerateSecPerFrame number
---@field FramerateSecPerFrameIdx integer
---@field FramerateSecPerFrameCount integer
---@field FramerateSecPerFrameAccum number
---@field WantCaptureMouseNextFrame integer
---@field WantCaptureKeyboardNextFrame integer
---@field WantTextInputNextFrame integer
---@field TempBuffer ImVector_char
---@field TempKeychordName userdata
ImGuiContext = {
    ---ImGuiContext(ImFontAtlas* shared_font_atlas)
	---@param shared_font_atlas ImFontAtlas
    ---@return ImGuiContext
    new = function (shared_font_atlas) end
}

---@class ImGuiContextHook
---@field HookId userdata
---@field Type ImGuiContextHookType
---@field Owner userdata
---@field Callback userdata
---@field UserData userdata
ImGuiContextHook = {
    ---ImGuiContextHook()
    ---@return ImGuiContextHook
    new = function () end
}

---@class ImGuiDataTypeInfo
---@field Size userdata
---@field Name string
---@field PrintFmt string
---@field ScanFmt string
ImGuiDataTypeInfo = {
    
}

---@class ImGuiDataTypeStorage
---@field Data userdata
ImGuiDataTypeStorage = {
    
}

---@class ImGuiDeactivatedItemData
---@field ID userdata
---@field ElapseFrame integer
---@field HasBeenEditedBefore boolean
---@field IsAlive boolean
ImGuiDeactivatedItemData = {
    
}

---@class ImGuiDebugAllocEntry
---@field FrameCount integer
---@field AllocCount userdata
---@field FreeCount userdata
ImGuiDebugAllocEntry = {
    
}

---@class ImGuiDebugAllocInfo
---@field TotalAllocCount integer
---@field TotalFreeCount integer
---@field LastEntriesIdx userdata
---@field LastEntriesBuf ImGuiDebugAllocEntry
ImGuiDebugAllocInfo = {
    ---ImGuiDebugAllocInfo()
    ---@return ImGuiDebugAllocInfo
    new = function () end
}

---@class ImGuiDebugItemPathQuery
---@field MainID userdata
---@field Active boolean
---@field Complete boolean
---@field Step userdata
---@field Results ImVector_ImGuiStackLevelInfo
---@field ResultsDescBuf ImGuiTextBuffer
---@field ResultPathBuf ImGuiTextBuffer
ImGuiDebugItemPathQuery = {
    ---ImGuiDebugItemPathQuery()
    ---@return ImGuiDebugItemPathQuery
    new = function () end
}

---@class ImGuiErrorRecoveryState
---@field SizeOfWindowStack integer
---@field SizeOfIDStack integer
---@field SizeOfTreeStack integer
---@field SizeOfColorStack integer
---@field SizeOfStyleVarStack integer
---@field SizeOfFontStack integer
---@field SizeOfFocusScopeStack integer
---@field SizeOfGroupStack integer
---@field SizeOfItemFlagsStack integer
---@field SizeOfBeginPopupStack integer
---@field SizeOfDisabledStack integer
ImGuiErrorRecoveryState = {
    ---ImGuiErrorRecoveryState()
    ---@return ImGuiErrorRecoveryState
    new = function () end
}

---@class ImGuiFocusScopeData
---@field ID userdata
---@field WindowID userdata
ImGuiFocusScopeData = {
    
}

---@class ImGuiGroupData
---@field WindowID userdata
---@field BackupCursorPos ImVec2
---@field BackupCursorMaxPos ImVec2
---@field BackupCursorPosPrevLine ImVec2
---@field BackupIndent ImVec1
---@field BackupGroupOffset ImVec1
---@field BackupCurrLineSize ImVec2
---@field BackupCurrLineTextBaseOffset number
---@field BackupActiveIdIsAlive userdata
---@field BackupActiveIdHasBeenEditedThisFrame boolean
---@field BackupDeactivatedIdIsAlive boolean
---@field BackupHoveredIdIsAlive boolean
---@field BackupIsSameLine boolean
---@field EmitItem boolean
ImGuiGroupData = {
    
}

---@class ImGuiIDStackTool
---@field OptHexEncodeNonAsciiChars boolean
---@field OptCopyToClipboardOnCtrlC boolean
---@field LastActiveFrame integer
---@field CopyToClipboardLastTime number
ImGuiIDStackTool = {
    ---ImGuiIDStackTool()
    ---@return ImGuiIDStackTool
    new = function () end
}

---@class ImGuiIO
---@field ConfigFlags ImGuiConfigFlags
---@field BackendFlags ImGuiBackendFlags
---@field DisplaySize ImVec2
---@field DisplayFramebufferScale ImVec2
---@field DeltaTime number
---@field IniSavingRate number
---@field IniFilename string
---@field LogFilename string
---@field UserData userdata
---@field Fonts ImFontAtlas
---@field FontDefault ImFont
---@field FontAllowUserScaling boolean
---@field ConfigNavSwapGamepadButtons boolean
---@field ConfigNavMoveSetMousePos boolean
---@field ConfigNavCaptureKeyboard boolean
---@field ConfigNavEscapeClearFocusItem boolean
---@field ConfigNavEscapeClearFocusWindow boolean
---@field ConfigNavCursorVisibleAuto boolean
---@field ConfigNavCursorVisibleAlways boolean
---@field MouseDrawCursor boolean
---@field ConfigMacOSXBehaviors boolean
---@field ConfigInputTrickleEventQueue boolean
---@field ConfigInputTextCursorBlink boolean
---@field ConfigInputTextEnterKeepActive boolean
---@field ConfigDragClickToInputText boolean
---@field ConfigWindowsResizeFromEdges boolean
---@field ConfigWindowsMoveFromTitleBarOnly boolean
---@field ConfigWindowsCopyContentsWithCtrlC boolean
---@field ConfigScrollbarScrollByPage boolean
---@field ConfigMemoryCompactTimer number
---@field MouseDoubleClickTime number
---@field MouseDoubleClickMaxDist number
---@field MouseDragThreshold number
---@field KeyRepeatDelay number
---@field KeyRepeatRate number
---@field ConfigErrorRecovery boolean
---@field ConfigErrorRecoveryEnableAssert boolean
---@field ConfigErrorRecoveryEnableDebugLog boolean
---@field ConfigErrorRecoveryEnableTooltip boolean
---@field ConfigDebugIsDebuggerPresent boolean
---@field ConfigDebugHighlightIdConflicts boolean
---@field ConfigDebugHighlightIdConflictsShowItemPicker boolean
---@field ConfigDebugBeginReturnValueOnce boolean
---@field ConfigDebugBeginReturnValueLoop boolean
---@field ConfigDebugIgnoreFocusLoss boolean
---@field ConfigDebugIniSettings boolean
---@field BackendPlatformName string
---@field BackendRendererName string
---@field BackendPlatformUserData userdata
---@field BackendRendererUserData userdata
---@field BackendLanguageUserData userdata
---@field WantCaptureMouse boolean
---@field WantCaptureKeyboard boolean
---@field WantTextInput boolean
---@field WantSetMousePos boolean
---@field WantSaveIniSettings boolean
---@field NavActive boolean
---@field NavVisible boolean
---@field Framerate number
---@field MetricsRenderVertices integer
---@field MetricsRenderIndices integer
---@field MetricsRenderWindows integer
---@field MetricsActiveWindows integer
---@field MouseDelta ImVec2
---@field Ctx ImGuiContext
---@field MousePos ImVec2
---@field MouseDown boolean
---@field MouseWheel number
---@field MouseWheelH number
---@field MouseSource ImGuiMouseSource
---@field KeyCtrl boolean
---@field KeyShift boolean
---@field KeyAlt boolean
---@field KeySuper boolean
---@field KeyMods userdata
---@field KeysData ImGuiKeyData
---@field WantCaptureMouseUnlessPopupClose boolean
---@field MousePosPrev ImVec2
---@field MouseClickedPos ImVec2
---@field MouseClickedTime number
---@field MouseClicked boolean
---@field MouseDoubleClicked boolean
---@field MouseClickedCount userdata
---@field MouseClickedLastCount userdata
---@field MouseReleased boolean
---@field MouseReleasedTime number
---@field MouseDownOwned boolean
---@field MouseDownOwnedUnlessPopupClose boolean
---@field MouseWheelRequestAxisSwap boolean
---@field MouseCtrlLeftAsRightClick boolean
---@field MouseDownDuration number
---@field MouseDownDurationPrev number
---@field MouseDragMaxDistanceSqr number
---@field PenPressure number
---@field AppFocusLost boolean
---@field AppAcceptingEvents boolean
---@field InputQueueSurrogate userdata
---@field InputQueueCharacters ImVector_ImWchar
ImGuiIO = {
    ---AddFocusEvent(bool focused)
	---@param self ImGuiIO
	---@param focused boolean
    AddFocusEvent = function (self, focused) end,
	---AddInputCharacter(unsigned int c)
	---@param self ImGuiIO
	---@param c integer
    AddInputCharacter = function (self, c) end,
	---AddInputCharacterUTF16(ImWchar16 c)
	---@param self ImGuiIO
	---@param c userdata
    AddInputCharacterUTF16 = function (self, c) end,
	---AddInputCharactersUTF8(const char* str)
	---@param self ImGuiIO
	---@param str string
    AddInputCharactersUTF8 = function (self, str) end,
	---AddKeyAnalogEvent(ImGuiKey key,bool down,float v)
	---@param self ImGuiIO
	---@param key ImGuiKey
	---@param down boolean
	---@param v number
    AddKeyAnalogEvent = function (self, key, down, v) end,
	---AddKeyEvent(ImGuiKey key,bool down)
	---@param self ImGuiIO
	---@param key ImGuiKey
	---@param down boolean
    AddKeyEvent = function (self, key, down) end,
	---AddMouseButtonEvent(int button,bool down)
	---@param self ImGuiIO
	---@param button integer
	---@param down boolean
    AddMouseButtonEvent = function (self, button, down) end,
	---AddMousePosEvent(float x,float y)
	---@param self ImGuiIO
	---@param x number
	---@param y number
    AddMousePosEvent = function (self, x, y) end,
	---AddMouseSourceEvent(ImGuiMouseSource source)
	---@param self ImGuiIO
	---@param source ImGuiMouseSource
    AddMouseSourceEvent = function (self, source) end,
	---AddMouseWheelEvent(float wheel_x,float wheel_y)
	---@param self ImGuiIO
	---@param wheel_x number
	---@param wheel_y number
    AddMouseWheelEvent = function (self, wheel_x, wheel_y) end,
	---ClearEventsQueue()
	---@param self ImGuiIO
    ClearEventsQueue = function (self) end,
	---ClearInputKeys()
	---@param self ImGuiIO
    ClearInputKeys = function (self) end,
	---ClearInputMouse()
	---@param self ImGuiIO
    ClearInputMouse = function (self) end,
	---ImGuiIO()
    ---@return ImGuiIO
    new = function () end,
	---SetAppAcceptingEvents(bool accepting_events)
	---@param self ImGuiIO
	---@param accepting_events boolean
    SetAppAcceptingEvents = function (self, accepting_events) end,
	---SetKeyEventNativeData(ImGuiKey key,int native_keycode,int native_scancode,int native_legacy_index=-1)
	---@param self ImGuiIO
	---@param key ImGuiKey
	---@param native_keycode integer
	---@param native_scancode integer
	---@param native_legacy_index integer
    SetKeyEventNativeData = function (self, key, native_keycode, native_scancode, native_legacy_index) end
}

---@class ImGuiInputEvent
---@field Type ImGuiInputEventType
---@field Source ImGuiInputSource
---@field EventId integer
---@field MousePos ImGuiInputEventMousePos
---@field MouseWheel ImGuiInputEventMouseWheel
---@field MouseButton ImGuiInputEventMouseButton
---@field Key ImGuiInputEventKey
---@field Text ImGuiInputEventText
---@field AppFocused ImGuiInputEventAppFocused
---@field AddedByTestEngine boolean
ImGuiInputEvent = {
    ---ImGuiInputEvent()
    ---@return ImGuiInputEvent
    new = function () end
}

---@class ImGuiInputEventAppFocused
---@field Focused boolean
ImGuiInputEventAppFocused = {
    
}

---@class ImGuiInputEventKey
---@field Key ImGuiKey
---@field Down boolean
---@field AnalogValue number
ImGuiInputEventKey = {
    
}

---@class ImGuiInputEventMouseButton
---@field Button integer
---@field Down boolean
---@field MouseSource ImGuiMouseSource
ImGuiInputEventMouseButton = {
    
}

---@class ImGuiInputEventMousePos
---@field PosX number
---@field PosY number
---@field MouseSource ImGuiMouseSource
ImGuiInputEventMousePos = {
    
}

---@class ImGuiInputEventMouseWheel
---@field WheelX number
---@field WheelY number
---@field MouseSource ImGuiMouseSource
ImGuiInputEventMouseWheel = {
    
}

---@class ImGuiInputEventText
---@field Char integer
ImGuiInputEventText = {
    
}

---@class ImGuiInputTextCallbackData
---@field Ctx ImGuiContext
---@field EventFlag ImGuiInputTextFlags
---@field Flags ImGuiInputTextFlags
---@field UserData userdata
---@field EventChar userdata
---@field EventKey ImGuiKey
---@field Buf userdata
---@field BufTextLen integer
---@field BufSize integer
---@field BufDirty boolean
---@field CursorPos integer
---@field SelectionStart integer
---@field SelectionEnd integer
ImGuiInputTextCallbackData = {
    ---ClearSelection()
	---@param self ImGuiInputTextCallbackData
    ClearSelection = function (self) end,
	---DeleteChars(int pos,int bytes_count)
	---@param self ImGuiInputTextCallbackData
	---@param pos integer
	---@param bytes_count integer
    DeleteChars = function (self, pos, bytes_count) end,
	---HasSelection()
	---@param self ImGuiInputTextCallbackData
    ---@return boolean
    HasSelection = function (self) end,
	---ImGuiInputTextCallbackData()
    ---@return ImGuiInputTextCallbackData
    new = function () end,
	---InsertChars(int pos,const char* text,const char* text_end=((void*)0))
	---@param self ImGuiInputTextCallbackData
	---@param pos integer
	---@param text string
	---@param text_end string
    InsertChars = function (self, pos, text, text_end) end,
	---SelectAll()
	---@param self ImGuiInputTextCallbackData
    SelectAll = function (self) end
}

---@class ImGuiInputTextDeactivatedState
---@field ID userdata
---@field TextA ImVector_char
ImGuiInputTextDeactivatedState = {
    ---ClearFreeMemory()
	---@param self ImGuiInputTextDeactivatedState
    ClearFreeMemory = function (self) end,
	---ImGuiInputTextDeactivatedState()
    ---@return ImGuiInputTextDeactivatedState
    new = function () end
}

---@class ImGuiInputTextState
---@field Ctx ImGuiContext
---@field Stb userdata
---@field Flags ImGuiInputTextFlags
---@field ID userdata
---@field TextLen integer
---@field TextSrc string
---@field TextA ImVector_char
---@field TextToRevertTo ImVector_char
---@field CallbackTextBackup ImVector_char
---@field BufCapacity integer
---@field Scroll ImVec2
---@field LineCount integer
---@field WrapWidth number
---@field CursorAnim number
---@field CursorFollow boolean
---@field CursorCenterY boolean
---@field SelectedAllMouseLock boolean
---@field Edited boolean
---@field WantReloadUserBuf boolean
---@field LastMoveDirectionLR userdata
---@field ReloadSelectionStart integer
---@field ReloadSelectionEnd integer
ImGuiInputTextState = {
    ---ClearFreeMemory()
	---@param self ImGuiInputTextState
    ClearFreeMemory = function (self) end,
	---ClearSelection()
	---@param self ImGuiInputTextState
    ClearSelection = function (self) end,
	---ClearText()
	---@param self ImGuiInputTextState
    ClearText = function (self) end,
	---CursorAnimReset()
	---@param self ImGuiInputTextState
    CursorAnimReset = function (self) end,
	---CursorClamp()
	---@param self ImGuiInputTextState
    CursorClamp = function (self) end,
	---GetCursorPos()
	---@param self ImGuiInputTextState
    ---@return integer
    GetCursorPos = function (self) end,
	---GetPreferredOffsetX()
	---@param self ImGuiInputTextState
    ---@return number
    GetPreferredOffsetX = function (self) end,
	---GetSelectionEnd()
	---@param self ImGuiInputTextState
    ---@return integer
    GetSelectionEnd = function (self) end,
	---GetSelectionStart()
	---@param self ImGuiInputTextState
    ---@return integer
    GetSelectionStart = function (self) end,
	---HasSelection()
	---@param self ImGuiInputTextState
    ---@return boolean
    HasSelection = function (self) end,
	---ImGuiInputTextState()
    ---@return ImGuiInputTextState
    new = function () end,
	---OnCharPressed(unsigned int c)
	---@param self ImGuiInputTextState
	---@param c integer
    OnCharPressed = function (self, c) end,
	---OnKeyPressed(int key)
	---@param self ImGuiInputTextState
	---@param key integer
    OnKeyPressed = function (self, key) end,
	---ReloadUserBufAndKeepSelection()
	---@param self ImGuiInputTextState
    ReloadUserBufAndKeepSelection = function (self) end,
	---ReloadUserBufAndMoveToEnd()
	---@param self ImGuiInputTextState
    ReloadUserBufAndMoveToEnd = function (self) end,
	---ReloadUserBufAndSelectAll()
	---@param self ImGuiInputTextState
    ReloadUserBufAndSelectAll = function (self) end,
	---SelectAll()
	---@param self ImGuiInputTextState
    SelectAll = function (self) end
}

---@class ImGuiKeyData
---@field Down boolean
---@field DownDuration number
---@field DownDurationPrev number
---@field AnalogValue number
ImGuiKeyData = {
    
}

---@class ImGuiKeyOwnerData
---@field OwnerCurr userdata
---@field OwnerNext userdata
---@field LockThisFrame boolean
---@field LockUntilRelease boolean
ImGuiKeyOwnerData = {
    ---ImGuiKeyOwnerData()
    ---@return ImGuiKeyOwnerData
    new = function () end
}

---@class ImGuiKeyRoutingData
---@field NextEntryIndex userdata
---@field Mods userdata
---@field RoutingCurrScore userdata
---@field RoutingNextScore userdata
---@field RoutingCurr userdata
---@field RoutingNext userdata
ImGuiKeyRoutingData = {
    ---ImGuiKeyRoutingData()
    ---@return ImGuiKeyRoutingData
    new = function () end
}

---@class ImGuiKeyRoutingTable
---@field Index userdata
---@field Entries ImVector_ImGuiKeyRoutingData
---@field EntriesNext ImVector_ImGuiKeyRoutingData
ImGuiKeyRoutingTable = {
    ---Clear()
	---@param self ImGuiKeyRoutingTable
    Clear = function (self) end,
	---ImGuiKeyRoutingTable()
    ---@return ImGuiKeyRoutingTable
    new = function () end
}

---@class ImGuiLastItemData
---@field ID userdata
---@field ItemFlags ImGuiItemFlags
---@field StatusFlags ImGuiItemStatusFlags
---@field Rect ImRect
---@field NavRect ImRect
---@field DisplayRect ImRect
---@field ClipRect ImRect
---@field Shortcut userdata
ImGuiLastItemData = {
    ---ImGuiLastItemData()
    ---@return ImGuiLastItemData
    new = function () end
}

---@class ImGuiListClipper
---@field Ctx ImGuiContext
---@field DisplayStart integer
---@field DisplayEnd integer
---@field ItemsCount integer
---@field ItemsHeight number
---@field StartPosY number
---@field StartSeekOffsetY number
---@field TempData userdata
---@field Flags ImGuiListClipperFlags
ImGuiListClipper = {
    ---Begin(int items_count,float items_height=-1.0f)
	---@param self ImGuiListClipper
	---@param items_count integer
	---@param items_height number
    Begin = function (self, items_count, items_height) end,
	---End()
	---@param self ImGuiListClipper
    End = function (self) end,
	---ImGuiListClipper()
    ---@return ImGuiListClipper
    new = function () end,
	---IncludeItemByIndex(int item_index)
	---@param self ImGuiListClipper
	---@param item_index integer
    IncludeItemByIndex = function (self, item_index) end,
	---IncludeItemsByIndex(int item_begin,int item_end)
	---@param self ImGuiListClipper
	---@param item_begin integer
	---@param item_end integer
    IncludeItemsByIndex = function (self, item_begin, item_end) end,
	---SeekCursorForItem(int item_index)
	---@param self ImGuiListClipper
	---@param item_index integer
    SeekCursorForItem = function (self, item_index) end,
	---Step()
	---@param self ImGuiListClipper
    ---@return boolean
    Step = function (self) end
}

---@class ImGuiListClipperData
---@field ListClipper ImGuiListClipper
---@field LossynessOffset number
---@field StepNo integer
---@field ItemsFrozen integer
---@field Ranges ImVector_ImGuiListClipperRange
ImGuiListClipperData = {
    ---ImGuiListClipperData()
    ---@return ImGuiListClipperData
    new = function () end,
	---Reset(ImGuiListClipper* clipper)
	---@param self ImGuiListClipperData
	---@param clipper ImGuiListClipper
    Reset = function (self, clipper) end
}

---@class ImGuiListClipperRange
---@field Min integer
---@field Max integer
---@field PosToIndexConvert boolean
---@field PosToIndexOffsetMin userdata
---@field PosToIndexOffsetMax userdata
ImGuiListClipperRange = {
    ---FromIndices(int min,int max)
	---@param min integer
	---@param max integer
    ---@return ImGuiListClipperRange
    FromIndices = function (min, max) end,
	---FromPositions(float y1,float y2,int off_min,int off_max)
	---@param y1 number
	---@param y2 number
	---@param off_min integer
	---@param off_max integer
    ---@return ImGuiListClipperRange
    FromPositions = function (y1, y2, off_min, off_max) end
}

---@class ImGuiLocEntry
---@field Key ImGuiLocKey
---@field Text string
ImGuiLocEntry = {
    
}

---@class ImGuiMenuColumns
---@field TotalWidth integer
---@field NextTotalWidth integer
---@field Spacing userdata
---@field OffsetIcon userdata
---@field OffsetLabel userdata
---@field OffsetShortcut userdata
---@field OffsetMark userdata
---@field Widths userdata
ImGuiMenuColumns = {
    ---CalcNextTotalWidth(bool update_offsets)
	---@param self ImGuiMenuColumns
	---@param update_offsets boolean
    CalcNextTotalWidth = function (self, update_offsets) end,
	---DeclColumns(float w_icon,float w_label,float w_shortcut,float w_mark)
	---@param self ImGuiMenuColumns
	---@param w_icon number
	---@param w_label number
	---@param w_shortcut number
	---@param w_mark number
    ---@return number
    DeclColumns = function (self, w_icon, w_label, w_shortcut, w_mark) end,
	---ImGuiMenuColumns()
    ---@return ImGuiMenuColumns
    new = function () end,
	---Update(float spacing,bool window_reappearing)
	---@param self ImGuiMenuColumns
	---@param spacing number
	---@param window_reappearing boolean
    Update = function (self, spacing, window_reappearing) end
}

---@class ImGuiMetricsConfig
---@field ShowDebugLog boolean
---@field ShowIDStackTool boolean
---@field ShowWindowsRects boolean
---@field ShowWindowsBeginOrder boolean
---@field ShowTablesRects boolean
---@field ShowDrawCmdMesh boolean
---@field ShowDrawCmdBoundingBoxes boolean
---@field ShowTextEncodingViewer boolean
---@field ShowTextureUsedRect boolean
---@field ShowWindowsRectsType integer
---@field ShowTablesRectsType integer
---@field HighlightMonitorIdx integer
---@field HighlightViewportID userdata
---@field ShowFontPreview boolean
ImGuiMetricsConfig = {
    
}

---@class ImGuiMultiSelectIO
---@field Requests ImVector_ImGuiSelectionRequest
---@field RangeSrcItem userdata
---@field NavIdItem userdata
---@field NavIdSelected boolean
---@field RangeSrcReset boolean
---@field ItemsCount integer
ImGuiMultiSelectIO = {
    
}

---@class ImGuiMultiSelectState
---@field Window ImGuiWindow
---@field ID userdata
---@field LastFrameActive integer
---@field LastSelectionSize integer
---@field RangeSelected userdata
---@field NavIdSelected userdata
---@field RangeSrcItem userdata
---@field NavIdItem userdata
ImGuiMultiSelectState = {
    ---ImGuiMultiSelectState()
    ---@return ImGuiMultiSelectState
    new = function () end
}

---@class ImGuiMultiSelectTempData
---@field IO ImGuiMultiSelectIO
---@field Storage ImGuiMultiSelectState
---@field FocusScopeId userdata
---@field Flags ImGuiMultiSelectFlags
---@field ScopeRectMin ImVec2
---@field BackupCursorMaxPos ImVec2
---@field LastSubmittedItem userdata
---@field BoxSelectId userdata
---@field KeyMods userdata
---@field LoopRequestSetAll userdata
---@field IsEndIO boolean
---@field IsFocused boolean
---@field IsKeyboardSetRange boolean
---@field NavIdPassedBy boolean
---@field RangeSrcPassedBy boolean
---@field RangeDstPassedBy boolean
ImGuiMultiSelectTempData = {
    ---Clear()
	---@param self ImGuiMultiSelectTempData
    Clear = function (self) end,
	---ClearIO()
	---@param self ImGuiMultiSelectTempData
    ClearIO = function (self) end,
	---ImGuiMultiSelectTempData()
    ---@return ImGuiMultiSelectTempData
    new = function () end
}

---@class ImGuiNavItemData
---@field Window ImGuiWindow
---@field ID userdata
---@field FocusScopeId userdata
---@field RectRel ImRect
---@field ItemFlags ImGuiItemFlags
---@field DistBox number
---@field DistCenter number
---@field DistAxial number
---@field SelectionUserData userdata
ImGuiNavItemData = {
    ---Clear()
	---@param self ImGuiNavItemData
    Clear = function (self) end,
	---ImGuiNavItemData()
    ---@return ImGuiNavItemData
    new = function () end
}

---@class ImGuiNextItemData
---@field HasFlags ImGuiNextItemDataFlags
---@field ItemFlags ImGuiItemFlags
---@field FocusScopeId userdata
---@field SelectionUserData userdata
---@field Width number
---@field Shortcut userdata
---@field ShortcutFlags ImGuiInputFlags
---@field OpenVal boolean
---@field OpenCond userdata
---@field RefVal ImGuiDataTypeStorage
---@field StorageId userdata
ImGuiNextItemData = {
    ---ClearFlags()
	---@param self ImGuiNextItemData
    ClearFlags = function (self) end,
	---ImGuiNextItemData()
    ---@return ImGuiNextItemData
    new = function () end
}

---@class ImGuiNextWindowData
---@field HasFlags ImGuiNextWindowDataFlags
---@field PosCond ImGuiCond
---@field SizeCond ImGuiCond
---@field CollapsedCond ImGuiCond
---@field PosVal ImVec2
---@field PosPivotVal ImVec2
---@field SizeVal ImVec2
---@field ContentSizeVal ImVec2
---@field ScrollVal ImVec2
---@field WindowFlags ImGuiWindowFlags
---@field ChildFlags ImGuiChildFlags
---@field CollapsedVal boolean
---@field SizeConstraintRect ImRect
---@field SizeCallback userdata
---@field SizeCallbackUserData userdata
---@field BgAlphaVal number
---@field MenuBarOffsetMinVal ImVec2
---@field RefreshFlagsVal ImGuiWindowRefreshFlags
ImGuiNextWindowData = {
    ---ClearFlags()
	---@param self ImGuiNextWindowData
    ClearFlags = function (self) end,
	---ImGuiNextWindowData()
    ---@return ImGuiNextWindowData
    new = function () end
}

---@class ImGuiOldColumnData
---@field OffsetNorm number
---@field OffsetNormBeforeResize number
---@field Flags ImGuiOldColumnFlags
---@field ClipRect ImRect
ImGuiOldColumnData = {
    ---ImGuiOldColumnData()
    ---@return ImGuiOldColumnData
    new = function () end
}

---@class ImGuiOldColumns
---@field ID userdata
---@field Flags ImGuiOldColumnFlags
---@field IsFirstFrame boolean
---@field IsBeingResized boolean
---@field Current integer
---@field Count integer
---@field OffMinX number
---@field OffMaxX number
---@field LineMinY number
---@field LineMaxY number
---@field HostCursorPosY number
---@field HostCursorMaxPosX number
---@field HostInitialClipRect ImRect
---@field HostBackupClipRect ImRect
---@field HostBackupParentWorkRect ImRect
---@field Columns ImVector_ImGuiOldColumnData
---@field Splitter ImDrawListSplitter
ImGuiOldColumns = {
    ---ImGuiOldColumns()
    ---@return ImGuiOldColumns
    new = function () end
}

---@class ImGuiOnceUponAFrame
---@field RefFrame integer
ImGuiOnceUponAFrame = {
    ---ImGuiOnceUponAFrame()
    ---@return ImGuiOnceUponAFrame
    new = function () end
}

---@class ImGuiPayload
---@field Data userdata
---@field DataSize integer
---@field SourceId userdata
---@field SourceParentId userdata
---@field DataFrameCount integer
---@field DataType userdata
---@field Preview boolean
---@field Delivery boolean
ImGuiPayload = {
    ---Clear()
	---@param self ImGuiPayload
    Clear = function (self) end,
	---ImGuiPayload()
    ---@return ImGuiPayload
    new = function () end,
	---IsDataType(const char* type)
	---@param self ImGuiPayload
	---@param type string
    ---@return boolean
    IsDataType = function (self, type) end,
	---IsDelivery()
	---@param self ImGuiPayload
    ---@return boolean
    IsDelivery = function (self) end,
	---IsPreview()
	---@param self ImGuiPayload
    ---@return boolean
    IsPreview = function (self) end
}

---@class ImGuiPlatformIO
---@field Platform_GetClipboardTextFn userdata
---@field Platform_SetClipboardTextFn userdata
---@field Platform_ClipboardUserData userdata
---@field Platform_OpenInShellFn userdata
---@field Platform_OpenInShellUserData userdata
---@field Platform_SetImeDataFn userdata
---@field Platform_ImeUserData userdata
---@field Platform_LocaleDecimalPoint userdata
---@field Renderer_TextureMaxWidth integer
---@field Renderer_TextureMaxHeight integer
---@field Renderer_RenderState userdata
---@field Textures ImVector_ImTextureDataPtr
ImGuiPlatformIO = {
    ---ClearPlatformHandlers()
	---@param self ImGuiPlatformIO
    ClearPlatformHandlers = function (self) end,
	---ClearRendererHandlers()
	---@param self ImGuiPlatformIO
    ClearRendererHandlers = function (self) end,
	---ImGuiPlatformIO()
    ---@return ImGuiPlatformIO
    new = function () end
}

---@class ImGuiPlatformImeData
---@field WantVisible boolean
---@field WantTextInput boolean
---@field InputPos ImVec2
---@field InputLineHeight number
---@field ViewportId userdata
ImGuiPlatformImeData = {
    ---ImGuiPlatformImeData()
    ---@return ImGuiPlatformImeData
    new = function () end
}

---@class ImGuiPopupData
---@field PopupId userdata
---@field Window ImGuiWindow
---@field RestoreNavWindow ImGuiWindow
---@field ParentNavLayer integer
---@field OpenFrameCount integer
---@field OpenParentId userdata
---@field OpenPopupPos ImVec2
---@field OpenMousePos ImVec2
ImGuiPopupData = {
    ---ImGuiPopupData()
    ---@return ImGuiPopupData
    new = function () end
}

---@class ImGuiPtrOrIndex
---@field Ptr userdata
---@field Index integer
ImGuiPtrOrIndex = {
    ---ImGuiPtrOrIndex(void* ptr)
	---@param ptr userdata
    ---@return ImGuiPtrOrIndex
    new = function (ptr) end,
	---ImGuiPtrOrIndex(int index)
	---@param index integer
    ---@return ImGuiPtrOrIndex
    new = function (index) end
}

---@class ImGuiSelectionBasicStorage
---@field Size integer
---@field PreserveOrder boolean
---@field UserData userdata
---@field AdapterIndexToStorageId userdata
---@field _SelectionOrder integer
---@field _Storage ImGuiStorage
ImGuiSelectionBasicStorage = {
    ---ApplyRequests(ImGuiMultiSelectIO* ms_io)
	---@param self ImGuiSelectionBasicStorage
	---@param ms_io ImGuiMultiSelectIO
    ApplyRequests = function (self, ms_io) end,
	---Clear()
	---@param self ImGuiSelectionBasicStorage
    Clear = function (self) end,
	---Contains(ImGuiID id)
	---@param self ImGuiSelectionBasicStorage
	---@param id userdata
    ---@return boolean
    Contains = function (self, id) end,
	---GetNextSelectedItem(void** opaque_it,ImGuiID* out_id)
	---@param self ImGuiSelectionBasicStorage
	---@param opaque_it userdata
	---@param out_id userdata
    ---@return boolean
    GetNextSelectedItem = function (self, opaque_it, out_id) end,
	---GetStorageIdFromIndex(int idx)
	---@param self ImGuiSelectionBasicStorage
	---@param idx integer
    ---@return userdata
    GetStorageIdFromIndex = function (self, idx) end,
	---ImGuiSelectionBasicStorage()
    ---@return ImGuiSelectionBasicStorage
    new = function () end,
	---SetItemSelected(ImGuiID id,bool selected)
	---@param self ImGuiSelectionBasicStorage
	---@param id userdata
	---@param selected boolean
    SetItemSelected = function (self, id, selected) end,
	---Swap(ImGuiSelectionBasicStorage& r)
	---@param self ImGuiSelectionBasicStorage
	---@param r ImGuiSelectionBasicStorage
    Swap = function (self, r) end
}

---@class ImGuiSelectionExternalStorage
---@field UserData userdata
---@field AdapterSetItemSelected userdata
ImGuiSelectionExternalStorage = {
    ---ApplyRequests(ImGuiMultiSelectIO* ms_io)
	---@param self ImGuiSelectionExternalStorage
	---@param ms_io ImGuiMultiSelectIO
    ApplyRequests = function (self, ms_io) end,
	---ImGuiSelectionExternalStorage()
    ---@return ImGuiSelectionExternalStorage
    new = function () end
}

---@class ImGuiSelectionRequest
---@field Type ImGuiSelectionRequestType
---@field Selected boolean
---@field RangeDirection userdata
---@field RangeFirstItem userdata
---@field RangeLastItem userdata
ImGuiSelectionRequest = {
    
}

---@class ImGuiSettingsHandler
---@field TypeName string
---@field TypeHash userdata
---@field ClearAllFn userdata
---@field ReadInitFn userdata
---@field ReadOpenFn userdata
---@field ReadLineFn userdata
---@field ApplyAllFn userdata
---@field WriteAllFn userdata
---@field UserData userdata
ImGuiSettingsHandler = {
    ---ImGuiSettingsHandler()
    ---@return ImGuiSettingsHandler
    new = function () end
}

---@class ImGuiShrinkWidthItem
---@field Index integer
---@field Width number
---@field InitialWidth number
ImGuiShrinkWidthItem = {
    
}

---@class ImGuiSizeCallbackData
---@field UserData userdata
---@field Pos ImVec2
---@field CurrentSize ImVec2
---@field DesiredSize ImVec2
ImGuiSizeCallbackData = {
    
}

---@class ImGuiStackLevelInfo
---@field ID userdata
---@field QueryFrameCount userdata
---@field QuerySuccess boolean
---@field DataType userdata
---@field DescOffset integer
ImGuiStackLevelInfo = {
    ---ImGuiStackLevelInfo()
    ---@return ImGuiStackLevelInfo
    new = function () end
}

---@class ImGuiStorage
---@field Data ImVector_ImGuiStoragePair
ImGuiStorage = {
    ---BuildSortByKey()
	---@param self ImGuiStorage
    BuildSortByKey = function (self) end,
	---Clear()
	---@param self ImGuiStorage
    Clear = function (self) end,
	---GetBool(ImGuiID key,bool default_val=false)
	---@param self ImGuiStorage
	---@param key userdata
	---@param default_val boolean
    ---@return boolean
    GetBool = function (self, key, default_val) end,
	---GetBoolRef(ImGuiID key,bool default_val=false)
	---@param self ImGuiStorage
	---@param key userdata
	---@param default_val boolean
    ---@return userdata
    GetBoolRef = function (self, key, default_val) end,
	---GetFloat(ImGuiID key,float default_val=0.0f)
	---@param self ImGuiStorage
	---@param key userdata
	---@param default_val number
    ---@return number
    GetFloat = function (self, key, default_val) end,
	---GetFloatRef(ImGuiID key,float default_val=0.0f)
	---@param self ImGuiStorage
	---@param key userdata
	---@param default_val number
    ---@return userdata
    GetFloatRef = function (self, key, default_val) end,
	---GetInt(ImGuiID key,int default_val=0)
	---@param self ImGuiStorage
	---@param key userdata
	---@param default_val integer
    ---@return integer
    GetInt = function (self, key, default_val) end,
	---GetIntRef(ImGuiID key,int default_val=0)
	---@param self ImGuiStorage
	---@param key userdata
	---@param default_val integer
    ---@return userdata
    GetIntRef = function (self, key, default_val) end,
	---GetVoidPtr(ImGuiID key)
	---@param self ImGuiStorage
	---@param key userdata
    ---@return userdata
    GetVoidPtr = function (self, key) end,
	---GetVoidPtrRef(ImGuiID key,void* default_val=((void*)0))
	---@param self ImGuiStorage
	---@param key userdata
	---@param default_val userdata
    ---@return userdata
    GetVoidPtrRef = function (self, key, default_val) end,
	---SetAllInt(int val)
	---@param self ImGuiStorage
	---@param val integer
    SetAllInt = function (self, val) end,
	---SetBool(ImGuiID key,bool val)
	---@param self ImGuiStorage
	---@param key userdata
	---@param val boolean
    SetBool = function (self, key, val) end,
	---SetFloat(ImGuiID key,float val)
	---@param self ImGuiStorage
	---@param key userdata
	---@param val number
    SetFloat = function (self, key, val) end,
	---SetInt(ImGuiID key,int val)
	---@param self ImGuiStorage
	---@param key userdata
	---@param val integer
    SetInt = function (self, key, val) end,
	---SetVoidPtr(ImGuiID key,void* val)
	---@param self ImGuiStorage
	---@param key userdata
	---@param val userdata
    SetVoidPtr = function (self, key, val) end
}

---@class ImGuiStoragePair
---@field key userdata
---@field val_i integer
---@field val_f number
---@field val_p userdata
ImGuiStoragePair = {
    ---ImGuiStoragePair(ImGuiID _key,int _val)
	---@param _key userdata
	---@param _val integer
    ---@return ImGuiStoragePair
    new = function (_key, _val) end,
	---ImGuiStoragePair(ImGuiID _key,float _val)
	---@param _key userdata
	---@param _val number
    ---@return ImGuiStoragePair
    new = function (_key, _val) end,
	---ImGuiStoragePair(ImGuiID _key,void* _val)
	---@param _key userdata
	---@param _val userdata
    ---@return ImGuiStoragePair
    new = function (_key, _val) end
}

---@class ImGuiStyle
---@field FontSizeBase number
---@field FontScaleMain number
---@field FontScaleDpi number
---@field Alpha number
---@field DisabledAlpha number
---@field WindowPadding ImVec2
---@field WindowRounding number
---@field WindowBorderSize number
---@field WindowBorderHoverPadding number
---@field WindowMinSize ImVec2
---@field WindowTitleAlign ImVec2
---@field WindowMenuButtonPosition ImGuiDir
---@field ChildRounding number
---@field ChildBorderSize number
---@field PopupRounding number
---@field PopupBorderSize number
---@field FramePadding ImVec2
---@field FrameRounding number
---@field FrameBorderSize number
---@field ItemSpacing ImVec2
---@field ItemInnerSpacing ImVec2
---@field CellPadding ImVec2
---@field TouchExtraPadding ImVec2
---@field IndentSpacing number
---@field ColumnsMinSpacing number
---@field ScrollbarSize number
---@field ScrollbarRounding number
---@field ScrollbarPadding number
---@field GrabMinSize number
---@field GrabRounding number
---@field LogSliderDeadzone number
---@field ImageBorderSize number
---@field TabRounding number
---@field TabBorderSize number
---@field TabMinWidthBase number
---@field TabMinWidthShrink number
---@field TabCloseButtonMinWidthSelected number
---@field TabCloseButtonMinWidthUnselected number
---@field TabBarBorderSize number
---@field TabBarOverlineSize number
---@field TableAngledHeadersAngle number
---@field TableAngledHeadersTextAlign ImVec2
---@field TreeLinesFlags ImGuiTreeNodeFlags
---@field TreeLinesSize number
---@field TreeLinesRounding number
---@field DragDropTargetRounding number
---@field DragDropTargetBorderSize number
---@field DragDropTargetPadding number
---@field ColorButtonPosition ImGuiDir
---@field ButtonTextAlign ImVec2
---@field SelectableTextAlign ImVec2
---@field SeparatorTextBorderSize number
---@field SeparatorTextAlign ImVec2
---@field SeparatorTextPadding ImVec2
---@field DisplayWindowPadding ImVec2
---@field DisplaySafeAreaPadding ImVec2
---@field MouseCursorScale number
---@field AntiAliasedLines boolean
---@field AntiAliasedLinesUseTex boolean
---@field AntiAliasedFill boolean
---@field CurveTessellationTol number
---@field CircleTessellationMaxError number
---@field Colors ImVec4
---@field HoverStationaryDelay number
---@field HoverDelayShort number
---@field HoverDelayNormal number
---@field HoverFlagsForTooltipMouse ImGuiHoveredFlags
---@field HoverFlagsForTooltipNav ImGuiHoveredFlags
---@field _MainScale number
---@field _NextFrameFontSizeBase number
ImGuiStyle = {
    ---ImGuiStyle()
    ---@return ImGuiStyle
    new = function () end,
	---ScaleAllSizes(float scale_factor)
	---@param self ImGuiStyle
	---@param scale_factor number
    ScaleAllSizes = function (self, scale_factor) end
}

---@class ImGuiStyleMod
---@field VarIdx ImGuiStyleVar
---@field BackupInt integer
---@field BackupFloat number
ImGuiStyleMod = {
    ---ImGuiStyleMod(ImGuiStyleVar idx,int v)
	---@param idx ImGuiStyleVar
	---@param v integer
    ---@return ImGuiStyleMod
    new = function (idx, v) end,
	---ImGuiStyleMod(ImGuiStyleVar idx,float v)
	---@param idx ImGuiStyleVar
	---@param v number
    ---@return ImGuiStyleMod
    new = function (idx, v) end,
	---ImGuiStyleMod(ImGuiStyleVar idx,ImVec2 v)
	---@param idx ImGuiStyleVar
	---@param v ImVec2
    ---@return ImGuiStyleMod
    new = function (idx, v) end
}

---@class ImGuiStyleVarInfo
---@field Count integer
---@field DataType ImGuiDataType
---@field Offset integer
ImGuiStyleVarInfo = {
    ---GetVarPtr(void* parent)
	---@param self ImGuiStyleVarInfo
	---@param parent userdata
    ---@return userdata
    GetVarPtr = function (self, parent) end
}

---@class ImGuiTabBar
---@field Window ImGuiWindow
---@field Tabs ImVector_ImGuiTabItem
---@field Flags ImGuiTabBarFlags
---@field ID userdata
---@field SelectedTabId userdata
---@field NextSelectedTabId userdata
---@field VisibleTabId userdata
---@field CurrFrameVisible integer
---@field PrevFrameVisible integer
---@field BarRect ImRect
---@field BarRectPrevWidth number
---@field CurrTabsContentsHeight number
---@field PrevTabsContentsHeight number
---@field WidthAllTabs number
---@field WidthAllTabsIdeal number
---@field ScrollingAnim number
---@field ScrollingTarget number
---@field ScrollingTargetDistToVisibility number
---@field ScrollingSpeed number
---@field ScrollingRectMinX number
---@field ScrollingRectMaxX number
---@field SeparatorMinX number
---@field SeparatorMaxX number
---@field ReorderRequestTabId userdata
---@field ReorderRequestOffset userdata
---@field BeginCount userdata
---@field WantLayout boolean
---@field VisibleTabWasSubmitted boolean
---@field TabsAddedNew boolean
---@field ScrollButtonEnabled boolean
---@field TabsActiveCount userdata
---@field LastTabItemIdx userdata
---@field ItemSpacingY number
---@field FramePadding ImVec2
---@field BackupCursorPos ImVec2
---@field TabsNames ImGuiTextBuffer
ImGuiTabBar = {
    ---ImGuiTabBar()
    ---@return ImGuiTabBar
    new = function () end
}

---@class ImGuiTabItem
---@field ID userdata
---@field Flags ImGuiTabItemFlags
---@field LastFrameVisible integer
---@field LastFrameSelected integer
---@field Offset number
---@field Width number
---@field ContentWidth number
---@field RequestedWidth number
---@field NameOffset userdata
---@field BeginOrder userdata
---@field IndexDuringLayout userdata
---@field WantClose boolean
ImGuiTabItem = {
    ---ImGuiTabItem()
    ---@return ImGuiTabItem
    new = function () end
}

---@class ImGuiTable
---@field ID userdata
---@field Flags ImGuiTableFlags
---@field RawData userdata
---@field TempData ImGuiTableTempData
---@field Columns userdata
---@field DisplayOrderToIndex userdata
---@field RowCellData userdata
---@field EnabledMaskByDisplayOrder userdata
---@field EnabledMaskByIndex userdata
---@field VisibleMaskByIndex userdata
---@field SettingsLoadedFlags ImGuiTableFlags
---@field SettingsOffset integer
---@field LastFrameActive integer
---@field ColumnsCount integer
---@field CurrentRow integer
---@field CurrentColumn integer
---@field InstanceCurrent userdata
---@field InstanceInteracted userdata
---@field RowPosY1 number
---@field RowPosY2 number
---@field RowMinHeight number
---@field RowCellPaddingY number
---@field RowTextBaseline number
---@field RowIndentOffsetX number
---@field RowFlags ImGuiTableRowFlags
---@field LastRowFlags ImGuiTableRowFlags
---@field RowBgColorCounter integer
---@field RowBgColor integer
---@field BorderColorStrong integer
---@field BorderColorLight integer
---@field BorderX1 number
---@field BorderX2 number
---@field HostIndentX number
---@field MinColumnWidth number
---@field OuterPaddingX number
---@field CellPaddingX number
---@field CellSpacingX1 number
---@field CellSpacingX2 number
---@field InnerWidth number
---@field ColumnsGivenWidth number
---@field ColumnsAutoFitWidth number
---@field ColumnsStretchSumWeights number
---@field ResizedColumnNextWidth number
---@field ResizeLockMinContentsX2 number
---@field RefScale number
---@field AngledHeadersHeight number
---@field AngledHeadersSlope number
---@field OuterRect ImRect
---@field InnerRect ImRect
---@field WorkRect ImRect
---@field InnerClipRect ImRect
---@field BgClipRect ImRect
---@field Bg0ClipRectForDrawCmd ImRect
---@field Bg2ClipRectForDrawCmd ImRect
---@field HostClipRect ImRect
---@field HostBackupInnerClipRect ImRect
---@field OuterWindow ImGuiWindow
---@field InnerWindow ImGuiWindow
---@field ColumnsNames ImGuiTextBuffer
---@field DrawSplitter ImDrawListSplitter
---@field InstanceDataFirst ImGuiTableInstanceData
---@field InstanceDataExtra ImVector_ImGuiTableInstanceData
---@field SortSpecsSingle ImGuiTableColumnSortSpecs
---@field SortSpecsMulti ImVector_ImGuiTableColumnSortSpecs
---@field SortSpecs ImGuiTableSortSpecs
---@field SortSpecsCount userdata
---@field ColumnsEnabledCount userdata
---@field ColumnsEnabledFixedCount userdata
---@field DeclColumnsCount userdata
---@field AngledHeadersCount userdata
---@field HoveredColumnBody userdata
---@field HoveredColumnBorder userdata
---@field HighlightColumnHeader userdata
---@field AutoFitSingleColumn userdata
---@field ResizedColumn userdata
---@field LastResizedColumn userdata
---@field HeldHeaderColumn userdata
---@field ReorderColumn userdata
---@field ReorderColumnDir userdata
---@field LeftMostEnabledColumn userdata
---@field RightMostEnabledColumn userdata
---@field LeftMostStretchedColumn userdata
---@field RightMostStretchedColumn userdata
---@field ContextPopupColumn userdata
---@field FreezeRowsRequest userdata
---@field FreezeRowsCount userdata
---@field FreezeColumnsRequest userdata
---@field FreezeColumnsCount userdata
---@field RowCellDataCurrent userdata
---@field DummyDrawChannel userdata
---@field Bg2DrawChannelCurrent userdata
---@field Bg2DrawChannelUnfrozen userdata
---@field NavLayer userdata
---@field IsLayoutLocked boolean
---@field IsInsideRow boolean
---@field IsInitializing boolean
---@field IsSortSpecsDirty boolean
---@field IsUsingHeaders boolean
---@field IsContextPopupOpen boolean
---@field DisableDefaultContextMenu boolean
---@field IsSettingsRequestLoad boolean
---@field IsSettingsDirty boolean
---@field IsDefaultDisplayOrder boolean
---@field IsResetAllRequest boolean
---@field IsResetDisplayOrderRequest boolean
---@field IsUnfrozenRows boolean
---@field IsDefaultSizingPolicy boolean
---@field IsActiveIdAliveBeforeTable boolean
---@field IsActiveIdInTable boolean
---@field HasScrollbarYCurr boolean
---@field HasScrollbarYPrev boolean
---@field MemoryCompacted boolean
---@field HostSkipItems boolean
ImGuiTable = {
    ---ImGuiTable()
    ---@return ImGuiTable
    new = function () end
}

---@class ImGuiTableCellData
---@field BgColor integer
---@field Column userdata
ImGuiTableCellData = {
    
}

---@class ImGuiTableColumn
---@field Flags ImGuiTableColumnFlags
---@field WidthGiven number
---@field MinX number
---@field MaxX number
---@field WidthRequest number
---@field WidthAuto number
---@field WidthMax number
---@field StretchWeight number
---@field InitStretchWeightOrWidth number
---@field ClipRect ImRect
---@field UserID userdata
---@field WorkMinX number
---@field WorkMaxX number
---@field ItemWidth number
---@field ContentMaxXFrozen number
---@field ContentMaxXUnfrozen number
---@field ContentMaxXHeadersUsed number
---@field ContentMaxXHeadersIdeal number
---@field NameOffset userdata
---@field DisplayOrder userdata
---@field IndexWithinEnabledSet userdata
---@field PrevEnabledColumn userdata
---@field NextEnabledColumn userdata
---@field SortOrder userdata
---@field DrawChannelCurrent userdata
---@field DrawChannelFrozen userdata
---@field DrawChannelUnfrozen userdata
---@field IsEnabled boolean
---@field IsUserEnabled boolean
---@field IsUserEnabledNextFrame boolean
---@field IsVisibleX boolean
---@field IsVisibleY boolean
---@field IsRequestOutput boolean
---@field IsSkipItems boolean
---@field IsPreserveWidthAuto boolean
---@field NavLayerCurrent userdata
---@field AutoFitQueue userdata
---@field CannotSkipItemsQueue userdata
---@field SortDirection userdata
---@field SortDirectionsAvailCount userdata
---@field SortDirectionsAvailMask userdata
---@field SortDirectionsAvailList userdata
ImGuiTableColumn = {
    ---ImGuiTableColumn()
    ---@return ImGuiTableColumn
    new = function () end
}

---@class ImGuiTableColumnSettings
---@field WidthOrWeight number
---@field UserID userdata
---@field Index userdata
---@field DisplayOrder userdata
---@field SortOrder userdata
---@field SortDirection userdata
---@field IsEnabled userdata
---@field IsStretch userdata
ImGuiTableColumnSettings = {
    ---ImGuiTableColumnSettings()
    ---@return ImGuiTableColumnSettings
    new = function () end
}

---@class ImGuiTableColumnSortSpecs
---@field ColumnUserID userdata
---@field ColumnIndex userdata
---@field SortOrder userdata
---@field SortDirection ImGuiSortDirection
ImGuiTableColumnSortSpecs = {
    ---ImGuiTableColumnSortSpecs()
    ---@return ImGuiTableColumnSortSpecs
    new = function () end
}

---@class ImGuiTableHeaderData
---@field Index userdata
---@field TextColor integer
---@field BgColor0 integer
---@field BgColor1 integer
ImGuiTableHeaderData = {
    
}

---@class ImGuiTableInstanceData
---@field TableInstanceID userdata
---@field LastOuterHeight number
---@field LastTopHeadersRowHeight number
---@field LastFrozenHeight number
---@field HoveredRowLast integer
---@field HoveredRowNext integer
ImGuiTableInstanceData = {
    ---ImGuiTableInstanceData()
    ---@return ImGuiTableInstanceData
    new = function () end
}

---@class ImGuiTableSettings
---@field ID userdata
---@field SaveFlags ImGuiTableFlags
---@field RefScale number
---@field ColumnsCount userdata
---@field ColumnsCountMax userdata
---@field WantApply boolean
ImGuiTableSettings = {
    ---GetColumnSettings()
	---@param self ImGuiTableSettings
    ---@return ImGuiTableColumnSettings
    GetColumnSettings = function (self) end,
	---ImGuiTableSettings()
    ---@return ImGuiTableSettings
    new = function () end
}

---@class ImGuiTableSortSpecs
---@field Specs ImGuiTableColumnSortSpecs
---@field SpecsCount integer
---@field SpecsDirty boolean
ImGuiTableSortSpecs = {
    ---ImGuiTableSortSpecs()
    ---@return ImGuiTableSortSpecs
    new = function () end
}

---@class ImGuiTableTempData
---@field WindowID userdata
---@field TableIndex integer
---@field LastTimeActive number
---@field AngledHeadersExtraWidth number
---@field AngledHeadersRequests ImVector_ImGuiTableHeaderData
---@field UserOuterSize ImVec2
---@field DrawSplitter ImDrawListSplitter
---@field HostBackupWorkRect ImRect
---@field HostBackupParentWorkRect ImRect
---@field HostBackupPrevLineSize ImVec2
---@field HostBackupCurrLineSize ImVec2
---@field HostBackupCursorMaxPos ImVec2
---@field HostBackupColumnsOffset ImVec1
---@field HostBackupItemWidth number
---@field HostBackupItemWidthStackSize integer
ImGuiTableTempData = {
    ---ImGuiTableTempData()
    ---@return ImGuiTableTempData
    new = function () end
}

---@class ImGuiTextBuffer
---@field Buf ImVector_char
ImGuiTextBuffer = {
    ---ImGuiTextBuffer()
    ---@return ImGuiTextBuffer
    new = function () end,
	---append(const char* str,const char* str_end=((void*)0))
	---@param self ImGuiTextBuffer
	---@param str string
	---@param str_end string
    append = function (self, str, str_end) end,
	---appendf(ImGuiTextBuffer* self, const char* fmt, ...)
	---@param self ImGuiTextBuffer
	---@param fmt userdata
	---@param ... any
    appendf = function (self, fmt, ...) end,
	---appendfv(const char* fmt,va_list args)
	---@param self ImGuiTextBuffer
	---@param fmt string
	---@param args userdata
    appendfv = function (self, fmt, args) end,
	---begin()
	---@param self ImGuiTextBuffer
    ---@return string
    begin = function (self) end,
	---c_str()
	---@param self ImGuiTextBuffer
    ---@return string
    c_str = function (self) end,
	---clear()
	---@param self ImGuiTextBuffer
    clear = function (self) end,
	---empty()
	---@param self ImGuiTextBuffer
    ---@return boolean
    empty = function (self) end,
	---end()
	---@param self ImGuiTextBuffer
    ---@return string
    ["end"] = function (self) end,
	---reserve(int capacity)
	---@param self ImGuiTextBuffer
	---@param capacity integer
    reserve = function (self, capacity) end,
	---resize(int size)
	---@param self ImGuiTextBuffer
	---@param size integer
    resize = function (self, size) end,
	---size()
	---@param self ImGuiTextBuffer
    ---@return integer
    size = function (self) end
}

---@class ImGuiTextFilter
---@field InputBuf userdata
---@field Filters ImVector_ImGuiTextRange
---@field CountGrep integer
ImGuiTextFilter = {
    ---Build()
	---@param self ImGuiTextFilter
    Build = function (self) end,
	---Clear()
	---@param self ImGuiTextFilter
    Clear = function (self) end,
	---Draw(const char* label="Filter(inc,-exc)",float width=0.0f)
	---@param self ImGuiTextFilter
	---@param label string
	---@param width number
    ---@return boolean
    Draw = function (self, label, width) end,
	---ImGuiTextFilter(const char* default_filter="")
	---@param default_filter string
    ---@return ImGuiTextFilter
    new = function (default_filter) end,
	---IsActive()
	---@param self ImGuiTextFilter
    ---@return boolean
    IsActive = function (self) end,
	---PassFilter(const char* text,const char* text_end=((void*)0))
	---@param self ImGuiTextFilter
	---@param text string
	---@param text_end string
    ---@return boolean
    PassFilter = function (self, text, text_end) end
}

---@class ImGuiTextIndex
---@field Offsets ImVector_int
---@field EndOffset integer
ImGuiTextIndex = {
    ---append(const char* base,int old_size,int new_size)
	---@param self ImGuiTextIndex
	---@param base string
	---@param old_size integer
	---@param new_size integer
    append = function (self, base, old_size, new_size) end,
	---clear()
	---@param self ImGuiTextIndex
    clear = function (self) end,
	---get_line_begin(const char* base,int n)
	---@param self ImGuiTextIndex
	---@param base string
	---@param n integer
    ---@return string
    get_line_begin = function (self, base, n) end,
	---get_line_end(const char* base,int n)
	---@param self ImGuiTextIndex
	---@param base string
	---@param n integer
    ---@return string
    get_line_end = function (self, base, n) end,
	---size()
	---@param self ImGuiTextIndex
    ---@return integer
    size = function (self) end
}

---@class ImGuiTextRange
---@field b string
---@field e string
ImGuiTextRange = {
    ---ImGuiTextRange()
    ---@return ImGuiTextRange
    new = function () end,
	---ImGuiTextRange(const char* _b,const char* _e)
	---@param _b string
	---@param _e string
    ---@return ImGuiTextRange
    new = function (_b, _e) end,
	---empty()
	---@param self ImGuiTextRange
    ---@return boolean
    empty = function (self) end,
	---split(char separator,ImVector<ImGuiTextRange>* out)
	---@param self ImGuiTextRange
	---@param separator userdata
	---@param out ImVector_ImGuiTextRange
    split = function (self, separator, out) end
}

---@class ImGuiTreeNodeStackData
---@field ID userdata
---@field TreeFlags ImGuiTreeNodeFlags
---@field ItemFlags ImGuiItemFlags
---@field NavRect ImRect
---@field DrawLinesX1 number
---@field DrawLinesToNodesY2 number
---@field DrawLinesTableColumn userdata
ImGuiTreeNodeStackData = {
    
}

---@class ImGuiTypingSelectRequest
---@field Flags ImGuiTypingSelectFlags
---@field SearchBufferLen integer
---@field SearchBuffer string
---@field SelectRequest boolean
---@field SingleCharMode boolean
---@field SingleCharSize userdata
ImGuiTypingSelectRequest = {
    
}

---@class ImGuiTypingSelectState
---@field Request ImGuiTypingSelectRequest
---@field SearchBuffer userdata
---@field FocusScope userdata
---@field LastRequestFrame integer
---@field LastRequestTime number
---@field SingleCharModeLock boolean
ImGuiTypingSelectState = {
    ---Clear()
	---@param self ImGuiTypingSelectState
    Clear = function (self) end,
	---ImGuiTypingSelectState()
    ---@return ImGuiTypingSelectState
    new = function () end
}

---@class ImGuiViewport
---@field ID userdata
---@field Flags ImGuiViewportFlags
---@field Pos ImVec2
---@field Size ImVec2
---@field FramebufferScale ImVec2
---@field WorkPos ImVec2
---@field WorkSize ImVec2
---@field PlatformHandle userdata
---@field PlatformHandleRaw userdata
ImGuiViewport = {
    ---GetCenter()
	---@param self ImGuiViewport
    ---@return userdata
    GetCenter = function (self) end,
	---GetWorkCenter()
	---@param self ImGuiViewport
    ---@return userdata
    GetWorkCenter = function (self) end,
	---ImGuiViewport()
    ---@return ImGuiViewport
    new = function () end
}

---@class ImGuiViewportP
---@field _ImGuiViewport ImGuiViewport
---@field BgFgDrawListsLastFrame integer
---@field BgFgDrawLists ImDrawList
---@field DrawDataP ImDrawData
---@field DrawDataBuilder ImDrawDataBuilder
---@field WorkInsetMin ImVec2
---@field WorkInsetMax ImVec2
---@field BuildWorkInsetMin ImVec2
---@field BuildWorkInsetMax ImVec2
ImGuiViewportP = {
    ---CalcWorkRectPos(const ImVec2& inset_min)
	---@param self ImGuiViewportP
	---@param inset_min ImVec2
    ---@return userdata
    CalcWorkRectPos = function (self, inset_min) end,
	---CalcWorkRectSize(const ImVec2& inset_min,const ImVec2& inset_max)
	---@param self ImGuiViewportP
	---@param inset_min ImVec2
	---@param inset_max ImVec2
    ---@return userdata
    CalcWorkRectSize = function (self, inset_min, inset_max) end,
	---GetBuildWorkRect()
	---@param self ImGuiViewportP
    ---@return userdata
    GetBuildWorkRect = function (self) end,
	---GetMainRect()
	---@param self ImGuiViewportP
    ---@return userdata
    GetMainRect = function (self) end,
	---GetWorkRect()
	---@param self ImGuiViewportP
    ---@return userdata
    GetWorkRect = function (self) end,
	---ImGuiViewportP()
    ---@return ImGuiViewportP
    new = function () end,
	---UpdateWorkRect()
	---@param self ImGuiViewportP
    UpdateWorkRect = function (self) end
}

---@class ImGuiWindow
---@field Ctx ImGuiContext
---@field Name userdata
---@field ID userdata
---@field Flags ImGuiWindowFlags
---@field ChildFlags ImGuiChildFlags
---@field Viewport ImGuiViewportP
---@field Pos ImVec2
---@field Size ImVec2
---@field SizeFull ImVec2
---@field ContentSize ImVec2
---@field ContentSizeIdeal ImVec2
---@field ContentSizeExplicit ImVec2
---@field WindowPadding ImVec2
---@field WindowRounding number
---@field WindowBorderSize number
---@field TitleBarHeight number
---@field MenuBarHeight number
---@field DecoOuterSizeX1 number
---@field DecoOuterSizeY1 number
---@field DecoOuterSizeX2 number
---@field DecoOuterSizeY2 number
---@field DecoInnerSizeX1 number
---@field DecoInnerSizeY1 number
---@field NameBufLen integer
---@field MoveId userdata
---@field ChildId userdata
---@field PopupId userdata
---@field Scroll ImVec2
---@field ScrollMax ImVec2
---@field ScrollTarget ImVec2
---@field ScrollTargetCenterRatio ImVec2
---@field ScrollTargetEdgeSnapDist ImVec2
---@field ScrollbarSizes ImVec2
---@field ScrollbarX boolean
---@field ScrollbarY boolean
---@field ScrollbarXStabilizeEnabled boolean
---@field ScrollbarXStabilizeToggledHistory userdata
---@field Active boolean
---@field WasActive boolean
---@field WriteAccessed boolean
---@field Collapsed boolean
---@field WantCollapseToggle boolean
---@field SkipItems boolean
---@field SkipRefresh boolean
---@field Appearing boolean
---@field Hidden boolean
---@field IsFallbackWindow boolean
---@field IsExplicitChild boolean
---@field HasCloseButton boolean
---@field ResizeBorderHovered userdata
---@field ResizeBorderHeld userdata
---@field BeginCount integer
---@field BeginCountPreviousFrame integer
---@field BeginOrderWithinParent integer
---@field BeginOrderWithinContext integer
---@field FocusOrder integer
---@field AutoPosLastDirection ImGuiDir
---@field AutoFitFramesX userdata
---@field AutoFitFramesY userdata
---@field AutoFitOnlyGrows boolean
---@field HiddenFramesCanSkipItems userdata
---@field HiddenFramesCannotSkipItems userdata
---@field HiddenFramesForRenderOnly userdata
---@field DisableInputsFrames userdata
---@field BgClickFlags ImGuiWindowBgClickFlags
---@field SetWindowPosAllowFlags ImGuiCond
---@field SetWindowSizeAllowFlags ImGuiCond
---@field SetWindowCollapsedAllowFlags ImGuiCond
---@field SetWindowPosVal ImVec2
---@field SetWindowPosPivot ImVec2
---@field IDStack ImVector_ImGuiID
---@field DC ImGuiWindowTempData
---@field OuterRectClipped ImRect
---@field InnerRect ImRect
---@field InnerClipRect ImRect
---@field WorkRect ImRect
---@field ParentWorkRect ImRect
---@field ClipRect ImRect
---@field ContentRegionRect ImRect
---@field HitTestHoleSize ImVec2ih
---@field HitTestHoleOffset ImVec2ih
---@field LastFrameActive integer
---@field LastTimeActive number
---@field ItemWidthDefault number
---@field StateStorage ImGuiStorage
---@field ColumnsStorage ImVector_ImGuiOldColumns
---@field FontWindowScale number
---@field FontWindowScaleParents number
---@field FontRefSize number
---@field SettingsOffset integer
---@field DrawList ImDrawList
---@field DrawListInst ImDrawList
---@field ParentWindow ImGuiWindow
---@field ParentWindowInBeginStack ImGuiWindow
---@field RootWindow ImGuiWindow
---@field RootWindowPopupTree ImGuiWindow
---@field RootWindowForTitleBarHighlight ImGuiWindow
---@field RootWindowForNav ImGuiWindow
---@field ParentWindowForFocusRoute ImGuiWindow
---@field NavLastChildNavWindow ImGuiWindow
---@field NavLastIds userdata
---@field NavRectRel ImRect
---@field NavPreferredScoringPosRel ImVec2
---@field NavRootFocusScopeId userdata
---@field MemoryDrawListIdxCapacity integer
---@field MemoryDrawListVtxCapacity integer
---@field MemoryCompacted boolean
ImGuiWindow = {
    ---GetID(const char* str,const char* str_end=((void*)0))
	---@param self ImGuiWindow
	---@param str string
	---@param str_end string
    ---@return userdata
    GetID = function (self, str, str_end) end,
	---GetID(const void* ptr)
	---@param self ImGuiWindow
	---@param ptr userdata
    ---@return userdata
    GetID = function (self, ptr) end,
	---GetID(int n)
	---@param self ImGuiWindow
	---@param n integer
    ---@return userdata
    GetID = function (self, n) end,
	---GetIDFromPos(const ImVec2& p_abs)
	---@param self ImGuiWindow
	---@param p_abs ImVec2
    ---@return userdata
    GetIDFromPos = function (self, p_abs) end,
	---GetIDFromRectangle(const ImRect& r_abs)
	---@param self ImGuiWindow
	---@param r_abs ImRect
    ---@return userdata
    GetIDFromRectangle = function (self, r_abs) end,
	---ImGuiWindow(ImGuiContext* context,const char* name)
	---@param context ImGuiContext
	---@param name string
    ---@return ImGuiWindow
    new = function (context, name) end,
	---MenuBarRect()
	---@param self ImGuiWindow
    ---@return userdata
    MenuBarRect = function (self) end,
	---Rect()
	---@param self ImGuiWindow
    ---@return userdata
    Rect = function (self) end,
	---TitleBarRect()
	---@param self ImGuiWindow
    ---@return userdata
    TitleBarRect = function (self) end
}

---@class ImGuiWindowSettings
---@field ID userdata
---@field Pos ImVec2ih
---@field Size ImVec2ih
---@field Collapsed boolean
---@field IsChild boolean
---@field WantApply boolean
---@field WantDelete boolean
ImGuiWindowSettings = {
    ---GetName()
	---@param self ImGuiWindowSettings
    ---@return userdata
    GetName = function (self) end,
	---ImGuiWindowSettings()
    ---@return ImGuiWindowSettings
    new = function () end
}

---@class ImGuiWindowStackData
---@field Window ImGuiWindow
---@field ParentLastItemDataBackup ImGuiLastItemData
---@field StackSizesInBegin ImGuiErrorRecoveryState
---@field DisabledOverrideReenable boolean
---@field DisabledOverrideReenableAlphaBackup number
ImGuiWindowStackData = {
    
}

---@class ImGuiWindowTempData
---@field CursorPos ImVec2
---@field CursorPosPrevLine ImVec2
---@field CursorStartPos ImVec2
---@field CursorMaxPos ImVec2
---@field IdealMaxPos ImVec2
---@field CurrLineSize ImVec2
---@field PrevLineSize ImVec2
---@field CurrLineTextBaseOffset number
---@field PrevLineTextBaseOffset number
---@field IsSameLine boolean
---@field IsSetPos boolean
---@field Indent ImVec1
---@field ColumnsOffset ImVec1
---@field GroupOffset ImVec1
---@field CursorStartPosLossyness ImVec2
---@field NavLayerCurrent ImGuiNavLayer
---@field NavLayersActiveMask integer
---@field NavLayersActiveMaskNext integer
---@field NavIsScrollPushableX boolean
---@field NavHideHighlightOneFrame boolean
---@field NavWindowHasScrollY boolean
---@field MenuBarAppending boolean
---@field MenuBarOffset ImVec2
---@field MenuColumns ImGuiMenuColumns
---@field TreeDepth integer
---@field TreeHasStackDataDepthMask integer
---@field TreeRecordsClippedNodesY2Mask integer
---@field ChildWindows ImVector_ImGuiWindowPtr
---@field StateStorage ImGuiStorage
---@field CurrentColumns ImGuiOldColumns
---@field CurrentTableIdx integer
---@field LayoutType ImGuiLayoutType
---@field ParentLayoutType ImGuiLayoutType
---@field ModalDimBgColor integer
---@field WindowItemStatusFlags ImGuiItemStatusFlags
---@field ChildItemStatusFlags ImGuiItemStatusFlags
---@field ItemWidth number
---@field TextWrapPos number
---@field ItemWidthStack ImVector_float
---@field TextWrapPosStack ImVector_float
ImGuiWindowTempData = {
    
}

---@class ImRect
---@field Min ImVec2
---@field Max ImVec2
ImRect = {
    ---Add(const ImVec2& p)
	---@param self ImRect
	---@param p ImVec2
    Add = function (self, p) end,
	---Add(const ImRect& r)
	---@param self ImRect
	---@param r ImRect
    Add = function (self, r) end,
	---AsVec4()
	---@param self ImRect
    ---@return userdata
    AsVec4 = function (self) end,
	---ClipWith(const ImRect& r)
	---@param self ImRect
	---@param r ImRect
    ClipWith = function (self, r) end,
	---ClipWithFull(const ImRect& r)
	---@param self ImRect
	---@param r ImRect
    ClipWithFull = function (self, r) end,
	---Contains(const ImVec2& p)
	---@param self ImRect
	---@param p ImVec2
    ---@return boolean
    Contains = function (self, p) end,
	---Contains(const ImRect& r)
	---@param self ImRect
	---@param r ImRect
    ---@return boolean
    Contains = function (self, r) end,
	---ContainsWithPad(const ImVec2& p,const ImVec2& pad)
	---@param self ImRect
	---@param p ImVec2
	---@param pad ImVec2
    ---@return boolean
    ContainsWithPad = function (self, p, pad) end,
	---Expand(const float amount)
	---@param self ImRect
	---@param amount userdata
    Expand = function (self, amount) end,
	---Expand(const ImVec2& amount)
	---@param self ImRect
	---@param amount ImVec2
    Expand = function (self, amount) end,
	---Floor()
	---@param self ImRect
    Floor = function (self) end,
	---GetArea()
	---@param self ImRect
    ---@return number
    GetArea = function (self) end,
	---GetBL()
	---@param self ImRect
    ---@return userdata
    GetBL = function (self) end,
	---GetBR()
	---@param self ImRect
    ---@return userdata
    GetBR = function (self) end,
	---GetCenter()
	---@param self ImRect
    ---@return userdata
    GetCenter = function (self) end,
	---GetHeight()
	---@param self ImRect
    ---@return number
    GetHeight = function (self) end,
	---GetSize()
	---@param self ImRect
    ---@return userdata
    GetSize = function (self) end,
	---GetTL()
	---@param self ImRect
    ---@return userdata
    GetTL = function (self) end,
	---GetTR()
	---@param self ImRect
    ---@return userdata
    GetTR = function (self) end,
	---GetWidth()
	---@param self ImRect
    ---@return number
    GetWidth = function (self) end,
	---ImRect()
    ---@return ImRect
    new = function () end,
	---ImRect(const ImVec2& min,const ImVec2& max)
	---@param min ImVec2
	---@param max ImVec2
    ---@return ImRect
    new = function (min, max) end,
	---ImRect(const ImVec4& v)
	---@param v ImVec4
    ---@return ImRect
    new = function (v) end,
	---ImRect(float x1,float y1,float x2,float y2)
	---@param x1 number
	---@param y1 number
	---@param x2 number
	---@param y2 number
    ---@return ImRect
    new = function (x1, y1, x2, y2) end,
	---IsInverted()
	---@param self ImRect
    ---@return boolean
    IsInverted = function (self) end,
	---Overlaps(const ImRect& r)
	---@param self ImRect
	---@param r ImRect
    ---@return boolean
    Overlaps = function (self, r) end,
	---ToVec4()
	---@param self ImRect
    ---@return userdata
    ToVec4 = function (self) end,
	---Translate(const ImVec2& d)
	---@param self ImRect
	---@param d ImVec2
    Translate = function (self, d) end,
	---TranslateX(float dx)
	---@param self ImRect
	---@param dx number
    TranslateX = function (self, dx) end,
	---TranslateY(float dy)
	---@param self ImRect
	---@param dy number
    TranslateY = function (self, dy) end
}

---@class ImTextureData
---@field UniqueID integer
---@field Status ImTextureStatus
---@field BackendUserData userdata
---@field TexID userdata
---@field Format ImTextureFormat
---@field Width integer
---@field Height integer
---@field BytesPerPixel integer
---@field Pixels userdata
---@field UsedRect ImTextureRect
---@field UpdateRect ImTextureRect
---@field Updates ImVector_ImTextureRect
---@field UnusedFrames integer
---@field RefCount integer
---@field UseColors boolean
---@field WantDestroyNextFrame boolean
ImTextureData = {
    ---Create(ImTextureFormat format,int w,int h)
	---@param self ImTextureData
	---@param format ImTextureFormat
	---@param w integer
	---@param h integer
    Create = function (self, format, w, h) end,
	---DestroyPixels()
	---@param self ImTextureData
    DestroyPixels = function (self) end,
	---GetPitch()
	---@param self ImTextureData
    ---@return integer
    GetPitch = function (self) end,
	---GetPixels()
	---@param self ImTextureData
    ---@return userdata
    GetPixels = function (self) end,
	---GetPixelsAt(int x,int y)
	---@param self ImTextureData
	---@param x integer
	---@param y integer
    ---@return userdata
    GetPixelsAt = function (self, x, y) end,
	---GetSizeInBytes()
	---@param self ImTextureData
    ---@return integer
    GetSizeInBytes = function (self) end,
	---GetTexID()
	---@param self ImTextureData
    ---@return userdata
    GetTexID = function (self) end,
	---GetTexRef()
	---@param self ImTextureData
    ---@return userdata
    GetTexRef = function (self) end,
	---ImTextureData()
    ---@return ImTextureData
    new = function () end,
	---SetStatus(ImTextureStatus status)
	---@param self ImTextureData
	---@param status ImTextureStatus
    SetStatus = function (self, status) end,
	---SetTexID(ImTextureID tex_id)
	---@param self ImTextureData
	---@param tex_id userdata
    SetTexID = function (self, tex_id) end
}

---@class ImTextureRect
---@field x integer
---@field y integer
---@field w integer
---@field h integer
ImTextureRect = {
    
}

---@class ImTextureRef
---@field _TexData ImTextureData
---@field _TexID userdata
ImTextureRef = {
    ---GetTexID()
	---@param self ImTextureRef
    ---@return userdata
    GetTexID = function (self) end,
	---ImTextureRef()
    ---@return ImTextureRef
    new = function () end,
	---ImTextureRef(ImTextureID tex_id)
	---@param tex_id userdata
    ---@return ImTextureRef
    new = function (tex_id) end
}

---@class ImVec1
---@field x number
ImVec1 = {
    ---ImVec1()
    ---@return ImVec1
    new = function () end,
	---ImVec1(float _x)
	---@param _x number
    ---@return ImVec1
    new = function (_x) end
}

---@class ImVec2
---@field x number
---@field y number
ImVec2 = {
    ---ImVec2()
    ---@return ImVec2
    new = function () end,
	---ImVec2(float _x,float _y)
	---@param _x number
	---@param _y number
    ---@return ImVec2
    new = function (_x, _y) end
}

---@class ImVec2i
---@field x integer
---@field y integer
ImVec2i = {
    ---ImVec2i()
    ---@return ImVec2i
    new = function () end,
	---ImVec2i(int _x,int _y)
	---@param _x integer
	---@param _y integer
    ---@return ImVec2i
    new = function (_x, _y) end
}

---@class ImVec2ih
---@field x integer
---@field y integer
ImVec2ih = {
    ---ImVec2ih()
    ---@return ImVec2ih
    new = function () end,
	---ImVec2ih(short _x,short _y)
	---@param _x integer
	---@param _y integer
    ---@return ImVec2ih
    new = function (_x, _y) end,
	---ImVec2ih(const ImVec2& rhs)
	---@param rhs ImVec2
    ---@return ImVec2ih
    new = function (rhs) end
}

---@class ImVec4
---@field x number
---@field y number
---@field z number
---@field w number
ImVec4 = {
    ---ImVec4()
    ---@return ImVec4
    new = function () end,
	---ImVec4(float _x,float _y,float _z,float _w)
	---@param _x number
	---@param _y number
	---@param _z number
	---@param _w number
    ---@return ImVec4
    new = function (_x, _y, _z, _w) end
}

---@class stbrp_context_opaque
---@field data userdata
stbrp_context_opaque = {
    
}

---@class ImVector_ImGuiGroupData
ImVector_ImGuiGroupData = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiGroupData[]
    Data = nil
}

---@class ImVector_ImFontConfig
ImVector_ImFontConfig = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImFontConfig[]
    Data = nil
}

---@class ImVector_ImGuiViewportPPtr
ImVector_ImGuiViewportPPtr = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiViewportP[]
    Data = nil
}

---@class ImVector_ImDrawChannel
ImVector_ImDrawChannel = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImDrawChannel[]
    Data = nil
}

---@class ImVector_ImGuiInputEvent
ImVector_ImGuiInputEvent = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiInputEvent[]
    Data = nil
}

---@class ImVector_ImGuiTableTempData
ImVector_ImGuiTableTempData = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiTableTempData[]
    Data = nil
}

---@class ImVector_ImGuiPtrOrIndex
ImVector_ImGuiPtrOrIndex = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiPtrOrIndex[]
    Data = nil
}

---@class ImVector_ImU32
ImVector_ImU32 = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type integer[]
    Data = nil
}

---@class ImVector_ImFontPtr
ImVector_ImFontPtr = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImFont[]
    Data = nil
}

---@class ImVector_ImFontAtlasRectEntry
ImVector_ImFontAtlasRectEntry = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImFontAtlasRectEntry[]
    Data = nil
}

---@class ImVector_ImGuiShrinkWidthItem
ImVector_ImGuiShrinkWidthItem = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiShrinkWidthItem[]
    Data = nil
}

---@class ImVector_ImVec2
ImVector_ImVec2 = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImVec2[]
    Data = nil
}

---@class ImVector_ImGuiItemFlags
ImVector_ImGuiItemFlags = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiItemFlags[]
    Data = nil
}

---@class ImVector_ImTextureDataPtr
ImVector_ImTextureDataPtr = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImTextureData[]
    Data = nil
}

---@class ImVector_ImGuiContextHook
ImVector_ImGuiContextHook = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiContextHook[]
    Data = nil
}

---@class ImVector_ImWchar
ImVector_ImWchar = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type userdata[]
    Data = nil
}

---@class ImVector_ImGuiListClipperRange
ImVector_ImGuiListClipperRange = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiListClipperRange[]
    Data = nil
}

---@class ImVector_ImGuiListClipperData
ImVector_ImGuiListClipperData = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiListClipperData[]
    Data = nil
}

---@class ImVector_ImGuiStyleMod
ImVector_ImGuiStyleMod = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiStyleMod[]
    Data = nil
}

---@class ImVector_ImDrawListSharedDataPtr
ImVector_ImDrawListSharedDataPtr = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImDrawListSharedData[]
    Data = nil
}

---@class ImVector_ImGuiWindowPtr
ImVector_ImGuiWindowPtr = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiWindow[]
    Data = nil
}

---@class ImVector_ImFontConfigPtr
ImVector_ImFontConfigPtr = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImFontConfig[]
    Data = nil
}

---@class ImVector_ImDrawCmd
ImVector_ImDrawCmd = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImDrawCmd[]
    Data = nil
}

---@class ImVector_ImTextureRect
ImVector_ImTextureRect = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImTextureRect[]
    Data = nil
}

---@class ImVector_ImU16
ImVector_ImU16 = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type userdata[]
    Data = nil
}

---@class ImVector_ImGuiKeyRoutingData
ImVector_ImGuiKeyRoutingData = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiKeyRoutingData[]
    Data = nil
}

---@class ImVector_ImGuiWindowStackData
ImVector_ImGuiWindowStackData = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiWindowStackData[]
    Data = nil
}

---@class ImVector_ImGuiTabItem
ImVector_ImGuiTabItem = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiTabItem[]
    Data = nil
}

---@class ImVector_ImFontAtlasPtr
ImVector_ImFontAtlasPtr = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImFontAtlas[]
    Data = nil
}

---@class ImVector_ImTextureRef
ImVector_ImTextureRef = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImTextureRef[]
    Data = nil
}

---@class ImVector_ImGuiTableHeaderData
ImVector_ImGuiTableHeaderData = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiTableHeaderData[]
    Data = nil
}

---@class ImVector_stbrp_node_im
ImVector_stbrp_node_im = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type userdata[]
    Data = nil
}

---@class ImVector_ImGuiColorMod
ImVector_ImGuiColorMod = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiColorMod[]
    Data = nil
}

---@class ImVector_ImGuiTreeNodeStackData
ImVector_ImGuiTreeNodeStackData = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiTreeNodeStackData[]
    Data = nil
}

---@class ImVector_ImGuiPopupData
ImVector_ImGuiPopupData = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiPopupData[]
    Data = nil
}

---@class ImVector_ImU8
ImVector_ImU8 = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type userdata[]
    Data = nil
}

---@class ImVector_ImDrawIdx
ImVector_ImDrawIdx = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type userdata[]
    Data = nil
}

---@class ImVector_ImGuiStackLevelInfo
ImVector_ImGuiStackLevelInfo = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiStackLevelInfo[]
    Data = nil
}

---@class ImVector_ImGuiSelectionRequest
ImVector_ImGuiSelectionRequest = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiSelectionRequest[]
    Data = nil
}

---@class ImVector_ImGuiTextRange
ImVector_ImGuiTextRange = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiTextRange[]
    Data = nil
}

---@class ImVector_ImFontStackData
ImVector_ImFontStackData = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImFontStackData[]
    Data = nil
}

---@class ImVector_ImGuiSettingsHandler
ImVector_ImGuiSettingsHandler = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiSettingsHandler[]
    Data = nil
}

---@class ImVector_unsigned_char
ImVector_unsigned_char = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type integer[]
    Data = nil
}

---@class ImVector_int
ImVector_int = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type integer[]
    Data = nil
}

---@class ImVector_ImGuiOldColumnData
ImVector_ImGuiOldColumnData = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiOldColumnData[]
    Data = nil
}

---@class ImVector_ImVec4
ImVector_ImVec4 = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImVec4[]
    Data = nil
}

---@class ImVector_ImDrawListPtr
ImVector_ImDrawListPtr = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImDrawList[]
    Data = nil
}

---@class ImVector_ImDrawVert
ImVector_ImDrawVert = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImDrawVert[]
    Data = nil
}

---@class ImVector_ImGuiTableColumnSortSpecs
ImVector_ImGuiTableColumnSortSpecs = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiTableColumnSortSpecs[]
    Data = nil
}

---@class ImVector_ImGuiMultiSelectTempData
ImVector_ImGuiMultiSelectTempData = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiMultiSelectTempData[]
    Data = nil
}

---@class ImVector_ImFontGlyph
ImVector_ImFontGlyph = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImFontGlyph[]
    Data = nil
}

---@class ImVector_ImGuiFocusScopeData
ImVector_ImGuiFocusScopeData = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiFocusScopeData[]
    Data = nil
}

---@class ImVector_char
ImVector_char = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type userdata[]
    Data = nil
}

---@class ImVector_float
ImVector_float = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type number[]
    Data = nil
}

---@class ImVector_ImGuiTableInstanceData
ImVector_ImGuiTableInstanceData = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiTableInstanceData[]
    Data = nil
}

---@class ImVector_ImGuiID
ImVector_ImGuiID = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type userdata[]
    Data = nil
}

---@class ImVector_ImGuiOldColumns
ImVector_ImGuiOldColumns = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiOldColumns[]
    Data = nil
}

---@class ImVector_ImGuiStoragePair
ImVector_ImGuiStoragePair = {
    ---@type integer
    Size = nil,
    ---@type integer
    Capacity = nil,
    ---@type ImGuiStoragePair[]
    Data = nil
}

ImGui = {
    ---DebugEditFontLoaderFlags(ImGuiFreeTypeLoaderFlags* p_font_loader_flags)
	---@param p_font_loader_flags ImGuiFreeTypeLoaderFlags
    ---@return boolean
    DebugEditFontLoaderFlags = function (p_font_loader_flags) end,
	---GetFontLoader()
    ---@return ImFontLoader
    GetFontLoader = function () end,
	---SetAllocatorFunctions(void*(*alloc_func)(size_t sz,void* user_data),void(*free_func)(void* ptr,void* user_data),void* user_data=nullptr)
	---@param alloc_func userdata
	---@param free_func userdata
	---@param user_data userdata
    SetAllocatorFunctions = function (alloc_func, free_func, user_data) end,
	---AcceptDragDropPayload(const char* type,ImGuiDragDropFlags flags=0)
	---@param type string
	---@param flags ImGuiDragDropFlags
    ---@return ImGuiPayload
    AcceptDragDropPayload = function (type, flags) end,
	---ActivateItemByID(ImGuiID id)
	---@param id userdata
    ActivateItemByID = function (id) end,
	---AddContextHook(ImGuiContext* context,const ImGuiContextHook* hook)
	---@param context ImGuiContext
	---@param hook ImGuiContextHook
    ---@return userdata
    AddContextHook = function (context, hook) end,
	---AddDrawListToDrawDataEx(ImDrawData* draw_data,ImVector<ImDrawList*>* out_list,ImDrawList* draw_list)
	---@param draw_data ImDrawData
	---@param out_list ImVector_ImDrawListPtr
	---@param draw_list ImDrawList
    AddDrawListToDrawDataEx = function (draw_data, out_list, draw_list) end,
	---AddSettingsHandler(const ImGuiSettingsHandler* handler)
	---@param handler ImGuiSettingsHandler
    AddSettingsHandler = function (handler) end,
	---AlignTextToFramePadding()
    AlignTextToFramePadding = function () end,
	---ArrowButton(const char* str_id,ImGuiDir dir)
	---@param str_id string
	---@param dir ImGuiDir
    ---@return boolean
    ArrowButton = function (str_id, dir) end,
	---ArrowButtonEx(const char* str_id,ImGuiDir dir,ImVec2 size_arg,ImGuiButtonFlags flags=0)
	---@param str_id string
	---@param dir ImGuiDir
	---@param size_arg ImVec2
	---@param flags ImGuiButtonFlags
    ---@return boolean
    ArrowButtonEx = function (str_id, dir, size_arg, flags) end,
	---Begin(const char* name,bool* p_open=((void*)0),ImGuiWindowFlags flags=0)
	---@param name string,
	---@param p_open boolean|nil,
	---@param flags ImGuiWindowFlags
    ---@return boolean, boolean
    Begin = function (name, p_open, flags) end,
	---BeginBoxSelect(const ImRect& scope_rect,ImGuiWindow* window,ImGuiID box_select_id,ImGuiMultiSelectFlags ms_flags)
	---@param scope_rect ImRect
	---@param window ImGuiWindow
	---@param box_select_id userdata
	---@param ms_flags ImGuiMultiSelectFlags
    ---@return boolean
    BeginBoxSelect = function (scope_rect, window, box_select_id, ms_flags) end,
	---BeginChild(const char* str_id,const ImVec2& size=ImVec2(0,0),ImGuiChildFlags child_flags=0,ImGuiWindowFlags window_flags=0)
	---@param str_id string
	---@param size ImVec2
	---@param child_flags ImGuiChildFlags
	---@param window_flags ImGuiWindowFlags
    ---@return boolean
    BeginChild = function (str_id, size, child_flags, window_flags) end,
	---BeginChild(ImGuiID id,const ImVec2& size=ImVec2(0,0),ImGuiChildFlags child_flags=0,ImGuiWindowFlags window_flags=0)
	---@param id userdata
	---@param size ImVec2
	---@param child_flags ImGuiChildFlags
	---@param window_flags ImGuiWindowFlags
    ---@return boolean
    BeginChild = function (id, size, child_flags, window_flags) end,
	---BeginChildEx(const char* name,ImGuiID id,const ImVec2& size_arg,ImGuiChildFlags child_flags,ImGuiWindowFlags window_flags)
	---@param name string
	---@param id userdata
	---@param size_arg ImVec2
	---@param child_flags ImGuiChildFlags
	---@param window_flags ImGuiWindowFlags
    ---@return boolean
    BeginChildEx = function (name, id, size_arg, child_flags, window_flags) end,
	---BeginColumns(const char* str_id,int count,ImGuiOldColumnFlags flags=0)
	---@param str_id string
	---@param count integer
	---@param flags ImGuiOldColumnFlags
    BeginColumns = function (str_id, count, flags) end,
	---BeginCombo(const char* label,const char* preview_value,ImGuiComboFlags flags=0)
	---@param label string
	---@param preview_value string
	---@param flags ImGuiComboFlags
    ---@return boolean
    BeginCombo = function (label, preview_value, flags) end,
	---BeginComboPopup(ImGuiID popup_id,const ImRect& bb,ImGuiComboFlags flags)
	---@param popup_id userdata
	---@param bb ImRect
	---@param flags ImGuiComboFlags
    ---@return boolean
    BeginComboPopup = function (popup_id, bb, flags) end,
	---BeginComboPreview()
    ---@return boolean
    BeginComboPreview = function () end,
	---BeginDisabled(bool disabled=true)
	---@param disabled boolean
    BeginDisabled = function (disabled) end,
	---BeginDisabledOverrideReenable()
    BeginDisabledOverrideReenable = function () end,
	---BeginDragDropSource(ImGuiDragDropFlags flags=0)
	---@param flags ImGuiDragDropFlags
    ---@return boolean
    BeginDragDropSource = function (flags) end,
	---BeginDragDropTarget()
    ---@return boolean
    BeginDragDropTarget = function () end,
	---BeginDragDropTargetCustom(const ImRect& bb,ImGuiID id)
	---@param bb ImRect
	---@param id userdata
    ---@return boolean
    BeginDragDropTargetCustom = function (bb, id) end,
	---BeginDragDropTargetViewport(ImGuiViewport* viewport,const ImRect* p_bb=((void*)0))
	---@param viewport ImGuiViewport
	---@param p_bb ImRect
    ---@return boolean
    BeginDragDropTargetViewport = function (viewport, p_bb) end,
	---BeginErrorTooltip()
    ---@return boolean
    BeginErrorTooltip = function () end,
	---BeginGroup()
    BeginGroup = function () end,
	---BeginItemTooltip()
    ---@return boolean
    BeginItemTooltip = function () end,
	---BeginListBox(const char* label,const ImVec2& size=ImVec2(0,0))
	---@param label string
	---@param size ImVec2
    ---@return boolean
    BeginListBox = function (label, size) end,
	---BeginMainMenuBar()
    ---@return boolean
    BeginMainMenuBar = function () end,
	---BeginMenu(const char* label,bool enabled=true)
	---@param label string
	---@param enabled boolean
    ---@return boolean
    BeginMenu = function (label, enabled) end,
	---BeginMenuBar()
    ---@return boolean
    BeginMenuBar = function () end,
	---BeginMenuEx(const char* label,const char* icon,bool enabled=true)
	---@param label string
	---@param icon string
	---@param enabled boolean
    ---@return boolean
    BeginMenuEx = function (label, icon, enabled) end,
	---BeginMultiSelect(ImGuiMultiSelectFlags flags,int selection_size=-1,int items_count=-1)
	---@param flags ImGuiMultiSelectFlags
	---@param selection_size integer
	---@param items_count integer
    ---@return ImGuiMultiSelectIO
    BeginMultiSelect = function (flags, selection_size, items_count) end,
	---BeginPopup(const char* str_id,ImGuiWindowFlags flags=0)
	---@param str_id string
	---@param flags ImGuiWindowFlags
    ---@return boolean
    BeginPopup = function (str_id, flags) end,
	---BeginPopupContextItem(const char* str_id=((void*)0),ImGuiPopupFlags popup_flags=1)
	---@param str_id string
	---@param popup_flags ImGuiPopupFlags
    ---@return boolean
    BeginPopupContextItem = function (str_id, popup_flags) end,
	---BeginPopupContextVoid(const char* str_id=((void*)0),ImGuiPopupFlags popup_flags=1)
	---@param str_id string
	---@param popup_flags ImGuiPopupFlags
    ---@return boolean
    BeginPopupContextVoid = function (str_id, popup_flags) end,
	---BeginPopupContextWindow(const char* str_id=((void*)0),ImGuiPopupFlags popup_flags=1)
	---@param str_id string
	---@param popup_flags ImGuiPopupFlags
    ---@return boolean
    BeginPopupContextWindow = function (str_id, popup_flags) end,
	---BeginPopupEx(ImGuiID id,ImGuiWindowFlags extra_window_flags)
	---@param id userdata
	---@param extra_window_flags ImGuiWindowFlags
    ---@return boolean
    BeginPopupEx = function (id, extra_window_flags) end,
	---BeginPopupMenuEx(ImGuiID id,const char* label,ImGuiWindowFlags extra_window_flags)
	---@param id userdata
	---@param label string
	---@param extra_window_flags ImGuiWindowFlags
    ---@return boolean
    BeginPopupMenuEx = function (id, label, extra_window_flags) end,
	---BeginPopupModal(const char* name,bool* p_open=((void*)0),ImGuiWindowFlags flags=0)
	---@param name string,
	---@param p_open boolean|nil,
	---@param flags ImGuiWindowFlags
    ---@return boolean, boolean
    BeginPopupModal = function (name, p_open, flags) end,
	---BeginTabBar(const char* str_id,ImGuiTabBarFlags flags=0)
	---@param str_id string
	---@param flags ImGuiTabBarFlags
    ---@return boolean
    BeginTabBar = function (str_id, flags) end,
	---BeginTabBarEx(ImGuiTabBar* tab_bar,const ImRect& bb,ImGuiTabBarFlags flags)
	---@param tab_bar ImGuiTabBar
	---@param bb ImRect
	---@param flags ImGuiTabBarFlags
    ---@return boolean
    BeginTabBarEx = function (tab_bar, bb, flags) end,
	---BeginTabItem(const char* label,bool* p_open=((void*)0),ImGuiTabItemFlags flags=0)
	---@param label string,
	---@param p_open boolean|nil,
	---@param flags ImGuiTabItemFlags
    ---@return boolean, boolean
    BeginTabItem = function (label, p_open, flags) end,
	---BeginTable(const char* str_id,int columns,ImGuiTableFlags flags=0,const ImVec2& outer_size=ImVec2(0.0f,0.0f),float inner_width=0.0f)
	---@param str_id string
	---@param columns integer
	---@param flags ImGuiTableFlags
	---@param outer_size ImVec2
	---@param inner_width number
    ---@return boolean
    BeginTable = function (str_id, columns, flags, outer_size, inner_width) end,
	---BeginTableEx(const char* name,ImGuiID id,int columns_count,ImGuiTableFlags flags=0,const ImVec2& outer_size=ImVec2(0,0),float inner_width=0.0f)
	---@param name string
	---@param id userdata
	---@param columns_count integer
	---@param flags ImGuiTableFlags
	---@param outer_size ImVec2
	---@param inner_width number
    ---@return boolean
    BeginTableEx = function (name, id, columns_count, flags, outer_size, inner_width) end,
	---BeginTooltip()
    ---@return boolean
    BeginTooltip = function () end,
	---BeginTooltipEx(ImGuiTooltipFlags tooltip_flags,ImGuiWindowFlags extra_window_flags)
	---@param tooltip_flags ImGuiTooltipFlags
	---@param extra_window_flags ImGuiWindowFlags
    ---@return boolean
    BeginTooltipEx = function (tooltip_flags, extra_window_flags) end,
	---BeginTooltipHidden()
    ---@return boolean
    BeginTooltipHidden = function () end,
	---BeginViewportSideBar(const char* name,ImGuiViewport* viewport,ImGuiDir dir,float size,ImGuiWindowFlags window_flags)
	---@param name string
	---@param viewport ImGuiViewport
	---@param dir ImGuiDir
	---@param size number
	---@param window_flags ImGuiWindowFlags
    ---@return boolean
    BeginViewportSideBar = function (name, viewport, dir, size, window_flags) end,
	---BringWindowToDisplayBack(ImGuiWindow* window)
	---@param window ImGuiWindow
    BringWindowToDisplayBack = function (window) end,
	---BringWindowToDisplayBehind(ImGuiWindow* window,ImGuiWindow* above_window)
	---@param window ImGuiWindow
	---@param above_window ImGuiWindow
    BringWindowToDisplayBehind = function (window, above_window) end,
	---BringWindowToDisplayFront(ImGuiWindow* window)
	---@param window ImGuiWindow
    BringWindowToDisplayFront = function (window) end,
	---BringWindowToFocusFront(ImGuiWindow* window)
	---@param window ImGuiWindow
    BringWindowToFocusFront = function (window) end,
	---Bullet()
    Bullet = function () end,
	---BulletText(const char* fmt,...)
	---@param fmt string
	---@param ... any
    BulletText = function (fmt, ...) end,
	---BulletTextV(const char* fmt,va_list args)
	---@param fmt string
	---@param args userdata
    BulletTextV = function (fmt, args) end,
	---Button(const char* label,const ImVec2& size=ImVec2(0,0))
	---@param label string
	---@param size ImVec2
    ---@return boolean
    Button = function (label, size) end,
	---ButtonBehavior(const ImRect& bb,ImGuiID id,bool* out_hovered,bool* out_held,ImGuiButtonFlags flags=0)
	---@param bb ImRect,
	---@param id userdata,
	---@param out_hovered boolean|nil,
	---@param out_held boolean|nil,
	---@param flags ImGuiButtonFlags
    ---@return boolean, boolean, boolean
    ButtonBehavior = function (bb, id, out_hovered, out_held, flags) end,
	---ButtonEx(const char* label,const ImVec2& size_arg=ImVec2(0,0),ImGuiButtonFlags flags=0)
	---@param label string
	---@param size_arg ImVec2
	---@param flags ImGuiButtonFlags
    ---@return boolean
    ButtonEx = function (label, size_arg, flags) end,
	---CalcClipRectVisibleItemsY(const ImRect& clip_rect,const ImVec2& pos,float items_height,int* out_visible_start,int* out_visible_end)
	---@param clip_rect ImRect,
	---@param pos ImVec2,
	---@param items_height number,
	---@param out_visible_start integer|nil,
	---@param out_visible_end integer|nil
    ---@return integer, integer
    CalcClipRectVisibleItemsY = function (clip_rect, pos, items_height, out_visible_start, out_visible_end) end,
	---CalcItemSize(ImVec2 size,float default_w,float default_h)
	---@param size ImVec2
	---@param default_w number
	---@param default_h number
    ---@return userdata
    CalcItemSize = function (size, default_w, default_h) end,
	---CalcItemWidth()
    ---@return number
    CalcItemWidth = function () end,
	---CalcTypematicRepeatAmount(float t0,float t1,float repeat_delay,float repeat_rate)
	---@param t0 number
	---@param t1 number
	---@param repeat_delay number
	---@param repeat_rate number
    ---@return integer
    CalcTypematicRepeatAmount = function (t0, t1, repeat_delay, repeat_rate) end,
	---CalcWindowNextAutoFitSize(ImGuiWindow* window)
	---@param window ImGuiWindow
    ---@return userdata
    CalcWindowNextAutoFitSize = function (window) end,
	---CalcWrapWidthForPos(const ImVec2& pos,float wrap_pos_x)
	---@param pos ImVec2
	---@param wrap_pos_x number
    ---@return number
    CalcWrapWidthForPos = function (pos, wrap_pos_x) end,
	---CallContextHooks(ImGuiContext* context,ImGuiContextHookType type)
	---@param context ImGuiContext
	---@param type ImGuiContextHookType
    CallContextHooks = function (context, type) end,
	---Checkbox(const char* label,bool* v)
	---@param label string,
	---@param v boolean|nil
    ---@return boolean, boolean
    Checkbox = function (label, v) end,
	---CheckboxFlags(const char* label,int* flags,int flags_value)
	---@param label string,
	---@param flags integer|nil,
	---@param flags_value integer
    ---@return boolean, integer
    CheckboxFlags = function (label, flags, flags_value) end,
	---CheckboxFlags(const char* label,unsigned int* flags,unsigned int flags_value)
	---@param label string,
	---@param flags integer|nil,
	---@param flags_value integer
    ---@return boolean, integer
    CheckboxFlags = function (label, flags, flags_value) end,
	---CheckboxFlags(const char* label,ImS64* flags,ImS64 flags_value)
	---@param label string
	---@param flags userdata
	---@param flags_value userdata
    ---@return boolean
    CheckboxFlags = function (label, flags, flags_value) end,
	---CheckboxFlags(const char* label,ImU64* flags,ImU64 flags_value)
	---@param label string
	---@param flags userdata
	---@param flags_value userdata
    ---@return boolean
    CheckboxFlags = function (label, flags, flags_value) end,
	---ClearActiveID()
    ClearActiveID = function () end,
	---ClearDragDrop()
    ClearDragDrop = function () end,
	---ClearIniSettings()
    ClearIniSettings = function () end,
	---ClearWindowSettings(const char* name)
	---@param name string
    ClearWindowSettings = function (name) end,
	---CloseButton(ImGuiID id,const ImVec2& pos)
	---@param id userdata
	---@param pos ImVec2
    ---@return boolean
    CloseButton = function (id, pos) end,
	---CloseCurrentPopup()
    CloseCurrentPopup = function () end,
	---ClosePopupToLevel(int remaining,bool restore_focus_to_window_under_popup)
	---@param remaining integer
	---@param restore_focus_to_window_under_popup boolean
    ClosePopupToLevel = function (remaining, restore_focus_to_window_under_popup) end,
	---ClosePopupsExceptModals()
    ClosePopupsExceptModals = function () end,
	---ClosePopupsOverWindow(ImGuiWindow* ref_window,bool restore_focus_to_window_under_popup)
	---@param ref_window ImGuiWindow
	---@param restore_focus_to_window_under_popup boolean
    ClosePopupsOverWindow = function (ref_window, restore_focus_to_window_under_popup) end,
	---CollapseButton(ImGuiID id,const ImVec2& pos)
	---@param id userdata
	---@param pos ImVec2
    ---@return boolean
    CollapseButton = function (id, pos) end,
	---CollapsingHeader(const char* label,ImGuiTreeNodeFlags flags=0)
	---@param label string
	---@param flags ImGuiTreeNodeFlags
    ---@return boolean
    CollapsingHeader = function (label, flags) end,
	---CollapsingHeader(const char* label,bool* p_visible,ImGuiTreeNodeFlags flags=0)
	---@param label string,
	---@param p_visible boolean|nil,
	---@param flags ImGuiTreeNodeFlags
    ---@return boolean, boolean
    CollapsingHeader = function (label, p_visible, flags) end,
	---ColorButton(const char* desc_id,const ImVec4& col,ImGuiColorEditFlags flags=0,const ImVec2& size=ImVec2(0,0))
	---@param desc_id string
	---@param col ImVec4
	---@param flags ImGuiColorEditFlags
	---@param size ImVec2
    ---@return boolean
    ColorButton = function (desc_id, col, flags, size) end,
	---ColorConvertFloat4ToU32(const ImVec4& in)
	---@param in_ ImVec4
    ---@return integer
    ColorConvertFloat4ToU32 = function (in_) end,
	---ColorConvertHSVtoRGB(float h,float s,float v,float& out_r,float& out_g,float& out_b)
	---@param h number,
	---@param s number,
	---@param v number,
	---@param out_r number|nil,
	---@param out_g number|nil,
	---@param out_b number|nil
    ---@return number, number, number
    ColorConvertHSVtoRGB = function (h, s, v, out_r, out_g, out_b) end,
	---ColorConvertRGBtoHSV(float r,float g,float b,float& out_h,float& out_s,float& out_v)
	---@param r number,
	---@param g number,
	---@param b number,
	---@param out_h number|nil,
	---@param out_s number|nil,
	---@param out_v number|nil
    ---@return number, number, number
    ColorConvertRGBtoHSV = function (r, g, b, out_h, out_s, out_v) end,
	---ColorConvertU32ToFloat4(ImU32 in)
	---@param in_ integer
    ---@return userdata
    ColorConvertU32ToFloat4 = function (in_) end,
	---ColorEdit3(const char* label,float col[3],ImGuiColorEditFlags flags=0)
	---@param label string
	---@param col userdata
	---@param flags ImGuiColorEditFlags
    ---@return boolean
    ColorEdit3 = function (label, col, flags) end,
	---ColorEdit4(const char* label,float col[4],ImGuiColorEditFlags flags=0)
	---@param label string
	---@param col userdata
	---@param flags ImGuiColorEditFlags
    ---@return boolean
    ColorEdit4 = function (label, col, flags) end,
	---ColorEditOptionsPopup(const float* col,ImGuiColorEditFlags flags)
	---@param col userdata
	---@param flags ImGuiColorEditFlags
    ColorEditOptionsPopup = function (col, flags) end,
	---ColorPicker3(const char* label,float col[3],ImGuiColorEditFlags flags=0)
	---@param label string
	---@param col userdata
	---@param flags ImGuiColorEditFlags
    ---@return boolean
    ColorPicker3 = function (label, col, flags) end,
	---ColorPicker4(const char* label,float col[4],ImGuiColorEditFlags flags=0,const float* ref_col=((void*)0))
	---@param label string
	---@param col userdata
	---@param flags ImGuiColorEditFlags
	---@param ref_col userdata
    ---@return boolean
    ColorPicker4 = function (label, col, flags, ref_col) end,
	---ColorPickerOptionsPopup(const float* ref_col,ImGuiColorEditFlags flags)
	---@param ref_col userdata
	---@param flags ImGuiColorEditFlags
    ColorPickerOptionsPopup = function (ref_col, flags) end,
	---ColorTooltip(const char* text,const float* col,ImGuiColorEditFlags flags)
	---@param text string
	---@param col userdata
	---@param flags ImGuiColorEditFlags
    ColorTooltip = function (text, col, flags) end,
	---Columns(int count=1,const char* id=((void*)0),bool borders=true)
	---@param count integer
	---@param id string
	---@param borders boolean
    Columns = function (count, id, borders) end,
	---Combo(const char* label,int* current_item,const char* const items[],int items_count,int popup_max_height_in_items=-1)
	---@param label string,
	---@param current_item integer|nil,
	---@param items userdata,
	---@param items_count integer,
	---@param popup_max_height_in_items integer
    ---@return boolean, integer
    Combo = function (label, current_item, items, items_count, popup_max_height_in_items) end,
	---Combo(const char* label,int* current_item,const char* items_separated_by_zeros,int popup_max_height_in_items=-1)
	---@param label string,
	---@param current_item integer|nil,
	---@param items_separated_by_zeros string,
	---@param popup_max_height_in_items integer
    ---@return boolean, integer
    Combo = function (label, current_item, items_separated_by_zeros, popup_max_height_in_items) end,
	---Combo(const char* label,int* current_item,const char*(*getter)(void* user_data,int idx),void* user_data,int items_count,int popup_max_height_in_items=-1)
	---@param label string,
	---@param current_item integer|nil,
	---@param getter userdata,
	---@param user_data userdata,
	---@param items_count integer,
	---@param popup_max_height_in_items integer
    ---@return boolean, integer
    Combo = function (label, current_item, getter, user_data, items_count, popup_max_height_in_items) end,
	---ConvertSingleModFlagToKey(ImGuiKey key)
	---@param key ImGuiKey
    ---@return ImGuiKey
    ConvertSingleModFlagToKey = function (key) end,
	---CreateContext(ImFontAtlas* shared_font_atlas=((void*)0))
	---@param shared_font_atlas ImFontAtlas
    ---@return ImGuiContext
    CreateContext = function (shared_font_atlas) end,
	---CreateNewWindowSettings(const char* name)
	---@param name string
    ---@return ImGuiWindowSettings
    CreateNewWindowSettings = function (name) end,
	---DataTypeApplyFromText(const char* buf,ImGuiDataType data_type,void* p_data,const char* format,void* p_data_when_empty=((void*)0))
	---@param buf string
	---@param data_type ImGuiDataType
	---@param p_data userdata
	---@param format string
	---@param p_data_when_empty userdata
    ---@return boolean
    DataTypeApplyFromText = function (buf, data_type, p_data, format, p_data_when_empty) end,
	---DataTypeApplyOp(ImGuiDataType data_type,int op,void* output,const void* arg_1,const void* arg_2)
	---@param data_type ImGuiDataType
	---@param op integer
	---@param output userdata
	---@param arg_1 userdata
	---@param arg_2 userdata
    DataTypeApplyOp = function (data_type, op, output, arg_1, arg_2) end,
	---DataTypeClamp(ImGuiDataType data_type,void* p_data,const void* p_min,const void* p_max)
	---@param data_type ImGuiDataType
	---@param p_data userdata
	---@param p_min userdata
	---@param p_max userdata
    ---@return boolean
    DataTypeClamp = function (data_type, p_data, p_min, p_max) end,
	---DataTypeCompare(ImGuiDataType data_type,const void* arg_1,const void* arg_2)
	---@param data_type ImGuiDataType
	---@param arg_1 userdata
	---@param arg_2 userdata
    ---@return integer
    DataTypeCompare = function (data_type, arg_1, arg_2) end,
	---DataTypeFormatString(char* buf,int buf_size,ImGuiDataType data_type,const void* p_data,const char* format)
	---@param buf userdata
	---@param buf_size integer
	---@param data_type ImGuiDataType
	---@param p_data userdata
	---@param format string
    ---@return integer
    DataTypeFormatString = function (buf, buf_size, data_type, p_data, format) end,
	---DataTypeGetInfo(ImGuiDataType data_type)
	---@param data_type ImGuiDataType
    ---@return ImGuiDataTypeInfo
    DataTypeGetInfo = function (data_type) end,
	---DataTypeIsZero(ImGuiDataType data_type,const void* p_data)
	---@param data_type ImGuiDataType
	---@param p_data userdata
    ---@return boolean
    DataTypeIsZero = function (data_type, p_data) end,
	---DebugAllocHook(ImGuiDebugAllocInfo* info,int frame_count,void* ptr,size_t size)
	---@param info ImGuiDebugAllocInfo
	---@param frame_count integer
	---@param ptr userdata
	---@param size userdata
    DebugAllocHook = function (info, frame_count, ptr, size) end,
	---DebugBreakButton(const char* label,const char* description_of_location)
	---@param label string
	---@param description_of_location string
    ---@return boolean
    DebugBreakButton = function (label, description_of_location) end,
	---DebugBreakButtonTooltip(bool keyboard_only,const char* description_of_location)
	---@param keyboard_only boolean
	---@param description_of_location string
    DebugBreakButtonTooltip = function (keyboard_only, description_of_location) end,
	---DebugBreakClearData()
    DebugBreakClearData = function () end,
	---DebugCheckVersionAndDataLayout(const char* version_str,size_t sz_io,size_t sz_style,size_t sz_vec2,size_t sz_vec4,size_t sz_drawvert,size_t sz_drawidx)
	---@param version_str string
	---@param sz_io userdata
	---@param sz_style userdata
	---@param sz_vec2 userdata
	---@param sz_vec4 userdata
	---@param sz_drawvert userdata
	---@param sz_drawidx userdata
    ---@return boolean
    DebugCheckVersionAndDataLayout = function (version_str, sz_io, sz_style, sz_vec2, sz_vec4, sz_drawvert, sz_drawidx) end,
	---DebugDrawCursorPos(ImU32 col=(((ImU32)(255)<<24)|((ImU32)(0)<<16)|((ImU32)(0)<<8)|((ImU32)(255)<<0)))
	---@param col integer
    DebugDrawCursorPos = function (col) end,
	---DebugDrawItemRect(ImU32 col=(((ImU32)(255)<<24)|((ImU32)(0)<<16)|((ImU32)(0)<<8)|((ImU32)(255)<<0)))
	---@param col integer
    DebugDrawItemRect = function (col) end,
	---DebugDrawLineExtents(ImU32 col=(((ImU32)(255)<<24)|((ImU32)(0)<<16)|((ImU32)(0)<<8)|((ImU32)(255)<<0)))
	---@param col integer
    DebugDrawLineExtents = function (col) end,
	---DebugFlashStyleColor(ImGuiCol idx)
	---@param idx ImGuiCol
    DebugFlashStyleColor = function (idx) end,
	---DebugHookIdInfo(ImGuiID id,ImGuiDataType data_type,const void* data_id,const void* data_id_end)
	---@param id userdata
	---@param data_type ImGuiDataType
	---@param data_id userdata
	---@param data_id_end userdata
    DebugHookIdInfo = function (id, data_type, data_id, data_id_end) end,
	---DebugLocateItem(ImGuiID target_id)
	---@param target_id userdata
    DebugLocateItem = function (target_id) end,
	---DebugLocateItemOnHover(ImGuiID target_id)
	---@param target_id userdata
    DebugLocateItemOnHover = function (target_id) end,
	---DebugLocateItemResolveWithLastItem()
    DebugLocateItemResolveWithLastItem = function () end,
	---DebugLog(const char* fmt,...)
	---@param fmt string
	---@param ... any
    DebugLog = function (fmt, ...) end,
	---DebugLogV(const char* fmt,va_list args)
	---@param fmt string
	---@param args userdata
    DebugLogV = function (fmt, args) end,
	---DebugNodeColumns(ImGuiOldColumns* columns)
	---@param columns ImGuiOldColumns
    DebugNodeColumns = function (columns) end,
	---DebugNodeDrawCmdShowMeshAndBoundingBox(ImDrawList* out_draw_list,const ImDrawList* draw_list,const ImDrawCmd* draw_cmd,bool show_mesh,bool show_aabb)
	---@param out_draw_list ImDrawList
	---@param draw_list ImDrawList
	---@param draw_cmd ImDrawCmd
	---@param show_mesh boolean
	---@param show_aabb boolean
    DebugNodeDrawCmdShowMeshAndBoundingBox = function (out_draw_list, draw_list, draw_cmd, show_mesh, show_aabb) end,
	---DebugNodeDrawList(ImGuiWindow* window,ImGuiViewportP* viewport,const ImDrawList* draw_list,const char* label)
	---@param window ImGuiWindow
	---@param viewport ImGuiViewportP
	---@param draw_list ImDrawList
	---@param label string
    DebugNodeDrawList = function (window, viewport, draw_list, label) end,
	---DebugNodeFont(ImFont* font)
	---@param font ImFont
    DebugNodeFont = function (font) end,
	---DebugNodeFontGlyph(ImFont* font,const ImFontGlyph* glyph)
	---@param font ImFont
	---@param glyph ImFontGlyph
    DebugNodeFontGlyph = function (font, glyph) end,
	---DebugNodeFontGlyphesForSrcMask(ImFont* font,ImFontBaked* baked,int src_mask)
	---@param font ImFont
	---@param baked ImFontBaked
	---@param src_mask integer
    DebugNodeFontGlyphesForSrcMask = function (font, baked, src_mask) end,
	---DebugNodeInputTextState(ImGuiInputTextState* state)
	---@param state ImGuiInputTextState
    DebugNodeInputTextState = function (state) end,
	---DebugNodeMultiSelectState(ImGuiMultiSelectState* state)
	---@param state ImGuiMultiSelectState
    DebugNodeMultiSelectState = function (state) end,
	---DebugNodeStorage(ImGuiStorage* storage,const char* label)
	---@param storage ImGuiStorage
	---@param label string
    DebugNodeStorage = function (storage, label) end,
	---DebugNodeTabBar(ImGuiTabBar* tab_bar,const char* label)
	---@param tab_bar ImGuiTabBar
	---@param label string
    DebugNodeTabBar = function (tab_bar, label) end,
	---DebugNodeTable(ImGuiTable* table)
	---@param table ImGuiTable
    DebugNodeTable = function (table) end,
	---DebugNodeTableSettings(ImGuiTableSettings* settings)
	---@param settings ImGuiTableSettings
    DebugNodeTableSettings = function (settings) end,
	---DebugNodeTexture(ImTextureData* tex,int int_id,const ImFontAtlasRect* highlight_rect=((void*)0))
	---@param tex ImTextureData
	---@param int_id integer
	---@param highlight_rect ImFontAtlasRect
    DebugNodeTexture = function (tex, int_id, highlight_rect) end,
	---DebugNodeTypingSelectState(ImGuiTypingSelectState* state)
	---@param state ImGuiTypingSelectState
    DebugNodeTypingSelectState = function (state) end,
	---DebugNodeViewport(ImGuiViewportP* viewport)
	---@param viewport ImGuiViewportP
    DebugNodeViewport = function (viewport) end,
	---DebugNodeWindow(ImGuiWindow* window,const char* label)
	---@param window ImGuiWindow
	---@param label string
    DebugNodeWindow = function (window, label) end,
	---DebugNodeWindowSettings(ImGuiWindowSettings* settings)
	---@param settings ImGuiWindowSettings
    DebugNodeWindowSettings = function (settings) end,
	---DebugNodeWindowsList(ImVector<ImGuiWindow*>* windows,const char* label)
	---@param windows ImVector_ImGuiWindowPtr
	---@param label string
    DebugNodeWindowsList = function (windows, label) end,
	---DebugNodeWindowsListByBeginStackParent(ImGuiWindow** windows,int windows_size,ImGuiWindow* parent_in_begin_stack)
	---@param windows userdata
	---@param windows_size integer
	---@param parent_in_begin_stack ImGuiWindow
    DebugNodeWindowsListByBeginStackParent = function (windows, windows_size, parent_in_begin_stack) end,
	---DebugRenderKeyboardPreview(ImDrawList* draw_list)
	---@param draw_list ImDrawList
    DebugRenderKeyboardPreview = function (draw_list) end,
	---DebugRenderViewportThumbnail(ImDrawList* draw_list,ImGuiViewportP* viewport,const ImRect& bb)
	---@param draw_list ImDrawList
	---@param viewport ImGuiViewportP
	---@param bb ImRect
    DebugRenderViewportThumbnail = function (draw_list, viewport, bb) end,
	---DebugStartItemPicker()
    DebugStartItemPicker = function () end,
	---DebugTextEncoding(const char* text)
	---@param text string
    DebugTextEncoding = function (text) end,
	---DebugTextUnformattedWithLocateItem(const char* line_begin,const char* line_end)
	---@param line_begin string
	---@param line_end string
    DebugTextUnformattedWithLocateItem = function (line_begin, line_end) end,
	---DestroyContext(ImGuiContext* ctx=((void*)0))
	---@param ctx ImGuiContext
    DestroyContext = function (ctx) end,
	---DragBehavior(ImGuiID id,ImGuiDataType data_type,void* p_v,float v_speed,const void* p_min,const void* p_max,const char* format,ImGuiSliderFlags flags)
	---@param id userdata
	---@param data_type ImGuiDataType
	---@param p_v userdata
	---@param v_speed number
	---@param p_min userdata
	---@param p_max userdata
	---@param format string
	---@param flags ImGuiSliderFlags
    ---@return boolean
    DragBehavior = function (id, data_type, p_v, v_speed, p_min, p_max, format, flags) end,
	---DragFloat(const char* label,float* v,float v_speed=1.0f,float v_min=0.0f,float v_max=0.0f,const char* format="%.3f",ImGuiSliderFlags flags=0)
	---@param label string,
	---@param v number|nil,
	---@param v_speed number,
	---@param v_min number,
	---@param v_max number,
	---@param format string,
	---@param flags ImGuiSliderFlags
    ---@return boolean, number
    DragFloat = function (label, v, v_speed, v_min, v_max, format, flags) end,
	---DragFloat2(const char* label,float v[2],float v_speed=1.0f,float v_min=0.0f,float v_max=0.0f,const char* format="%.3f",ImGuiSliderFlags flags=0)
	---@param label string
	---@param v userdata
	---@param v_speed number
	---@param v_min number
	---@param v_max number
	---@param format string
	---@param flags ImGuiSliderFlags
    ---@return boolean
    DragFloat2 = function (label, v, v_speed, v_min, v_max, format, flags) end,
	---DragFloat3(const char* label,float v[3],float v_speed=1.0f,float v_min=0.0f,float v_max=0.0f,const char* format="%.3f",ImGuiSliderFlags flags=0)
	---@param label string
	---@param v userdata
	---@param v_speed number
	---@param v_min number
	---@param v_max number
	---@param format string
	---@param flags ImGuiSliderFlags
    ---@return boolean
    DragFloat3 = function (label, v, v_speed, v_min, v_max, format, flags) end,
	---DragFloat4(const char* label,float v[4],float v_speed=1.0f,float v_min=0.0f,float v_max=0.0f,const char* format="%.3f",ImGuiSliderFlags flags=0)
	---@param label string
	---@param v userdata
	---@param v_speed number
	---@param v_min number
	---@param v_max number
	---@param format string
	---@param flags ImGuiSliderFlags
    ---@return boolean
    DragFloat4 = function (label, v, v_speed, v_min, v_max, format, flags) end,
	---DragFloatRange2(const char* label,float* v_current_min,float* v_current_max,float v_speed=1.0f,float v_min=0.0f,float v_max=0.0f,const char* format="%.3f",const char* format_max=((void*)0),ImGuiSliderFlags flags=0)
	---@param label string,
	---@param v_current_min number|nil,
	---@param v_current_max number|nil,
	---@param v_speed number,
	---@param v_min number,
	---@param v_max number,
	---@param format string,
	---@param format_max string,
	---@param flags ImGuiSliderFlags
    ---@return boolean, number, number
    DragFloatRange2 = function (label, v_current_min, v_current_max, v_speed, v_min, v_max, format, format_max, flags) end,
	---DragInt(const char* label,int* v,float v_speed=1.0f,int v_min=0,int v_max=0,const char* format="%d",ImGuiSliderFlags flags=0)
	---@param label string,
	---@param v integer|nil,
	---@param v_speed number,
	---@param v_min integer,
	---@param v_max integer,
	---@param format string,
	---@param flags ImGuiSliderFlags
    ---@return boolean, integer
    DragInt = function (label, v, v_speed, v_min, v_max, format, flags) end,
	---DragInt2(const char* label,int v[2],float v_speed=1.0f,int v_min=0,int v_max=0,const char* format="%d",ImGuiSliderFlags flags=0)
	---@param label string
	---@param v userdata
	---@param v_speed number
	---@param v_min integer
	---@param v_max integer
	---@param format string
	---@param flags ImGuiSliderFlags
    ---@return boolean
    DragInt2 = function (label, v, v_speed, v_min, v_max, format, flags) end,
	---DragInt3(const char* label,int v[3],float v_speed=1.0f,int v_min=0,int v_max=0,const char* format="%d",ImGuiSliderFlags flags=0)
	---@param label string
	---@param v userdata
	---@param v_speed number
	---@param v_min integer
	---@param v_max integer
	---@param format string
	---@param flags ImGuiSliderFlags
    ---@return boolean
    DragInt3 = function (label, v, v_speed, v_min, v_max, format, flags) end,
	---DragInt4(const char* label,int v[4],float v_speed=1.0f,int v_min=0,int v_max=0,const char* format="%d",ImGuiSliderFlags flags=0)
	---@param label string
	---@param v userdata
	---@param v_speed number
	---@param v_min integer
	---@param v_max integer
	---@param format string
	---@param flags ImGuiSliderFlags
    ---@return boolean
    DragInt4 = function (label, v, v_speed, v_min, v_max, format, flags) end,
	---DragIntRange2(const char* label,int* v_current_min,int* v_current_max,float v_speed=1.0f,int v_min=0,int v_max=0,const char* format="%d",const char* format_max=((void*)0),ImGuiSliderFlags flags=0)
	---@param label string,
	---@param v_current_min integer|nil,
	---@param v_current_max integer|nil,
	---@param v_speed number,
	---@param v_min integer,
	---@param v_max integer,
	---@param format string,
	---@param format_max string,
	---@param flags ImGuiSliderFlags
    ---@return boolean, integer, integer
    DragIntRange2 = function (label, v_current_min, v_current_max, v_speed, v_min, v_max, format, format_max, flags) end,
	---DragScalar(const char* label,ImGuiDataType data_type,void* p_data,float v_speed=1.0f,const void* p_min=((void*)0),const void* p_max=((void*)0),const char* format=((void*)0),ImGuiSliderFlags flags=0)
	---@param label string
	---@param data_type ImGuiDataType
	---@param p_data userdata
	---@param v_speed number
	---@param p_min userdata
	---@param p_max userdata
	---@param format string
	---@param flags ImGuiSliderFlags
    ---@return boolean
    DragScalar = function (label, data_type, p_data, v_speed, p_min, p_max, format, flags) end,
	---DragScalarN(const char* label,ImGuiDataType data_type,void* p_data,int components,float v_speed=1.0f,const void* p_min=((void*)0),const void* p_max=((void*)0),const char* format=((void*)0),ImGuiSliderFlags flags=0)
	---@param label string
	---@param data_type ImGuiDataType
	---@param p_data userdata
	---@param components integer
	---@param v_speed number
	---@param p_min userdata
	---@param p_max userdata
	---@param format string
	---@param flags ImGuiSliderFlags
    ---@return boolean
    DragScalarN = function (label, data_type, p_data, components, v_speed, p_min, p_max, format, flags) end,
	---Dummy(const ImVec2& size)
	---@param size ImVec2
    Dummy = function (size) end,
	---End()
    End = function () end,
	---EndBoxSelect(const ImRect& scope_rect,ImGuiMultiSelectFlags ms_flags)
	---@param scope_rect ImRect
	---@param ms_flags ImGuiMultiSelectFlags
    EndBoxSelect = function (scope_rect, ms_flags) end,
	---EndChild()
    EndChild = function () end,
	---EndColumns()
    EndColumns = function () end,
	---EndCombo()
    EndCombo = function () end,
	---EndComboPreview()
    EndComboPreview = function () end,
	---EndDisabled()
    EndDisabled = function () end,
	---EndDisabledOverrideReenable()
    EndDisabledOverrideReenable = function () end,
	---EndDragDropSource()
    EndDragDropSource = function () end,
	---EndDragDropTarget()
    EndDragDropTarget = function () end,
	---EndErrorTooltip()
    EndErrorTooltip = function () end,
	---EndFrame()
    EndFrame = function () end,
	---EndGroup()
    EndGroup = function () end,
	---EndListBox()
    EndListBox = function () end,
	---EndMainMenuBar()
    EndMainMenuBar = function () end,
	---EndMenu()
    EndMenu = function () end,
	---EndMenuBar()
    EndMenuBar = function () end,
	---EndMultiSelect()
    ---@return ImGuiMultiSelectIO
    EndMultiSelect = function () end,
	---EndPopup()
    EndPopup = function () end,
	---EndTabBar()
    EndTabBar = function () end,
	---EndTabItem()
    EndTabItem = function () end,
	---EndTable()
    EndTable = function () end,
	---EndTooltip()
    EndTooltip = function () end,
	---ErrorCheckEndFrameFinalizeErrorTooltip()
    ErrorCheckEndFrameFinalizeErrorTooltip = function () end,
	---ErrorCheckUsingSetCursorPosToExtendParentBoundaries()
    ErrorCheckUsingSetCursorPosToExtendParentBoundaries = function () end,
	---ErrorLog(const char* msg)
	---@param msg string
    ---@return boolean
    ErrorLog = function (msg) end,
	---ErrorRecoveryStoreState(ImGuiErrorRecoveryState* state_out)
	---@param state_out ImGuiErrorRecoveryState
    ErrorRecoveryStoreState = function (state_out) end,
	---ErrorRecoveryTryToRecoverState(const ImGuiErrorRecoveryState* state_in)
	---@param state_in ImGuiErrorRecoveryState
    ErrorRecoveryTryToRecoverState = function (state_in) end,
	---ErrorRecoveryTryToRecoverWindowState(const ImGuiErrorRecoveryState* state_in)
	---@param state_in ImGuiErrorRecoveryState
    ErrorRecoveryTryToRecoverWindowState = function (state_in) end,
	---FindBestWindowPosForPopup(ImGuiWindow* window)
	---@param window ImGuiWindow
    ---@return userdata
    FindBestWindowPosForPopup = function (window) end,
	---FindBestWindowPosForPopupEx(const ImVec2& ref_pos,const ImVec2& size,ImGuiDir* last_dir,const ImRect& r_outer,const ImRect& r_avoid,ImGuiPopupPositionPolicy policy)
	---@param ref_pos ImVec2
	---@param size ImVec2
	---@param last_dir ImGuiDir
	---@param r_outer ImRect
	---@param r_avoid ImRect
	---@param policy ImGuiPopupPositionPolicy
    ---@return userdata
    FindBestWindowPosForPopupEx = function (ref_pos, size, last_dir, r_outer, r_avoid, policy) end,
	---FindBlockingModal(ImGuiWindow* window)
	---@param window ImGuiWindow
    ---@return ImGuiWindow
    FindBlockingModal = function (window) end,
	---FindBottomMostVisibleWindowWithinBeginStack(ImGuiWindow* window)
	---@param window ImGuiWindow
    ---@return ImGuiWindow
    FindBottomMostVisibleWindowWithinBeginStack = function (window) end,
	---FindHoveredWindowEx(const ImVec2& pos,bool find_first_and_in_any_viewport,ImGuiWindow** out_hovered_window,ImGuiWindow** out_hovered_window_under_moving_window)
	---@param pos ImVec2
	---@param find_first_and_in_any_viewport boolean
	---@param out_hovered_window userdata
	---@param out_hovered_window_under_moving_window userdata
    FindHoveredWindowEx = function (pos, find_first_and_in_any_viewport, out_hovered_window, out_hovered_window_under_moving_window) end,
	---FindOrCreateColumns(ImGuiWindow* window,ImGuiID id)
	---@param window ImGuiWindow
	---@param id userdata
    ---@return ImGuiOldColumns
    FindOrCreateColumns = function (window, id) end,
	---FindRenderedTextEnd(const char* text,const char* text_end=((void*)0))
	---@param text string
	---@param text_end string
    ---@return string
    FindRenderedTextEnd = function (text, text_end) end,
	---FindSettingsHandler(const char* type_name)
	---@param type_name string
    ---@return ImGuiSettingsHandler
    FindSettingsHandler = function (type_name) end,
	---FindWindowByID(ImGuiID id)
	---@param id userdata
    ---@return ImGuiWindow
    FindWindowByID = function (id) end,
	---FindWindowByName(const char* name)
	---@param name string
    ---@return ImGuiWindow
    FindWindowByName = function (name) end,
	---FindWindowDisplayIndex(ImGuiWindow* window)
	---@param window ImGuiWindow
    ---@return integer
    FindWindowDisplayIndex = function (window) end,
	---FindWindowSettingsByID(ImGuiID id)
	---@param id userdata
    ---@return ImGuiWindowSettings
    FindWindowSettingsByID = function (id) end,
	---FindWindowSettingsByWindow(ImGuiWindow* window)
	---@param window ImGuiWindow
    ---@return ImGuiWindowSettings
    FindWindowSettingsByWindow = function (window) end,
	---FixupKeyChord(ImGuiKeyChord key_chord)
	---@param key_chord userdata
    ---@return userdata
    FixupKeyChord = function (key_chord) end,
	---FocusItem()
    FocusItem = function () end,
	---FocusTopMostWindowUnderOne(ImGuiWindow* under_this_window,ImGuiWindow* ignore_window,ImGuiViewport* filter_viewport,ImGuiFocusRequestFlags flags)
	---@param under_this_window ImGuiWindow
	---@param ignore_window ImGuiWindow
	---@param filter_viewport ImGuiViewport
	---@param flags ImGuiFocusRequestFlags
    FocusTopMostWindowUnderOne = function (under_this_window, ignore_window, filter_viewport, flags) end,
	---FocusWindow(ImGuiWindow* window,ImGuiFocusRequestFlags flags=0)
	---@param window ImGuiWindow
	---@param flags ImGuiFocusRequestFlags
    FocusWindow = function (window, flags) end,
	---GcAwakeTransientWindowBuffers(ImGuiWindow* window)
	---@param window ImGuiWindow
    GcAwakeTransientWindowBuffers = function (window) end,
	---GcCompactTransientMiscBuffers()
    GcCompactTransientMiscBuffers = function () end,
	---GcCompactTransientWindowBuffers(ImGuiWindow* window)
	---@param window ImGuiWindow
    GcCompactTransientWindowBuffers = function (window) end,
	---GetActiveID()
    ---@return userdata
    GetActiveID = function () end,
	---GetAllocatorFunctions(ImGuiMemAllocFunc* p_alloc_func,ImGuiMemFreeFunc* p_free_func,void** p_user_data)
	---@param p_alloc_func userdata
	---@param p_free_func userdata
	---@param p_user_data userdata
    GetAllocatorFunctions = function (p_alloc_func, p_free_func, p_user_data) end,
	---GetBackgroundDrawList()
    ---@return ImDrawList
    GetBackgroundDrawList = function () end,
	---GetBackgroundDrawList(ImGuiViewport* viewport)
	---@param viewport ImGuiViewport
    ---@return ImDrawList
    GetBackgroundDrawList = function (viewport) end,
	---GetBoxSelectState(ImGuiID id)
	---@param id userdata
    ---@return ImGuiBoxSelectState
    GetBoxSelectState = function (id) end,
	---GetClipboardText()
    ---@return string
    GetClipboardText = function () end,
	---GetColorU32(ImGuiCol idx,float alpha_mul=1.0f)
	---@param idx ImGuiCol
	---@param alpha_mul number
    ---@return integer
    GetColorU32 = function (idx, alpha_mul) end,
	---GetColorU32(const ImVec4& col)
	---@param col ImVec4
    ---@return integer
    GetColorU32 = function (col) end,
	---GetColorU32(ImU32 col,float alpha_mul=1.0f)
	---@param col integer
	---@param alpha_mul number
    ---@return integer
    GetColorU32 = function (col, alpha_mul) end,
	---GetColumnIndex()
    ---@return integer
    GetColumnIndex = function () end,
	---GetColumnNormFromOffset(const ImGuiOldColumns* columns,float offset)
	---@param columns ImGuiOldColumns
	---@param offset number
    ---@return number
    GetColumnNormFromOffset = function (columns, offset) end,
	---GetColumnOffset(int column_index=-1)
	---@param column_index integer
    ---@return number
    GetColumnOffset = function (column_index) end,
	---GetColumnOffsetFromNorm(const ImGuiOldColumns* columns,float offset_norm)
	---@param columns ImGuiOldColumns
	---@param offset_norm number
    ---@return number
    GetColumnOffsetFromNorm = function (columns, offset_norm) end,
	---GetColumnWidth(int column_index=-1)
	---@param column_index integer
    ---@return number
    GetColumnWidth = function (column_index) end,
	---GetColumnsCount()
    ---@return integer
    GetColumnsCount = function () end,
	---GetColumnsID(const char* str_id,int count)
	---@param str_id string
	---@param count integer
    ---@return userdata
    GetColumnsID = function (str_id, count) end,
	---GetContentRegionAvail()
    ---@return userdata
    GetContentRegionAvail = function () end,
	---GetCurrentContext()
    ---@return ImGuiContext
    GetCurrentContext = function () end,
	---GetCurrentFocusScope()
    ---@return userdata
    GetCurrentFocusScope = function () end,
	---GetCurrentTabBar()
    ---@return ImGuiTabBar
    GetCurrentTabBar = function () end,
	---GetCurrentTable()
    ---@return ImGuiTable
    GetCurrentTable = function () end,
	---GetCurrentWindow()
    ---@return ImGuiWindow
    GetCurrentWindow = function () end,
	---GetCurrentWindowRead()
    ---@return ImGuiWindow
    GetCurrentWindowRead = function () end,
	---GetCursorPos()
    ---@return userdata
    GetCursorPos = function () end,
	---GetCursorPosX()
    ---@return number
    GetCursorPosX = function () end,
	---GetCursorPosY()
    ---@return number
    GetCursorPosY = function () end,
	---GetCursorScreenPos()
    ---@return userdata
    GetCursorScreenPos = function () end,
	---GetCursorStartPos()
    ---@return userdata
    GetCursorStartPos = function () end,
	---GetDefaultFont()
    ---@return ImFont
    GetDefaultFont = function () end,
	---GetDragDropPayload()
    ---@return ImGuiPayload
    GetDragDropPayload = function () end,
	---GetDrawData()
    ---@return ImDrawData
    GetDrawData = function () end,
	---GetDrawListSharedData()
    ---@return ImDrawListSharedData
    GetDrawListSharedData = function () end,
	---GetFocusID()
    ---@return userdata
    GetFocusID = function () end,
	---GetFont()
    ---@return ImFont
    GetFont = function () end,
	---GetFontBaked()
    ---@return ImFontBaked
    GetFontBaked = function () end,
	---GetFontRasterizerDensity()
    ---@return number
    GetFontRasterizerDensity = function () end,
	---GetFontSize()
    ---@return number
    GetFontSize = function () end,
	---GetFontTexUvWhitePixel()
    ---@return userdata
    GetFontTexUvWhitePixel = function () end,
	---GetForegroundDrawList()
    ---@return ImDrawList
    GetForegroundDrawList = function () end,
	---GetForegroundDrawList(ImGuiWindow* window)
	---@param window ImGuiWindow
    ---@return ImDrawList
    GetForegroundDrawList = function (window) end,
	---GetForegroundDrawList(ImGuiViewport* viewport)
	---@param viewport ImGuiViewport
    ---@return ImDrawList
    GetForegroundDrawList = function (viewport) end,
	---GetFrameCount()
    ---@return integer
    GetFrameCount = function () end,
	---GetFrameHeight()
    ---@return number
    GetFrameHeight = function () end,
	---GetFrameHeightWithSpacing()
    ---@return number
    GetFrameHeightWithSpacing = function () end,
	---GetHoveredID()
    ---@return userdata
    GetHoveredID = function () end,
	---GetID(const char* str_id)
	---@param str_id string
    ---@return userdata
    GetID = function (str_id) end,
	---GetID(const char* str_id_begin,const char* str_id_end)
	---@param str_id_begin string
	---@param str_id_end string
    ---@return userdata
    GetID = function (str_id_begin, str_id_end) end,
	---GetID(const void* ptr_id)
	---@param ptr_id userdata
    ---@return userdata
    GetID = function (ptr_id) end,
	---GetID(int int_id)
	---@param int_id integer
    ---@return userdata
    GetID = function (int_id) end,
	---GetIDWithSeed(const char* str_id_begin,const char* str_id_end,ImGuiID seed)
	---@param str_id_begin string
	---@param str_id_end string
	---@param seed userdata
    ---@return userdata
    GetIDWithSeed = function (str_id_begin, str_id_end, seed) end,
	---GetIDWithSeed(int n,ImGuiID seed)
	---@param n integer
	---@param seed userdata
    ---@return userdata
    GetIDWithSeed = function (n, seed) end,
	---GetIO()
    ---@return ImGuiIO
    GetIO = function () end,
	---GetIO(ImGuiContext* ctx)
	---@param ctx ImGuiContext
    ---@return ImGuiIO
    GetIO = function (ctx) end,
	---GetInputTextState(ImGuiID id)
	---@param id userdata
    ---@return ImGuiInputTextState
    GetInputTextState = function (id) end,
	---GetItemFlags()
    ---@return ImGuiItemFlags
    GetItemFlags = function () end,
	---GetItemID()
    ---@return userdata
    GetItemID = function () end,
	---GetItemRectMax()
    ---@return userdata
    GetItemRectMax = function () end,
	---GetItemRectMin()
    ---@return userdata
    GetItemRectMin = function () end,
	---GetItemRectSize()
    ---@return userdata
    GetItemRectSize = function () end,
	---GetItemStatusFlags()
    ---@return ImGuiItemStatusFlags
    GetItemStatusFlags = function () end,
	---GetKeyChordName(ImGuiKeyChord key_chord)
	---@param key_chord userdata
    ---@return string
    GetKeyChordName = function (key_chord) end,
	---GetKeyData(ImGuiContext* ctx,ImGuiKey key)
	---@param ctx ImGuiContext
	---@param key ImGuiKey
    ---@return ImGuiKeyData
    GetKeyData = function (ctx, key) end,
	---GetKeyData(ImGuiKey key)
	---@param key ImGuiKey
    ---@return ImGuiKeyData
    GetKeyData = function (key) end,
	---GetKeyMagnitude2d(ImGuiKey key_left,ImGuiKey key_right,ImGuiKey key_up,ImGuiKey key_down)
	---@param key_left ImGuiKey
	---@param key_right ImGuiKey
	---@param key_up ImGuiKey
	---@param key_down ImGuiKey
    ---@return userdata
    GetKeyMagnitude2d = function (key_left, key_right, key_up, key_down) end,
	---GetKeyName(ImGuiKey key)
	---@param key ImGuiKey
    ---@return string
    GetKeyName = function (key) end,
	---GetKeyOwner(ImGuiKey key)
	---@param key ImGuiKey
    ---@return userdata
    GetKeyOwner = function (key) end,
	---GetKeyOwnerData(ImGuiContext* ctx,ImGuiKey key)
	---@param ctx ImGuiContext
	---@param key ImGuiKey
    ---@return ImGuiKeyOwnerData
    GetKeyOwnerData = function (ctx, key) end,
	---GetKeyPressedAmount(ImGuiKey key,float repeat_delay,float rate)
	---@param key ImGuiKey
	---@param repeat_delay number
	---@param rate number
    ---@return integer
    GetKeyPressedAmount = function (key, repeat_delay, rate) end,
	---GetMainViewport()
    ---@return ImGuiViewport
    GetMainViewport = function () end,
	---GetMouseClickedCount(ImGuiMouseButton button)
	---@param button ImGuiMouseButton
    ---@return integer
    GetMouseClickedCount = function (button) end,
	---GetMouseCursor()
    ---@return ImGuiMouseCursor
    GetMouseCursor = function () end,
	---GetMouseDragDelta(ImGuiMouseButton button=0,float lock_threshold=-1.0f)
	---@param button ImGuiMouseButton
	---@param lock_threshold number
    ---@return userdata
    GetMouseDragDelta = function (button, lock_threshold) end,
	---GetMousePos()
    ---@return userdata
    GetMousePos = function () end,
	---GetMousePosOnOpeningCurrentPopup()
    ---@return userdata
    GetMousePosOnOpeningCurrentPopup = function () end,
	---GetMultiSelectState(ImGuiID id)
	---@param id userdata
    ---@return ImGuiMultiSelectState
    GetMultiSelectState = function (id) end,
	---GetNavTweakPressedAmount(ImGuiAxis axis)
	---@param axis ImGuiAxis
    ---@return number
    GetNavTweakPressedAmount = function (axis) end,
	---GetPlatformIO()
    ---@return ImGuiPlatformIO
    GetPlatformIO = function () end,
	---GetPlatformIO(ImGuiContext* ctx)
	---@param ctx ImGuiContext
    ---@return ImGuiPlatformIO
    GetPlatformIO = function (ctx) end,
	---GetPopupAllowedExtentRect(ImGuiWindow* window)
	---@param window ImGuiWindow
    ---@return userdata
    GetPopupAllowedExtentRect = function (window) end,
	---GetRoundedFontSize(float size)
	---@param size number
    ---@return number
    GetRoundedFontSize = function (size) end,
	---GetScrollMaxX()
    ---@return number
    GetScrollMaxX = function () end,
	---GetScrollMaxY()
    ---@return number
    GetScrollMaxY = function () end,
	---GetScrollX()
    ---@return number
    GetScrollX = function () end,
	---GetScrollY()
    ---@return number
    GetScrollY = function () end,
	---GetShortcutRoutingData(ImGuiKeyChord key_chord)
	---@param key_chord userdata
    ---@return ImGuiKeyRoutingData
    GetShortcutRoutingData = function (key_chord) end,
	---GetStateStorage()
    ---@return ImGuiStorage
    GetStateStorage = function () end,
	---GetStyle()
    ---@return ImGuiStyle
    GetStyle = function () end,
	---GetStyleColorName(ImGuiCol idx)
	---@param idx ImGuiCol
    ---@return string
    GetStyleColorName = function (idx) end,
	---GetStyleColorVec4(ImGuiCol idx)
	---@param idx ImGuiCol
    ---@return userdata
    GetStyleColorVec4 = function (idx) end,
	---GetStyleVarInfo(ImGuiStyleVar idx)
	---@param idx ImGuiStyleVar
    ---@return ImGuiStyleVarInfo
    GetStyleVarInfo = function (idx) end,
	---GetTextLineHeight()
    ---@return number
    GetTextLineHeight = function () end,
	---GetTextLineHeightWithSpacing()
    ---@return number
    GetTextLineHeightWithSpacing = function () end,
	---GetTime()
    ---@return number
    GetTime = function () end,
	---GetTopMostAndVisiblePopupModal()
    ---@return ImGuiWindow
    GetTopMostAndVisiblePopupModal = function () end,
	---GetTopMostPopupModal()
    ---@return ImGuiWindow
    GetTopMostPopupModal = function () end,
	---GetTreeNodeToLabelSpacing()
    ---@return number
    GetTreeNodeToLabelSpacing = function () end,
	---GetTypematicRepeatRate(ImGuiInputFlags flags,float* repeat_delay,float* repeat_rate)
	---@param flags ImGuiInputFlags,
	---@param repeat_delay number|nil,
	---@param repeat_rate number|nil
    ---@return number, number
    GetTypematicRepeatRate = function (flags, repeat_delay, repeat_rate) end,
	---GetTypingSelectRequest(ImGuiTypingSelectFlags flags=ImGuiTypingSelectFlags_None)
	---@param flags ImGuiTypingSelectFlags
    ---@return ImGuiTypingSelectRequest
    GetTypingSelectRequest = function (flags) end,
	---GetVersion()
    ---@return string
    GetVersion = function () end,
	---GetWindowDrawList()
    ---@return ImDrawList
    GetWindowDrawList = function () end,
	---GetWindowHeight()
    ---@return number
    GetWindowHeight = function () end,
	---GetWindowPos()
    ---@return userdata
    GetWindowPos = function () end,
	---GetWindowResizeBorderID(ImGuiWindow* window,ImGuiDir dir)
	---@param window ImGuiWindow
	---@param dir ImGuiDir
    ---@return userdata
    GetWindowResizeBorderID = function (window, dir) end,
	---GetWindowResizeCornerID(ImGuiWindow* window,int n)
	---@param window ImGuiWindow
	---@param n integer
    ---@return userdata
    GetWindowResizeCornerID = function (window, n) end,
	---GetWindowScrollbarID(ImGuiWindow* window,ImGuiAxis axis)
	---@param window ImGuiWindow
	---@param axis ImGuiAxis
    ---@return userdata
    GetWindowScrollbarID = function (window, axis) end,
	---GetWindowScrollbarRect(ImGuiWindow* window,ImGuiAxis axis)
	---@param window ImGuiWindow
	---@param axis ImGuiAxis
    ---@return userdata
    GetWindowScrollbarRect = function (window, axis) end,
	---GetWindowSize()
    ---@return userdata
    GetWindowSize = function () end,
	---GetWindowWidth()
    ---@return number
    GetWindowWidth = function () end,
	---Image(ImTextureRef tex_ref,const ImVec2& image_size,const ImVec2& uv0=ImVec2(0,0),const ImVec2& uv1=ImVec2(1,1))
	---@param tex_ref ImTextureRef
	---@param image_size ImVec2
	---@param uv0 ImVec2
	---@param uv1 ImVec2
    Image = function (tex_ref, image_size, uv0, uv1) end,
	---ImageButton(const char* str_id,ImTextureRef tex_ref,const ImVec2& image_size,const ImVec2& uv0=ImVec2(0,0),const ImVec2& uv1=ImVec2(1,1),const ImVec4& bg_col=ImVec4(0,0,0,0),const ImVec4& tint_col=ImVec4(1,1,1,1))
	---@param str_id string
	---@param tex_ref ImTextureRef
	---@param image_size ImVec2
	---@param uv0 ImVec2
	---@param uv1 ImVec2
	---@param bg_col ImVec4
	---@param tint_col ImVec4
    ---@return boolean
    ImageButton = function (str_id, tex_ref, image_size, uv0, uv1, bg_col, tint_col) end,
	---ImageButtonEx(ImGuiID id,ImTextureRef tex_ref,const ImVec2& image_size,const ImVec2& uv0,const ImVec2& uv1,const ImVec4& bg_col,const ImVec4& tint_col,ImGuiButtonFlags flags=0)
	---@param id userdata
	---@param tex_ref ImTextureRef
	---@param image_size ImVec2
	---@param uv0 ImVec2
	---@param uv1 ImVec2
	---@param bg_col ImVec4
	---@param tint_col ImVec4
	---@param flags ImGuiButtonFlags
    ---@return boolean
    ImageButtonEx = function (id, tex_ref, image_size, uv0, uv1, bg_col, tint_col, flags) end,
	---ImageWithBg(ImTextureRef tex_ref,const ImVec2& image_size,const ImVec2& uv0=ImVec2(0,0),const ImVec2& uv1=ImVec2(1,1),const ImVec4& bg_col=ImVec4(0,0,0,0),const ImVec4& tint_col=ImVec4(1,1,1,1))
	---@param tex_ref ImTextureRef
	---@param image_size ImVec2
	---@param uv0 ImVec2
	---@param uv1 ImVec2
	---@param bg_col ImVec4
	---@param tint_col ImVec4
    ImageWithBg = function (tex_ref, image_size, uv0, uv1, bg_col, tint_col) end,
	---Indent(float indent_w=0.0f)
	---@param indent_w number
    Indent = function (indent_w) end,
	---Initialize()
    Initialize = function () end,
	---InputDouble(const char* label,double* v,double step=0.0,double step_fast=0.0,const char* format="%.6f",ImGuiInputTextFlags flags=0)
	---@param label string,
	---@param v number|nil,
	---@param step number,
	---@param step_fast number,
	---@param format string,
	---@param flags ImGuiInputTextFlags
    ---@return boolean, number
    InputDouble = function (label, v, step, step_fast, format, flags) end,
	---InputFloat(const char* label,float* v,float step=0.0f,float step_fast=0.0f,const char* format="%.3f",ImGuiInputTextFlags flags=0)
	---@param label string,
	---@param v number|nil,
	---@param step number,
	---@param step_fast number,
	---@param format string,
	---@param flags ImGuiInputTextFlags
    ---@return boolean, number
    InputFloat = function (label, v, step, step_fast, format, flags) end,
	---InputFloat2(const char* label,float v[2],const char* format="%.3f",ImGuiInputTextFlags flags=0)
	---@param label string
	---@param v userdata
	---@param format string
	---@param flags ImGuiInputTextFlags
    ---@return boolean
    InputFloat2 = function (label, v, format, flags) end,
	---InputFloat3(const char* label,float v[3],const char* format="%.3f",ImGuiInputTextFlags flags=0)
	---@param label string
	---@param v userdata
	---@param format string
	---@param flags ImGuiInputTextFlags
    ---@return boolean
    InputFloat3 = function (label, v, format, flags) end,
	---InputFloat4(const char* label,float v[4],const char* format="%.3f",ImGuiInputTextFlags flags=0)
	---@param label string
	---@param v userdata
	---@param format string
	---@param flags ImGuiInputTextFlags
    ---@return boolean
    InputFloat4 = function (label, v, format, flags) end,
	---InputInt(const char* label,int* v,int step=1,int step_fast=100,ImGuiInputTextFlags flags=0)
	---@param label string,
	---@param v integer|nil,
	---@param step integer,
	---@param step_fast integer,
	---@param flags ImGuiInputTextFlags
    ---@return boolean, integer
    InputInt = function (label, v, step, step_fast, flags) end,
	---InputInt2(const char* label,int v[2],ImGuiInputTextFlags flags=0)
	---@param label string
	---@param v userdata
	---@param flags ImGuiInputTextFlags
    ---@return boolean
    InputInt2 = function (label, v, flags) end,
	---InputInt3(const char* label,int v[3],ImGuiInputTextFlags flags=0)
	---@param label string
	---@param v userdata
	---@param flags ImGuiInputTextFlags
    ---@return boolean
    InputInt3 = function (label, v, flags) end,
	---InputInt4(const char* label,int v[4],ImGuiInputTextFlags flags=0)
	---@param label string
	---@param v userdata
	---@param flags ImGuiInputTextFlags
    ---@return boolean
    InputInt4 = function (label, v, flags) end,
	---InputScalar(const char* label,ImGuiDataType data_type,void* p_data,const void* p_step=((void*)0),const void* p_step_fast=((void*)0),const char* format=((void*)0),ImGuiInputTextFlags flags=0)
	---@param label string
	---@param data_type ImGuiDataType
	---@param p_data userdata
	---@param p_step userdata
	---@param p_step_fast userdata
	---@param format string
	---@param flags ImGuiInputTextFlags
    ---@return boolean
    InputScalar = function (label, data_type, p_data, p_step, p_step_fast, format, flags) end,
	---InputScalarN(const char* label,ImGuiDataType data_type,void* p_data,int components,const void* p_step=((void*)0),const void* p_step_fast=((void*)0),const char* format=((void*)0),ImGuiInputTextFlags flags=0)
	---@param label string
	---@param data_type ImGuiDataType
	---@param p_data userdata
	---@param components integer
	---@param p_step userdata
	---@param p_step_fast userdata
	---@param format string
	---@param flags ImGuiInputTextFlags
    ---@return boolean
    InputScalarN = function (label, data_type, p_data, components, p_step, p_step_fast, format, flags) end,
	---InputTextDeactivateHook(ImGuiID id)
	---@param id userdata
    InputTextDeactivateHook = function (id) end,
	---InputTextEx(const char* label,const char* hint,char* buf,int buf_size,const ImVec2& size_arg,ImGuiInputTextFlags flags,ImGuiInputTextCallback callback=((void*)0),void* user_data=((void*)0))
	---@param label string
	---@param hint string
	---@param buf userdata
	---@param buf_size integer
	---@param size_arg ImVec2
	---@param flags ImGuiInputTextFlags
	---@param callback userdata
	---@param user_data userdata
    ---@return boolean
    InputTextEx = function (label, hint, buf, buf_size, size_arg, flags, callback, user_data) end,
	---InvisibleButton(const char* str_id,const ImVec2& size,ImGuiButtonFlags flags=0)
	---@param str_id string
	---@param size ImVec2
	---@param flags ImGuiButtonFlags
    ---@return boolean
    InvisibleButton = function (str_id, size, flags) end,
	---IsActiveIdUsingNavDir(ImGuiDir dir)
	---@param dir ImGuiDir
    ---@return boolean
    IsActiveIdUsingNavDir = function (dir) end,
	---IsAliasKey(ImGuiKey key)
	---@param key ImGuiKey
    ---@return boolean
    IsAliasKey = function (key) end,
	---IsAnyItemActive()
    ---@return boolean
    IsAnyItemActive = function () end,
	---IsAnyItemFocused()
    ---@return boolean
    IsAnyItemFocused = function () end,
	---IsAnyItemHovered()
    ---@return boolean
    IsAnyItemHovered = function () end,
	---IsAnyMouseDown()
    ---@return boolean
    IsAnyMouseDown = function () end,
	---IsClippedEx(const ImRect& bb,ImGuiID id)
	---@param bb ImRect
	---@param id userdata
    ---@return boolean
    IsClippedEx = function (bb, id) end,
	---IsDragDropActive()
    ---@return boolean
    IsDragDropActive = function () end,
	---IsDragDropPayloadBeingAccepted()
    ---@return boolean
    IsDragDropPayloadBeingAccepted = function () end,
	---IsGamepadKey(ImGuiKey key)
	---@param key ImGuiKey
    ---@return boolean
    IsGamepadKey = function (key) end,
	---IsItemActivated()
    ---@return boolean
    IsItemActivated = function () end,
	---IsItemActive()
    ---@return boolean
    IsItemActive = function () end,
	---IsItemActiveAsInputText()
    ---@return boolean
    IsItemActiveAsInputText = function () end,
	---IsItemClicked(ImGuiMouseButton mouse_button=0)
	---@param mouse_button ImGuiMouseButton
    ---@return boolean
    IsItemClicked = function (mouse_button) end,
	---IsItemDeactivated()
    ---@return boolean
    IsItemDeactivated = function () end,
	---IsItemDeactivatedAfterEdit()
    ---@return boolean
    IsItemDeactivatedAfterEdit = function () end,
	---IsItemEdited()
    ---@return boolean
    IsItemEdited = function () end,
	---IsItemFocused()
    ---@return boolean
    IsItemFocused = function () end,
	---IsItemHovered(ImGuiHoveredFlags flags=0)
	---@param flags ImGuiHoveredFlags
    ---@return boolean
    IsItemHovered = function (flags) end,
	---IsItemToggledOpen()
    ---@return boolean
    IsItemToggledOpen = function () end,
	---IsItemToggledSelection()
    ---@return boolean
    IsItemToggledSelection = function () end,
	---IsItemVisible()
    ---@return boolean
    IsItemVisible = function () end,
	---IsKeyChordPressed(ImGuiKeyChord key_chord)
	---@param key_chord userdata
    ---@return boolean
    IsKeyChordPressed = function (key_chord) end,
	---IsKeyChordPressed(ImGuiKeyChord key_chord,ImGuiInputFlags flags,ImGuiID owner_id=0)
	---@param key_chord userdata
	---@param flags ImGuiInputFlags
	---@param owner_id userdata
    ---@return boolean
    IsKeyChordPressed = function (key_chord, flags, owner_id) end,
	---IsKeyDown(ImGuiKey key)
	---@param key ImGuiKey
    ---@return boolean
    IsKeyDown = function (key) end,
	---IsKeyDown(ImGuiKey key,ImGuiID owner_id)
	---@param key ImGuiKey
	---@param owner_id userdata
    ---@return boolean
    IsKeyDown = function (key, owner_id) end,
	---IsKeyPressed(ImGuiKey key,bool repeat=true)
	---@param key ImGuiKey
	---@param repeat_ boolean
    ---@return boolean
    IsKeyPressed = function (key, repeat_) end,
	---IsKeyPressed(ImGuiKey key,ImGuiInputFlags flags,ImGuiID owner_id=0)
	---@param key ImGuiKey
	---@param flags ImGuiInputFlags
	---@param owner_id userdata
    ---@return boolean
    IsKeyPressed = function (key, flags, owner_id) end,
	---IsKeyReleased(ImGuiKey key)
	---@param key ImGuiKey
    ---@return boolean
    IsKeyReleased = function (key) end,
	---IsKeyReleased(ImGuiKey key,ImGuiID owner_id)
	---@param key ImGuiKey
	---@param owner_id userdata
    ---@return boolean
    IsKeyReleased = function (key, owner_id) end,
	---IsKeyboardKey(ImGuiKey key)
	---@param key ImGuiKey
    ---@return boolean
    IsKeyboardKey = function (key) end,
	---IsLRModKey(ImGuiKey key)
	---@param key ImGuiKey
    ---@return boolean
    IsLRModKey = function (key) end,
	---IsLegacyKey(ImGuiKey key)
	---@param key ImGuiKey
    ---@return boolean
    IsLegacyKey = function (key) end,
	---IsMouseClicked(ImGuiMouseButton button,bool repeat=false)
	---@param button ImGuiMouseButton
	---@param repeat_ boolean
    ---@return boolean
    IsMouseClicked = function (button, repeat_) end,
	---IsMouseClicked(ImGuiMouseButton button,ImGuiInputFlags flags,ImGuiID owner_id=0)
	---@param button ImGuiMouseButton
	---@param flags ImGuiInputFlags
	---@param owner_id userdata
    ---@return boolean
    IsMouseClicked = function (button, flags, owner_id) end,
	---IsMouseDoubleClicked(ImGuiMouseButton button)
	---@param button ImGuiMouseButton
    ---@return boolean
    IsMouseDoubleClicked = function (button) end,
	---IsMouseDoubleClicked(ImGuiMouseButton button,ImGuiID owner_id)
	---@param button ImGuiMouseButton
	---@param owner_id userdata
    ---@return boolean
    IsMouseDoubleClicked = function (button, owner_id) end,
	---IsMouseDown(ImGuiMouseButton button)
	---@param button ImGuiMouseButton
    ---@return boolean
    IsMouseDown = function (button) end,
	---IsMouseDown(ImGuiMouseButton button,ImGuiID owner_id)
	---@param button ImGuiMouseButton
	---@param owner_id userdata
    ---@return boolean
    IsMouseDown = function (button, owner_id) end,
	---IsMouseDragPastThreshold(ImGuiMouseButton button,float lock_threshold=-1.0f)
	---@param button ImGuiMouseButton
	---@param lock_threshold number
    ---@return boolean
    IsMouseDragPastThreshold = function (button, lock_threshold) end,
	---IsMouseDragging(ImGuiMouseButton button,float lock_threshold=-1.0f)
	---@param button ImGuiMouseButton
	---@param lock_threshold number
    ---@return boolean
    IsMouseDragging = function (button, lock_threshold) end,
	---IsMouseHoveringRect(const ImVec2& r_min,const ImVec2& r_max,bool clip=true)
	---@param r_min ImVec2
	---@param r_max ImVec2
	---@param clip boolean
    ---@return boolean
    IsMouseHoveringRect = function (r_min, r_max, clip) end,
	---IsMouseKey(ImGuiKey key)
	---@param key ImGuiKey
    ---@return boolean
    IsMouseKey = function (key) end,
	---IsMousePosValid(const ImVec2* mouse_pos=((void*)0))
	---@param mouse_pos ImVec2
    ---@return boolean
    IsMousePosValid = function (mouse_pos) end,
	---IsMouseReleased(ImGuiMouseButton button)
	---@param button ImGuiMouseButton
    ---@return boolean
    IsMouseReleased = function (button) end,
	---IsMouseReleased(ImGuiMouseButton button,ImGuiID owner_id)
	---@param button ImGuiMouseButton
	---@param owner_id userdata
    ---@return boolean
    IsMouseReleased = function (button, owner_id) end,
	---IsMouseReleasedWithDelay(ImGuiMouseButton button,float delay)
	---@param button ImGuiMouseButton
	---@param delay number
    ---@return boolean
    IsMouseReleasedWithDelay = function (button, delay) end,
	---IsNamedKey(ImGuiKey key)
	---@param key ImGuiKey
    ---@return boolean
    IsNamedKey = function (key) end,
	---IsNamedKeyOrMod(ImGuiKey key)
	---@param key ImGuiKey
    ---@return boolean
    IsNamedKeyOrMod = function (key) end,
	---IsPopupOpen(const char* str_id,ImGuiPopupFlags flags=0)
	---@param str_id string
	---@param flags ImGuiPopupFlags
    ---@return boolean
    IsPopupOpen = function (str_id, flags) end,
	---IsPopupOpen(ImGuiID id,ImGuiPopupFlags popup_flags)
	---@param id userdata
	---@param popup_flags ImGuiPopupFlags
    ---@return boolean
    IsPopupOpen = function (id, popup_flags) end,
	---IsRectVisible(const ImVec2& size)
	---@param size ImVec2
    ---@return boolean
    IsRectVisible = function (size) end,
	---IsRectVisible(const ImVec2& rect_min,const ImVec2& rect_max)
	---@param rect_min ImVec2
	---@param rect_max ImVec2
    ---@return boolean
    IsRectVisible = function (rect_min, rect_max) end,
	---IsWindowAbove(ImGuiWindow* potential_above,ImGuiWindow* potential_below)
	---@param potential_above ImGuiWindow
	---@param potential_below ImGuiWindow
    ---@return boolean
    IsWindowAbove = function (potential_above, potential_below) end,
	---IsWindowAppearing()
    ---@return boolean
    IsWindowAppearing = function () end,
	---IsWindowChildOf(ImGuiWindow* window,ImGuiWindow* potential_parent,bool popup_hierarchy)
	---@param window ImGuiWindow
	---@param potential_parent ImGuiWindow
	---@param popup_hierarchy boolean
    ---@return boolean
    IsWindowChildOf = function (window, potential_parent, popup_hierarchy) end,
	---IsWindowCollapsed()
    ---@return boolean
    IsWindowCollapsed = function () end,
	---IsWindowContentHoverable(ImGuiWindow* window,ImGuiHoveredFlags flags=0)
	---@param window ImGuiWindow
	---@param flags ImGuiHoveredFlags
    ---@return boolean
    IsWindowContentHoverable = function (window, flags) end,
	---IsWindowFocused(ImGuiFocusedFlags flags=0)
	---@param flags ImGuiFocusedFlags
    ---@return boolean
    IsWindowFocused = function (flags) end,
	---IsWindowHovered(ImGuiHoveredFlags flags=0)
	---@param flags ImGuiHoveredFlags
    ---@return boolean
    IsWindowHovered = function (flags) end,
	---IsWindowInBeginStack(ImGuiWindow* window)
	---@param window ImGuiWindow
    ---@return boolean
    IsWindowInBeginStack = function (window) end,
	---IsWindowNavFocusable(ImGuiWindow* window)
	---@param window ImGuiWindow
    ---@return boolean
    IsWindowNavFocusable = function (window) end,
	---IsWindowWithinBeginStackOf(ImGuiWindow* window,ImGuiWindow* potential_parent)
	---@param window ImGuiWindow
	---@param potential_parent ImGuiWindow
    ---@return boolean
    IsWindowWithinBeginStackOf = function (window, potential_parent) end,
	---ItemAdd(const ImRect& bb,ImGuiID id,const ImRect* nav_bb=((void*)0),ImGuiItemFlags extra_flags=0)
	---@param bb ImRect
	---@param id userdata
	---@param nav_bb ImRect
	---@param extra_flags ImGuiItemFlags
    ---@return boolean
    ItemAdd = function (bb, id, nav_bb, extra_flags) end,
	---ItemHoverable(const ImRect& bb,ImGuiID id,ImGuiItemFlags item_flags)
	---@param bb ImRect
	---@param id userdata
	---@param item_flags ImGuiItemFlags
    ---@return boolean
    ItemHoverable = function (bb, id, item_flags) end,
	---ItemSize(const ImVec2& size,float text_baseline_y=-1.0f)
	---@param size ImVec2
	---@param text_baseline_y number
    ItemSize = function (size, text_baseline_y) end,
	---ItemSize(const ImRect& bb,float text_baseline_y=-1.0f)
	---@param bb ImRect
	---@param text_baseline_y number
    ItemSize = function (bb, text_baseline_y) end,
	---KeepAliveID(ImGuiID id)
	---@param id userdata
    KeepAliveID = function (id) end,
	---LabelText(const char* label,const char* fmt,...)
	---@param label string
	---@param fmt string
	---@param ... any
    LabelText = function (label, fmt, ...) end,
	---LabelTextV(const char* label,const char* fmt,va_list args)
	---@param label string
	---@param fmt string
	---@param args userdata
    LabelTextV = function (label, fmt, args) end,
	---ListBox(const char* label,int* current_item,const char* const items[],int items_count,int height_in_items=-1)
	---@param label string,
	---@param current_item integer|nil,
	---@param items userdata,
	---@param items_count integer,
	---@param height_in_items integer
    ---@return boolean, integer
    ListBox = function (label, current_item, items, items_count, height_in_items) end,
	---ListBox(const char* label,int* current_item,const char*(*getter)(void* user_data,int idx),void* user_data,int items_count,int height_in_items=-1)
	---@param label string,
	---@param current_item integer|nil,
	---@param getter userdata,
	---@param user_data userdata,
	---@param items_count integer,
	---@param height_in_items integer
    ---@return boolean, integer
    ListBox = function (label, current_item, getter, user_data, items_count, height_in_items) end,
	---LoadIniSettingsFromDisk(const char* ini_filename)
	---@param ini_filename string
    LoadIniSettingsFromDisk = function (ini_filename) end,
	---LoadIniSettingsFromMemory(const char* ini_data,size_t ini_size=0)
	---@param ini_data string
	---@param ini_size userdata
    LoadIniSettingsFromMemory = function (ini_data, ini_size) end,
	---LocalizeGetMsg(ImGuiLocKey key)
	---@param key ImGuiLocKey
    ---@return string
    LocalizeGetMsg = function (key) end,
	---LocalizeRegisterEntries(const ImGuiLocEntry* entries,int count)
	---@param entries ImGuiLocEntry
	---@param count integer
    LocalizeRegisterEntries = function (entries, count) end,
	---LogBegin(ImGuiLogFlags flags,int auto_open_depth)
	---@param flags ImGuiLogFlags
	---@param auto_open_depth integer
    LogBegin = function (flags, auto_open_depth) end,
	---LogButtons()
    LogButtons = function () end,
	---LogFinish()
    LogFinish = function () end,
	---LogRenderedText(const ImVec2* ref_pos,const char* text,const char* text_end=((void*)0))
	---@param ref_pos ImVec2
	---@param text string
	---@param text_end string
    LogRenderedText = function (ref_pos, text, text_end) end,
	---LogSetNextTextDecoration(const char* prefix,const char* suffix)
	---@param prefix string
	---@param suffix string
    LogSetNextTextDecoration = function (prefix, suffix) end,
	---LogText(const char* fmt,...)
	---@param fmt string
	---@param ... any
    LogText = function (fmt, ...) end,
	---LogTextV(const char* fmt,va_list args)
	---@param fmt string
	---@param args userdata
    LogTextV = function (fmt, args) end,
	---LogToBuffer(int auto_open_depth=-1)
	---@param auto_open_depth integer
    LogToBuffer = function (auto_open_depth) end,
	---LogToClipboard(int auto_open_depth=-1)
	---@param auto_open_depth integer
    LogToClipboard = function (auto_open_depth) end,
	---LogToFile(int auto_open_depth=-1,const char* filename=((void*)0))
	---@param auto_open_depth integer
	---@param filename string
    LogToFile = function (auto_open_depth, filename) end,
	---LogToTTY(int auto_open_depth=-1)
	---@param auto_open_depth integer
    LogToTTY = function (auto_open_depth) end,
	---MarkIniSettingsDirty()
    MarkIniSettingsDirty = function () end,
	---MarkIniSettingsDirty(ImGuiWindow* window)
	---@param window ImGuiWindow
    MarkIniSettingsDirty = function (window) end,
	---MarkItemEdited(ImGuiID id)
	---@param id userdata
    MarkItemEdited = function (id) end,
	---MemAlloc(size_t size)
	---@param size userdata
    ---@return userdata
    MemAlloc = function (size) end,
	---MemFree(void* ptr)
	---@param ptr userdata
    MemFree = function (ptr) end,
	---MenuItem(const char* label,const char* shortcut=((void*)0),bool selected=false,bool enabled=true)
	---@param label string
	---@param shortcut string
	---@param selected boolean
	---@param enabled boolean
    ---@return boolean
    MenuItem = function (label, shortcut, selected, enabled) end,
	---MenuItem(const char* label,const char* shortcut,bool* p_selected,bool enabled=true)
	---@param label string,
	---@param shortcut string,
	---@param p_selected boolean|nil,
	---@param enabled boolean
    ---@return boolean, boolean
    MenuItem = function (label, shortcut, p_selected, enabled) end,
	---MenuItemEx(const char* label,const char* icon,const char* shortcut=((void*)0),bool selected=false,bool enabled=true)
	---@param label string
	---@param icon string
	---@param shortcut string
	---@param selected boolean
	---@param enabled boolean
    ---@return boolean
    MenuItemEx = function (label, icon, shortcut, selected, enabled) end,
	---MouseButtonToKey(ImGuiMouseButton button)
	---@param button ImGuiMouseButton
    ---@return ImGuiKey
    MouseButtonToKey = function (button) end,
	---MultiSelectAddSetAll(ImGuiMultiSelectTempData* ms,bool selected)
	---@param ms ImGuiMultiSelectTempData
	---@param selected boolean
    MultiSelectAddSetAll = function (ms, selected) end,
	---MultiSelectAddSetRange(ImGuiMultiSelectTempData* ms,bool selected,int range_dir,ImGuiSelectionUserData first_item,ImGuiSelectionUserData last_item)
	---@param ms ImGuiMultiSelectTempData
	---@param selected boolean
	---@param range_dir integer
	---@param first_item userdata
	---@param last_item userdata
    MultiSelectAddSetRange = function (ms, selected, range_dir, first_item, last_item) end,
	---MultiSelectItemFooter(ImGuiID id,bool* p_selected,bool* p_pressed)
	---@param id userdata,
	---@param p_selected boolean|nil,
	---@param p_pressed boolean|nil
    ---@return boolean, boolean
    MultiSelectItemFooter = function (id, p_selected, p_pressed) end,
	---MultiSelectItemHeader(ImGuiID id,bool* p_selected,ImGuiButtonFlags* p_button_flags)
	---@param id userdata,
	---@param p_selected boolean|nil,
	---@param p_button_flags ImGuiButtonFlags
    ---@return boolean
    MultiSelectItemHeader = function (id, p_selected, p_button_flags) end,
	---NavClearPreferredPosForAxis(ImGuiAxis axis)
	---@param axis ImGuiAxis
    NavClearPreferredPosForAxis = function (axis) end,
	---NavHighlightActivated(ImGuiID id)
	---@param id userdata
    NavHighlightActivated = function (id) end,
	---NavInitRequestApplyResult()
    NavInitRequestApplyResult = function () end,
	---NavInitWindow(ImGuiWindow* window,bool force_reinit)
	---@param window ImGuiWindow
	---@param force_reinit boolean
    NavInitWindow = function (window, force_reinit) end,
	---NavMoveRequestApplyResult()
    NavMoveRequestApplyResult = function () end,
	---NavMoveRequestButNoResultYet()
    ---@return boolean
    NavMoveRequestButNoResultYet = function () end,
	---NavMoveRequestCancel()
    NavMoveRequestCancel = function () end,
	---NavMoveRequestForward(ImGuiDir move_dir,ImGuiDir clip_dir,ImGuiNavMoveFlags move_flags,ImGuiScrollFlags scroll_flags)
	---@param move_dir ImGuiDir
	---@param clip_dir ImGuiDir
	---@param move_flags ImGuiNavMoveFlags
	---@param scroll_flags ImGuiScrollFlags
    NavMoveRequestForward = function (move_dir, clip_dir, move_flags, scroll_flags) end,
	---NavMoveRequestResolveWithLastItem(ImGuiNavItemData* result)
	---@param result ImGuiNavItemData
    NavMoveRequestResolveWithLastItem = function (result) end,
	---NavMoveRequestResolveWithPastTreeNode(ImGuiNavItemData* result,const ImGuiTreeNodeStackData* tree_node_data)
	---@param result ImGuiNavItemData
	---@param tree_node_data ImGuiTreeNodeStackData
    NavMoveRequestResolveWithPastTreeNode = function (result, tree_node_data) end,
	---NavMoveRequestSubmit(ImGuiDir move_dir,ImGuiDir clip_dir,ImGuiNavMoveFlags move_flags,ImGuiScrollFlags scroll_flags)
	---@param move_dir ImGuiDir
	---@param clip_dir ImGuiDir
	---@param move_flags ImGuiNavMoveFlags
	---@param scroll_flags ImGuiScrollFlags
    NavMoveRequestSubmit = function (move_dir, clip_dir, move_flags, scroll_flags) end,
	---NavMoveRequestTryWrapping(ImGuiWindow* window,ImGuiNavMoveFlags move_flags)
	---@param window ImGuiWindow
	---@param move_flags ImGuiNavMoveFlags
    NavMoveRequestTryWrapping = function (window, move_flags) end,
	---NavUpdateCurrentWindowIsScrollPushableX()
    NavUpdateCurrentWindowIsScrollPushableX = function () end,
	---NewFrame()
    NewFrame = function () end,
	---NewLine()
    NewLine = function () end,
	---NextColumn()
    NextColumn = function () end,
	---OpenPopup(const char* str_id,ImGuiPopupFlags popup_flags=0)
	---@param str_id string
	---@param popup_flags ImGuiPopupFlags
    OpenPopup = function (str_id, popup_flags) end,
	---OpenPopup(ImGuiID id,ImGuiPopupFlags popup_flags=0)
	---@param id userdata
	---@param popup_flags ImGuiPopupFlags
    OpenPopup = function (id, popup_flags) end,
	---OpenPopupEx(ImGuiID id,ImGuiPopupFlags popup_flags=ImGuiPopupFlags_None)
	---@param id userdata
	---@param popup_flags ImGuiPopupFlags
    OpenPopupEx = function (id, popup_flags) end,
	---OpenPopupOnItemClick(const char* str_id=((void*)0),ImGuiPopupFlags popup_flags=1)
	---@param str_id string
	---@param popup_flags ImGuiPopupFlags
    OpenPopupOnItemClick = function (str_id, popup_flags) end,
	---PlotEx(ImGuiPlotType plot_type,const char* label,float(*values_getter)(void* data,int idx),void* data,int values_count,int values_offset,const char* overlay_text,float scale_min,float scale_max,const ImVec2& size_arg)
	---@param plot_type ImGuiPlotType
	---@param label string
	---@param values_getter userdata
	---@param data userdata
	---@param values_count integer
	---@param values_offset integer
	---@param overlay_text string
	---@param scale_min number
	---@param scale_max number
	---@param size_arg ImVec2
    ---@return integer
    PlotEx = function (plot_type, label, values_getter, data, values_count, values_offset, overlay_text, scale_min, scale_max, size_arg) end,
	---PlotHistogram(const char* label,const float* values,int values_count,int values_offset=0,const char* overlay_text=((void*)0),float scale_min=3.40282346638528859811704183484516925e+38F,float scale_max=3.40282346638528859811704183484516925e+38F,ImVec2 graph_size=ImVec2(0,0),int stride=sizeof(float))
	---@param label string
	---@param values userdata
	---@param values_count integer
	---@param values_offset integer
	---@param overlay_text string
	---@param scale_min number
	---@param scale_max number
	---@param graph_size ImVec2
	---@param stride integer
    PlotHistogram = function (label, values, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size, stride) end,
	---PlotHistogram(const char* label,float(*values_getter)(void* data,int idx),void* data,int values_count,int values_offset=0,const char* overlay_text=((void*)0),float scale_min=3.40282346638528859811704183484516925e+38F,float scale_max=3.40282346638528859811704183484516925e+38F,ImVec2 graph_size=ImVec2(0,0))
	---@param label string
	---@param values_getter userdata
	---@param data userdata
	---@param values_count integer
	---@param values_offset integer
	---@param overlay_text string
	---@param scale_min number
	---@param scale_max number
	---@param graph_size ImVec2
    PlotHistogram = function (label, values_getter, data, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size) end,
	---PlotLines(const char* label,const float* values,int values_count,int values_offset=0,const char* overlay_text=((void*)0),float scale_min=3.40282346638528859811704183484516925e+38F,float scale_max=3.40282346638528859811704183484516925e+38F,ImVec2 graph_size=ImVec2(0,0),int stride=sizeof(float))
	---@param label string
	---@param values userdata
	---@param values_count integer
	---@param values_offset integer
	---@param overlay_text string
	---@param scale_min number
	---@param scale_max number
	---@param graph_size ImVec2
	---@param stride integer
    PlotLines = function (label, values, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size, stride) end,
	---PlotLines(const char* label,float(*values_getter)(void* data,int idx),void* data,int values_count,int values_offset=0,const char* overlay_text=((void*)0),float scale_min=3.40282346638528859811704183484516925e+38F,float scale_max=3.40282346638528859811704183484516925e+38F,ImVec2 graph_size=ImVec2(0,0))
	---@param label string
	---@param values_getter userdata
	---@param data userdata
	---@param values_count integer
	---@param values_offset integer
	---@param overlay_text string
	---@param scale_min number
	---@param scale_max number
	---@param graph_size ImVec2
    PlotLines = function (label, values_getter, data, values_count, values_offset, overlay_text, scale_min, scale_max, graph_size) end,
	---PopClipRect()
    PopClipRect = function () end,
	---PopColumnsBackground()
    PopColumnsBackground = function () end,
	---PopFocusScope()
    PopFocusScope = function () end,
	---PopFont()
    PopFont = function () end,
	---PopID()
    PopID = function () end,
	---PopItemFlag()
    PopItemFlag = function () end,
	---PopItemWidth()
    PopItemWidth = function () end,
	---PopPasswordFont()
    PopPasswordFont = function () end,
	---PopStyleColor(int count=1)
	---@param count integer
    PopStyleColor = function (count) end,
	---PopStyleVar(int count=1)
	---@param count integer
    PopStyleVar = function (count) end,
	---PopTextWrapPos()
    PopTextWrapPos = function () end,
	---ProgressBar(float fraction,const ImVec2& size_arg=ImVec2(-1.17549435082228750796873653722224568e-38F,0),const char* overlay=((void*)0))
	---@param fraction number
	---@param size_arg ImVec2
	---@param overlay string
    ProgressBar = function (fraction, size_arg, overlay) end,
	---PushClipRect(const ImVec2& clip_rect_min,const ImVec2& clip_rect_max,bool intersect_with_current_clip_rect)
	---@param clip_rect_min ImVec2
	---@param clip_rect_max ImVec2
	---@param intersect_with_current_clip_rect boolean
    PushClipRect = function (clip_rect_min, clip_rect_max, intersect_with_current_clip_rect) end,
	---PushColumnClipRect(int column_index)
	---@param column_index integer
    PushColumnClipRect = function (column_index) end,
	---PushColumnsBackground()
    PushColumnsBackground = function () end,
	---PushFocusScope(ImGuiID id)
	---@param id userdata
    PushFocusScope = function (id) end,
	---PushFont(ImFont* font,float font_size_base_unscaled)
	---@param font ImFont
	---@param font_size_base_unscaled number
    PushFont = function (font, font_size_base_unscaled) end,
	---PushID(const char* str_id)
	---@param str_id string
    PushID = function (str_id) end,
	---PushID(const char* str_id_begin,const char* str_id_end)
	---@param str_id_begin string
	---@param str_id_end string
    PushID = function (str_id_begin, str_id_end) end,
	---PushID(const void* ptr_id)
	---@param ptr_id userdata
    PushID = function (ptr_id) end,
	---PushID(int int_id)
	---@param int_id integer
    PushID = function (int_id) end,
	---PushItemFlag(ImGuiItemFlags option,bool enabled)
	---@param option ImGuiItemFlags
	---@param enabled boolean
    PushItemFlag = function (option, enabled) end,
	---PushItemWidth(float item_width)
	---@param item_width number
    PushItemWidth = function (item_width) end,
	---PushMultiItemsWidths(int components,float width_full)
	---@param components integer
	---@param width_full number
    PushMultiItemsWidths = function (components, width_full) end,
	---PushOverrideID(ImGuiID id)
	---@param id userdata
    PushOverrideID = function (id) end,
	---PushPasswordFont()
    PushPasswordFont = function () end,
	---PushStyleColor(ImGuiCol idx,ImU32 col)
	---@param idx ImGuiCol
	---@param col integer
    PushStyleColor = function (idx, col) end,
	---PushStyleColor(ImGuiCol idx,const ImVec4& col)
	---@param idx ImGuiCol
	---@param col ImVec4
    PushStyleColor = function (idx, col) end,
	---PushStyleVar(ImGuiStyleVar idx,float val)
	---@param idx ImGuiStyleVar
	---@param val number
    PushStyleVar = function (idx, val) end,
	---PushStyleVar(ImGuiStyleVar idx,const ImVec2& val)
	---@param idx ImGuiStyleVar
	---@param val ImVec2
    PushStyleVar = function (idx, val) end,
	---PushStyleVarX(ImGuiStyleVar idx,float val_x)
	---@param idx ImGuiStyleVar
	---@param val_x number
    PushStyleVarX = function (idx, val_x) end,
	---PushStyleVarY(ImGuiStyleVar idx,float val_y)
	---@param idx ImGuiStyleVar
	---@param val_y number
    PushStyleVarY = function (idx, val_y) end,
	---PushTextWrapPos(float wrap_local_pos_x=0.0f)
	---@param wrap_local_pos_x number
    PushTextWrapPos = function (wrap_local_pos_x) end,
	---RadioButton(const char* label,bool active)
	---@param label string
	---@param active boolean
    ---@return boolean
    RadioButton = function (label, active) end,
	---RadioButton(const char* label,int* v,int v_button)
	---@param label string,
	---@param v integer|nil,
	---@param v_button integer
    ---@return boolean, integer
    RadioButton = function (label, v, v_button) end,
	---RegisterFontAtlas(ImFontAtlas* atlas)
	---@param atlas ImFontAtlas
    RegisterFontAtlas = function (atlas) end,
	---RegisterUserTexture(ImTextureData* tex)
	---@param tex ImTextureData
    RegisterUserTexture = function (tex) end,
	---RemoveContextHook(ImGuiContext* context,ImGuiID hook_to_remove)
	---@param context ImGuiContext
	---@param hook_to_remove userdata
    RemoveContextHook = function (context, hook_to_remove) end,
	---RemoveSettingsHandler(const char* type_name)
	---@param type_name string
    RemoveSettingsHandler = function (type_name) end,
	---Render()
    Render = function () end,
	---RenderArrow(ImDrawList* draw_list,ImVec2 pos,ImU32 col,ImGuiDir dir,float scale=1.0f)
	---@param draw_list ImDrawList
	---@param pos ImVec2
	---@param col integer
	---@param dir ImGuiDir
	---@param scale number
    RenderArrow = function (draw_list, pos, col, dir, scale) end,
	---RenderArrowPointingAt(ImDrawList* draw_list,ImVec2 pos,ImVec2 half_sz,ImGuiDir direction,ImU32 col)
	---@param draw_list ImDrawList
	---@param pos ImVec2
	---@param half_sz ImVec2
	---@param direction ImGuiDir
	---@param col integer
    RenderArrowPointingAt = function (draw_list, pos, half_sz, direction, col) end,
	---RenderBullet(ImDrawList* draw_list,ImVec2 pos,ImU32 col)
	---@param draw_list ImDrawList
	---@param pos ImVec2
	---@param col integer
    RenderBullet = function (draw_list, pos, col) end,
	---RenderCheckMark(ImDrawList* draw_list,ImVec2 pos,ImU32 col,float sz)
	---@param draw_list ImDrawList
	---@param pos ImVec2
	---@param col integer
	---@param sz number
    RenderCheckMark = function (draw_list, pos, col, sz) end,
	---RenderColorRectWithAlphaCheckerboard(ImDrawList* draw_list,ImVec2 p_min,ImVec2 p_max,ImU32 fill_col,float grid_step,ImVec2 grid_off,float rounding=0.0f,ImDrawFlags flags=0)
	---@param draw_list ImDrawList
	---@param p_min ImVec2
	---@param p_max ImVec2
	---@param fill_col integer
	---@param grid_step number
	---@param grid_off ImVec2
	---@param rounding number
	---@param flags ImDrawFlags
    RenderColorRectWithAlphaCheckerboard = function (draw_list, p_min, p_max, fill_col, grid_step, grid_off, rounding, flags) end,
	---RenderDragDropTargetRectEx(ImDrawList* draw_list,const ImRect& bb)
	---@param draw_list ImDrawList
	---@param bb ImRect
    RenderDragDropTargetRectEx = function (draw_list, bb) end,
	---RenderDragDropTargetRectForItem(const ImRect& bb)
	---@param bb ImRect
    RenderDragDropTargetRectForItem = function (bb) end,
	---RenderFrame(ImVec2 p_min,ImVec2 p_max,ImU32 fill_col,bool borders=true,float rounding=0.0f)
	---@param p_min ImVec2
	---@param p_max ImVec2
	---@param fill_col integer
	---@param borders boolean
	---@param rounding number
    RenderFrame = function (p_min, p_max, fill_col, borders, rounding) end,
	---RenderFrameBorder(ImVec2 p_min,ImVec2 p_max,float rounding=0.0f)
	---@param p_min ImVec2
	---@param p_max ImVec2
	---@param rounding number
    RenderFrameBorder = function (p_min, p_max, rounding) end,
	---RenderMouseCursor(ImVec2 pos,float scale,ImGuiMouseCursor mouse_cursor,ImU32 col_fill,ImU32 col_border,ImU32 col_shadow)
	---@param pos ImVec2
	---@param scale number
	---@param mouse_cursor ImGuiMouseCursor
	---@param col_fill integer
	---@param col_border integer
	---@param col_shadow integer
    RenderMouseCursor = function (pos, scale, mouse_cursor, col_fill, col_border, col_shadow) end,
	---RenderNavCursor(const ImRect& bb,ImGuiID id,ImGuiNavRenderCursorFlags flags=ImGuiNavRenderCursorFlags_None)
	---@param bb ImRect
	---@param id userdata
	---@param flags ImGuiNavRenderCursorFlags
    RenderNavCursor = function (bb, id, flags) end,
	---RenderRectFilledRangeH(ImDrawList* draw_list,const ImRect& rect,ImU32 col,float x_start_norm,float x_end_norm,float rounding)
	---@param draw_list ImDrawList
	---@param rect ImRect
	---@param col integer
	---@param x_start_norm number
	---@param x_end_norm number
	---@param rounding number
    RenderRectFilledRangeH = function (draw_list, rect, col, x_start_norm, x_end_norm, rounding) end,
	---RenderRectFilledWithHole(ImDrawList* draw_list,const ImRect& outer,const ImRect& inner,ImU32 col,float rounding)
	---@param draw_list ImDrawList
	---@param outer ImRect
	---@param inner ImRect
	---@param col integer
	---@param rounding number
    RenderRectFilledWithHole = function (draw_list, outer, inner, col, rounding) end,
	---RenderText(ImVec2 pos,const char* text,const char* text_end=((void*)0),bool hide_text_after_hash=true)
	---@param pos ImVec2
	---@param text string
	---@param text_end string
	---@param hide_text_after_hash boolean
    RenderText = function (pos, text, text_end, hide_text_after_hash) end,
	---RenderTextClipped(const ImVec2& pos_min,const ImVec2& pos_max,const char* text,const char* text_end,const ImVec2* text_size_if_known,const ImVec2& align=ImVec2(0,0),const ImRect* clip_rect=((void*)0))
	---@param pos_min ImVec2
	---@param pos_max ImVec2
	---@param text string
	---@param text_end string
	---@param text_size_if_known ImVec2
	---@param align ImVec2
	---@param clip_rect ImRect
    RenderTextClipped = function (pos_min, pos_max, text, text_end, text_size_if_known, align, clip_rect) end,
	---RenderTextClippedEx(ImDrawList* draw_list,const ImVec2& pos_min,const ImVec2& pos_max,const char* text,const char* text_end,const ImVec2* text_size_if_known,const ImVec2& align=ImVec2(0,0),const ImRect* clip_rect=((void*)0))
	---@param draw_list ImDrawList
	---@param pos_min ImVec2
	---@param pos_max ImVec2
	---@param text string
	---@param text_end string
	---@param text_size_if_known ImVec2
	---@param align ImVec2
	---@param clip_rect ImRect
    RenderTextClippedEx = function (draw_list, pos_min, pos_max, text, text_end, text_size_if_known, align, clip_rect) end,
	---RenderTextEllipsis(ImDrawList* draw_list,const ImVec2& pos_min,const ImVec2& pos_max,float ellipsis_max_x,const char* text,const char* text_end,const ImVec2* text_size_if_known)
	---@param draw_list ImDrawList
	---@param pos_min ImVec2
	---@param pos_max ImVec2
	---@param ellipsis_max_x number
	---@param text string
	---@param text_end string
	---@param text_size_if_known ImVec2
    RenderTextEllipsis = function (draw_list, pos_min, pos_max, ellipsis_max_x, text, text_end, text_size_if_known) end,
	---RenderTextWrapped(ImVec2 pos,const char* text,const char* text_end,float wrap_width)
	---@param pos ImVec2
	---@param text string
	---@param text_end string
	---@param wrap_width number
    RenderTextWrapped = function (pos, text, text_end, wrap_width) end,
	---ResetMouseDragDelta(ImGuiMouseButton button=0)
	---@param button ImGuiMouseButton
    ResetMouseDragDelta = function (button) end,
	---SameLine(float offset_from_start_x=0.0f,float spacing=-1.0f)
	---@param offset_from_start_x number
	---@param spacing number
    SameLine = function (offset_from_start_x, spacing) end,
	---SaveIniSettingsToDisk(const char* ini_filename)
	---@param ini_filename string
    SaveIniSettingsToDisk = function (ini_filename) end,
	---SaveIniSettingsToMemory(size_t* out_ini_size=((void*)0))
	---@param out_ini_size userdata
    ---@return string
    SaveIniSettingsToMemory = function (out_ini_size) end,
	---ScaleWindowsInViewport(ImGuiViewportP* viewport,float scale)
	---@param viewport ImGuiViewportP
	---@param scale number
    ScaleWindowsInViewport = function (viewport, scale) end,
	---ScrollToBringRectIntoView(ImGuiWindow* window,const ImRect& rect)
	---@param window ImGuiWindow
	---@param rect ImRect
    ScrollToBringRectIntoView = function (window, rect) end,
	---ScrollToItem(ImGuiScrollFlags flags=0)
	---@param flags ImGuiScrollFlags
    ScrollToItem = function (flags) end,
	---ScrollToRect(ImGuiWindow* window,const ImRect& rect,ImGuiScrollFlags flags=0)
	---@param window ImGuiWindow
	---@param rect ImRect
	---@param flags ImGuiScrollFlags
    ScrollToRect = function (window, rect, flags) end,
	---ScrollToRectEx(ImGuiWindow* window,const ImRect& rect,ImGuiScrollFlags flags=0)
	---@param window ImGuiWindow
	---@param rect ImRect
	---@param flags ImGuiScrollFlags
    ---@return userdata
    ScrollToRectEx = function (window, rect, flags) end,
	---Scrollbar(ImGuiAxis axis)
	---@param axis ImGuiAxis
    Scrollbar = function (axis) end,
	---ScrollbarEx(const ImRect& bb,ImGuiID id,ImGuiAxis axis,ImS64* p_scroll_v,ImS64 avail_v,ImS64 contents_v,ImDrawFlags draw_rounding_flags=0)
	---@param bb ImRect
	---@param id userdata
	---@param axis ImGuiAxis
	---@param p_scroll_v userdata
	---@param avail_v userdata
	---@param contents_v userdata
	---@param draw_rounding_flags ImDrawFlags
    ---@return boolean
    ScrollbarEx = function (bb, id, axis, p_scroll_v, avail_v, contents_v, draw_rounding_flags) end,
	---Selectable(const char* label,bool selected=false,ImGuiSelectableFlags flags=0,const ImVec2& size=ImVec2(0,0))
	---@param label string
	---@param selected boolean
	---@param flags ImGuiSelectableFlags
	---@param size ImVec2
    ---@return boolean
    Selectable = function (label, selected, flags, size) end,
	---Selectable(const char* label,bool* p_selected,ImGuiSelectableFlags flags=0,const ImVec2& size=ImVec2(0,0))
	---@param label string,
	---@param p_selected boolean|nil,
	---@param flags ImGuiSelectableFlags,
	---@param size ImVec2
    ---@return boolean, boolean
    Selectable = function (label, p_selected, flags, size) end,
	---Separator()
    Separator = function () end,
	---SeparatorEx(ImGuiSeparatorFlags flags,float thickness=1.0f)
	---@param flags ImGuiSeparatorFlags
	---@param thickness number
    SeparatorEx = function (flags, thickness) end,
	---SeparatorText(const char* label)
	---@param label string
    SeparatorText = function (label) end,
	---SeparatorTextEx(ImGuiID id,const char* label,const char* label_end,float extra_width)
	---@param id userdata
	---@param label string
	---@param label_end string
	---@param extra_width number
    SeparatorTextEx = function (id, label, label_end, extra_width) end,
	---SetActiveID(ImGuiID id,ImGuiWindow* window)
	---@param id userdata
	---@param window ImGuiWindow
    SetActiveID = function (id, window) end,
	---SetActiveIdUsingAllKeyboardKeys()
    SetActiveIdUsingAllKeyboardKeys = function () end,
	---SetAllocatorFunctions(ImGuiMemAllocFunc alloc_func,ImGuiMemFreeFunc free_func,void* user_data=((void*)0))
	---@param alloc_func userdata
	---@param free_func userdata
	---@param user_data userdata
    SetAllocatorFunctions = function (alloc_func, free_func, user_data) end,
	---SetClipboardText(const char* text)
	---@param text string
    SetClipboardText = function (text) end,
	---SetColorEditOptions(ImGuiColorEditFlags flags)
	---@param flags ImGuiColorEditFlags
    SetColorEditOptions = function (flags) end,
	---SetColumnOffset(int column_index,float offset_x)
	---@param column_index integer
	---@param offset_x number
    SetColumnOffset = function (column_index, offset_x) end,
	---SetColumnWidth(int column_index,float width)
	---@param column_index integer
	---@param width number
    SetColumnWidth = function (column_index, width) end,
	---SetCurrentContext(ImGuiContext* ctx)
	---@param ctx ImGuiContext
    SetCurrentContext = function (ctx) end,
	---SetCurrentFont(ImFont* font,float font_size_before_scaling,float font_size_after_scaling)
	---@param font ImFont
	---@param font_size_before_scaling number
	---@param font_size_after_scaling number
    SetCurrentFont = function (font, font_size_before_scaling, font_size_after_scaling) end,
	---SetCursorPos(const ImVec2& local_pos)
	---@param local_pos ImVec2
    SetCursorPos = function (local_pos) end,
	---SetCursorPosX(float local_x)
	---@param local_x number
    SetCursorPosX = function (local_x) end,
	---SetCursorPosY(float local_y)
	---@param local_y number
    SetCursorPosY = function (local_y) end,
	---SetCursorScreenPos(const ImVec2& pos)
	---@param pos ImVec2
    SetCursorScreenPos = function (pos) end,
	---SetDragDropPayload(const char* type,const void* data,size_t sz,ImGuiCond cond=0)
	---@param type string
	---@param data userdata
	---@param sz userdata
	---@param cond ImGuiCond
    ---@return boolean
    SetDragDropPayload = function (type, data, sz, cond) end,
	---SetFocusID(ImGuiID id,ImGuiWindow* window)
	---@param id userdata
	---@param window ImGuiWindow
    SetFocusID = function (id, window) end,
	---SetFontRasterizerDensity(float rasterizer_density)
	---@param rasterizer_density number
    SetFontRasterizerDensity = function (rasterizer_density) end,
	---SetHoveredID(ImGuiID id)
	---@param id userdata
    SetHoveredID = function (id) end,
	---SetItemDefaultFocus()
    SetItemDefaultFocus = function () end,
	---SetItemKeyOwner(ImGuiKey key)
	---@param key ImGuiKey
    SetItemKeyOwner = function (key) end,
	---SetItemKeyOwner(ImGuiKey key,ImGuiInputFlags flags)
	---@param key ImGuiKey
	---@param flags ImGuiInputFlags
    SetItemKeyOwner = function (key, flags) end,
	---SetItemTooltip(const char* fmt,...)
	---@param fmt string
	---@param ... any
    SetItemTooltip = function (fmt, ...) end,
	---SetItemTooltipV(const char* fmt,va_list args)
	---@param fmt string
	---@param args userdata
    SetItemTooltipV = function (fmt, args) end,
	---SetKeyOwner(ImGuiKey key,ImGuiID owner_id,ImGuiInputFlags flags=0)
	---@param key ImGuiKey
	---@param owner_id userdata
	---@param flags ImGuiInputFlags
    SetKeyOwner = function (key, owner_id, flags) end,
	---SetKeyOwnersForKeyChord(ImGuiKeyChord key,ImGuiID owner_id,ImGuiInputFlags flags=0)
	---@param key userdata
	---@param owner_id userdata
	---@param flags ImGuiInputFlags
    SetKeyOwnersForKeyChord = function (key, owner_id, flags) end,
	---SetKeyboardFocusHere(int offset=0)
	---@param offset integer
    SetKeyboardFocusHere = function (offset) end,
	---SetLastItemData(ImGuiID item_id,ImGuiItemFlags item_flags,ImGuiItemStatusFlags status_flags,const ImRect& item_rect)
	---@param item_id userdata
	---@param item_flags ImGuiItemFlags
	---@param status_flags ImGuiItemStatusFlags
	---@param item_rect ImRect
    SetLastItemData = function (item_id, item_flags, status_flags, item_rect) end,
	---SetMouseCursor(ImGuiMouseCursor cursor_type)
	---@param cursor_type ImGuiMouseCursor
    SetMouseCursor = function (cursor_type) end,
	---SetNavCursorVisible(bool visible)
	---@param visible boolean
    SetNavCursorVisible = function (visible) end,
	---SetNavCursorVisibleAfterMove()
    SetNavCursorVisibleAfterMove = function () end,
	---SetNavFocusScope(ImGuiID focus_scope_id)
	---@param focus_scope_id userdata
    SetNavFocusScope = function (focus_scope_id) end,
	---SetNavID(ImGuiID id,ImGuiNavLayer nav_layer,ImGuiID focus_scope_id,const ImRect& rect_rel)
	---@param id userdata
	---@param nav_layer ImGuiNavLayer
	---@param focus_scope_id userdata
	---@param rect_rel ImRect
    SetNavID = function (id, nav_layer, focus_scope_id, rect_rel) end,
	---SetNavWindow(ImGuiWindow* window)
	---@param window ImGuiWindow
    SetNavWindow = function (window) end,
	---SetNextFrameWantCaptureKeyboard(bool want_capture_keyboard)
	---@param want_capture_keyboard boolean
    SetNextFrameWantCaptureKeyboard = function (want_capture_keyboard) end,
	---SetNextFrameWantCaptureMouse(bool want_capture_mouse)
	---@param want_capture_mouse boolean
    SetNextFrameWantCaptureMouse = function (want_capture_mouse) end,
	---SetNextItemAllowOverlap()
    SetNextItemAllowOverlap = function () end,
	---SetNextItemOpen(bool is_open,ImGuiCond cond=0)
	---@param is_open boolean
	---@param cond ImGuiCond
    SetNextItemOpen = function (is_open, cond) end,
	---SetNextItemRefVal(ImGuiDataType data_type,void* p_data)
	---@param data_type ImGuiDataType
	---@param p_data userdata
    SetNextItemRefVal = function (data_type, p_data) end,
	---SetNextItemSelectionUserData(ImGuiSelectionUserData selection_user_data)
	---@param selection_user_data userdata
    SetNextItemSelectionUserData = function (selection_user_data) end,
	---SetNextItemShortcut(ImGuiKeyChord key_chord,ImGuiInputFlags flags=0)
	---@param key_chord userdata
	---@param flags ImGuiInputFlags
    SetNextItemShortcut = function (key_chord, flags) end,
	---SetNextItemStorageID(ImGuiID storage_id)
	---@param storage_id userdata
    SetNextItemStorageID = function (storage_id) end,
	---SetNextItemWidth(float item_width)
	---@param item_width number
    SetNextItemWidth = function (item_width) end,
	---SetNextWindowBgAlpha(float alpha)
	---@param alpha number
    SetNextWindowBgAlpha = function (alpha) end,
	---SetNextWindowCollapsed(bool collapsed,ImGuiCond cond=0)
	---@param collapsed boolean
	---@param cond ImGuiCond
    SetNextWindowCollapsed = function (collapsed, cond) end,
	---SetNextWindowContentSize(const ImVec2& size)
	---@param size ImVec2
    SetNextWindowContentSize = function (size) end,
	---SetNextWindowFocus()
    SetNextWindowFocus = function () end,
	---SetNextWindowPos(const ImVec2& pos,ImGuiCond cond=0,const ImVec2& pivot=ImVec2(0,0))
	---@param pos ImVec2
	---@param cond ImGuiCond
	---@param pivot ImVec2
    SetNextWindowPos = function (pos, cond, pivot) end,
	---SetNextWindowRefreshPolicy(ImGuiWindowRefreshFlags flags)
	---@param flags ImGuiWindowRefreshFlags
    SetNextWindowRefreshPolicy = function (flags) end,
	---SetNextWindowScroll(const ImVec2& scroll)
	---@param scroll ImVec2
    SetNextWindowScroll = function (scroll) end,
	---SetNextWindowSize(const ImVec2& size,ImGuiCond cond=0)
	---@param size ImVec2
	---@param cond ImGuiCond
    SetNextWindowSize = function (size, cond) end,
	---SetNextWindowSizeConstraints(const ImVec2& size_min,const ImVec2& size_max,ImGuiSizeCallback custom_callback=((void*)0),void* custom_callback_data=((void*)0))
	---@param size_min ImVec2
	---@param size_max ImVec2
	---@param custom_callback userdata
	---@param custom_callback_data userdata
    SetNextWindowSizeConstraints = function (size_min, size_max, custom_callback, custom_callback_data) end,
	---SetScrollFromPosX(float local_x,float center_x_ratio=0.5f)
	---@param local_x number
	---@param center_x_ratio number
    SetScrollFromPosX = function (local_x, center_x_ratio) end,
	---SetScrollFromPosX(ImGuiWindow* window,float local_x,float center_x_ratio)
	---@param window ImGuiWindow
	---@param local_x number
	---@param center_x_ratio number
    SetScrollFromPosX = function (window, local_x, center_x_ratio) end,
	---SetScrollFromPosY(float local_y,float center_y_ratio=0.5f)
	---@param local_y number
	---@param center_y_ratio number
    SetScrollFromPosY = function (local_y, center_y_ratio) end,
	---SetScrollFromPosY(ImGuiWindow* window,float local_y,float center_y_ratio)
	---@param window ImGuiWindow
	---@param local_y number
	---@param center_y_ratio number
    SetScrollFromPosY = function (window, local_y, center_y_ratio) end,
	---SetScrollHereX(float center_x_ratio=0.5f)
	---@param center_x_ratio number
    SetScrollHereX = function (center_x_ratio) end,
	---SetScrollHereY(float center_y_ratio=0.5f)
	---@param center_y_ratio number
    SetScrollHereY = function (center_y_ratio) end,
	---SetScrollX(float scroll_x)
	---@param scroll_x number
    SetScrollX = function (scroll_x) end,
	---SetScrollX(ImGuiWindow* window,float scroll_x)
	---@param window ImGuiWindow
	---@param scroll_x number
    SetScrollX = function (window, scroll_x) end,
	---SetScrollY(float scroll_y)
	---@param scroll_y number
    SetScrollY = function (scroll_y) end,
	---SetScrollY(ImGuiWindow* window,float scroll_y)
	---@param window ImGuiWindow
	---@param scroll_y number
    SetScrollY = function (window, scroll_y) end,
	---SetShortcutRouting(ImGuiKeyChord key_chord,ImGuiInputFlags flags,ImGuiID owner_id)
	---@param key_chord userdata
	---@param flags ImGuiInputFlags
	---@param owner_id userdata
    ---@return boolean
    SetShortcutRouting = function (key_chord, flags, owner_id) end,
	---SetStateStorage(ImGuiStorage* storage)
	---@param storage ImGuiStorage
    SetStateStorage = function (storage) end,
	---SetTabItemClosed(const char* tab_or_docked_window_label)
	---@param tab_or_docked_window_label string
    SetTabItemClosed = function (tab_or_docked_window_label) end,
	---SetTooltip(const char* fmt,...)
	---@param fmt string
	---@param ... any
    SetTooltip = function (fmt, ...) end,
	---SetTooltipV(const char* fmt,va_list args)
	---@param fmt string
	---@param args userdata
    SetTooltipV = function (fmt, args) end,
	---SetWindowClipRectBeforeSetChannel(ImGuiWindow* window,const ImRect& clip_rect)
	---@param window ImGuiWindow
	---@param clip_rect ImRect
    SetWindowClipRectBeforeSetChannel = function (window, clip_rect) end,
	---SetWindowCollapsed(bool collapsed,ImGuiCond cond=0)
	---@param collapsed boolean
	---@param cond ImGuiCond
    SetWindowCollapsed = function (collapsed, cond) end,
	---SetWindowCollapsed(const char* name,bool collapsed,ImGuiCond cond=0)
	---@param name string
	---@param collapsed boolean
	---@param cond ImGuiCond
    SetWindowCollapsed = function (name, collapsed, cond) end,
	---SetWindowCollapsed(ImGuiWindow* window,bool collapsed,ImGuiCond cond=0)
	---@param window ImGuiWindow
	---@param collapsed boolean
	---@param cond ImGuiCond
    SetWindowCollapsed = function (window, collapsed, cond) end,
	---SetWindowFocus()
    SetWindowFocus = function () end,
	---SetWindowFocus(const char* name)
	---@param name string
    SetWindowFocus = function (name) end,
	---SetWindowHiddenAndSkipItemsForCurrentFrame(ImGuiWindow* window)
	---@param window ImGuiWindow
    SetWindowHiddenAndSkipItemsForCurrentFrame = function (window) end,
	---SetWindowHitTestHole(ImGuiWindow* window,const ImVec2& pos,const ImVec2& size)
	---@param window ImGuiWindow
	---@param pos ImVec2
	---@param size ImVec2
    SetWindowHitTestHole = function (window, pos, size) end,
	---SetWindowParentWindowForFocusRoute(ImGuiWindow* window,ImGuiWindow* parent_window)
	---@param window ImGuiWindow
	---@param parent_window ImGuiWindow
    SetWindowParentWindowForFocusRoute = function (window, parent_window) end,
	---SetWindowPos(const ImVec2& pos,ImGuiCond cond=0)
	---@param pos ImVec2
	---@param cond ImGuiCond
    SetWindowPos = function (pos, cond) end,
	---SetWindowPos(const char* name,const ImVec2& pos,ImGuiCond cond=0)
	---@param name string
	---@param pos ImVec2
	---@param cond ImGuiCond
    SetWindowPos = function (name, pos, cond) end,
	---SetWindowPos(ImGuiWindow* window,const ImVec2& pos,ImGuiCond cond=0)
	---@param window ImGuiWindow
	---@param pos ImVec2
	---@param cond ImGuiCond
    SetWindowPos = function (window, pos, cond) end,
	---SetWindowSize(const ImVec2& size,ImGuiCond cond=0)
	---@param size ImVec2
	---@param cond ImGuiCond
    SetWindowSize = function (size, cond) end,
	---SetWindowSize(const char* name,const ImVec2& size,ImGuiCond cond=0)
	---@param name string
	---@param size ImVec2
	---@param cond ImGuiCond
    SetWindowSize = function (name, size, cond) end,
	---SetWindowSize(ImGuiWindow* window,const ImVec2& size,ImGuiCond cond=0)
	---@param window ImGuiWindow
	---@param size ImVec2
	---@param cond ImGuiCond
    SetWindowSize = function (window, size, cond) end,
	---SetWindowViewport(ImGuiWindow* window,ImGuiViewportP* viewport)
	---@param window ImGuiWindow
	---@param viewport ImGuiViewportP
    SetWindowViewport = function (window, viewport) end,
	---ShadeVertsLinearColorGradientKeepAlpha(ImDrawList* draw_list,int vert_start_idx,int vert_end_idx,ImVec2 gradient_p0,ImVec2 gradient_p1,ImU32 col0,ImU32 col1)
	---@param draw_list ImDrawList
	---@param vert_start_idx integer
	---@param vert_end_idx integer
	---@param gradient_p0 ImVec2
	---@param gradient_p1 ImVec2
	---@param col0 integer
	---@param col1 integer
    ShadeVertsLinearColorGradientKeepAlpha = function (draw_list, vert_start_idx, vert_end_idx, gradient_p0, gradient_p1, col0, col1) end,
	---ShadeVertsLinearUV(ImDrawList* draw_list,int vert_start_idx,int vert_end_idx,const ImVec2& a,const ImVec2& b,const ImVec2& uv_a,const ImVec2& uv_b,bool clamp)
	---@param draw_list ImDrawList
	---@param vert_start_idx integer
	---@param vert_end_idx integer
	---@param a ImVec2
	---@param b ImVec2
	---@param uv_a ImVec2
	---@param uv_b ImVec2
	---@param clamp boolean
    ShadeVertsLinearUV = function (draw_list, vert_start_idx, vert_end_idx, a, b, uv_a, uv_b, clamp) end,
	---ShadeVertsTransformPos(ImDrawList* draw_list,int vert_start_idx,int vert_end_idx,const ImVec2& pivot_in,float cos_a,float sin_a,const ImVec2& pivot_out)
	---@param draw_list ImDrawList
	---@param vert_start_idx integer
	---@param vert_end_idx integer
	---@param pivot_in ImVec2
	---@param cos_a number
	---@param sin_a number
	---@param pivot_out ImVec2
    ShadeVertsTransformPos = function (draw_list, vert_start_idx, vert_end_idx, pivot_in, cos_a, sin_a, pivot_out) end,
	---Shortcut(ImGuiKeyChord key_chord,ImGuiInputFlags flags=0)
	---@param key_chord userdata
	---@param flags ImGuiInputFlags
    ---@return boolean
    Shortcut = function (key_chord, flags) end,
	---Shortcut(ImGuiKeyChord key_chord,ImGuiInputFlags flags,ImGuiID owner_id)
	---@param key_chord userdata
	---@param flags ImGuiInputFlags
	---@param owner_id userdata
    ---@return boolean
    Shortcut = function (key_chord, flags, owner_id) end,
	---ShowAboutWindow(bool* p_open=((void*)0))
	---@param p_open boolean|nil
    ---@return boolean
    ShowAboutWindow = function (p_open) end,
	---ShowDebugLogWindow(bool* p_open=((void*)0))
	---@param p_open boolean|nil
    ---@return boolean
    ShowDebugLogWindow = function (p_open) end,
	---ShowDemoWindow(bool* p_open=((void*)0))
	---@param p_open boolean|nil
    ---@return boolean
    ShowDemoWindow = function (p_open) end,
	---ShowFontAtlas(ImFontAtlas* atlas)
	---@param atlas ImFontAtlas
    ShowFontAtlas = function (atlas) end,
	---ShowFontSelector(const char* label)
	---@param label string
    ShowFontSelector = function (label) end,
	---ShowIDStackToolWindow(bool* p_open=((void*)0))
	---@param p_open boolean|nil
    ---@return boolean
    ShowIDStackToolWindow = function (p_open) end,
	---ShowMetricsWindow(bool* p_open=((void*)0))
	---@param p_open boolean|nil
    ---@return boolean
    ShowMetricsWindow = function (p_open) end,
	---ShowStyleEditor(ImGuiStyle* ref=((void*)0))
	---@param ref ImGuiStyle
    ShowStyleEditor = function (ref) end,
	---ShowStyleSelector(const char* label)
	---@param label string
    ---@return boolean
    ShowStyleSelector = function (label) end,
	---ShowUserGuide()
    ShowUserGuide = function () end,
	---ShrinkWidths(ImGuiShrinkWidthItem* items,int count,float width_excess,float width_min)
	---@param items ImGuiShrinkWidthItem
	---@param count integer
	---@param width_excess number
	---@param width_min number
    ShrinkWidths = function (items, count, width_excess, width_min) end,
	---Shutdown()
    Shutdown = function () end,
	---SliderAngle(const char* label,float* v_rad,float v_degrees_min=-360.0f,float v_degrees_max=+360.0f,const char* format="%.0f deg",ImGuiSliderFlags flags=0)
	---@param label string,
	---@param v_rad number|nil,
	---@param v_degrees_min number,
	---@param v_degrees_max number,
	---@param format string,
	---@param flags ImGuiSliderFlags
    ---@return boolean, number
    SliderAngle = function (label, v_rad, v_degrees_min, v_degrees_max, format, flags) end,
	---SliderBehavior(const ImRect& bb,ImGuiID id,ImGuiDataType data_type,void* p_v,const void* p_min,const void* p_max,const char* format,ImGuiSliderFlags flags,ImRect* out_grab_bb)
	---@param bb ImRect
	---@param id userdata
	---@param data_type ImGuiDataType
	---@param p_v userdata
	---@param p_min userdata
	---@param p_max userdata
	---@param format string
	---@param flags ImGuiSliderFlags
	---@param out_grab_bb ImRect
    ---@return boolean
    SliderBehavior = function (bb, id, data_type, p_v, p_min, p_max, format, flags, out_grab_bb) end,
	---SliderFloat(const char* label,float* v,float v_min,float v_max,const char* format="%.3f",ImGuiSliderFlags flags=0)
	---@param label string,
	---@param v number|nil,
	---@param v_min number,
	---@param v_max number,
	---@param format string,
	---@param flags ImGuiSliderFlags
    ---@return boolean, number
    SliderFloat = function (label, v, v_min, v_max, format, flags) end,
	---SliderFloat2(const char* label,float v[2],float v_min,float v_max,const char* format="%.3f",ImGuiSliderFlags flags=0)
	---@param label string
	---@param v userdata
	---@param v_min number
	---@param v_max number
	---@param format string
	---@param flags ImGuiSliderFlags
    ---@return boolean
    SliderFloat2 = function (label, v, v_min, v_max, format, flags) end,
	---SliderFloat3(const char* label,float v[3],float v_min,float v_max,const char* format="%.3f",ImGuiSliderFlags flags=0)
	---@param label string
	---@param v userdata
	---@param v_min number
	---@param v_max number
	---@param format string
	---@param flags ImGuiSliderFlags
    ---@return boolean
    SliderFloat3 = function (label, v, v_min, v_max, format, flags) end,
	---SliderFloat4(const char* label,float v[4],float v_min,float v_max,const char* format="%.3f",ImGuiSliderFlags flags=0)
	---@param label string
	---@param v userdata
	---@param v_min number
	---@param v_max number
	---@param format string
	---@param flags ImGuiSliderFlags
    ---@return boolean
    SliderFloat4 = function (label, v, v_min, v_max, format, flags) end,
	---SliderInt(const char* label,int* v,int v_min,int v_max,const char* format="%d",ImGuiSliderFlags flags=0)
	---@param label string,
	---@param v integer|nil,
	---@param v_min integer,
	---@param v_max integer,
	---@param format string,
	---@param flags ImGuiSliderFlags
    ---@return boolean, integer
    SliderInt = function (label, v, v_min, v_max, format, flags) end,
	---SliderInt2(const char* label,int v[2],int v_min,int v_max,const char* format="%d",ImGuiSliderFlags flags=0)
	---@param label string
	---@param v userdata
	---@param v_min integer
	---@param v_max integer
	---@param format string
	---@param flags ImGuiSliderFlags
    ---@return boolean
    SliderInt2 = function (label, v, v_min, v_max, format, flags) end,
	---SliderInt3(const char* label,int v[3],int v_min,int v_max,const char* format="%d",ImGuiSliderFlags flags=0)
	---@param label string
	---@param v userdata
	---@param v_min integer
	---@param v_max integer
	---@param format string
	---@param flags ImGuiSliderFlags
    ---@return boolean
    SliderInt3 = function (label, v, v_min, v_max, format, flags) end,
	---SliderInt4(const char* label,int v[4],int v_min,int v_max,const char* format="%d",ImGuiSliderFlags flags=0)
	---@param label string
	---@param v userdata
	---@param v_min integer
	---@param v_max integer
	---@param format string
	---@param flags ImGuiSliderFlags
    ---@return boolean
    SliderInt4 = function (label, v, v_min, v_max, format, flags) end,
	---SliderScalar(const char* label,ImGuiDataType data_type,void* p_data,const void* p_min,const void* p_max,const char* format=((void*)0),ImGuiSliderFlags flags=0)
	---@param label string
	---@param data_type ImGuiDataType
	---@param p_data userdata
	---@param p_min userdata
	---@param p_max userdata
	---@param format string
	---@param flags ImGuiSliderFlags
    ---@return boolean
    SliderScalar = function (label, data_type, p_data, p_min, p_max, format, flags) end,
	---SliderScalarN(const char* label,ImGuiDataType data_type,void* p_data,int components,const void* p_min,const void* p_max,const char* format=((void*)0),ImGuiSliderFlags flags=0)
	---@param label string
	---@param data_type ImGuiDataType
	---@param p_data userdata
	---@param components integer
	---@param p_min userdata
	---@param p_max userdata
	---@param format string
	---@param flags ImGuiSliderFlags
    ---@return boolean
    SliderScalarN = function (label, data_type, p_data, components, p_min, p_max, format, flags) end,
	---SmallButton(const char* label)
	---@param label string
    ---@return boolean
    SmallButton = function (label) end,
	---Spacing()
    Spacing = function () end,
	---SplitterBehavior(const ImRect& bb,ImGuiID id,ImGuiAxis axis,float* size1,float* size2,float min_size1,float min_size2,float hover_extend=0.0f,float hover_visibility_delay=0.0f,ImU32 bg_col=0)
	---@param bb ImRect,
	---@param id userdata,
	---@param axis ImGuiAxis,
	---@param size1 number|nil,
	---@param size2 number|nil,
	---@param min_size1 number,
	---@param min_size2 number,
	---@param hover_extend number,
	---@param hover_visibility_delay number,
	---@param bg_col integer
    ---@return boolean, number, number
    SplitterBehavior = function (bb, id, axis, size1, size2, min_size1, min_size2, hover_extend, hover_visibility_delay, bg_col) end,
	---StartMouseMovingWindow(ImGuiWindow* window)
	---@param window ImGuiWindow
    StartMouseMovingWindow = function (window) end,
	---StopMouseMovingWindow()
    StopMouseMovingWindow = function () end,
	---StyleColorsClassic(ImGuiStyle* dst=((void*)0))
	---@param dst ImGuiStyle
    StyleColorsClassic = function (dst) end,
	---StyleColorsDark(ImGuiStyle* dst=((void*)0))
	---@param dst ImGuiStyle
    StyleColorsDark = function (dst) end,
	---StyleColorsLight(ImGuiStyle* dst=((void*)0))
	---@param dst ImGuiStyle
    StyleColorsLight = function (dst) end,
	---TabBarCloseTab(ImGuiTabBar* tab_bar,ImGuiTabItem* tab)
	---@param tab_bar ImGuiTabBar
	---@param tab ImGuiTabItem
    TabBarCloseTab = function (tab_bar, tab) end,
	---TabBarFindByID(ImGuiID id)
	---@param id userdata
    ---@return ImGuiTabBar
    TabBarFindByID = function (id) end,
	---TabBarFindTabByID(ImGuiTabBar* tab_bar,ImGuiID tab_id)
	---@param tab_bar ImGuiTabBar
	---@param tab_id userdata
    ---@return ImGuiTabItem
    TabBarFindTabByID = function (tab_bar, tab_id) end,
	---TabBarFindTabByOrder(ImGuiTabBar* tab_bar,int order)
	---@param tab_bar ImGuiTabBar
	---@param order integer
    ---@return ImGuiTabItem
    TabBarFindTabByOrder = function (tab_bar, order) end,
	---TabBarGetCurrentTab(ImGuiTabBar* tab_bar)
	---@param tab_bar ImGuiTabBar
    ---@return ImGuiTabItem
    TabBarGetCurrentTab = function (tab_bar) end,
	---TabBarGetTabName(ImGuiTabBar* tab_bar,ImGuiTabItem* tab)
	---@param tab_bar ImGuiTabBar
	---@param tab ImGuiTabItem
    ---@return string
    TabBarGetTabName = function (tab_bar, tab) end,
	---TabBarGetTabOrder(ImGuiTabBar* tab_bar,ImGuiTabItem* tab)
	---@param tab_bar ImGuiTabBar
	---@param tab ImGuiTabItem
    ---@return integer
    TabBarGetTabOrder = function (tab_bar, tab) end,
	---TabBarProcessReorder(ImGuiTabBar* tab_bar)
	---@param tab_bar ImGuiTabBar
    ---@return boolean
    TabBarProcessReorder = function (tab_bar) end,
	---TabBarQueueFocus(ImGuiTabBar* tab_bar,ImGuiTabItem* tab)
	---@param tab_bar ImGuiTabBar
	---@param tab ImGuiTabItem
    TabBarQueueFocus = function (tab_bar, tab) end,
	---TabBarQueueFocus(ImGuiTabBar* tab_bar,const char* tab_name)
	---@param tab_bar ImGuiTabBar
	---@param tab_name string
    TabBarQueueFocus = function (tab_bar, tab_name) end,
	---TabBarQueueReorder(ImGuiTabBar* tab_bar,ImGuiTabItem* tab,int offset)
	---@param tab_bar ImGuiTabBar
	---@param tab ImGuiTabItem
	---@param offset integer
    TabBarQueueReorder = function (tab_bar, tab, offset) end,
	---TabBarQueueReorderFromMousePos(ImGuiTabBar* tab_bar,ImGuiTabItem* tab,ImVec2 mouse_pos)
	---@param tab_bar ImGuiTabBar
	---@param tab ImGuiTabItem
	---@param mouse_pos ImVec2
    TabBarQueueReorderFromMousePos = function (tab_bar, tab, mouse_pos) end,
	---TabBarRemove(ImGuiTabBar* tab_bar)
	---@param tab_bar ImGuiTabBar
    TabBarRemove = function (tab_bar) end,
	---TabBarRemoveTab(ImGuiTabBar* tab_bar,ImGuiID tab_id)
	---@param tab_bar ImGuiTabBar
	---@param tab_id userdata
    TabBarRemoveTab = function (tab_bar, tab_id) end,
	---TabItemBackground(ImDrawList* draw_list,const ImRect& bb,ImGuiTabItemFlags flags,ImU32 col)
	---@param draw_list ImDrawList
	---@param bb ImRect
	---@param flags ImGuiTabItemFlags
	---@param col integer
    TabItemBackground = function (draw_list, bb, flags, col) end,
	---TabItemButton(const char* label,ImGuiTabItemFlags flags=0)
	---@param label string
	---@param flags ImGuiTabItemFlags
    ---@return boolean
    TabItemButton = function (label, flags) end,
	---TabItemCalcSize(const char* label,bool has_close_button_or_unsaved_marker)
	---@param label string
	---@param has_close_button_or_unsaved_marker boolean
    ---@return userdata
    TabItemCalcSize = function (label, has_close_button_or_unsaved_marker) end,
	---TabItemCalcSize(ImGuiWindow* window)
	---@param window ImGuiWindow
    ---@return userdata
    TabItemCalcSize = function (window) end,
	---TabItemEx(ImGuiTabBar* tab_bar,const char* label,bool* p_open,ImGuiTabItemFlags flags,ImGuiWindow* docked_window)
	---@param tab_bar ImGuiTabBar,
	---@param label string,
	---@param p_open boolean|nil,
	---@param flags ImGuiTabItemFlags,
	---@param docked_window ImGuiWindow
    ---@return boolean, boolean
    TabItemEx = function (tab_bar, label, p_open, flags, docked_window) end,
	---TabItemLabelAndCloseButton(ImDrawList* draw_list,const ImRect& bb,ImGuiTabItemFlags flags,ImVec2 frame_padding,const char* label,ImGuiID tab_id,ImGuiID close_button_id,bool is_contents_visible,bool* out_just_closed,bool* out_text_clipped)
	---@param draw_list ImDrawList,
	---@param bb ImRect,
	---@param flags ImGuiTabItemFlags,
	---@param frame_padding ImVec2,
	---@param label string,
	---@param tab_id userdata,
	---@param close_button_id userdata,
	---@param is_contents_visible boolean,
	---@param out_just_closed boolean|nil,
	---@param out_text_clipped boolean|nil
    ---@return boolean, boolean
    TabItemLabelAndCloseButton = function (draw_list, bb, flags, frame_padding, label, tab_id, close_button_id, is_contents_visible, out_just_closed, out_text_clipped) end,
	---TabItemSpacing(const char* str_id,ImGuiTabItemFlags flags,float width)
	---@param str_id string
	---@param flags ImGuiTabItemFlags
	---@param width number
    TabItemSpacing = function (str_id, flags, width) end,
	---TableAngledHeadersRow()
    TableAngledHeadersRow = function () end,
	---TableAngledHeadersRowEx(ImGuiID row_id,float angle,float max_label_width,const ImGuiTableHeaderData* data,int data_count)
	---@param row_id userdata
	---@param angle number
	---@param max_label_width number
	---@param data ImGuiTableHeaderData
	---@param data_count integer
    TableAngledHeadersRowEx = function (row_id, angle, max_label_width, data, data_count) end,
	---TableBeginApplyRequests(ImGuiTable* table)
	---@param table ImGuiTable
    TableBeginApplyRequests = function (table) end,
	---TableBeginCell(ImGuiTable* table,int column_n)
	---@param table ImGuiTable
	---@param column_n integer
    TableBeginCell = function (table, column_n) end,
	---TableBeginContextMenuPopup(ImGuiTable* table)
	---@param table ImGuiTable
    ---@return boolean
    TableBeginContextMenuPopup = function (table) end,
	---TableBeginInitMemory(ImGuiTable* table,int columns_count)
	---@param table ImGuiTable
	---@param columns_count integer
    TableBeginInitMemory = function (table, columns_count) end,
	---TableBeginRow(ImGuiTable* table)
	---@param table ImGuiTable
    TableBeginRow = function (table) end,
	---TableCalcMaxColumnWidth(const ImGuiTable* table,int column_n)
	---@param table ImGuiTable
	---@param column_n integer
    ---@return number
    TableCalcMaxColumnWidth = function (table, column_n) end,
	---TableDrawBorders(ImGuiTable* table)
	---@param table ImGuiTable
    TableDrawBorders = function (table) end,
	---TableDrawDefaultContextMenu(ImGuiTable* table,ImGuiTableFlags flags_for_section_to_display)
	---@param table ImGuiTable
	---@param flags_for_section_to_display ImGuiTableFlags
    TableDrawDefaultContextMenu = function (table, flags_for_section_to_display) end,
	---TableEndCell(ImGuiTable* table)
	---@param table ImGuiTable
    TableEndCell = function (table) end,
	---TableEndRow(ImGuiTable* table)
	---@param table ImGuiTable
    TableEndRow = function (table) end,
	---TableFindByID(ImGuiID id)
	---@param id userdata
    ---@return ImGuiTable
    TableFindByID = function (id) end,
	---TableFixColumnSortDirection(ImGuiTable* table,ImGuiTableColumn* column)
	---@param table ImGuiTable
	---@param column ImGuiTableColumn
    TableFixColumnSortDirection = function (table, column) end,
	---TableGcCompactSettings()
    TableGcCompactSettings = function () end,
	---TableGcCompactTransientBuffers(ImGuiTable* table)
	---@param table ImGuiTable
    TableGcCompactTransientBuffers = function (table) end,
	---TableGcCompactTransientBuffers(ImGuiTableTempData* table)
	---@param table ImGuiTableTempData
    TableGcCompactTransientBuffers = function (table) end,
	---TableGetBoundSettings(ImGuiTable* table)
	---@param table ImGuiTable
    ---@return ImGuiTableSettings
    TableGetBoundSettings = function (table) end,
	---TableGetCellBgRect(const ImGuiTable* table,int column_n)
	---@param table ImGuiTable
	---@param column_n integer
    ---@return userdata
    TableGetCellBgRect = function (table, column_n) end,
	---TableGetColumnCount()
    ---@return integer
    TableGetColumnCount = function () end,
	---TableGetColumnFlags(int column_n=-1)
	---@param column_n integer
    ---@return ImGuiTableColumnFlags
    TableGetColumnFlags = function (column_n) end,
	---TableGetColumnIndex()
    ---@return integer
    TableGetColumnIndex = function () end,
	---TableGetColumnName(int column_n=-1)
	---@param column_n integer
    ---@return string
    TableGetColumnName = function (column_n) end,
	---TableGetColumnName(const ImGuiTable* table,int column_n)
	---@param table ImGuiTable
	---@param column_n integer
    ---@return string
    TableGetColumnName = function (table, column_n) end,
	---TableGetColumnNextSortDirection(ImGuiTableColumn* column)
	---@param column ImGuiTableColumn
    ---@return ImGuiSortDirection
    TableGetColumnNextSortDirection = function (column) end,
	---TableGetColumnResizeID(ImGuiTable* table,int column_n,int instance_no=0)
	---@param table ImGuiTable
	---@param column_n integer
	---@param instance_no integer
    ---@return userdata
    TableGetColumnResizeID = function (table, column_n, instance_no) end,
	---TableGetColumnWidthAuto(ImGuiTable* table,ImGuiTableColumn* column)
	---@param table ImGuiTable
	---@param column ImGuiTableColumn
    ---@return number
    TableGetColumnWidthAuto = function (table, column) end,
	---TableGetHeaderAngledMaxLabelWidth()
    ---@return number
    TableGetHeaderAngledMaxLabelWidth = function () end,
	---TableGetHeaderRowHeight()
    ---@return number
    TableGetHeaderRowHeight = function () end,
	---TableGetHoveredColumn()
    ---@return integer
    TableGetHoveredColumn = function () end,
	---TableGetHoveredRow()
    ---@return integer
    TableGetHoveredRow = function () end,
	---TableGetInstanceData(ImGuiTable* table,int instance_no)
	---@param table ImGuiTable
	---@param instance_no integer
    ---@return ImGuiTableInstanceData
    TableGetInstanceData = function (table, instance_no) end,
	---TableGetInstanceID(ImGuiTable* table,int instance_no)
	---@param table ImGuiTable
	---@param instance_no integer
    ---@return userdata
    TableGetInstanceID = function (table, instance_no) end,
	---TableGetRowIndex()
    ---@return integer
    TableGetRowIndex = function () end,
	---TableGetSortSpecs()
    ---@return ImGuiTableSortSpecs
    TableGetSortSpecs = function () end,
	---TableHeader(const char* label)
	---@param label string
    TableHeader = function (label) end,
	---TableHeadersRow()
    TableHeadersRow = function () end,
	---TableLoadSettings(ImGuiTable* table)
	---@param table ImGuiTable
    TableLoadSettings = function (table) end,
	---TableMergeDrawChannels(ImGuiTable* table)
	---@param table ImGuiTable
    TableMergeDrawChannels = function (table) end,
	---TableNextColumn()
    ---@return boolean
    TableNextColumn = function () end,
	---TableNextRow(ImGuiTableRowFlags row_flags=0,float min_row_height=0.0f)
	---@param row_flags ImGuiTableRowFlags
	---@param min_row_height number
    TableNextRow = function (row_flags, min_row_height) end,
	---TableOpenContextMenu(int column_n=-1)
	---@param column_n integer
    TableOpenContextMenu = function (column_n) end,
	---TablePopBackgroundChannel()
    TablePopBackgroundChannel = function () end,
	---TablePopColumnChannel()
    TablePopColumnChannel = function () end,
	---TablePushBackgroundChannel()
    TablePushBackgroundChannel = function () end,
	---TablePushColumnChannel(int column_n)
	---@param column_n integer
    TablePushColumnChannel = function (column_n) end,
	---TableRemove(ImGuiTable* table)
	---@param table ImGuiTable
    TableRemove = function (table) end,
	---TableResetSettings(ImGuiTable* table)
	---@param table ImGuiTable
    TableResetSettings = function (table) end,
	---TableSaveSettings(ImGuiTable* table)
	---@param table ImGuiTable
    TableSaveSettings = function (table) end,
	---TableSetBgColor(ImGuiTableBgTarget target,ImU32 color,int column_n=-1)
	---@param target ImGuiTableBgTarget
	---@param color integer
	---@param column_n integer
    TableSetBgColor = function (target, color, column_n) end,
	---TableSetColumnEnabled(int column_n,bool v)
	---@param column_n integer
	---@param v boolean
    TableSetColumnEnabled = function (column_n, v) end,
	---TableSetColumnIndex(int column_n)
	---@param column_n integer
    ---@return boolean
    TableSetColumnIndex = function (column_n) end,
	---TableSetColumnSortDirection(int column_n,ImGuiSortDirection sort_direction,bool append_to_sort_specs)
	---@param column_n integer
	---@param sort_direction ImGuiSortDirection
	---@param append_to_sort_specs boolean
    TableSetColumnSortDirection = function (column_n, sort_direction, append_to_sort_specs) end,
	---TableSetColumnWidth(int column_n,float width)
	---@param column_n integer
	---@param width number
    TableSetColumnWidth = function (column_n, width) end,
	---TableSetColumnWidthAutoAll(ImGuiTable* table)
	---@param table ImGuiTable
    TableSetColumnWidthAutoAll = function (table) end,
	---TableSetColumnWidthAutoSingle(ImGuiTable* table,int column_n)
	---@param table ImGuiTable
	---@param column_n integer
    TableSetColumnWidthAutoSingle = function (table, column_n) end,
	---TableSettingsAddSettingsHandler()
    TableSettingsAddSettingsHandler = function () end,
	---TableSettingsCreate(ImGuiID id,int columns_count)
	---@param id userdata
	---@param columns_count integer
    ---@return ImGuiTableSettings
    TableSettingsCreate = function (id, columns_count) end,
	---TableSettingsFindByID(ImGuiID id)
	---@param id userdata
    ---@return ImGuiTableSettings
    TableSettingsFindByID = function (id) end,
	---TableSetupColumn(const char* label,ImGuiTableColumnFlags flags=0,float init_width_or_weight=0.0f,ImGuiID user_id=0)
	---@param label string
	---@param flags ImGuiTableColumnFlags
	---@param init_width_or_weight number
	---@param user_id userdata
    TableSetupColumn = function (label, flags, init_width_or_weight, user_id) end,
	---TableSetupDrawChannels(ImGuiTable* table)
	---@param table ImGuiTable
    TableSetupDrawChannels = function (table) end,
	---TableSetupScrollFreeze(int cols,int rows)
	---@param cols integer
	---@param rows integer
    TableSetupScrollFreeze = function (cols, rows) end,
	---TableSortSpecsBuild(ImGuiTable* table)
	---@param table ImGuiTable
    TableSortSpecsBuild = function (table) end,
	---TableSortSpecsSanitize(ImGuiTable* table)
	---@param table ImGuiTable
    TableSortSpecsSanitize = function (table) end,
	---TableUpdateBorders(ImGuiTable* table)
	---@param table ImGuiTable
    TableUpdateBorders = function (table) end,
	---TableUpdateColumnsWeightFromWidth(ImGuiTable* table)
	---@param table ImGuiTable
    TableUpdateColumnsWeightFromWidth = function (table) end,
	---TableUpdateLayout(ImGuiTable* table)
	---@param table ImGuiTable
    TableUpdateLayout = function (table) end,
	---TeleportMousePos(const ImVec2& pos)
	---@param pos ImVec2
    TeleportMousePos = function (pos) end,
	---TempInputIsActive(ImGuiID id)
	---@param id userdata
    ---@return boolean
    TempInputIsActive = function (id) end,
	---TempInputScalar(const ImRect& bb,ImGuiID id,const char* label,ImGuiDataType data_type,void* p_data,const char* format,const void* p_clamp_min=((void*)0),const void* p_clamp_max=((void*)0))
	---@param bb ImRect
	---@param id userdata
	---@param label string
	---@param data_type ImGuiDataType
	---@param p_data userdata
	---@param format string
	---@param p_clamp_min userdata
	---@param p_clamp_max userdata
    ---@return boolean
    TempInputScalar = function (bb, id, label, data_type, p_data, format, p_clamp_min, p_clamp_max) end,
	---TempInputText(const ImRect& bb,ImGuiID id,const char* label,char* buf,int buf_size,ImGuiInputTextFlags flags)
	---@param bb ImRect
	---@param id userdata
	---@param label string
	---@param buf userdata
	---@param buf_size integer
	---@param flags ImGuiInputTextFlags
    ---@return boolean
    TempInputText = function (bb, id, label, buf, buf_size, flags) end,
	---TestKeyOwner(ImGuiKey key,ImGuiID owner_id)
	---@param key ImGuiKey
	---@param owner_id userdata
    ---@return boolean
    TestKeyOwner = function (key, owner_id) end,
	---TestShortcutRouting(ImGuiKeyChord key_chord,ImGuiID owner_id)
	---@param key_chord userdata
	---@param owner_id userdata
    ---@return boolean
    TestShortcutRouting = function (key_chord, owner_id) end,
	---Text(const char* fmt,...)
	---@param fmt string
	---@param ... any
    Text = function (fmt, ...) end,
	---TextAligned(float align_x,float size_x,const char* fmt,...)
	---@param align_x number
	---@param size_x number
	---@param fmt string
	---@param ... any
    TextAligned = function (align_x, size_x, fmt, ...) end,
	---TextAlignedV(float align_x,float size_x,const char* fmt,va_list args)
	---@param align_x number
	---@param size_x number
	---@param fmt string
	---@param args userdata
    TextAlignedV = function (align_x, size_x, fmt, args) end,
	---TextColored(const ImVec4& col,const char* fmt,...)
	---@param col ImVec4
	---@param fmt string
	---@param ... any
    TextColored = function (col, fmt, ...) end,
	---TextColoredV(const ImVec4& col,const char* fmt,va_list args)
	---@param col ImVec4
	---@param fmt string
	---@param args userdata
    TextColoredV = function (col, fmt, args) end,
	---TextDisabled(const char* fmt,...)
	---@param fmt string
	---@param ... any
    TextDisabled = function (fmt, ...) end,
	---TextDisabledV(const char* fmt,va_list args)
	---@param fmt string
	---@param args userdata
    TextDisabledV = function (fmt, args) end,
	---TextEx(const char* text,const char* text_end=((void*)0),ImGuiTextFlags flags=0)
	---@param text string
	---@param text_end string
	---@param flags ImGuiTextFlags
    TextEx = function (text, text_end, flags) end,
	---TextLink(const char* label)
	---@param label string
    ---@return boolean
    TextLink = function (label) end,
	---TextLinkOpenURL(const char* label,const char* url=((void*)0))
	---@param label string
	---@param url string
    ---@return boolean
    TextLinkOpenURL = function (label, url) end,
	---TextV(const char* fmt,va_list args)
	---@param fmt string
	---@param args userdata
    TextV = function (fmt, args) end,
	---TextWrapped(const char* fmt,...)
	---@param fmt string
	---@param ... any
    TextWrapped = function (fmt, ...) end,
	---TextWrappedV(const char* fmt,va_list args)
	---@param fmt string
	---@param args userdata
    TextWrappedV = function (fmt, args) end,
	---TreeNode(const char* label)
	---@param label string
    ---@return boolean
    TreeNode = function (label) end,
	---TreeNode(const char* str_id,const char* fmt,...)
	---@param str_id string
	---@param fmt string
	---@param ... any
    ---@return boolean
    TreeNode = function (str_id, fmt, ...) end,
	---TreeNode(const void* ptr_id,const char* fmt,...)
	---@param ptr_id userdata
	---@param fmt string
	---@param ... any
    ---@return boolean
    TreeNode = function (ptr_id, fmt, ...) end,
	---TreeNodeBehavior(ImGuiID id,ImGuiTreeNodeFlags flags,const char* label,const char* label_end=((void*)0))
	---@param id userdata
	---@param flags ImGuiTreeNodeFlags
	---@param label string
	---@param label_end string
    ---@return boolean
    TreeNodeBehavior = function (id, flags, label, label_end) end,
	---TreeNodeDrawLineToChildNode(const ImVec2& target_pos)
	---@param target_pos ImVec2
    TreeNodeDrawLineToChildNode = function (target_pos) end,
	---TreeNodeDrawLineToTreePop(const ImGuiTreeNodeStackData* data)
	---@param data ImGuiTreeNodeStackData
    TreeNodeDrawLineToTreePop = function (data) end,
	---TreeNodeEx(const char* label,ImGuiTreeNodeFlags flags=0)
	---@param label string
	---@param flags ImGuiTreeNodeFlags
    ---@return boolean
    TreeNodeEx = function (label, flags) end,
	---TreeNodeEx(const char* str_id,ImGuiTreeNodeFlags flags,const char* fmt,...)
	---@param str_id string
	---@param flags ImGuiTreeNodeFlags
	---@param fmt string
	---@param ... any
    ---@return boolean
    TreeNodeEx = function (str_id, flags, fmt, ...) end,
	---TreeNodeEx(const void* ptr_id,ImGuiTreeNodeFlags flags,const char* fmt,...)
	---@param ptr_id userdata
	---@param flags ImGuiTreeNodeFlags
	---@param fmt string
	---@param ... any
    ---@return boolean
    TreeNodeEx = function (ptr_id, flags, fmt, ...) end,
	---TreeNodeExV(const char* str_id,ImGuiTreeNodeFlags flags,const char* fmt,va_list args)
	---@param str_id string
	---@param flags ImGuiTreeNodeFlags
	---@param fmt string
	---@param args userdata
    ---@return boolean
    TreeNodeExV = function (str_id, flags, fmt, args) end,
	---TreeNodeExV(const void* ptr_id,ImGuiTreeNodeFlags flags,const char* fmt,va_list args)
	---@param ptr_id userdata
	---@param flags ImGuiTreeNodeFlags
	---@param fmt string
	---@param args userdata
    ---@return boolean
    TreeNodeExV = function (ptr_id, flags, fmt, args) end,
	---TreeNodeGetOpen(ImGuiID storage_id)
	---@param storage_id userdata
    ---@return boolean
    TreeNodeGetOpen = function (storage_id) end,
	---TreeNodeSetOpen(ImGuiID storage_id,bool open)
	---@param storage_id userdata
	---@param open boolean
    TreeNodeSetOpen = function (storage_id, open) end,
	---TreeNodeUpdateNextOpen(ImGuiID storage_id,ImGuiTreeNodeFlags flags)
	---@param storage_id userdata
	---@param flags ImGuiTreeNodeFlags
    ---@return boolean
    TreeNodeUpdateNextOpen = function (storage_id, flags) end,
	---TreeNodeV(const char* str_id,const char* fmt,va_list args)
	---@param str_id string
	---@param fmt string
	---@param args userdata
    ---@return boolean
    TreeNodeV = function (str_id, fmt, args) end,
	---TreeNodeV(const void* ptr_id,const char* fmt,va_list args)
	---@param ptr_id userdata
	---@param fmt string
	---@param args userdata
    ---@return boolean
    TreeNodeV = function (ptr_id, fmt, args) end,
	---TreePop()
    TreePop = function () end,
	---TreePush(const char* str_id)
	---@param str_id string
    TreePush = function (str_id) end,
	---TreePush(const void* ptr_id)
	---@param ptr_id userdata
    TreePush = function (ptr_id) end,
	---TreePushOverrideID(ImGuiID id)
	---@param id userdata
    TreePushOverrideID = function (id) end,
	---TypingSelectFindBestLeadingMatch(ImGuiTypingSelectRequest* req,int items_count,const char*(*get_item_name_func)(void*,int),void* user_data)
	---@param req ImGuiTypingSelectRequest
	---@param items_count integer
	---@param get_item_name_func userdata
	---@param user_data userdata
    ---@return integer
    TypingSelectFindBestLeadingMatch = function (req, items_count, get_item_name_func, user_data) end,
	---TypingSelectFindMatch(ImGuiTypingSelectRequest* req,int items_count,const char*(*get_item_name_func)(void*,int),void* user_data,int nav_item_idx)
	---@param req ImGuiTypingSelectRequest
	---@param items_count integer
	---@param get_item_name_func userdata
	---@param user_data userdata
	---@param nav_item_idx integer
    ---@return integer
    TypingSelectFindMatch = function (req, items_count, get_item_name_func, user_data, nav_item_idx) end,
	---TypingSelectFindNextSingleCharMatch(ImGuiTypingSelectRequest* req,int items_count,const char*(*get_item_name_func)(void*,int),void* user_data,int nav_item_idx)
	---@param req ImGuiTypingSelectRequest
	---@param items_count integer
	---@param get_item_name_func userdata
	---@param user_data userdata
	---@param nav_item_idx integer
    ---@return integer
    TypingSelectFindNextSingleCharMatch = function (req, items_count, get_item_name_func, user_data, nav_item_idx) end,
	---Unindent(float indent_w=0.0f)
	---@param indent_w number
    Unindent = function (indent_w) end,
	---UnregisterFontAtlas(ImFontAtlas* atlas)
	---@param atlas ImFontAtlas
    UnregisterFontAtlas = function (atlas) end,
	---UnregisterUserTexture(ImTextureData* tex)
	---@param tex ImTextureData
    UnregisterUserTexture = function (tex) end,
	---UpdateCurrentFontSize(float restore_font_size_after_scaling)
	---@param restore_font_size_after_scaling number
    UpdateCurrentFontSize = function (restore_font_size_after_scaling) end,
	---UpdateHoveredWindowAndCaptureFlags(const ImVec2& mouse_pos)
	---@param mouse_pos ImVec2
    UpdateHoveredWindowAndCaptureFlags = function (mouse_pos) end,
	---UpdateInputEvents(bool trickle_fast_inputs)
	---@param trickle_fast_inputs boolean
    UpdateInputEvents = function (trickle_fast_inputs) end,
	---UpdateMouseMovingWindowEndFrame()
    UpdateMouseMovingWindowEndFrame = function () end,
	---UpdateMouseMovingWindowNewFrame()
    UpdateMouseMovingWindowNewFrame = function () end,
	---UpdateWindowParentAndRootLinks(ImGuiWindow* window,ImGuiWindowFlags flags,ImGuiWindow* parent_window)
	---@param window ImGuiWindow
	---@param flags ImGuiWindowFlags
	---@param parent_window ImGuiWindow
    UpdateWindowParentAndRootLinks = function (window, flags, parent_window) end,
	---UpdateWindowSkipRefresh(ImGuiWindow* window)
	---@param window ImGuiWindow
    UpdateWindowSkipRefresh = function (window) end,
	---VSliderFloat(const char* label,const ImVec2& size,float* v,float v_min,float v_max,const char* format="%.3f",ImGuiSliderFlags flags=0)
	---@param label string,
	---@param size ImVec2,
	---@param v number|nil,
	---@param v_min number,
	---@param v_max number,
	---@param format string,
	---@param flags ImGuiSliderFlags
    ---@return boolean, number
    VSliderFloat = function (label, size, v, v_min, v_max, format, flags) end,
	---VSliderInt(const char* label,const ImVec2& size,int* v,int v_min,int v_max,const char* format="%d",ImGuiSliderFlags flags=0)
	---@param label string,
	---@param size ImVec2,
	---@param v integer|nil,
	---@param v_min integer,
	---@param v_max integer,
	---@param format string,
	---@param flags ImGuiSliderFlags
    ---@return boolean, integer
    VSliderInt = function (label, size, v, v_min, v_max, format, flags) end,
	---VSliderScalar(const char* label,const ImVec2& size,ImGuiDataType data_type,void* p_data,const void* p_min,const void* p_max,const char* format=((void*)0),ImGuiSliderFlags flags=0)
	---@param label string
	---@param size ImVec2
	---@param data_type ImGuiDataType
	---@param p_data userdata
	---@param p_min userdata
	---@param p_max userdata
	---@param format string
	---@param flags ImGuiSliderFlags
    ---@return boolean
    VSliderScalar = function (label, size, data_type, p_data, p_min, p_max, format, flags) end,
	---Value(const char* prefix,bool b)
	---@param prefix string
	---@param b boolean
    Value = function (prefix, b) end,
	---Value(const char* prefix,int v)
	---@param prefix string
	---@param v integer
    Value = function (prefix, v) end,
	---Value(const char* prefix,unsigned int v)
	---@param prefix string
	---@param v integer
    Value = function (prefix, v) end,
	---Value(const char* prefix,float v,const char* float_format=((void*)0))
	---@param prefix string
	---@param v number
	---@param float_format string
    Value = function (prefix, v, float_format) end,
	---WindowPosAbsToRel(ImGuiWindow* window,const ImVec2& p)
	---@param window ImGuiWindow
	---@param p ImVec2
    ---@return userdata
    WindowPosAbsToRel = function (window, p) end,
	---WindowPosRelToAbs(ImGuiWindow* window,const ImVec2& p)
	---@param window ImGuiWindow
	---@param p ImVec2
    ---@return userdata
    WindowPosRelToAbs = function (window, p) end,
	---WindowRectAbsToRel(ImGuiWindow* window,const ImRect& r)
	---@param window ImGuiWindow
	---@param r ImRect
    ---@return userdata
    WindowRectAbsToRel = function (window, r) end,
	---WindowRectRelToAbs(ImGuiWindow* window,const ImRect& r)
	---@param window ImGuiWindow
	---@param r ImRect
    ---@return userdata
    WindowRectRelToAbs = function (window, r) end
}

---@param text string
---@param endText userdata | nil
ImGui.TextUnformatted = function(text, endText) end

---@param label string
---@param str string
---@param flags integer
---@param callback function | nil
---@param userData userdata | nil
---@return boolean, string
ImGui.InputText = function(label, str, flags, callback, userData) end

---@param label string
---@param str string
---@param size ImVec2
---@param flags integer
---@param callback function | nil
---@param userData userdata | nil
---@return boolean, string
ImGui.InputTextMultiline = function(label, str, size, flags, callback, userData) end

---@param label string
---@param hint string
---@param str string
---@param flags integer
---@param callback function | nil
---@param userData userdata | nil
---@return boolean, string
ImGui.InputTextWithHint = function(label, hint, str, flags, callback, userData) end

---CalcTextSize(ImVec2 *pOut, const char* text,const char* text_end=((void*)0),bool hide_text_after_double_hash=false,float wrap_width=-1.0f)
---@param pOut ImVec2
---@param text string
---@param endText string | nil
---@param hideTextAfterDoubleHash boolean
---@param wrapWidth number
ImGui.CalcTextSize = function(pOut, text, endText, hideTextAfterDoubleHash, wrapWidth) end

---@return ImGuiWindow
getCurrentWindowGImgui = function() end
