#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "objective.h"
#include <CPlayerPed.h>
#include <extensions/ScriptCommands.h>
#include <imgui.h>
#include <in_game/actor_paths_editing.h>
#include <ktcoro_wait.hpp>
#include <localization.h>
#include <objectives_manager.h>
#include <random>
#include <window_manager.h>

namespace objectives::player_random_spawn {
constexpr const char* TYPE = "core.player_random_spawn";

struct Data {
	std::vector<std::array<float, 3>> points;

	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectivePlayerRandomSpawnData");
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, points);
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Data, points);
};

inline std::string tr(const std::string& key) { return _(fmt::format("objectives.{}.{}", std::string(TYPE), key)); }

void renderEditor(Data& data) {
	ImGui::Dummy(ImVec2((SCL_PX).x * 300.f, 0.f)); // Add some space at the top
	if (ImGui::Button(tr("edit_points").c_str())) {
		WindowManager::instance().disableWindowRendering(true);
		ActorPathsEditing::openPathEditor(data.points, [&data](bool saveChanges, const PointsArray& points) {
			if (saveChanges) {
				data.points = points;
			}
			WindowManager::instance().disableWindowRendering(false);
		});
	}
}

inline ktwait execute(Data& data) {
	auto player = FindPlayerPed();
	if (!player) {
		co_return;
	}

	if (data.points.empty()) {
		co_return;
	}

	// Based on DYOM animation -11 (Hide) logic:
	// 1. Pick a random point from the list
	// 2. Store player's current position
	// 3. Teleport player to random point
	// 4. Rotate player to face original position

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<size_t> dist(0, data.points.size() - 1);
	size_t randomIndex = dist(gen);

	const auto& randomPoint = data.points[randomIndex];

	// Store current player position
	CVector originalPos = player->GetPosition();

	// Teleport to random point
	plugin::Command<plugin::Commands::SET_CHAR_COORDINATES>(player, randomPoint[0], randomPoint[1], randomPoint[2]);

	// Calculate angle to face original position
	float dx = originalPos.x - randomPoint[0];
	float dy = originalPos.y - randomPoint[1];
	float angle = atan2(dy, dx) * (180.0f / 3.14159265f) - 90.0f; // Convert to GTA angle format

	// Set player angle
	plugin::Command<plugin::Commands::SET_CHAR_HEADING>(player, angle);

	co_return;
}

Objective make() {
	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), Data{},
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute));
}

ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "player", .builder = make}; }
} // namespace objectives::player_random_spawn
