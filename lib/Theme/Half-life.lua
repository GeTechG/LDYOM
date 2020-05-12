stylel = {}


function stylel.apply_custom_style()
    imgui.SwitchContext()
    local style = imgui.GetStyle();
    local io = imgui.GetIO();
    local ImVec4 = imgui.ImVec4
    local ImVec2 = imgui.ImVec2
    local colors = style.Colors
    local clr = imgui.Col

    colors[clr.Text]                              = ImVec4(1.000, 1.000, 1.000, 1.000);
    colors[clr.TextDisabled]              = ImVec4(0.500, 0.500, 0.500, 1.000);
    colors[clr.WindowBg]                          = ImVec4(0.290, 0.340, 0.260, 1.000);
    colors[clr.ChildBg]                                = ImVec4(0.290, 0.340, 0.260, 1.000);
    colors[clr.PopupBg]                                = ImVec4(0.240, 0.270, 0.200, 1.000);
    colors[clr.Border]                          = ImVec4(0.540, 0.570, 0.510, 0.500);
    colors[clr.BorderShadow]              = ImVec4(0.140, 0.160, 0.110, 0.520);
    colors[clr.FrameBg]                                = ImVec4(0.240, 0.270, 0.200, 1.000);
    colors[clr.FrameBgHovered]          = ImVec4(0.270, 0.300, 0.230, 1.000);
    colors[clr.FrameBgActive]            = ImVec4(0.300, 0.340, 0.260, 1.000);
    colors[clr.TitleBg]                                = ImVec4(0.240, 0.270, 0.200, 1.000);
    colors[clr.TitleBgActive]            = ImVec4(0.290, 0.340, 0.260, 1.000);
    colors[clr.TitleBgCollapsed]          = ImVec4(0.000, 0.000, 0.000, 0.510);
    colors[clr.MenuBarBg]                        = ImVec4(0.240, 0.270, 0.200, 1.000);
    colors[clr.ScrollbarBg]                    = ImVec4(0.350, 0.420, 0.310, 1.000);
    colors[clr.ScrollbarGrab]            = ImVec4(0.280, 0.320, 0.240, 1.000);
    colors[clr.ScrollbarGrabHovered]   = ImVec4(0.250, 0.300, 0.220, 1.000);
    colors[clr.ScrollbarGrabActive]    = ImVec4(0.230, 0.270, 0.210, 1.000);
    colors[clr.CheckMark]                        = ImVec4(0.590, 0.540, 0.180, 1.000);
    colors[clr.SliderGrab]                      = ImVec4(0.350, 0.420, 0.310, 1.000);
    colors[clr.SliderGrabActive]          = ImVec4(0.540, 0.570, 0.510, 0.500);
    colors[clr.Button]                          = ImVec4(0.290, 0.340, 0.260, 0.400);
    colors[clr.ButtonHovered]            = ImVec4(0.350, 0.420, 0.310, 1.000);
    colors[clr.ButtonActive]              = ImVec4(0.540, 0.570, 0.510, 0.500);
    colors[clr.Header]                          = ImVec4(0.350, 0.420, 0.310, 1.000);
    colors[clr.HeaderHovered]            = ImVec4(0.350, 0.420, 0.310, 0.60);
    colors[clr.HeaderActive]              = ImVec4(0.540, 0.570, 0.510, 0.500);
    colors[clr.Separator]                        = ImVec4(0.140, 0.160, 0.110, 1.000);
    colors[clr.SeparatorHovered]          = ImVec4(0.540, 0.570, 0.510, 1.000);
    colors[clr.SeparatorActive]                = ImVec4(0.590, 0.540, 0.180, 1.000);
    colors[clr.ResizeGrip]                      = ImVec4(0.190, 0.230, 0.180, 0.000); -- grip invis
    colors[clr.ResizeGripHovered]        = ImVec4(0.540, 0.570, 0.510, 1.000);
    colors[clr.ResizeGripActive]          = ImVec4(0.590, 0.540, 0.180, 1.000);
    colors[clr.Tab]                                    = ImVec4(0.350, 0.420, 0.310, 1.000);
    colors[clr.TabHovered]                      = ImVec4(0.540, 0.570, 0.510, 0.780);
    colors[clr.TabActive]                        = ImVec4(0.590, 0.540, 0.180, 1.000);
    colors[clr.TabUnfocused]              = ImVec4(0.240, 0.270, 0.200, 1.000);
    colors[clr.TabUnfocusedActive]      = ImVec4(0.350, 0.420, 0.310, 1.000);
    colors[clr.PlotLines]                        = ImVec4(0.610, 0.610, 0.610, 1.000);
    colors[clr.PlotLinesHovered]          = ImVec4(0.590, 0.540, 0.180, 1.000);
    colors[clr.PlotHistogram]            = ImVec4(1.000, 0.780, 0.280, 1.000);
    colors[clr.PlotHistogramHovered]   = ImVec4(1.000, 0.600, 0.000, 1.000);
    colors[clr.TextSelectedBg]          = ImVec4(0.590, 0.540, 0.180, 1.000);
    colors[clr.DragDropTarget]          = ImVec4(0.730, 0.670, 0.240, 1.000);
    colors[clr.NavHighlight]              = ImVec4(0.590, 0.540, 0.180, 1.000);
    colors[clr.NavWindowingHighlight]  = ImVec4(1.000, 1.000, 1.000, 0.700);
    colors[clr.NavWindowingDimBg]        = ImVec4(0.800, 0.800, 0.800, 0.200);
    colors[clr.ModalWindowDimBg]          = ImVec4(0.800, 0.800, 0.800, 0.350);

    style.FrameBorderSize = 1.00;
    style.WindowRounding = 0.00;
    style.ChildRounding = 0.00;
    style.FrameRounding = 0.00;
    style.PopupRounding = 0.00;
    style.ScrollbarRounding = 0.00;
    style.GrabRounding = 0.00;
    style.TabRounding = 0.00;
 end

return stylel