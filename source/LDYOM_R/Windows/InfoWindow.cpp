#include "InfoWindow.h"

#include "fa.h"
#include "imgui.h"
#include "Localization/Localization.h"
#include <fmt/core.h>

#include "constants.h"

void Windows::InfoWindow::draw() {
	static auto& local = Localization::getInstance();

	const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
	ImGui::SetNextWindowPos(ImVec2(displaySize.x / 2.f, displaySize.y / 2.f), ImGuiCond_Appearing, ImVec2(.5f, .5f));
	ImGui::SetNextWindowSize(ImVec2(ImGui::GetFontSize() * 30.f, ImGui::GetFontSize() * 10.f), ImGuiCond_Appearing);
	if (ImGui::Begin(fmt::format("{} {}", ICON_FA_INFO, local.get("info.title")).c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::TextWrapped(local.get("info.version").c_str(), LDYOM_VERSION_STRING);
		ImGui::TextWrapped(local.get("info.author_ldyom").c_str(), "SKIC");
		ImGui::TextWrapped(local.get("info.mpack_author").c_str(), "SIZZZ");
		ImGui::TextWrapped(local.get("info.localization_author").c_str());

		const char* thanks = "Jasmijn Wellner (gvx), Nelarius, Alexey Generalov, Ivan Kogotko";

		ImGui::TextWrapped(local.get("info.special_thanks").c_str(), thanks);
	}
	ImGui::End();
}
