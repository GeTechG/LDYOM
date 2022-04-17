// ReSharper disable CppMemberFunctionMayBeStatic
#include "ProjectsWindowPopup.h"
#include "imgui.h"
#include "fmt/core.h"
#include "fa.h"
#include "ProjectsService.h"
#include "Localization/Localization.h"

void Windows::ProjectsWindowPopup::draw() {
	static auto& local = Localization::getInstance();

	static int selected_project = -1;

	const ImVec2 window_size = ImGui::GetIO().DisplaySize;

	if (ImGui::BeginPopupModal(fmt::format("{} {}", ICON_FA_THEATER_MASKS, local.get("projects.title")).c_str(), &this->show_, ImGuiWindowFlags_AlwaysAutoResize)) {

		ImGui::BeginChild("##projectsList", ImVec2(window_size.x / 3.0f, window_size.y / 3.0f), true);
		for (int i = 0; i < static_cast<int>(ProjectsService::getInstance().getProjectsName().size()); ++i) {
			auto projectName = ProjectsService::getInstance().getProjectsName().at(i);
			if (ImGui::Selectable(projectName.c_str(), selected_project == i)) {
				selected_project = i;
			}
		}
		ImGui::EndChild();
		// / (1/3 window) / (3 buttons) - 3.0
		const auto widthButton = window_size.x / 3.0f / 3.0f - 3.0f;
		ImGui::Button(local.get("projects.new_project").c_str(), ImVec2(widthButton, .0f));
		ImGui::SameLine();
		ImGui::Button(local.get("projects.load_project").c_str(), ImVec2(widthButton, .0f));
		ImGui::SameLine();
		ImGui::Button(local.get("projects.delete_project").c_str(), ImVec2(widthButton, .0f));

		ImGui::EndPopup();
	}
}

void Windows::ProjectsWindowPopup::setShow(bool show) {
	show_ = show;
}
