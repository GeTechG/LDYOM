#define IMGUI_DEFINE_MATH_OPERATORS
#include "welcome.h"
#include "quick_settings.h"
#include <localization.h>
#include <settings.h>
#include <window_manager.h>

void Welcome::renderContent(Welcome* self) {
	// Title
	const auto title = _("welcome.title");
	float windowWidth = ImGui::GetContentRegionAvail().x;
	float titleWidth = ImGui::CalcTextSize(title.c_str()).x;
	ImGui::SetCursorPosX((windowWidth - titleWidth) * 0.5f);
	ImGui::Text("%s", title.c_str());

	ImGui::Spacing();

	// Intro paragraph
	ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + windowWidth);
	ImGui::TextUnformatted(_("welcome.intro").c_str());
	ImGui::PopTextWrapPos();

	ImGui::Spacing();

	// Preset choice explanation
	ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + windowWidth);
	ImGui::TextUnformatted(_("welcome.preset_choice_explain").c_str());
	ImGui::PopTextWrapPos();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// Two side-by-side preset cards. Use InvisibleButton + manual drawList rendering — the
	// previous BeginChild/IsItemClicked pattern was flaky (ImGui's child-window-as-item
	// click detection is order/focus-dependent and dropped clicks unpredictably).
	const float availWidth = ImGui::GetContentRegionAvail().x;
	const float cardWidth = (availWidth - ImGui::GetStyle().ItemSpacing.x) * 0.5f;
	const float cardHeight = 100.0f;
	const ImVec2 cardSize = ImVec2(cardWidth, cardHeight);

	auto renderCard = [&](const char* id, const std::string& label, const std::string& desc, int presetIndex) {
		ImGui::PushID(id);
		const ImVec2 cardStart = ImGui::GetCursorScreenPos();
		const bool clicked = ImGui::InvisibleButton("##card_btn", cardSize);
		const bool hovered = ImGui::IsItemHovered();
		const bool active = ImGui::IsItemActive();

		ImDrawList* dl = ImGui::GetWindowDrawList();
		const ImU32 bgColor = ImGui::GetColorU32(active   ? ImGuiCol_FrameBgActive
		                                          : hovered ? ImGuiCol_FrameBgHovered
		                                                    : ImGuiCol_FrameBg);
		const ImU32 borderColor = ImGui::GetColorU32(ImGuiCol_Border);
		const ImVec2 cardEnd = ImVec2(cardStart.x + cardWidth, cardStart.y + cardHeight);
		dl->AddRectFilled(cardStart, cardEnd, bgColor, 4.0f);
		dl->AddRect(cardStart, cardEnd, borderColor, 4.0f);

		const float padding = 12.0f;
		const ImU32 textColor = ImGui::GetColorU32(ImGuiCol_Text);
		const ImVec2 labelSize = ImGui::CalcTextSize(label.c_str());
		dl->AddText(ImVec2(cardStart.x + (cardWidth - labelSize.x) * 0.5f, cardStart.y + padding), textColor,
		            label.c_str());
		dl->AddText(ImGui::GetFont(), ImGui::GetFontSize(),
		            ImVec2(cardStart.x + padding, cardStart.y + padding + labelSize.y + 6.0f), textColor,
		            desc.c_str(), nullptr, cardWidth - padding * 2.0f);

		ImGui::PopID();
		if (clicked) {
			self->applyPreset(presetIndex);
		}
	};

	renderCard("preset_simple", _("welcome.preset.simple.label"), _("welcome.preset.simple.description"), 0);
	ImGui::SameLine();
	renderCard("preset_advanced", _("welcome.preset.advanced.label"), _("welcome.preset.advanced.description"), 1);
}

void Welcome::applyPreset(int preset) {
	bool value = (preset == 0); // Simple = true, Advanced = false
	Settings::instance().setSetting<bool>("editor.entities.auto_attach_objective_specific", value);
	Settings::instance().setSetting<bool>("editor.entities.auto_link_spawn_objective", value);

	auto qs = WindowManager::instance().getWindowAs<QuickSettings>("quick_settings");
	if (qs.has_value() && qs.value() != nullptr) {
		qs.value()->setOnboardingMode(true);
	}

	// Order matters: open QS first (ModalPopupWindow::open sets m_open=true synchronously,
	// so the WindowManager render loop stays alive after we close ourselves below), then
	// close this window. If the user later dismisses QS via Esc and presses I again, the
	// onboarding gate in application.cpp will reopen welcome — onboarding.completed is only
	// written by the QS Continue handler.
	WindowManager::instance().openWindow("quick_settings");
	close();
}

Welcome::Welcome()
	: Window(_("welcome.title")) {
	setSize(600, 400);
	setSizeMin(480, 320);
	setPosition(640, 360);
	setPivot(0.5f, 0.5f);
	setFlags(ImGuiWindowFlags_NoCollapse);
	setRenderCallback<Welcome>(renderContent);
}

void Welcome::open() {
	m_selectedPreset = -1;
	Window::open();
}
