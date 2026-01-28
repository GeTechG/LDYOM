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
#include <utils/imgui_configurate.h>
#include <window_manager.h>

namespace objectives::player_move_by_points {
constexpr const char* TYPE = "core.player_move_by_points";

struct Data {
	std::vector<std::array<float, 3>> points;
	int moveType = 0; // 0 - walk, 1 - run, 2 - sprint, 3 - crouch
	bool loop = false;

	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectivePlayerMoveByPointsData");
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, points, moveType, loop);
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Data, points, moveType, loop);
};

inline std::string tr(const std::string& key) { return _(fmt::format("objectives.{}.{}", std::string(TYPE), key)); }

void renderEditor(Data& data) {
	ImGui::Dummy(ImVec2((SCL_PX).x * 300.f, 0.f)); // Add some space at the top
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	ImGui::Text(tr("move_type").c_str());
	ImGui::SameLine(availableWidth * 0.6f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::BeginCombo("##move_type", tr(fmt::format("move_types.{}", data.moveType)).c_str())) {
		for (int i = 0; i < 4; ++i) {
			if (ImGui::Selectable(tr(fmt::format("move_types.{}", i)).c_str(), data.moveType == i)) {
				data.moveType = i;
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Text(tr("loop").c_str());
	ImGui::SameLine(availableWidth * 0.6f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##loop", &data.loop);

	if (ImGui::Button(tr("edit_path").c_str())) {
		ActorPathsEditing::openPathEditor(nullptr, data.points, [&data](bool saveChanges, const PointsArray& points) {
			if (saveChanges) {
				data.points = points;
			}
		});
	}
}

inline ktwait execute(Data& data) {
	auto player = FindPlayerPed();
	if (!player) {
		co_return;
	}

	// Same logic as ActorMoveByPoints component
	auto speedType = 4; // walk
	if (data.moveType == 1) {
		speedType = 6; // run
	} else if (data.moveType == 2) {
		speedType = 7; // sprint
	} else if (data.moveType == 3) {
		plugin::Command<plugin::Commands::TASK_TOGGLE_DUCK>(player, true);
	}

	int sequenceTaskId;
	plugin::Command<plugin::Commands::OPEN_SEQUENCE_TASK>(&sequenceTaskId);
	plugin::Command<plugin::Commands::FLUSH_ROUTE>();

	for (size_t i = 0; i < data.points.size(); ++i) {
		if (i > 0 && i % 8 == 0) {
			plugin::Command<plugin::Commands::TASK_FOLLOW_POINT_ROUTE>(-1, speedType, 0);
			plugin::Command<plugin::Commands::FLUSH_ROUTE>();
		}
		const auto& point = data.points[i];
		plugin::Command<plugin::Commands::EXTEND_ROUTE>(point[0], point[1], point[2]);
	}

	plugin::Command<plugin::Commands::TASK_FOLLOW_POINT_ROUTE>(-1, speedType, 0);
	plugin::Command<plugin::Commands::SET_SEQUENCE_TO_REPEAT>(sequenceTaskId, data.loop);
	plugin::Command<plugin::Commands::CLOSE_SEQUENCE_TASK>(sequenceTaskId);
	plugin::Command<plugin::Commands::PERFORM_SEQUENCE_TASK>(player, sequenceTaskId);
	plugin::Command<plugin::Commands::CLEAR_SEQUENCE_TASK>(&sequenceTaskId);

	co_return;
}

Objective make() {
	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), Data{},
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute));
}

ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "player", .builder = make}; }
} // namespace objectives::player_move_by_points
