#include "actor_simple_task.h"
#include "components/actor.h"
#include "entity.h"
#include "extensions/ScriptCommands.h"
#include <project_player.h>

void components::ActorSimpleTask::sol_lua_register(sol::state_view lua_state) {
	auto ut = lua_state.new_usertype<ActorSimpleTask>("ActorSimpleTaskComponent");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, components::ActorSimpleTask, cast, taskId, executeTask);
}

components::ActorSimpleTask::ActorSimpleTask()
	: Component(TYPE) {}

void components::ActorSimpleTask::executeTask() {
	const auto actor = Actor::cast(this->entity->getComponent(Actor::TYPE));
	if (actor && actor->ped) {
		switch (taskId) {
			case 0:
				{
					int decisionMaker;
					plugin::Command<plugin::Commands::LOAD_CHAR_DECISION_MAKER>(0, &decisionMaker);
					plugin::Command<plugin::Commands::SET_CHAR_DECISION_MAKER>(actor->getPedRef(), decisionMaker);
					actor->ped->m_fHealth = 20.0f;
					actor->ped->m_fMaxHealth = 100.0f;
					plugin::Command<plugin::Commands::SET_CHAR_RELATIONSHIP>(
						actor->getPedRef(), eRelationshipType::TYPE_HATE, ePedType::PED_TYPE_PLAYER1);
					actor->ped->m_nWeaponAccuracy = 5;
					plugin::Command<plugin::Commands::TASK_DIE>(actor->getPedRef());
					plugin::Command<plugin::Commands::REMOVE_DECISION_MAKER>(decisionMaker);
					break;
				}
			case 1: plugin::Command<plugin::Commands::TASK_JUMP>(actor->getPedRef()); break;
			case 2:
				{
					float x, y, z;
					plugin::Command<plugin::Commands::GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS>(actor->getPedRef(), 0.0f,
					                                                                        10.0f, 0.0f, &x, &y, &z);
					plugin::Command<plugin::Commands::TASK_AIM_GUN_AT_COORD>(actor->getPedRef(), x, y, z, 1000000);
					break;
				}
			case 3: plugin::Command<plugin::Commands::TASK_DUCK>(actor->getPedRef()); break;
		}
	}
}

nlohmann::json components::ActorSimpleTask::to_json() const {
	auto j = this->Component::to_json();
	j["taskId"] = taskId;
	return j;
}

void components::ActorSimpleTask::from_json(const nlohmann::json& j) {
	this->Component::from_json(j);
	j.at("taskId").get_to(taskId);
}

void components::ActorSimpleTask::editorRender() {
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	ImGui::Text(tr("task").c_str());
	ImGui::SameLine(availableWidth * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::SliderInt("##taskId", &taskId, 0, 3, tr(fmt::format("tasks.{}", taskId)).c_str());
}

void components::ActorSimpleTask::onStart() {
	Component::onStart();
	if (IS_PLAYING) {
		const auto actor = Actor::cast(this->entity->getComponent(Actor::TYPE));
		if (actor) {
			if (actor->ped) {
				executeTask();
			}
			this->m_pedSpawnedConnection = std::make_optional(actor->onSpawned.connect([this]() { executeTask(); }));
		}
	}
}

void components::ActorSimpleTask::onUpdate(float deltaTime) { Component::onUpdate(deltaTime); }

void components::ActorSimpleTask::onReset() {
	Component::onReset();
	this->m_pedSpawnedConnection.reset();
}

Dependencies components::ActorSimpleTask::getDependencies() { return Dependencies{{Actor::TYPE}, true}; }