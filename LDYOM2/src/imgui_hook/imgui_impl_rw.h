// ImGui backend for RenderWare (GTA San Andreas)
// Based on: https://github.com/aap/librw
#pragma once

#include <imgui.h>

IMGUI_IMPL_API bool ImGui_ImplRW_Init();
IMGUI_IMPL_API void ImGui_ImplRW_Shutdown();
IMGUI_IMPL_API void ImGui_ImplRW_NewFrame();
IMGUI_IMPL_API bool ImGui_ImplRW_CreateDeviceObjects();
IMGUI_IMPL_API void ImGui_ImplRW_InvalidateDeviceObjects();
void ImGui_ImplRW_RenderDrawData(ImDrawData* draw_data);
