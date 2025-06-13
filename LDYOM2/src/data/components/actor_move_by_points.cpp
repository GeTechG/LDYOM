#include "actor_move_by_points.h"
#include "actor.h"
#include "entity.h"
#include "extensions/ScriptCommands.h"
#include <in_game/actor_paths_editing.h>
#include <project_player.h>
#include <window_manager.h>

void components::ActorMoveByPoints::sol_lua_register(sol::state_view lua_state) {
	auto ut = lua_state.new_usertype<ActorMoveByPoints>("ActorMoveByPointsComponent");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, components::ActorMoveByPoints, cast, points, moveType, loop);
}

components::ActorMoveByPoints::ActorMoveByPoints()
	: Component(TYPE) {}

void components::ActorMoveByPoints::run() {
	const auto actor = Actor::cast(this->entity->getComponent(Actor::TYPE));
	if (actor && actor->ped) {
		auto speedType = 4;
		if (moveType == 1) {
			speedType = 6;
		} else if (moveType == 2) {
			speedType = 7;
		} else if (moveType == 3) {
			plugin::Command<plugin::Commands::TASK_TOGGLE_DUCK>(actor->getPedRef(), true);
		}
		int sequenceTaskId;
		plugin::Command<plugin::Commands::OPEN_SEQUENCE_TASK>(&sequenceTaskId);
		plugin::Command<plugin::Commands::FLUSH_ROUTE>();
		for (size_t i = 0; i < points.size(); ++i) {
			if (i > 0 && i % 8 == 0) {
				plugin::Command<plugin::Commands::TASK_FOLLOW_POINT_ROUTE>(-1, speedType, 0);
				plugin::Command<plugin::Commands::FLUSH_ROUTE>();
			}
			const auto& point = points[i];
			plugin::Command<plugin::Commands::EXTEND_ROUTE>(point[0], point[1], point[2]);
		}
		plugin::Command<plugin::Commands::TASK_FOLLOW_POINT_ROUTE>(-1, speedType, 0);
		plugin::Command<plugin::Commands::SET_SEQUENCE_TO_REPEAT>(sequenceTaskId, loop);
		plugin::Command<plugin::Commands::CLOSE_SEQUENCE_TASK>(sequenceTaskId);
		plugin::Command<plugin::Commands::PERFORM_SEQUENCE_TASK>(actor->getPedRef(), sequenceTaskId);
		plugin::Command<plugin::Commands::CLEAR_SEQUENCE_TASK>(&sequenceTaskId);
	}
}

nlohmann::json components::ActorMoveByPoints::to_json() const {
	auto j = this->Component::to_json();
	j["points"] = points;
	j["moveType"] = moveType;
	j["loop"] = loop;
	return j;
}

void components::ActorMoveByPoints::from_json(const nlohmann::json& j) {
	this->Component::from_json(j);
	j.at("points").get_to(points);
	j.at("moveType").get_to(moveType);
	j.at("loop").get_to(loop);
}

void components::ActorMoveByPoints::editorRender() {
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	ImGui::Text(tr("move_type").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::BeginCombo("##move_type", tr(fmt::format("move_types.{}", moveType)).c_str())) {
		for (int i = 0; i < 4; ++i) {
			if (ImGui::Selectable(tr(fmt::format("move_types.{}", i)).c_str(), moveType == i)) {
				moveType = i;
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Text(tr("loop").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##loop", &loop);

	if (ImGui::Button(tr("edit_path").c_str())) {
		WindowManager::instance().disableWindowRendering(true);
		ActorPathsEditing::openPathEditor(points, [this](bool saveChanges, const PointsArray& points) {
			if (saveChanges) {
				this->points = points;
			}
			WindowManager::instance().disableWindowRendering(false);
		});
	}
}

void components::ActorMoveByPoints::onStart() {
	Component::onStart();
	if (IS_PLAYING) {
		const auto actor = Actor::cast(this->entity->getComponent(Actor::TYPE));
		if (actor) {
			if (actor->ped) {
				run();
			}
			this->m_pedSpawnedConnection = std::make_optional(actor->onSpawned.connect([this]() { run(); }));
		}
	}
}

void components::ActorMoveByPoints::onUpdate(float deltaTime) { Component::onUpdate(deltaTime); }

void components::ActorMoveByPoints::onReset() {
	Component::onReset();
	this->m_pedSpawnedConnection.reset();
}

Dependencies components::ActorMoveByPoints::getDependencies() { return Dependencies{{Actor::TYPE}, true}; }