#pragma once

#include <imgui.h>
#include <map>
#include <string>
#include <vector>

class ImGuiConfigurate {
	inline static ImVec2 screenScale;
	inline static ImFont* headerFont = nullptr;

  public:
	static void update(int width, int height);
	// Deprecated: prefer getUiScale() for a single scalar factor, or ui::em() for font-relative sizing.
	// Kept for transitional migration — do not use in new code.
	static float getGlobalScale();
	static ImVec2 getScreenScale();
	static void setGlobalScale(float scale);
	// Unified UI scale: screenScale.y * globalScale. Single scalar used across the framework.
	static float getUiScale();
	static ImFont* getHeaderFont();
};

// Deprecated: ImVec2 scale factor. Prefer ui::em() for font-relative, or ImGuiConfigurate::getUiScale() for framework use.
#define SCL_PX ImGuiConfigurate::getScreenScale() * ImGuiConfigurate::getGlobalScale()