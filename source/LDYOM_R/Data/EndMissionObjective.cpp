#include "EndMissionObjective.h"

#include <CTheScripts.h>
#include <extensions/ScriptCommands.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "ModelsService.h"
#include "strUtils.h"
#include "utils.h"

EndMissionObjective::EndMissionObjective(void *_new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.end_mission"));
	strlcat(this->name_.data(), suffix.c_str(), sizeof this->name_);
}

void EndMissionObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	utils::ToggleButton(local.get("general.show_text").c_str(), &this->showEndTextMission_);
	ImGui::BeginDisabled(!this->showEndTextMission_);

	ImGui::InputText(local.get("general.text").c_str(), this->endTextMission_.data(), this->endTextMission_.size());
	ImGui::SliderInt(local.get("general.text_style").c_str(), &this->styleTypeEndText_, 0, 5,
	                 local.getArray("general.text_styles").at(this->styleTypeEndText_).c_str());
	ImGui::DragFloat(local.get("general.time").c_str(), &this->endTextMissionTime_, 0.1f, 0.f, FLT_MAX);

	ImGui::EndDisabled();

	ImGui::Separator();

	ImGui::SliderInt(local.get("general.passed_tune").c_str(), &this->endMissionTune_, 1, 2);
}

ktwait EndMissionObjective::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	using namespace plugin;

	if (CTheScripts::ScriptSpace[CTheScripts::OnAMissionFlag] == 0) {
		result.setError(2, "The mission is not currently running.");
		co_return;
	}

	CTheScripts::ScriptSpace[CTheScripts::OnAMissionFlag] = 0;

	if (this->showEndTextMission_) {
		static std::string cp1251Text;
		cp1251Text = utf8ToCp1251(this->endTextMission_.data());
		gxtEncode(cp1251Text);
		CMessages::AddBigMessage(const_cast<char*>(cp1251Text.c_str()),
		                         static_cast<unsigned>(this->endTextMissionTime_ * 1000.f),
		                         ModelsService::getInstance().getTextStyles().at(this->styleTypeEndText_));
	}

	Command<Commands::PLAY_MISSION_PASSED_TUNE>(this->endMissionTune_);
}
