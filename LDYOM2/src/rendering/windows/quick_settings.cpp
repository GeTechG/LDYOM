#define IMGUI_DEFINE_MATH_OPERATORS
#include "quick_settings.h"
#include "hotkeys.h"
#include <fa_icons.h>
#include <fmt/format.h>
#include <imHotKey.h>
#include <localization.h>
#include <utils/imgui_configurate.h>
#include <utils/theme_loader.h>
#include <settings.h>

// Setup options for dropdowns
const float displayScales[] = {0.25f, 0.5f, 0.75f, 1.f, 1.1f, 1.25f, 1.5f, 1.75f, 2.f};

void QuickSettings::renderContent(Window* window) {
	float windowWidth = ImGui::GetContentRegionAvail().x;
	float windowHeight = ImGui::GetContentRegionAvail().y;

	float settingLabelWidth = windowWidth * 0.45f - ImGui::GetStyle().FramePadding.x;

	// Language
	ImGui::AlignTextToFramePadding();
	{
		const auto settingsName = _("quick_settings.language");
		ImGui::Text(settingsName.c_str());
		ImGui::SameLine(std::max(settingLabelWidth,
		                         ImGui::CalcTextSize(settingsName.c_str()).x + ImGui::GetStyle().FramePadding.x * 2));
	}
	const auto& languages = Localization::instance().getAvailableLocales();
	ImGui::SetNextItemWidth(-1.0f);
	if (ImGui::BeginCombo("##Language", Localization::instance().getCurrentLocale().c_str())) {
		for (int i = 0; i < (int)languages.size(); i++) {
			bool isSelected = languages[i] == Localization::instance().getCurrentLocale();
			if (ImGui::Selectable(languages[i].c_str(), isSelected)) {
				Localization::instance().setLocale(languages[i]);
			}
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	// Interface Theme
	ImGui::AlignTextToFramePadding();
	{
		const auto settingsName = _("quick_settings.interface_theme");
		ImGui::Text(settingsName.c_str());
		ImGui::SameLine(std::max(settingLabelWidth,
		                         ImGui::CalcTextSize(settingsName.c_str()).x + ImGui::GetStyle().FramePadding.x * 2));
	}
	const auto& themes = ThemeLoader::getAvailableThemes();
	const auto currentTheme = ThemeLoader::getCurrentThemeName();
	ImGui::SetNextItemWidth(-1.0f);
	if (ImGui::BeginCombo("##Theme", currentTheme.c_str())) {
		for (int i = 0; i < (int)themes.size(); i++) {
			bool isSelected = (themes[i] == currentTheme);
			if (ImGui::Selectable(themes[i].c_str(), isSelected))
				ThemeLoader::loadThemeFromJson(themes[i]);
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	// Display Scale
	ImGui::AlignTextToFramePadding();
	{
		const auto settingsName = _("quick_settings.display_scale");
		ImGui::Text(settingsName.c_str());
		ImGui::SameLine(std::max(settingLabelWidth,
		                         ImGui::CalcTextSize(settingsName.c_str()).x + ImGui::GetStyle().FramePadding.x * 2));
	}
	ImGui::SetNextItemWidth(-1.0f);
	const auto currentScale = fmt::format("{:.0f}%", ImGuiConfigurate::getGlobalScale() * 100.0f);
	if (ImGui::BeginCombo("##Scale", currentScale.c_str())) {
		for (int i = 0; i < IM_ARRAYSIZE(displayScales); i++) {
			bool isSelected = (std::abs(ImGuiConfigurate::getGlobalScale() - displayScales[i]) < FLT_EPSILON);
			const auto scaleText = fmt::format("{:.0f}%", displayScales[i] * 100.0f);
			if (ImGui::Selectable(scaleText.c_str(), isSelected))
				ImGuiConfigurate::setGlobalScale(displayScales[i]);
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	const auto hotkeysEditorTitle = _("hotkey_editor.title");
	if (ImGui::Button(_("quick_settings.edit_hotkeys").c_str(), ImVec2(-1.0f, 0.f))) {
		ImGui::OpenPopup(hotkeysEditorTitle.c_str());
	}

	// Hotkeys Editor
	auto& hotkeys = Hotkeys::instance().getHotkeys();
	ImHotKey::Edit(hotkeys.data(), hotkeys.size(), hotkeysEditorTitle.c_str(),
	               [](int index) { Hotkeys::instance().saveHotkey(index); });

	// Entity Creation Defaults
	ImGui::SeparatorText(_("quick_settings.entity_creation.section_header").c_str());

	bool autoAttach = Settings::instance().getSetting<bool>("editor.entities.auto_attach_objective_specific", true);
	if (ImGui::Checkbox(_("quick_settings.entity_creation.auto_attach.label").c_str(), &autoAttach)) {
		Settings::instance().setSetting<bool>("editor.entities.auto_attach_objective_specific", autoAttach);
	}
	if (ImGui::BeginItemTooltip()) {
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(_("quick_settings.entity_creation.auto_attach.tooltip").c_str());
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}

	ImGui::BeginDisabled(!autoAttach);
	bool autoLink = Settings::instance().getSetting<bool>("editor.entities.auto_link_spawn_objective", true);
	if (ImGui::Checkbox(_("quick_settings.entity_creation.auto_link.label").c_str(), &autoLink)) {
		Settings::instance().setSetting<bool>("editor.entities.auto_link_spawn_objective", autoLink);
	}
	if (ImGui::BeginItemTooltip()) {
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(_("quick_settings.entity_creation.auto_link.tooltip").c_str());
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
	if (!autoAttach) {
		ImGui::TextDisabled(_("quick_settings.entity_creation.auto_link.disabled_hint").c_str());
	}
	ImGui::EndDisabled();

	ImGui::Dummy(ImVec2(0.0f, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeight() -
	                              ImGui::GetStyle().FramePadding.y * 2));

	float buttonWidth = windowWidth * 0.3f;
	ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);

	if (ImGui::Button(_("quick_settings.close").c_str(), ImVec2(buttonWidth, 0.f))) {
		window->close();
	}
}

QuickSettings::QuickSettings()
	: ModalPopupWindow(_("quick_settings.title", ICON_FA_GEAR)) {
	setSize(450, 250);
	setSizeMin(300, 210);
	setPosition(640, 360);
	setPivot(0.5, 0.5);
	setRenderCallback(QuickSettings::renderContent);
}