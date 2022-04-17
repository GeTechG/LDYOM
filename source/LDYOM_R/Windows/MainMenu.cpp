#include "imgui.h"
#include "fa.h"
#include <fmt/core.h>

#include "SettingsWindow.h"
#include "Localization/Localization.h"

#include "MainMenu.h"

#include "EntitiesWindow.h"
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
            if (ImGui::Button(fmt::format("{} {}", ICON_FA_SHAPES, local.get("entities.title")).c_str(), ImVec2(200.0f, .0f)))
            {
				WindowsRenderService::getInstance().toggleWindow<MainMenu>(false);
				WindowsRenderService::getInstance().toggleWindow<EntitiesWindow>(true);
            }
			if (ImGui::Button(fmt::format("{} {}", ICON_FA_THEATER_MASKS, local.get("projects.title")).c_str(), ImVec2(200.0f, .0f)))
			{
				this->projectsWindowPopup_.setShow(true);
				ImGui::OpenPopup(fmt::format("{} {}", ICON_FA_THEATER_MASKS, local.get("projects.title")).c_str());
			}
			if (ImGui::Button(fmt::format("{} {}", ICON_FA_COGS, local.get("settings.title")).c_str(), ImVec2(200.0f, .0f)))
			{
				WindowsRenderService::getInstance().toggleWindow<MainMenu>(false);
				WindowsRenderService::getInstance().toggleWindow<SettingsWindow>(true);
			}

			this->projectsWindowPopup_.draw();

		}
		ImGui::End();
	}
}
