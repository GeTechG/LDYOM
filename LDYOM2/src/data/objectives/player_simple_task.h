#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "objective.h"
#include <CPlayerPed.h>
#include <extensions/ScriptCommands.h>
#include <gta_data/eRelationshipType.h>
#include <imgui.h>
#include <ktcoro_wait.hpp>
#include <localization.h>
#include <objectives_manager.h>
#include <utils/imgui_configurate.h>

namespace objectives::player_simple_task {
constexpr const char* TYPE = "core.player_simple_task";

struct Data {
	// 0 = Die, 1 = Jump, 2 = Aim, 3 = Duck
	int taskId = 0;

	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectivePlayerSimpleTaskData");
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, taskId);
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Data, taskId);
};

inline std::string tr(const std::string& key) { return _(fmt::format("objectives.{}.{}", std::string(TYPE), key)); }

void renderEditor(Data& data) {
	ImGui::Dummy(ImVec2((SCL_PX).x * 300.f, 0.f)); // Add some space at the top
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	ImGui::Text(tr("task").c_str());
	ImGui::SameLine(availableWidth * 0.6f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::SliderInt("##taskId", &data.taskId, 0, 3, tr(fmt::format("tasks.{}", data.taskId)).c_str());
}

inline ktwait execute(Data& data) {
	auto player = FindPlayerPed();
	if (!player) {
		co_return;
	}

	// Based on DYOM animation thread special animations
	switch (data.taskId) {
		case 0: // Die (-7 in DYOM)
			{
				int decisionMaker;
				plugin::Command<plugin::Commands::LOAD_CHAR_DECISION_MAKER>(0, &decisionMaker);
				plugin::Command<plugin::Commands::SET_CHAR_DECISION_MAKER>(player, decisionMaker);
				player->m_fHealth = 20.0f;
				player->m_fMaxHealth = 100.0f;
				plugin::Command<plugin::Commands::SET_CHAR_RELATIONSHIP>(player, eRelationshipType::TYPE_HATE,
				                                                         ePedType::PED_TYPE_PLAYER1);
				player->m_nWeaponAccuracy = 5;
				plugin::Command<plugin::Commands::TASK_DIE>(player);
				plugin::Command<plugin::Commands::REMOVE_DECISION_MAKER>(decisionMaker);
				break;
			}
		case 1: // Jump (-8 in DYOM)
			plugin::Command<plugin::Commands::TASK_JUMP>(player);
			break;
		case 2: // Aim (-12 in DYOM)
			{
				float x, y, z;
				plugin::Command<plugin::Commands::GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS>(player, 0.0f, 10.0f, 0.0f, &x,
				                                                                        &y, &z);
				plugin::Command<plugin::Commands::TASK_AIM_GUN_AT_COORD>(player, x, y, z, 1000000);
				break;
			}
		case 3: // Duck (not in DYOM as standalone, but used in crouch movement)
			plugin::Command<plugin::Commands::TASK_DUCK>(player);
			break;
	}

	co_return;
}

Objective make() {
	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), Data{},
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute));
}

ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "player", .builder = make}; }
} // namespace objectives::player_simple_task
