#include "PopupScenes.h"

#include "imgui.h"
#include "fmt/core.h"
#include "fa.h"
#include "ProjectsService.h"
#include "Localization/Localization.h"

void Windows::PopupScenes::open() {
	AbstractWindow::open();
}

void scenesList(int& selected_scene, const ImVec2 window_size) {
	ImGui::BeginChild("##scenesList", ImVec2(window_size.x / 3.0f, window_size.y / 3.0f), true);
	for (const auto& pair : ProjectsService::getInstance().getCurrentProject().getScenes()) {
		if (ImGui::Selectable(pair.second->getName(), selected_scene == pair.first)) {
			selected_scene = pair.first;
		}
	}
	ImGui::EndChild();
}

void Windows::PopupScenes::draw() {
	static auto& local = Localization::getInstance();

	static int selected_scene = -1;

	const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
	ImGui::SetNextWindowPos(ImVec2(displaySize.x / 2.f, displaySize.y / 2.f), ImGuiCond_Appearing, ImVec2(.5f, .5f));
	if (ImGui::BeginPopupModal(fmt::format("{} {}", ICON_FA_LANDMARK, local.get("scenes.title")).c_str(), &this->isShow(), ImGuiWindowFlags_AlwaysAutoResize)) {

		scenesList(selected_scene, displaySize);
		// (1/3 window) / (3 buttons) - 3.0
		const auto widthButton = ImGui::GetContentRegionAvail().x / 3.0f - 3.0f;
		if (ImGui::Button(local.get("scenes.new_scene").c_str(), ImVec2(widthButton, .0f))) {
			ProjectsService::getInstance().getCurrentProject().newScene();
			selected_scene = ProjectsService::getInstance().getCurrentProject().getCurrentSceneIndex();
		}
		ImGui::SameLine();
		if (ImGui::Button(local.get("scenes.load_scene").c_str(), ImVec2(widthButton, .0f))) {
			ProjectsService::getInstance().getCurrentProject().changeScene(selected_scene);
		}
		ImGui::SameLine();

		ImGui::BeginDisabled(ProjectsService::getInstance().getCurrentProject().getScenes().size() <= 1);
		if (ImGui::Button(local.get("scenes.delete_scene").c_str(), ImVec2(widthButton, .0f))) {
			ImGui::OpenPopup(popupDelete_.getName());
		}
		ImGui::EndDisabled();

		this->popupDelete_.draw([] {
			ProjectsService::getInstance().getCurrentProject().deleteScene(selected_scene);
		});

		ImGui::EndPopup();
	}
}
