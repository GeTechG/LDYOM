stylel = {}


function stylel.apply_custom_style()
    imgui.SwitchContext()
    local style = imgui.GetStyle();
    local io = imgui.GetIO();
    local ImVec4 = imgui.ImVec4
    local ImVec2 = imgui.ImVec2
    local colors = style.Colors
    local clr = imgui.Col

    style.WindowRounding    = 2.00;             -- Radius o0 window corners rounding. Set to 0.00 to have rectangular windows
    style.ScrollbarRounding = 3.00;             -- Radius o0 grab corners rounding 0or scrollbar
    style.GrabRounding      = 2.00;             -- Radius o0 grabs corners rounding. Set to 0.00 to have rectangular slider grabs.
    style.AntiAliasedLines  = true;
    style.AntiAliasedFill   = true;
    style.WindowRounding    = 2;
    style.ChildRounding     = 2;
    style.ScrollbarSize     = 16;
    style.ScrollbarRounding = 3;
    style.GrabRounding      = 2;
    style.IndentSpacing     = 22;
    style.Alpha             = 1.00;
    style.FrameRounding     = 3.00;

    style.ItemSpacing = imgui.ImVec2(4.0, 4.0)
    style.FramePadding = imgui.ImVec2(2.5, 3.5)

    colors[clr.Text]                   = ImVec4(0.000, 0.000, 0.000, 1.000);
    colors[clr.TextDisabled]          = ImVec4(0.600, 0.600, 0.600, 1.000);
    colors[clr.WindowBg]              = ImVec4(0.860, 0.860, 0.860, 1.000);
    --colors[clr.ChildWindowBg]         = ImVec4(0.000, 0.000, 0.000, 0.000);
    colors[clr.ChildBg]                = ImVec4(0.000, 0.000, 0.000, 0.090);
    colors[clr.PopupBg]                = ImVec4(0.930, 0.930, 0.930, 0.980);
    colors[clr.Border]                = ImVec4(0.710, 0.710, 0.710, 0.080);
    colors[clr.BorderShadow]          = ImVec4(0.000, 0.000, 0.000, 0.040);
    colors[clr.FrameBg]               = ImVec4(0.710, 0.710, 0.710, 0.550);
    colors[clr.FrameBgHovered]        = ImVec4(0.940, 0.940, 0.940, 0.550);
    colors[clr.FrameBgActive]         = ImVec4(0.710, 0.780, 0.690, 0.980);
    colors[clr.TitleBg]               = ImVec4(0.850, 0.850, 0.850, 1.000);
    colors[clr.TitleBgCollapsed]      = ImVec4(0.820, 0.780, 0.780, 0.510);
    colors[clr.TitleBgActive]         = ImVec4(0.780, 0.780, 0.780, 1.000);
    colors[clr.MenuBarBg]             = ImVec4(0.860, 0.860, 0.860, 1.000);
    colors[clr.ScrollbarBg]           = ImVec4(0.200, 0.250, 0.300, 0.610);
    colors[clr.ScrollbarGrab]         = ImVec4(0.900, 0.900, 0.900, 0.300);
    colors[clr.ScrollbarGrabHovered]  = ImVec4(0.920, 0.920, 0.920, 0.780);
    colors[clr.ScrollbarGrabActive]   = ImVec4(1.000, 1.000, 1.000, 1.000);
    colors[clr.CheckMark]             = ImVec4(0.1840, 0.4070, 0.1930, 1.000);
    colors[clr.SliderGrab]            = ImVec4(0.260, 0.590, 0.980, 0.780);
    colors[clr.SliderGrabActive]      = ImVec4(0.260, 0.590, 0.980, 1.000);
    colors[clr.Button]                = ImVec4(0.710, 0.780, 0.690, 0.400);
    colors[clr.ButtonHovered]         = ImVec4(0.7250, 0.8050, 0.7020, 1.000);
    colors[clr.ButtonActive]          = ImVec4(0.7930, 0.9000, 0.8360, 1.000);
    colors[clr.Header]                = ImVec4(0.710, 0.780, 0.690, 0.310);
    colors[clr.HeaderHovered]         = ImVec4(0.710, 0.780, 0.690, 0.800);
    colors[clr.HeaderActive]          = ImVec4(0.710, 0.780, 0.690, 1.000);
    colors[clr.Separator]              = ImVec4(0.390, 0.390, 0.390, 1.000);
    colors[clr.SeparatorHovered]       = ImVec4(0.140, 0.440, 0.800, 0.780);
    colors[clr.SeparatorActive]        = ImVec4(0.140, 0.440, 0.800, 1.000);
    colors[clr.ResizeGrip]            = ImVec4(1.000, 1.000, 1.000, 0.000);
    colors[clr.ResizeGripHovered]     = ImVec4(0.260, 0.590, 0.980, 0.450);
    colors[clr.ResizeGripActive]      = ImVec4(0.260, 0.590, 0.980, 0.780);
    colors[clr.PlotLines]             = ImVec4(0.390, 0.390, 0.390, 1.000);
    colors[clr.PlotLinesHovered]      = ImVec4(1.000, 0.430, 0.350, 1.000);
    colors[clr.PlotHistogram]         = ImVec4(0.900, 0.700, 0.000, 1.000);
    colors[clr.PlotHistogramHovered]  = ImVec4(1.000, 0.600, 0.000, 1.000);
    colors[clr.TextSelectedBg]        = ImVec4(0.260, 0.590, 0.980, 0.350);
    colors[clr.ModalWindowDimBg]  = ImVec4(0.200, 0.200, 0.200, 0.350);
    colors[clr.DragDropTarget]         = ImVec4(0.260, 0.590, 0.980, 0.950);
    colors[clr.NavHighlight]           = colors[clr.HeaderHovered];
    colors[clr.NavWindowingHighlight]  = ImVec4(0.700, 0.700, 0.700, 0.700);
 end

return stylel