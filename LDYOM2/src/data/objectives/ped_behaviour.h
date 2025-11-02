#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "objective.h"
#include <extensions/ScriptCommands.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <ktcoro_wait.hpp>
#include <localization.h>
#include <objectives_manager.h>
#include <project_player.h>
#include <utils/imgui_configurate.h>

namespace objectives::ped_behaviour {
constexpr const char* TYPE = "core.ped_behaviour";

struct Data {
	bool spawnPedestrians = true;
	bool spawnVehicles = true;
	bool riotMode = false;

	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectivePedBehaviourData");
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, spawnPedestrians, spawnVehicles, riotMode);
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Data, spawnPedestrians, spawnVehicles, riotMode);
};

std::string tr(const std::string& key) { return _(fmt::format("objectives.{}.{}", std::string(TYPE), key)); }

void renderEditor(Data& data) {
	ImGui::Dummy(ImVec2((SCL_PX).x * 300.f, 0.f)); // Add some space at the top

	ImGui::Text(tr("spawn_pedestrians").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.6f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##spawn_pedestrians", &data.spawnPedestrians);

	ImGui::Text(tr("spawn_vehicles").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.6f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##spawn_vehicles", &data.spawnVehicles);

	ImGui::Text(tr("riot_mode").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.6f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##riot_mode", &data.riotMode);
}

ktwait execute(Data& data) {
	// Set riot mode
	plugin::Command<plugin::Commands::SET_LA_RIOTS>(data.riotMode);

	// Handle pedestrians
	if (data.spawnPedestrians) {
		plugin::Command<plugin::Commands::SET_PED_DENSITY_MULTIPLIER>(1.0f);
	} else {
		plugin::Command<plugin::Commands::CLEAR_AREA_OF_CHARS>(-3000.0f, -3000.0f, -100.0f, 3000.0f, 3000.0f, 2000.0f);
		plugin::Command<plugin::Commands::SET_PED_DENSITY_MULTIPLIER>(0.0f);
	}

	// Handle vehicles
	if (data.spawnVehicles) {
		plugin::Command<plugin::Commands::SET_CAR_DENSITY_MULTIPLIER>(1.0f);
	} else {
		plugin::Command<plugin::Commands::CLEAR_AREA_OF_CARS>(-3000.0f, -3000.0f, -100.0f, 3000.0f, 3000.0f, 2000.0f);
		plugin::Command<plugin::Commands::SET_CAR_DENSITY_MULTIPLIER>(0.0f);
	}

	co_return;
}

Objective make() {
	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), Data{},
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute));
}

ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "", .builder = make}; }
} // namespace objectives::ped_behaviour
