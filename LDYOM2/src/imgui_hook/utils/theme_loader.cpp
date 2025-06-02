#define IMGUI_DEFINE_MATH_OPERATORS
#include "theme_loader.h"
#include "settings.h"
#include "logger.h"
#include "paths.h"
#include "utils/imgui_configurate.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace fs = std::filesystem;

const std::string ThemeLoader::THEMES_DIR_PATH = "themes";
std::vector<std::string> ThemeLoader::availableThemes;

bool ThemeLoader::isStyleVarVec2(ImGuiStyleVar_ var) {
	switch (var) {
	case ImGuiStyleVar_WindowPadding:
	case ImGuiStyleVar_WindowMinSize:
	case ImGuiStyleVar_WindowTitleAlign:
	case ImGuiStyleVar_FramePadding:
	case ImGuiStyleVar_ItemSpacing:
	case ImGuiStyleVar_ItemInnerSpacing:
	case ImGuiStyleVar_CellPadding:
	case ImGuiStyleVar_TableAngledHeadersTextAlign:
	case ImGuiStyleVar_ButtonTextAlign:
	case ImGuiStyleVar_SelectableTextAlign:
	case ImGuiStyleVar_SeparatorTextAlign:
	case ImGuiStyleVar_SeparatorTextPadding:
		return true;
	default:
		return false;
	}
}

void ThemeLoader::initialize() {
	availableThemes.clear();
	fs::path themesDir(LDYOM_PATH(THEMES_DIR_PATH));
	if (!fs::exists(themesDir)) {
		fs::create_directories(themesDir);
		LDYOM_INFO("Created themes directory: {}", themesDir.string());
	}
	for (const auto& entry : fs::directory_iterator(themesDir)) {
		if (entry.is_regular_file() && entry.path().extension() == ".json") {
			availableThemes.push_back(entry.path().stem().string());
		}
	}
}

