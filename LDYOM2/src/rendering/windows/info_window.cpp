#define IMGUI_DEFINE_MATH_OPERATORS
#include "info_window.h"
#include <fa_icons.h>
#include <fmt/format.h>
#include <localization.h>
#include <utils/imgui_configurate.h>
#include <window_manager.h>

void InfoWindow::renderContent(Window* window) {
	ImGui::TextUnformatted(fmt::format("{}: {}", _("info.version"), LDYOM_VERSION_STRING).c_str());
	ImGui::TextUnformatted(fmt::format("{}: {}", _("info.author_ldyom"), "SKIC").c_str());
	ImGui::TextUnformatted(fmt::format("{}: {}", _("info.mpack_author"), "SIZZZ").c_str());
	ImGui::Spacing();
	ImGui::TextUnformatted(
		fmt::format("{}: {}", _("info.special_thanks"), "Benjamin Strutui (Donation, testing)").c_str());

	ImGui::Spacing();
	float buttonWidth = ImGui::GetContentRegionAvail().x * 0.3f;
	ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - buttonWidth) * 0.5f);
	if (ImGui::Button(_("info.close").c_str(), ImVec2(buttonWidth, 0.f))) {
		window->close();
	}
}

InfoWindow::InfoWindow()
	: ModalPopupWindow(_("info.title", ICON_FA_INFO)) {
	setSize(400, 200);
	setPosition(640, 360);
	setPivot(0.5, 0.5);
	setRenderCallback(InfoWindow::renderContent);
}
