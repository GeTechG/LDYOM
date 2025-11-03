#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "objective.h"
#include <CWanted.h>
#include <CWorld.h>
#include <blip_color_utils.h>
#include <common.h>
#include <extensions/ScriptCommands.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <ktcoro_wait.hpp>
#include <localization.h>
#include <objectives_manager.h>
#include <project_player.h>
#include <string_utils.h>
#include <utils/imgui_configurate.h>

namespace objectives::wanted_level {
constexpr const char* TYPE = "core.wanted_level";

struct Data {
	int wantedLevel = 0;
	int maxWantedLevel = 6;
	int minWantedLevel = 0;

	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectiveWantedLevelData");
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, wantedLevel, maxWantedLevel, minWantedLevel);
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Data, wantedLevel, maxWantedLevel, minWantedLevel);
};

std::string tr(const std::string& key) { return _(fmt::format("objectives.{}.{}", std::string(TYPE), key)); }

void renderEditor(Data& data) {
	ImGui::Dummy(ImVec2((SCL_PX).x * 300.f, 0.f));
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	// Wanted Level
	ImGui::Text(tr("wanted_level").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::SliderInt("##wanted_level", &data.wantedLevel, 0, 6);

	// Max Wanted Level
	ImGui::Text(tr("max_wanted_level").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::SliderInt("##max_wanted_level", &data.maxWantedLevel, 0, 6);

	// Min Wanted Level
	ImGui::Text(tr("min_wanted_level").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::SliderInt("##min_wanted_level", &data.minWantedLevel, 0, 6);
}

ktwait execute(Data& data) {
	// Set wanted level
	CWorld::Players[0].m_PlayerData.m_pWanted->SetWantedLevel(data.wantedLevel);

	// Set max wanted level
	CWanted::SetMaximumWantedLevel(data.maxWantedLevel);

	// Set min wanted level
	CWorld::Players[0].m_PlayerData.m_pWanted->SetWantedLevelNoDrop(data.minWantedLevel);

	co_return;
}

Objective make() {
	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), Data{},
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute));
}

ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "", .builder = make}; }
} // namespace objectives::wanted_level
