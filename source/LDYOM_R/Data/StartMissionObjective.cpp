#include "StartMissionObjective.h"

#include <CCamera.h>
#include <CTheScripts.h>
#include <CWorld.h>
#include <extensions/ScriptCommands.h>

#include "fa.h"
#include "imgui.h"
#include "ModelsService.h"
#include "strUtils.h"
#include "utils.h"

StartMissionObjective::StartMissionObjective(void* _new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.start_mission"));
	strlcat(this->name_.data(), suffix.c_str(), sizeof this->name_);
}

void StartMissionObjective::draw(Localization& local) {
	utils::ToggleButton(local.get("mission.show_name").c_str(), &this->showNameMission_);
	ImGui::BeginDisabled(!this->showNameMission_);

	ImGui::InputText(local.get("mission.name_mission").c_str(), this->nameMission_.data(), this->nameMission_.size());
	ImGui::SliderInt(local.get("general.text_style").c_str(), &this->styleTypeName_, 0, 6, local.getArray("general.text_styles").at(this->styleTypeName_).c_str());
	ImGui::DragFloat(local.get("general.time").c_str(), &this->nameMissionTime_, 0.1f, 0.f, FLT_MAX);

	ImGui::EndDisabled();

	ImGui::Separator();

	utils::ToggleButton(local.get("general.fade").c_str(), &this->fade_);
	ImGui::BeginDisabled(!this->fade_);

	ImGui::DragFloat(local.get("general.fade_in_time").c_str(), &this->fadeInTime_, 0.1f, 0.f, FLT_MAX);
	ImGui::DragFloat(local.get("general.fade_out_time").c_str(), &this->fadeOutTime_, 0.1f, 0.f, FLT_MAX);

	ImGui::EndDisabled();

	ImGui::Separator();

	ImGui::Text(fmt::format("{} {}", ICON_FA_USERS, local.get("scene_settings.group_relations")).c_str());
	ImGui::BeginChild("##relations", ImVec2(0.f, (ImGui::GetFontSize() + 4.f) * 8.f), true);
	ImGui::PushItemWidth(125);
	for (int i = 0; i < 8; i++) {
		if (ImGui::TreeNode(fmt::format("{} {}", local.get("actor.group"), i + 1).c_str())) {
			for (int j = 0; j < 9; j++) {
				if (this->groupRelations[i][j] != -1) {
					auto groupName = j == 0 ? local.get("general.player") : fmt::format("{} {}", local.get("actor.group"), j);
					ImGui::SliderInt(groupName.c_str(), &this->groupRelations[i][j], 0, 4, local.getArray("scene_settings.relationship_types").at(this->groupRelations[i][j]).c_str());
				}
			}
			ImGui::TreePop();
		}
	}
	ImGui::PopItemWidth();
	ImGui::EndChild();

	ImGui::Separator();

	ImGui::SliderInt(fmt::format("{} {}", ICON_FA_STAR, local.get("general.min_wanted")).c_str(), &this->wantedMin, 0, 6);
	ImGui::SliderInt(fmt::format("{} {}", ICON_FA_STAR, local.get("general.max_wanted")).c_str(), &this->wantedMax, 0, 6);

	ImGui::Separator();

	utils::ToggleButton(fmt::format("{} {}", ICON_FA_SKULL_CROSSBONES, local.get("general.riot_mode")).c_str(), &this->riot);
}

ktwait StartMissionObjective::execute(Scene* scene, Result& result, ktcoro_tasklist& tasklist) {
	using namespace plugin;

	if (this->fade_) {
		TheCamera.Fade(this->fadeInTime_, 0);
		co_await std::chrono::duration<float>(this->fadeInTime_);
	}

	if (CTheScripts::ScriptSpace[CTheScripts::OnAMissionFlag] == 1) {
		result.setError(2, "Another mission is currently running. Complete the mission before starting a new one.");
		co_return;
	}

	CTheScripts::ScriptSpace[CTheScripts::OnAMissionFlag] = 1;

	Command<Commands::SET_LA_RIOTS>(this->riot);
	CWorld::Players[0].m_PlayerData.m_pWanted->SetWantedLevelNoDrop(this->wantedMin);
	CWanted::SetMaximumWantedLevel(this->wantedMax);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 9; j++) {
			if (this->groupRelations[i][j] != -1) {
				if (j == 0) {
					Command<Commands::SET_RELATIONSHIP>((int)this->groupRelations[i][j], 24 + i, 0);
					Command<Commands::SET_RELATIONSHIP>((int)this->groupRelations[i][j], 24 + i, 23);
					Command<Commands::SET_RELATIONSHIP>((int)this->groupRelations[i][j], 23, 24 + i);
				}
				else {
					if (i != j - 1) {
						Command<Commands::SET_RELATIONSHIP>((int)this->groupRelations[i][j], 24 + i, 24 + j - 1);
					}
				}
			}
		}
	}

	if (this->showNameMission_) {
		static std::string cp1251Text;
		cp1251Text = utf8ToCp1251(this->nameMission_.data());
		gxtEncode(cp1251Text);
		CMessages::AddBigMessage(const_cast<char*>(cp1251Text.c_str()), static_cast<unsigned>(this->nameMissionTime_ * 1000.f), ModelsService::getInstance().getTextStyles().at(this->styleTypeName_));
	}

	if (this->fade_) 
		TheCamera.Fade(this->fadeOutTime_, 1);
}
