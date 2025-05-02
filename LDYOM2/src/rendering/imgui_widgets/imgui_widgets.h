#pragma once
#include "imgui.h"

namespace ImGui {
bool TextSelectable(const char* label, bool selected = false, const ImVec2& size = ImVec2(0, 0));
bool TextButton(const char* label, const ImVec2& size = ImVec2(0, 0));
void TextGTAColored(const char* label);
bool SelectableGTAColored(const char* label, bool selected, ImGuiSelectableFlags flags = 0,
                          const ImVec2& size = ImVec2(0, 0));
} // namespace ImGui
