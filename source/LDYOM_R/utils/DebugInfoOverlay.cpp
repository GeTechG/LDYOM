#include <imgui.h>

#include "ProjectPlayerService.h"
#include "ProjectsService.h"


void debugInfoOverlay() {
	constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
	if (ImGui::Begin("##debugInfoOverlay", nullptr, windowFlags)) {
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 16.5f);
		ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

		auto &currentProject = ProjectsService::getInstance().getCurrentProject();
		ImGui::Text("Project: %s", currentProject.getProjectInfo()->name.c_str());
		ImGui::Text("Scene: %s", currentProject.getCurrentScene()->getName().c_str());

		if (ProjectPlayerService::getInstance().isProjectRunning()) {
			const auto &currentObjective = ProjectPlayerService::getInstance().getCurrentObjective();
			if (currentObjective.has_value()) {
				ImGui::Text("Objective: %s", currentObjective.value()->getName().c_str());
			}
		}
		ImGui::PopTextWrapPos();
	}
	ImGui::End();
}
