// ReSharper disable CppMemberFunctionMayBeStatic
#include "ProjectsWindowPopup.h"
#include "fa.h"
#include "imgui.h"
#include "PopupWarning.h"
#include "ProjectPlayerService.h"
#include "ProjectsService.h"
#include "Tasker.h"
#include "utils.h"
#include "fmt/core.h"
#include "Localization/Localization.h"

static int selected_project = -1;

Windows::ProjectsWindowPopup::ProjectsWindowPopup(): popupWarningLoadProject_("projects.load_project_warning"),
                                                     popupWarningDeleteProject_("projects.delete_project_warning") {
	this->missionIcon_ = utils::LoadTextureRequiredFromFile(L"LDYOM\\Resources\\mission_icon.png").value();
	ProjectsService::getInstance().onUpdate().connect([] {
		selected_project = -1;
	});
}

void projectsList(const ImVec2 windowSize, const ProjectsInfos &projectInfos) {
	ImGui::BeginChild("##projectsList", ImVec2(windowSize.x / 3.0f, windowSize.y / 3.0f), true);
	for (int i = 0; i < static_cast<int>(projectInfos.size()); ++i) {
		const auto projectInfo = projectInfos.at(i).get();
		if (ImGui::Selectable(projectInfo->name.c_str(), selected_project == i)) {
			selected_project = i;
		}
	}

	ImGui::EndChild();
}

void Windows::ProjectsWindowPopup::projectInfo(Localization &local, const ProjectsInfos &projectInfos) const {
	static const float BASE_WIDTH = ImGui::GetFontSize() * 18.75f;

	ImGui::BeginChild("##projectInfo", ImVec2(BASE_WIDTH + 40.0f, ImGui::GetItemRectSize().y), true);

	if (selected_project != -1) {
		const auto projectInfo = projectInfos.at(selected_project).get();
		Texture *icon;
		if (projectInfo->icon.has_value())
			icon = projectInfo->icon.value().get();
		else
			icon = this->missionIcon_.get();

		const float scaleX = BASE_WIDTH / static_cast<float>(icon->getWidth());
		const float scaleY = (BASE_WIDTH - 100.0f) / static_cast<float>(icon->getHeight());

		const float scale = min(scaleX, scaleY);

		const auto size = ImVec2(static_cast<float>(icon->getWidth()) * scale,
		                         static_cast<float>(icon->getHeight()) * scale);

		ImGui::Dummy(ImVec2(5.0f, 0.0f));
		ImGui::SameLine();
		ImGui::Image(icon->getTexture(), size);
		ImGui::PushTextWrapPos(BASE_WIDTH);
		ImGui::Text(fmt::format("{}: {}", local.get("project_info.name"), projectInfo->name).c_str());
		ImGui::Text(fmt::format("{}: {}", local.get("project_info.author"), projectInfo->authorName).c_str());
		ImGui::PopTextWrapPos();
	}

	ImGui::EndChild();
}

void Windows::ProjectsWindowPopup::draw() {
	static auto &local = Localization::getInstance();

	const ImVec2 window_size = ImGui::GetIO().DisplaySize;

	if (ImGui::BeginPopupModal(fmt::format("{} {}", ICON_FA_THEATER_MASKS, local.get("projects.title")).c_str(),
	                           &this->show_, ImGuiWindowFlags_AlwaysAutoResize)) {
		if (ImGui::BeginTabBar("projects")) {
			static int tabId = 0;

			if (ImGui::BeginTabItem(local.get("projects.development").c_str())) {
				if (tabId != 0) {
					selected_project = -1;
					tabId = 0;
				}

				projectsList(window_size, ProjectsService::getInstance().getProjectsInfos());

				ImGui::SameLine();
				projectInfo(local, ProjectsService::getInstance().getProjectsInfos());

				// / (1/3 window) / (3 buttons) - 3.0
				const auto widthButton = window_size.x / 3.0f / 4.0f - 3.0f;
				if (ImGui::Button(local.get("projects.save_project").c_str(), ImVec2(widthButton, .0f))) {
					ProjectsService::getInstance().saveCurrentProject();
				}
				ImGui::SameLine();
				ImGui::BeginDisabled(selected_project == -1);
				if (ImGui::Button(local.get("projects.load_project").c_str(), ImVec2(widthButton, .0f))) {
					ImGui::OpenPopup(popupWarningLoadProject_.getName().c_str());
				}
				ImGui::SameLine();
				if (ImGui::Button(local.get("projects.make_production").c_str(), ImVec2(widthButton, .0f))) {
					ProjectsService::getInstance().makeProjectProduction(selected_project);
				}
				ImGui::SameLine();
				if (ImGui::Button(local.get("projects.delete_project").c_str(), ImVec2(widthButton, .0f))) {
					ImGui::OpenPopup(popupWarningDeleteProject_.getName().c_str());
				}
				ImGui::EndDisabled();

				popupWarningLoadProject_.draw([&] {
					ProjectsService::getInstance().loadProject(selected_project);
				});
				popupWarningDeleteProject_.draw([&] {
					ProjectsService::getInstance().deleteProject(selected_project);
				});

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(local.get("projects.production").c_str())) {
				if (tabId != 1) {
					selected_project = -1;
					tabId = 1;
				}

				projectsList(window_size, ProjectsService::getInstance().getProductionProjectsInfos());

				ImGui::SameLine();
				projectInfo(local, ProjectsService::getInstance().getProductionProjectsInfos());

				// / (1/3 window) / (3 buttons) - 3.0
				const auto widthButton = window_size.x / 3.0f / 3.0f - 3.0f;
				ImGui::BeginDisabled(selected_project == -1);
				if (ImGui::Button(local.get("projects.play_project").c_str(), ImVec2(widthButton, .0f))) {
					ImGui::OpenPopup(popupWarningLoadProject_.getName().c_str());
				}
				ImGui::SameLine();
				if (ImGui::Button(local.get("projects.delete_project").c_str(), ImVec2(widthButton, .0f))) {
					ImGui::OpenPopup(popupWarningDeleteProject_.getName().c_str());
				}
				ImGui::EndDisabled();

				popupWarningDeleteProject_.draw([&] {
					ProjectsService::getInstance().deleteProductionProject(selected_project);
				});

				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

		ImGui::EndPopup();
	}
}

void Windows::ProjectsWindowPopup::setShow(bool show) {
	show_ = show;
}
