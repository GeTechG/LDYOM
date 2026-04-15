#include "imgui_configurate.h"
#include "fa_icons.h"
#include "imgui.h"
#include "logger.h"
#include "paths.h"
#include "settings.h"
#include "theme_loader.h"

#include "fa.cpp"
#include "imgui_hook.h"
#include "roboto.cpp"

#include <filesystem>
#include <fstream>
#include <iostream>

static const ImWchar* GetGlyphRanges() {
	static const ImWchar ranges[] = {
		0x0020,
		0x00FF, // Basic Latin + Latin Supplement
		0x0980,
		0x09FF, // Bengali
		0x2000,
		0x206F, // General Punctuation

		// Chinease
	    // 0x3000, 0x30FF, // CJK Symbols and Punctuations, Hiragana, Katakana
	    // 0x31F0, 0x31FF, // Katakana Phonetic Extensions
	    // 0xFF00, 0xFFEF, // Half-width characters
	    // 0xFFFD, 0xFFFD, // Invalid
	    // 0x4E00, 0x9FAF, // CJK Ideograms

		// Cyrillic
		0x0400,
		0x052F, // Cyrillic + Cyrillic Supplement
		0x2DE0,
		0x2DFF, // Cyrillic Extended-A
		0xA640,
		0xA69F, // Cyrillic Extended-B

		// Turkish
		0x011E,
		0x011F,
		0x015E,
		0x015F,
		0x0130,
		0x0131,
		0,
	};
	return &ranges[0];
}

void ImGuiConfigurate::update(int width, int height) {
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->Clear();
	screenScale = ImVec2(width / 1280.0f, height / 720.0f);
	const float rasterDensity = screenScale.y * getGlobalScale();
	constexpr float baseFontSize = 15.0f;
	constexpr float baseHeaderFontSize = baseFontSize * 1.2f;

	static constexpr ImWchar icon_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
	ImFontConfig config;
	config.GlyphExcludeRanges = icon_ranges;
	config.RasterizerDensity = rasterDensity;
	io.Fonts->AddFontFromMemoryCompressedTTF(roboto_compressed_data, roboto_compressed_size, baseFontSize, &config,
	                                         GetGlyphRanges());

	ImFontConfig config2;
	config2.MergeMode = true;
	config2.RasterizerDensity = rasterDensity;
	io.Fonts->AddFontFromMemoryCompressedTTF(FA_compressed_data, FA_compressed_size, baseFontSize, &config2,
	                                         icon_ranges);

	ImFontConfig headerConfig;
	headerConfig.RasterizerDensity = rasterDensity;
	headerFont = io.Fonts->AddFontFromMemoryCompressedTTF(roboto_compressed_data, roboto_compressed_size,
	                                                      baseHeaderFontSize, &headerConfig, GetGlyphRanges());

	ImGuiStyle* style = &ImGui::GetStyle();

	const std::string currentTheme = Settings::instance().getSetting<std::string>("theme", "");
	if (!currentTheme.empty()) {
		ThemeLoader::loadThemeFromJson(currentTheme);
	}

	style->FontScaleDpi = getUiScale();
}

float ImGuiConfigurate::getGlobalScale() { return Settings::instance().getSetting<float>("display_scale", 1.0f); }
ImVec2 ImGuiConfigurate::getScreenScale() { return screenScale; }
float ImGuiConfigurate::getUiScale() { return screenScale.y * getGlobalScale(); }
void ImGuiConfigurate::setGlobalScale(float scale) {
	Settings::instance().setSetting<float>("display_scale", scale);
	if (ImGui::GetCurrentContext()) {
		ImGui::GetStyle().FontScaleDpi = getUiScale();
	}
}
ImFont* ImGuiConfigurate::getHeaderFont() { return headerFont; }
