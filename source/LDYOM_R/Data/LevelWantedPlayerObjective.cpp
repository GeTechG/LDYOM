#include "LevelWantedPlayerObjective.h"

#include <CWorld.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "strUtils.h"

LevelWantedPlayerObjective::LevelWantedPlayerObjective(void *_new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.level_wanted_player"));
	strlcat(this->name_.data(), suffix.c_str(), sizeof this->name_);
}

void LevelWantedPlayerObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	ImGui::SliderInt(local.get("general.wanted").c_str(), &this->levelWanted_, 0, 6);
}

ktwait LevelWantedPlayerObjective::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	CWorld::Players[0].m_PlayerData.m_pWanted->SetWantedLevel(this->levelWanted_);
	co_return;
}
