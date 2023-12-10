#include <fmt/core.h>
#include "fa.h"
#include "imgui.h"

#include "SettingsWindow.h"
#include "Localization/Localization.h"

#include "MainMenu.h"

#include "ConsoleWindow.h"
#include "EntitiesWindow.h"
#include "FAQWindow.h"
#include "InfoWindow.h"
#include "ObjectivesWindow.h"
#include "ProjectInfoWindow.h"
#include "ProjectPlayerService.h"
#include "ProjectsService.h"
#include "Tasker.h"
#include "ToolsWindow.h"
#include "WindowsRenderService.h"

namespace Windows {
	void MainMenu::draw() {
		static auto &local = Localization::getInstance();

		const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
		ImGui::SetNextWindowPos(ImVec2(displaySize.x / 2.f, displaySize.y / 2.f), ImGuiCond_Appearing,
		                        ImVec2(.5f, .5f));
		if (ImGui::Begin(local.get("main_menu.title").c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
			if (ImGui::Button(fmt::format("{} {}", ICON_FA_TASKS, local.get("objective.objectives")).c_str(),
			                  ImVec2(200.0f, .0f))) {
				WindowsRenderService::getInstance().replaceWindow<MainMenu, ObjectivesWindow>();
			}
			if (ImGui::Button(fmt::format("{} {}", ICON_FA_SHAPES, local.get("entities.title")).c_str(),
			                  ImVec2(200.0f, .0f))) {
				WindowsRenderService::getInstance().replaceWindow<MainMenu, EntitiesWindow>();
			}

			ImGui::Separator();

			if (ImGui::Button(fmt::format("{} {}", ICON_FA_THEATER_MASKS, local.get("projects.title")).c_str(),
			                  ImVec2(200.0f, .0f))) {
				this->projectsWindowPopup_.setShow(true);
				ImGui::OpenPopup(fmt::format("{} {}", ICON_FA_THEATER_MASKS, local.get("projects.title")).c_str());
			}
			if (ImGui::Button(fmt::format("{} {}", ICON_FA_ADDRESS_CARD, local.get("project_info.title")).c_str(),
			                  ImVec2(200.0f, .0f))) {
				WindowsRenderService::getInstance().replaceWindow<MainMenu, ProjectInfoWindow>();
			}

			if (ImGui::Button(fmt::format("{} {}", ICON_FA_SAVE, local.get("general.save")).c_str(),
			                  ImVec2(200.0f, .0f))) {
				ProjectsService::getInstance().saveCurrentProject();
			}

			if (ImGui::Button(fmt::format("{} {}", ICON_FA_LANDMARK, local.get("scenes.title")).c_str(),
			                  ImVec2(200.0f, .0f))) {
				this->popupScenes_.open();
				ImGui::OpenPopup(fmt::format("{} {}", ICON_FA_LANDMARK, local.get("scenes.title")).c_str());
			}

			if (ImGui::Button(fmt::format("{} {}", ICON_FA_SLIDERS_H, local.get("scene_settings.title")).c_str(),
			                  ImVec2(200.0f, .0f))) {
				this->sceneSettings_.setShow(true);
				ImGui::OpenPopup(fmt::format("{} {}", ICON_FA_SLIDERS_H, local.get("scene_settings.title")).c_str());
			}

			if (ImGui::IsItemHovered()) {
				auto relations = fmt::format("{} {}", ICON_FA_USERS, local.get("scene_settings.group_relations"));
				auto time = fmt::format("{} {}", ICON_FA_CLOCK, local.get("general.game_time"));
				auto traffic = fmt::format("{} {}", ICON_FA_TRAFFIC_CONE, local.get("scene_settings.ped_car_traffic"));
				auto wanted = fmt::format("{} {}", ICON_FA_STAR, local.get("general.wanted"));
				auto weather = fmt::format("{} {}", ICON_FA_SUN_CLOUD, local.get("general.weather"));
				auto riotMode = fmt::format("{} {}", ICON_FA_SKULL_CROSSBONES, local.get("general.riot_mode"));
				ImGui::SetTooltip(fmt::format("{}\n{}\n{}\n{}\n{}\n{}", relations, time, traffic, wanted, weather,
				                              riotMode).c_str());
			}

			if (ImGui::Button(fmt::format("{} {}", ICON_FA_PLAY, local.get("general.play")).c_str(),
			                  ImVec2(200.0f, .0f))) {
				Tasker::getInstance().addTask("playProjectTask", []() -> ktwait {
					co_await ProjectPlayerService::getInstance().startProject(
						ProjectsService::getInstance().getCurrentProject().getCurrentSceneIndex(), 0);
				});
			}

			ImGui::Separator();

			if (ImGui::Button(fmt::format("{} {}", ICON_FA_QUESTION_CIRCLE, local.get("faq.title")).c_str(),
			                  ImVec2(200.0f, .0f))) {
				WindowsRenderService::getInstance().replaceWindow<MainMenu, FAQWindow>();
			}

			if (ImGui::Button(fmt::format("{} {}", ICON_FA_INFO, local.get("info.title")).c_str(),
			                  ImVec2(200.0f, .0f))) {
				WindowsRenderService::getInstance().replaceWindow<MainMenu, InfoWindow>();
			}

			if (ImGui::Button(fmt::format("{} {}", ICON_FA_TOOLS, local.get("tools.title")).c_str(),
			                  ImVec2(200.0f, .0f))) {
				WindowsRenderService::getInstance().replaceWindow<MainMenu, ToolsWindow>();
			}

			if (ImGui::Button(fmt::format("{} {}", ICON_FA_COGS, local.get("settings.title")).c_str(),
			                  ImVec2(200.0f, .0f))) {
				WindowsRenderService::getInstance().replaceWindow<MainMenu, SettingsWindow>();
			}

			if (ImGui::Button(fmt::format("{} {}", ICON_FA_SCROLL, local.get("console_window.title")).c_str(),
			                  ImVec2(200.0f, .0f))) {
				WindowsRenderService::getInstance().replaceWindow<MainMenu, ConsoleWindow>();
			}

			this->projectsWindowPopup_.draw();
			this->popupScenes_.draw();
			this->sceneSettings_.draw();
		}
		ImGui::End();
	}
}
