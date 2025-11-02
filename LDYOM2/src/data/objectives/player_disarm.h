#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "objective.h"
#include <CPlayerPed.h>
#include <extensions/ScriptCommands.h>
#include <imgui.h>
#include <ktcoro_wait.hpp>
#include <localization.h>
#include <objectives_manager.h>
#include <utils/imgui_configurate.h>

namespace objectives::player_disarm {
constexpr const char* TYPE = "core.player_disarm";

struct Data {
	int v;
	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectivePlayerDisarmData");
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Data, v);
};

inline std::string tr(const std::string& key) { return _(fmt::format("objectives.{}.{}", std::string(TYPE), key)); }

void renderEditor(Data& data) {
	ImGui::Dummy(ImVec2((SCL_PX).x * 300.f, (SCL_PX).y)); // Add some space at the top
}

inline ktwait execute(Data& data) {
	auto player = FindPlayerPed();
	if (!player) {
		co_return;
	}

	// Remove all weapons from player (DYOM command 048F)
	plugin::Command<plugin::Commands::REMOVE_ALL_CHAR_WEAPONS>(player);

	co_return;
}

Objective make() {
	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), Data{},
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute));
}

ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "player", .builder = make}; }
} // namespace objectives::player_disarm
