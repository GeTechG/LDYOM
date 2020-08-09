stylel = {}


function stylel.apply_custom_style()
    imgui.SwitchContext()
    local style = imgui.GetStyle();
    local io = imgui.GetIO();
    local ImVec4 = imgui.ImVec4
    local ImVec2 = imgui.ImVec2
    local colors = style.Colors
    local clr = imgui.Col

    -- light style 0rom Pac?me Danhiez (user itamago) https:--github.com/ocornut/imgui/pull/511#issuecomment-175719267
    style.Alpha = 1.0;
    style.FrameRounding = 3.0;
    colors[clr.Text] = ImVec4(1.0000, 1.0000, 1.0000, 1.0000);
    colors[clr.TextDisabled] = ImVec4(0.0000, 0.5430, 0.9830, 1.0000);
    colors[clr.WindowBg] = ImVec4(0.0000, 0.0000, 0.0000, 0.8950);
    colors[clr.ChildBg] = ImVec4(0.000, 0.000, 0.000, 0.000);
    colors[clr.PopupBg] = ImVec4(0.070, 0.070, 0.090, 1.000);
    colors[clr.Border] = ImVec4(0.1840, 0.8780, 0.0000, 0.5000);
    colors[clr.BorderShadow] = ImVec4(1.000, 1.000, 1.000, 0.100);
    colors[clr.FrameBg] = ImVec4(0.1600, 0.1600, 0.1600, 0.3150);
    colors[clr.FrameBgHovered] = ImVec4(0.1600, 0.1600, 0.1600, 0.3150);
    colors[clr.FrameBgActive] = ImVec4(0.2100, 0.2100, 0.2100, 0.6700);
    colors[clr.TitleBg] = ImVec4(0.0260, 0.5970, 0.0000, 1.0000);
    colors[clr.TitleBgCollapsed] = ImVec4(0.0990, 0.3150, 0.0000, 0.0000);
    colors[clr.TitleBgActive] = ImVec4(0.0260, 0.5970, 0.0000, 1.0000);
    colors[clr.MenuBarBg] = ImVec4(0.860, 0.860, 0.860, 1.000);
    colors[clr.ScrollbarBg] = ImVec4(0.0000, 0.0000, 0.0000, 0.8010);
    colors[clr.ScrollbarGrab] = ImVec4(0.2380, 0.2380, 0.2380, 1.0000);
    colors[clr.ScrollbarGrabHovered] = ImVec4(0.2380, 0.2380, 0.2380, 1.0000);
    colors[clr.ScrollbarGrabActive] = ImVec4(0.0040, 0.3810, 0.0000, 1.0000);
    --colors[clr.ComboBg] = ImVec4(0.860, 0.860, 0.860, 0.990);
    colors[clr.CheckMark] = ImVec4(0.0090, 0.8450, 0.0000, 1.0000);
    colors[clr.SliderGrab] = ImVec4(0.1390, 0.5080, 0.0000, 1.0000);
    colors[clr.SliderGrabActive] = ImVec4(0.1390, 0.5080, 0.0000, 1.0000);
    colors[clr.Button] = ImVec4(0.0000, 0.0000, 0.0000, 0.4000);
    colors[clr.ButtonHovered] = ImVec4(0.0000, 0.6190, 0.0140, 1.0000);
    colors[clr.ButtonActive] = ImVec4(0.060, 0.530, 0.980, 1.000);
    colors[clr.Header] = ImVec4(0.260, 0.590, 0.980, 0.310);
    colors[clr.HeaderHovered] = ImVec4(0.260, 0.590, 0.980, 0.800);
    colors[clr.HeaderActive] = ImVec4(0.260, 0.590, 0.980, 1.000);
    colors[clr.ResizeGrip] = ImVec4(0.0000, 1.0000, 0.2210, 0.5970);
    colors[clr.ResizeGripHovered] = ImVec4(0.260, 0.590, 0.980, 0.670);
    colors[clr.ResizeGripActive] = ImVec4(0.260, 0.590, 0.980, 0.950);
    --colors[clr.CloseButton] = ImVec4(0.590, 0.590, 0.590, 0.500);
    --colors[clr.CloseButtonHovered] = ImVec4(0.980, 0.390, 0.360, 1.000);
    --colors[clr.CloseButtonActive] = ImVec4(0.980, 0.390, 0.360, 1.000);
    colors[clr.PlotLines] = ImVec4(0.390, 0.390, 0.390, 1.000);
    colors[clr.PlotLinesHovered] = ImVec4(1.000, 0.430, 0.350, 1.000);
    colors[clr.PlotHistogram] = ImVec4(0.900, 0.700, 0.000, 1.000);
    colors[clr.PlotHistogramHovered] = ImVec4(1.000, 0.600, 0.000, 1.000);
    colors[clr.TextSelectedBg] = ImVec4(0.260, 0.590, 0.980, 0.350);
    colors[clr.ModalWindowDimBg] = ImVec4(0.200, 0.200, 0.200, 0.350);

    style.ItemInnerSpacing = ImVec2(5, 4); --ItemSpacing(ImVec2(7.0));
    
    style.ItemSpacing = imgui.ImVec2(4.0, 4.0)
    style.FramePadding = imgui.ImVec2(2.5, 3.5)
    style.WindowPadding = imgui.ImVec2(4.0, 4.0)

    style.ScrollbarSize = 16.0; --ItemSpacing(ImVec2(7.0));
    style.GrabMinSize = 8.0; --ItemSpacing(ImVec2(7.0));
    style.WindowBorderSize = 1.0; --ItemSpacing(ImVec2(7.0));
    style.FrameBorderSize = 1.0; --ItemSpacing(ImVec2(7.0));
    style.WindowRounding = 0.0; --ItemSpacing(ImVec2(7.0));
    style.FrameRounding = 1.0; --ItemSpacing(ImVec2(7.0));

    style.AntiAliasedLines = true;
    style.AntiAliasedFill = true;
 end

return stylel