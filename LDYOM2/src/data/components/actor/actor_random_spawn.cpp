#include "actor_random_spawn.h"
#include "components/actor.h"
#include "entity.h"
#include "extensions/ScriptCommands.h"
#include <in_game/actor_paths_editing.h>
#include <project_player.h>
#include <window_manager.h>

void components::ActorRandomSpawn::sol_lua_register(sol::state_view lua_state) {
	auto ut = lua_state.new_usertype<ActorRandomSpawn>("ActorRandomSpawnComponent");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, components::ActorRandomSpawn, cast, points);
}

components::ActorRandomSpawn::ActorRandomSpawn()
	: Component(TYPE) {}

void components::ActorRandomSpawn::apply() {
	const auto actor = Actor::cast(this->entity->getComponent(Actor::TYPE));
	if (actor && actor->ped) {
		const auto pointIndex = rand() % points.size();
		const auto& point = points[pointIndex];
		auto playerPos = FindPlayerPed()->GetPosition();
		playerPos.x -= point[0];
		playerPos.y -= point[1];
		float heading;
		plugin::Command<plugin::Commands::GET_HEADING_FROM_VECTOR_2D>(playerPos.x, playerPos.y, &heading);
		actor->ped->SetPosn(point[0], point[1], point[2]);
		plugin::Command<plugin::Commands::SET_CHAR_HEADING>(actor->getPedRef(), heading);
	}
}

nlohmann::json components::ActorRandomSpawn::to_json() const {
	auto j = this->Component::to_json();
	j["points"] = points;
	return j;
}

void components::ActorRandomSpawn::from_json(const nlohmann::json& j) {
	this->Component::from_json(j);
	j.at("points").get_to(points);
}

void components::ActorRandomSpawn::editorRender() {
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	if (ImGui::Button(tr("edit_points").c_str())) {
		WindowManager::instance().disableWindowRendering(true);
		ActorPathsEditing::openPathEditor(points, [this](bool saveChanges, const PointsArray& points) {
			if (saveChanges) {
				this->points = points;
			}
			WindowManager::instance().disableWindowRendering(false);
		});
	}
}

void components::ActorRandomSpawn::onStart() {
	Component::onStart();
	if (IS_PLAYING) {
		const auto actor = Actor::cast(this->entity->getComponent(Actor::TYPE));
		if (actor) {
			if (actor->ped) {
				apply();
			}
			this->m_pedSpawnedConnection = std::make_optional(actor->onSpawned.connect([this]() { apply(); }));
		}
	}
}

void components::ActorRandomSpawn::onUpdate(float deltaTime) { Component::onUpdate(deltaTime); }

void components::ActorRandomSpawn::onReset() {
	Component::onReset();
	this->m_pedSpawnedConnection.reset();
}

Dependencies components::ActorRandomSpawn::getDependencies() { return Dependencies{{Actor::TYPE}, true}; }