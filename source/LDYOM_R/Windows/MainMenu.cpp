#include "imgui.h"
#include "fa.h"
#include <fmt/core.h>

#include "SettingsWindow.h"
#include "Localization/Localization.h"

#include "MainMenu.h"

#include "EntitiesWindow.h"
#include "ObjectivesWindow.h"
#include "ProjectInfoWindow.h"
#include "ProjectPlayerService.h"
#include "ProjectsService.h"
#include "Tasker.h"
#include "WindowsRenderService.h"

namespace Windows
{

	void MainMenu::draw()
	{
		static auto& local = Localization::getInstance();

		const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
		ImGui::SetNextWindowPos(ImVec2(displaySize.x / 2.f, displaySize.y / 2.f), ImGuiCond_Appearing, ImVec2(.5f, .5f));
		if (ImGui::Begin(local.get("main_menu.title").c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (ImGui::Button(fmt::format("{} {}", ICON_FA_TASKS, local.get("objective.objectives")).c_str(), ImVec2(200.0f, .0f)))
			{
				WindowsRenderService::getInstance().toggleWindow<MainMenu>(false);
				WindowsRenderService::getInstance().toggleWindow<ObjectivesWindow>(true);
			}
            if (ImGui::Button(fmt::format("{} {}", ICON_FA_SHAPES, local.get("entities.title")).c_str(), ImVec2(200.0f, .0f)))
            {
				WindowsRenderService::getInstance().toggleWindow<MainMenu>(false);
				WindowsRenderService::getInstance().toggleWindow<EntitiesWindow>(true);
            }

			ImGui::Separator();

			if (ImGui::Button(fmt::format("{} {}", ICON_FA_THEATER_MASKS, local.get("projects.title")).c_str(), ImVec2(200.0f, .0f)))
			{
				this->projectsWindowPopup_.setShow(true);
				ImGui::OpenPopup(fmt::format("{} {}", ICON_FA_THEATER_MASKS, local.get("projects.title")).c_str());
			}
			if (ImGui::Button(fmt::format("{} {}", ICON_FA_ADDRESS_CARD, local.get("project_info.title")).c_str(), ImVec2(200.0f, .0f)))
			{
				WindowsRenderService::getInstance().toggleWindow<MainMenu>(false);
				WindowsRenderService::getInstance().toggleWindow<ProjectInfoWindow>(true);
			}

			if (ImGui::Button(fmt::format("{} {}", ICON_FA_SAVE, local.get("general.save")).c_str(), ImVec2(200.0f, .0f)))
			{
				ProjectsService::getInstance().saveCurrentProject();
			}

			if (ImGui::Button(fmt::format("{} {}", ICON_FA_LANDMARK, local.get("scenes.title")).c_str(), ImVec2(200.0f, .0f)))
			{
				this->popupScenes_.open();
				ImGui::OpenPopup(fmt::format("{} {}", ICON_FA_LANDMARK, local.get("scenes.title")).c_str());
			}

			if (ImGui::Button(fmt::format("{} {}", ICON_FA_PLAY, local.get("general.play")).c_str(), ImVec2(200.0f, .0f)))
			{
				Tasker::getInstance().addTask("playProjectTask", []() -> ktwait {
					co_await ProjectPlayerService::getInstance().startProject(ProjectsService::getInstance().getCurrentProject().getCurrentSceneIndex(), 0);
				});
			}

			ImGui::Separator();

			if (ImGui::Button(fmt::format("{} {}", ICON_FA_COGS, local.get("settings.title")).c_str(), ImVec2(200.0f, .0f)))
			{
				WindowsRenderService::getInstance().toggleWindow<MainMenu>(false);
				WindowsRenderService::getInstance().toggleWindow<SettingsWindow>(true);
			}

			this->projectsWindowPopup_.draw();
			this->popupScenes_.draw();

		}
		ImGui::End();
	}
}
