﻿#include "ProjectInfoWindow.h"

#include "imgui.h"
#include "Localization/Localization.h"
#include "fa.h"
#include "ProjectsService.h"
#include "fmt/core.h"

void Windows::ProjectInfoWindow::draw() {
	auto &local = Localization::getInstance();

	const auto & projectInfo = ProjectsService::getInstance().getCurrentProject().getProjectInfo();

	const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
	ImGui::SetNextWindowPos(ImVec2(displaySize.x / 2.f, displaySize.y / 2.f), ImGuiCond_Appearing, ImVec2(.5f, .5f));
	if (ImGui::Begin(fmt::format("{} {}", ICON_FA_ADDRESS_CARD, local.get("project_info.title")).c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {

		ImGui::InputText(local.get("project_info.name").c_str(), projectInfo->name, sizeof projectInfo->name);
		ImGui::InputText(local.get("project_info.author").c_str(), projectInfo->authorName, sizeof projectInfo->authorName);

		const auto sceneName = ProjectsService::getInstance().getCurrentProject().getScenes().contains(projectInfo->startScene) ?
			                       ProjectsService::getInstance().getCurrentProject().getScenes().at(projectInfo->startScene)->getName() : "";
		if (ImGui::BeginCombo(local.get("scenes.title").c_str(), sceneName)) {
			for (const auto &pair : ProjectsService::getInstance().getCurrentProject().getScenes()) {
				if (ImGui::Selectable(pair.second->getName(), pair.first == projectInfo->startScene)) {
					projectInfo->startScene = pair.first;
				}
			}
			ImGui::EndCombo();
		}
	}
	ImGui::End();
}