bool ThemeLoader::loadThemeFromJson(std::string_view themeName) {
	try {
		fs::path jsonFilePath(LDYOM_PATH(THEMES_DIR_PATH) + "/" + std::string(themeName) + ".json");
		std::ifstream file(jsonFilePath);
		if (!file.is_open()) {
			std::cerr << "Failed to open theme file: " << jsonFilePath << std::endl;
			return false;
		}

		json themeJson;
		file >> themeJson;

		ImGuiTheme theme;

		// Initialize all colors to default
		ImGuiStyle defaultStyle;
		for (int i = 0; i < ImGuiCol_COUNT; i++) {
			theme.colors[i] = defaultStyle.Colors[i];
		}

		// Load colors from JSON
		if (themeJson.contains("colors") && themeJson["colors"].is_object()) {
			auto& colorsJson = themeJson["colors"];

			// Define a mapping from string color names to ImGuiCol_ enum values
			const std::map<std::string, ImGuiCol_> colorMap = {
				{"Text", ImGuiCol_Text},
				{"TextDisabled", ImGuiCol_TextDisabled},
				{"WindowBg", ImGuiCol_WindowBg},
				{"ChildBg", ImGuiCol_ChildBg},
				{"PopupBg", ImGuiCol_PopupBg},
				{"Border", ImGuiCol_Border},
				{"BorderShadow", ImGuiCol_BorderShadow},
				{"FrameBg", ImGuiCol_FrameBg},
				{"FrameBgHovered", ImGuiCol_FrameBgHovered},
				{"FrameBgActive", ImGuiCol_FrameBgActive},
				{"TitleBg", ImGuiCol_TitleBg},
				{"TitleBgActive", ImGuiCol_TitleBgActive},
				{"TitleBgCollapsed", ImGuiCol_TitleBgCollapsed},
				{"MenuBarBg", ImGuiCol_MenuBarBg},
				{"ScrollbarBg", ImGuiCol_ScrollbarBg},
				{"ScrollbarGrab", ImGuiCol_ScrollbarGrab},
				{"ScrollbarGrabHovered", ImGuiCol_ScrollbarGrabHovered},
				{"ScrollbarGrabActive", ImGuiCol_ScrollbarGrabActive},
				{"CheckMark", ImGuiCol_CheckMark},
				{"SliderGrab", ImGuiCol_SliderGrab},
				{"SliderGrabActive", ImGuiCol_SliderGrabActive},
				{"Button", ImGuiCol_Button},
				{"ButtonHovered", ImGuiCol_ButtonHovered},
				{"ButtonActive", ImGuiCol_ButtonActive},
				{"Header", ImGuiCol_Header},
				{"HeaderHovered", ImGuiCol_HeaderHovered},
				{"HeaderActive", ImGuiCol_HeaderActive},
				{"Separator", ImGuiCol_Separator},
				{"SeparatorHovered", ImGuiCol_SeparatorHovered},
				{"SeparatorActive", ImGuiCol_SeparatorActive},
				{"ResizeGrip", ImGuiCol_ResizeGrip},
				{"ResizeGripHovered", ImGuiCol_ResizeGripHovered},
				{"ResizeGripActive", ImGuiCol_ResizeGripActive},
				{"TabHovered", ImGuiCol_TabHovered},
				{"Tab", ImGuiCol_Tab},
				{"TabSelected", ImGuiCol_TabSelected},
				{"TabSelectedOverline", ImGuiCol_TabSelectedOverline},
				{"TabDimmed", ImGuiCol_TabDimmed},
				{"TabDimmedSelected", ImGuiCol_TabDimmedSelected},
				{"TabDimmedSelectedOverline", ImGuiCol_TabDimmedSelectedOverline},
				{"PlotLines", ImGuiCol_PlotLines},
				{"PlotLinesHovered", ImGuiCol_PlotLinesHovered},
				{"PlotHistogram", ImGuiCol_PlotHistogram},
				{"PlotHistogramHovered", ImGuiCol_PlotHistogramHovered},
				{"TableHeaderBg", ImGuiCol_TableHeaderBg},
				{"TableBorderStrong", ImGuiCol_TableBorderStrong},
				{"TableBorderLight", ImGuiCol_TableBorderLight},
				{"TableRowBg", ImGuiCol_TableRowBg},
				{"TableRowBgAlt", ImGuiCol_TableRowBgAlt},
				{"TextLink", ImGuiCol_TextLink},
				{"TextSelectedBg", ImGuiCol_TextSelectedBg},
				{"DragDropTarget", ImGuiCol_DragDropTarget},
				{"NavCursor", ImGuiCol_NavCursor},
				{"NavWindowingHighlight", ImGuiCol_NavWindowingHighlight},
				{"NavWindowingDimBg", ImGuiCol_NavWindowingDimBg},
				{"ModalWindowDimBg", ImGuiCol_ModalWindowDimBg},
			};

			for (auto& [colorName, colorIndex] : colorMap) {
				if (colorsJson.contains(colorName) && colorsJson[colorName].is_array()) {
					auto& colorArray = colorsJson[colorName];
					if (colorArray.size() >= 4) {
						theme.colors[colorIndex] = ImVec4(colorArray[0].get<float>(), colorArray[1].get<float>(),
						                                  colorArray[2].get<float>(), colorArray[3].get<float>());
					}
				}
			}
		}

		// Load style variables from JSON
		if (themeJson.contains("style") && themeJson["style"].is_object()) {
			auto& styleJson = themeJson["style"];

			// Define a mapping from string style names to ImGuiStyleVar_ enum values
			const std::map<std::string, ImGuiStyleVar_> styleVarMap = {
				{"Alpha", ImGuiStyleVar_Alpha},
				{"DisabledAlpha", ImGuiStyleVar_DisabledAlpha},
				{"WindowPadding", ImGuiStyleVar_WindowPadding},
				{"WindowRounding", ImGuiStyleVar_WindowRounding},
				{"WindowBorderSize", ImGuiStyleVar_WindowBorderSize},
				{"WindowMinSize", ImGuiStyleVar_WindowMinSize},
				{"WindowTitleAlign", ImGuiStyleVar_WindowTitleAlign},
				{"ChildRounding", ImGuiStyleVar_ChildRounding},
				{"ChildBorderSize", ImGuiStyleVar_ChildBorderSize},
				{"PopupRounding", ImGuiStyleVar_PopupRounding},
				{"PopupBorderSize", ImGuiStyleVar_PopupBorderSize},
				{"FramePadding", ImGuiStyleVar_FramePadding},
				{"FrameRounding", ImGuiStyleVar_FrameRounding},
				{"FrameBorderSize", ImGuiStyleVar_FrameBorderSize},
				{"ItemSpacing", ImGuiStyleVar_ItemSpacing},
				{"ItemInnerSpacing", ImGuiStyleVar_ItemInnerSpacing},
				{"IndentSpacing", ImGuiStyleVar_IndentSpacing},
				{"CellPadding", ImGuiStyleVar_CellPadding},
				{"ScrollbarSize", ImGuiStyleVar_ScrollbarSize},
				{"ScrollbarRounding", ImGuiStyleVar_ScrollbarRounding},
				{"GrabMinSize", ImGuiStyleVar_GrabMinSize},
				{"GrabRounding", ImGuiStyleVar_GrabRounding},
				{"TabRounding", ImGuiStyleVar_TabRounding},
				{"TabBorderSize", ImGuiStyleVar_TabBorderSize},
				{"TabBarBorderSize", ImGuiStyleVar_TabBarBorderSize},
				{"TabBarOverlineSize", ImGuiStyleVar_TabBarOverlineSize},
				{"TableAngledHeadersAngle", ImGuiStyleVar_TableAngledHeadersAngle},
				{"TableAngledHeadersTextAlign", ImGuiStyleVar_TableAngledHeadersTextAlign},
				{"ButtonTextAlign", ImGuiStyleVar_ButtonTextAlign},
				{"SelectableTextAlign", ImGuiStyleVar_SelectableTextAlign},
				{"SeparatorTextBorderSize", ImGuiStyleVar_SeparatorTextBorderSize},
				{"SeparatorTextAlign", ImGuiStyleVar_SeparatorTextAlign},
				{"SeparatorTextPadding", ImGuiStyleVar_SeparatorTextPadding},
			};

			for (auto& [styleName, styleVar] : styleVarMap) {
				if (styleJson.contains(styleName)) {
					if (isStyleVarVec2(styleVar)) {
						// Handle Vec2 style vars
						if (styleJson[styleName].is_array() && styleJson[styleName].size() >= 2) {
							auto& styleArray = styleJson[styleName];
							theme.styleVarsVec2[styleVar] =
								ImVec2(styleArray[0].get<float>(), styleArray[1].get<float>());
						}
					} else {
						// Handle float style vars
						if (styleJson[styleName].is_number()) {
							theme.styleVarsFloat[styleVar] = styleJson[styleName].get<float>();
						}
					}
				}
			}
		}

		applyTheme(theme);
		Settings::instance().setSetting<std::string>("theme", std::string(themeName));

		return true;
	} catch (const std::exception& e) {
		LDYOM_ERROR("Error loading theme from JSON: {}", e.what());
		return false;
	}
}

