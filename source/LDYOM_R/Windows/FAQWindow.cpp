#include "FAQWindow.h"

#include "fa.h"
#include "imgui.h"
#include "Localization/Localization.h"
#include <fmt/core.h>

void Windows::FAQWindow::draw() {
	static auto& local = Localization::getInstance();

	const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
	ImGui::SetNextWindowPos(ImVec2(displaySize.x / 2.f, displaySize.y / 2.f), ImGuiCond_Appearing, ImVec2(.5f, .5f));
	ImGui::SetNextWindowSize(ImVec2(ImGui::GetFontSize() * 30.f, ImGui::GetFontSize() * 20.f), ImGuiCond_Appearing);
	if (ImGui::Begin(fmt::format("{} {}", ICON_FA_QUESTION_CIRCLE, local.get("faq.title")).c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::TextWrapped(local.get("faq.invalid_rename_project").c_str());
		ImGui::Separator();
		ImGui::TextWrapped(local.get("faq.error_1").c_str());
		ImGui::Separator();
		ImGui::TextWrapped(local.get("faq.error_2").c_str());
	}
	ImGui::End();
}
