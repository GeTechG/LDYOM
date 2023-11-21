#include "SceneSettingsPopup.h"

#include <CClock.h>
#include <CWeather.h>
#include "fa.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "ProjectsService.h"
#include "utils.h"
#include "../Data/SceneSettings.h"
#include "fmt/core.h"
#include "Localization/Localization.h"

void Windows::SceneSettingsWindowPopup::setShow(bool show) {
	this->show_ = show;
	auto &sceneSettings = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getSceneSettings();
	CClock::SetGameClock(sceneSettings.time[0], sceneSettings.time[1], 0);
	CWeather::ForceWeatherNow(static_cast<short>(sceneSettings.weather));
}

void Windows::SceneSettingsWindowPopup::draw() {
	static auto &local = Localization::getInstance();

	auto &sceneSettings = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getSceneSettings();

	if (ImGui::BeginPopupModal(fmt::format("{} {}", ICON_FA_SLIDERS_H, local.get("scene_settings.title")).c_str(),
	                           &this->show_, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::InputText(local.get("general.name").c_str(),
		                 &ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getName());

		utils::ToggleButton(local.get("scene_settings.apply").c_str(),
		                    &ProjectsService::getInstance().getCurrentProject().getCurrentScene()->
		                                                    isToggleSceneSettings());

		ImGui::Dummy(ImVec2(0.f, ImGui::GetFontSize()));

		ImGui::BeginDisabled(
			!ProjectsService::getInstance().getCurrentProject().getCurrentScene()->isToggleSceneSettings());

		ImGui::Text(fmt::format("{} {}", ICON_FA_USERS, local.get("scene_settings.group_relations")).c_str());
		ImGui::BeginChild("##relations", ImVec2(0.f, (ImGui::GetFontSize() + 4.f) * 8.f), true);
		ImGui::PushItemWidth(125);
		for (int i = 0; i < 8; i++) {
			if (ImGui::TreeNode(fmt::format("{} {}", local.get("actor.group"), i + 1).c_str())) {
				for (int j = 0; j < 9; j++) {
					if (sceneSettings.groupRelations[i][j] != -1) {
						auto groupName = j == 0
							                 ? local.get("general.player")
							                 : fmt::format("{} {}", local.get("actor.group"), j);
						ImGui::SliderInt(groupName.c_str(), &sceneSettings.groupRelations[i][j], 0, 4,
						                 local.getArray("scene_settings.relationship_types").at(
							                 sceneSettings.groupRelations[i][j]).c_str());
					}
				}
				ImGui::TreePop();
			}
		}
		ImGui::PopItemWidth();
		ImGui::EndChild();

		bool timeChanged = false;

		ImGui::Dummy(ImVec2(0.f, ImGui::GetFontSize()));

		ImGui::Text(fmt::format("{} {}", ICON_FA_CLOCK, local.get("general.game_time")).c_str());
		constexpr auto hourMin = static_cast<ImS8>(0);
		constexpr auto hourMax = static_cast<ImS8>(23);
		ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5.f);
		timeChanged |= ImGui::SliderScalar(local.get("general.hours").c_str(), ImGuiDataType_S8, &sceneSettings.time[0],
		                                   &hourMin, &hourMax);
		ImGui::SameLine();
		constexpr auto minutesMin = static_cast<ImS8>(0);
		constexpr auto minutesMax = static_cast<ImS8>(59);
		ImGui::SetNextItemWidth(ImGui::GetFontSize() * 5.f);
		timeChanged |= ImGui::SliderScalar(local.get("general.minutes").c_str(), ImGuiDataType_S8,
		                                   &sceneSettings.time[1], &minutesMin, &minutesMax);

		if (timeChanged) {
			CClock::SetGameClock(sceneSettings.time[0], sceneSettings.time[1], 0);
		}

		ImGui::Dummy(ImVec2(0.f, ImGui::GetFontSize()));

		ImGui::DragFloat(fmt::format("{} {}", ICON_FA_TRAFFIC_CONE, local.get("general.ped_traffic")).c_str(),
		                 &sceneSettings.trafficPed, 0.01f, 0.f, 2.f);
		ImGui::DragFloat(fmt::format("{} {}", ICON_FA_TRAFFIC_CONE, local.get("general.car_traffic")).c_str(),
		                 &sceneSettings.trafficCar, 0.01f, 0.f, 2.f);

		ImGui::Dummy(ImVec2(0.f, ImGui::GetFontSize()));

		ImGui::SliderInt(fmt::format("{} {}", ICON_FA_STAR, local.get("general.min_wanted")).c_str(),
		                 &sceneSettings.wantedMin, 0, 6);
		ImGui::SliderInt(fmt::format("{} {}", ICON_FA_STAR, local.get("general.max_wanted")).c_str(),
		                 &sceneSettings.wantedMax, 0, 6);

		ImGui::Dummy(ImVec2(0.f, ImGui::GetFontSize()));

		if (ImGui::DragInt(fmt::format("{} {}", ICON_FA_SUN_CLOUD, local.get("general.weather")).c_str(),
		                   &sceneSettings.weather, 0.1f, 0, 255)) {
			CWeather::ForceWeatherNow(static_cast<short>(sceneSettings.weather));
		}

		ImGui::Dummy(ImVec2(0.f, ImGui::GetFontSize()));

		utils::ToggleButton(fmt::format("{} {}", ICON_FA_SKULL_CROSSBONES, local.get("general.riot_mode")).c_str(),
		                    &sceneSettings.riot);

		ImGui::EndDisabled();

		ImGui::EndPopup();
	}
}
