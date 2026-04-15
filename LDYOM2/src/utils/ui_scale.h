#pragma once

#include <imgui.h>

namespace ui {
	// Font-relative sizing helpers. 1em == current font size (already scaled by FontScaleDpi).
	// Prefer these over raw pixel values or `* screenScale` multiplications for widget/layout sizing.
	inline float em(float n) { return ImGui::GetFontSize() * n; }
	inline ImVec2 em(float x, float y) { return ImVec2(ImGui::GetFontSize() * x, ImGui::GetFontSize() * y); }
} // namespace ui
