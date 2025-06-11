#include "actor_behaviour.h"
#include "actor.h"
#include "entity.h"
#include "extensions/ScriptCommands.h"
#include <project_player.h>

void components::ActorBehaviour::sol_lua_register(sol::state_view lua_state) {
	auto ut = lua_state.new_usertype<ActorBehaviour>("ActorBehaviourComponent");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, components::ActorBehaviour, cast, holdPosition, directAttack,
	                 followPlayer, applyBehaviour);
}

components::ActorBehaviour::ActorBehaviour()
	: Component(TYPE) {}

void components::ActorBehaviour::applyBehaviour() {
	const auto actor = Actor::cast(this->entity->getComponent(Actor::TYPE));
	if (actor && actor->ped) {
		if (holdPosition) {
			plugin::Command<plugin::Commands::TASK_STAY_IN_SAME_PLACE>(actor->getPedRef(), true);
		}
		if (directAttack) {
			plugin::Command<plugin::Commands::TASK_KILL_CHAR_ON_FOOT>(actor->getPedRef(),
			                                                          CPools::GetPedRef(FindPlayerPed()));
		}
		if (followPlayer) {
			int g;
			plugin::Command<plugin::Commands::GET_PLAYER_GROUP>(0, &g);
			plugin::Command<plugin::Commands::SET_GROUP_MEMBER>(g, actor->getPedRef());
		}
	}
}

nlohmann::json components::ActorBehaviour::to_json() const {
	auto j = this->Component::to_json();
	j["holdPosition"] = holdPosition;
	j["directAttack"] = directAttack;
	j["followPlayer"] = followPlayer;
	return j;
}

void components::ActorBehaviour::from_json(const nlohmann::json& j) {
	this->Component::from_json(j);
	j.at("holdPosition").get_to(holdPosition);
	j.at("directAttack").get_to(directAttack);
	j.at("followPlayer").get_to(followPlayer);
}

void components::ActorBehaviour::editorRender() {
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	ImGui::Text(tr("hold_position").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##holdPosition", &holdPosition);

	ImGui::Text(tr("direct_attack").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##directAttack", &directAttack);

	ImGui::Text(tr("follow_player").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##followPlayer", &followPlayer);
}

void components::ActorBehaviour::onStart() {
	Component::onStart();
	if (IS_PLAYING) {
		const auto actor = Actor::cast(this->entity->getComponent(Actor::TYPE));
		if (actor) {
			if (actor->ped) {
				applyBehaviour();
			}
			this->m_pedSpawnedConnection = std::make_optional(actor->onSpawned.connect([this]() { applyBehaviour(); }));
		}
	}
}

void components::ActorBehaviour::onUpdate(float deltaTime) { Component::onUpdate(deltaTime); }

void components::ActorBehaviour::onReset() {
	Component::onReset();
	this->m_pedSpawnedConnection.reset();
}

Dependencies components::ActorBehaviour::getDependencies() { return Dependencies{{Actor::TYPE}, true}; }