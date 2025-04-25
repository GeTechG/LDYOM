#pragma once

#include "imgui.h"
#include <map>
#include <string>
#include <vector>

struct ImGuiTheme {
	ImVec4 colors[ImGuiCol_COUNT];
	std::map<ImGuiStyleVar_, float> styleVarsFloat;
	std::map<ImGuiStyleVar_, ImVec2> styleVarsVec2;
};

class ThemeLoader {
  private:
	static const std::string THEMES_DIR_PATH;
	static std::vector<std::string> availableThemes;

	static void applyTheme(ImGuiTheme theme);
	static bool isStyleVarVec2(ImGuiStyleVar_ var);

  public:
	static void initialize();
	static bool loadThemeFromJson(std::string_view themeName);
	static std::string getCurrentThemeName();
	static const std::vector<std::string>& getAvailableThemes();
};