std::string ThemeLoader::getCurrentThemeName() {
	return Settings::instance().getSetting<std::string>("theme", "");
}

const std::vector<std::string>& ThemeLoader::getAvailableThemes() { return availableThemes; }

void ThemeLoader::applyTheme(ImGuiTheme theme) {
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec2 screenScale = ImGuiConfigurate::getScreenScale();

	for (int i = 0; i < ImGuiCol_COUNT; i++) {
		style.Colors[i] = theme.colors[i];
	}

	for (const auto& [styleVar, value] : theme.styleVarsFloat) {
		switch (styleVar) {
		case ImGuiStyleVar_Alpha:
			style.Alpha = value;
			break;
		case ImGuiStyleVar_DisabledAlpha:
			style.DisabledAlpha = value;
			break;
		case ImGuiStyleVar_WindowRounding:
			style.WindowRounding = value * screenScale.x;
			break;
		case ImGuiStyleVar_WindowBorderSize:
			style.WindowBorderSize = value * screenScale.x;
			break;
		case ImGuiStyleVar_ChildRounding:
			style.ChildRounding = value * screenScale.x;
			break;
		case ImGuiStyleVar_ChildBorderSize:
			style.ChildBorderSize = value * screenScale.x;
			break;
		case ImGuiStyleVar_PopupRounding:
			style.PopupRounding = value * screenScale.x;
			break;
		case ImGuiStyleVar_PopupBorderSize:
			style.PopupBorderSize = value * screenScale.x;
			break;
		case ImGuiStyleVar_FrameRounding:
			style.FrameRounding = value * screenScale.x;
			break;
		case ImGuiStyleVar_FrameBorderSize:
			style.FrameBorderSize = value * screenScale.x;
			break;
		case ImGuiStyleVar_IndentSpacing:
			style.IndentSpacing = value * screenScale.x;
			break;
		case ImGuiStyleVar_ScrollbarSize:
			style.ScrollbarSize = value * screenScale.x;
			break;
		case ImGuiStyleVar_ScrollbarRounding:
			style.ScrollbarRounding = value * screenScale.x;
			break;
		case ImGuiStyleVar_GrabMinSize:
			style.GrabMinSize = value * screenScale.x;
			break;
		case ImGuiStyleVar_GrabRounding:
			style.GrabRounding = value * screenScale.x;
			break;
		case ImGuiStyleVar_TabRounding:
			style.TabRounding = value * screenScale.x;
			break;
		case ImGuiStyleVar_TabBorderSize:
			style.TabBorderSize = value * screenScale.x;
			break;
		case ImGuiStyleVar_TabBarBorderSize:
			style.TabBarBorderSize = value * screenScale.x;
			break;
		case ImGuiStyleVar_TabBarOverlineSize:
			style.TabBarOverlineSize = value * screenScale.x;
			break;
		case ImGuiStyleVar_TableAngledHeadersAngle:
			style.TableAngledHeadersAngle = value;
			break;
		case ImGuiStyleVar_SeparatorTextBorderSize:
			style.SeparatorTextBorderSize = value * screenScale.x;
			break;
		default:
			break;
		}
	}

	for (const auto& [styleVar, value] : theme.styleVarsVec2) {
		switch (styleVar) {
		case ImGuiStyleVar_WindowPadding:
			style.WindowPadding = value * screenScale;
			break;
		case ImGuiStyleVar_WindowMinSize:
			style.WindowMinSize = value * screenScale;
			break;
		case ImGuiStyleVar_WindowTitleAlign:
			style.WindowTitleAlign = value;
			break;
		case ImGuiStyleVar_FramePadding:
			style.FramePadding = value * screenScale;
			break;
		case ImGuiStyleVar_ItemSpacing:
			style.ItemSpacing = value * screenScale;
			break;
		case ImGuiStyleVar_ItemInnerSpacing:
			style.ItemInnerSpacing = value * screenScale;
			break;
		case ImGuiStyleVar_CellPadding:
			style.CellPadding = value * screenScale;
			break;
		case ImGuiStyleVar_TableAngledHeadersTextAlign:
			style.TableAngledHeadersTextAlign = value;
			break;
		case ImGuiStyleVar_ButtonTextAlign:
			style.ButtonTextAlign = value;
			break;
		case ImGuiStyleVar_SelectableTextAlign:
			style.SelectableTextAlign = value;
			break;
		case ImGuiStyleVar_SeparatorTextAlign:
			style.SeparatorTextAlign = value;
			break;
		case ImGuiStyleVar_SeparatorTextPadding:
			style.SeparatorTextPadding = value;
			break;
		default:
			break;
		}
	}
}