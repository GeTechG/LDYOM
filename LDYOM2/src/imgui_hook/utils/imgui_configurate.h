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
	static float getGlobalScale();
	static ImVec2 getScreenScale();
	static void setGlobalScale(float scale);
	static ImFont* getHeaderFont();
};

#define SCL_PX ImGuiConfigurate::getScreenScale() * ImGuiConfigurate::getGlobalScale()