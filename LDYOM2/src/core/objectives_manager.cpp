#include "objectives_manager.h"
#include "scenes_manager.h"
#include <objectives/checkpoint.h>
#include <objectives/clock.h>
#include <objectives/countdown.h>
#include <objectives/cutscene.h>
#include <objectives/destroy_vehicle.h>
#include <objectives/enter_to_vehicle.h>
#include <objectives/interact_with_object.h>
#include <objectives/kill_actor_gang.h>
#include <objectives/money_add.h>
#include <objectives/money_substract.h>
#include <objectives/ped_behaviour.h>
#include <objectives/phone_call.h>
#include <objectives/pickup.h>
#include <objectives/player_animation.h>
#include <objectives/player_disarm.h>
#include <objectives/player_move_by_points.h>
#include <objectives/player_random_spawn.h>
#include <objectives/player_simple_task.h>
#include <objectives/player_teleport.h>
#include <objectives/player_teleport_to_car.h>
#include <objectives/player_vehicle_task.h>
#include <objectives/test.h>
#include <objectives/timelimit.h>
#include <objectives/timer_start.h>
#include <objectives/timeout.h>
#include <objectives/wait_signal.h>
#include <stdexcept>
#include <vector_utils.h>

ObjectivesManager& ObjectivesManager::instance() {
	static ObjectivesManager instance;
	return instance;
}

Objective ObjectivesManager::createObjective(std::string_view type) {
	auto it = m_objectivesBuilders.find(std::string(type));
	if (it != m_objectivesBuilders.end()) {
		auto& builderData = it->second;
		return builderData.builder();
	}
	throw std::runtime_error("Objective type not registered: " + std::string(type));
}

int ObjectivesManager::addNewObjective(std::string_view type) {
	Objective objective = this->createObjective(type);
	auto& objectives = ScenesManager::instance().getUnsafeCurrentScene().objectives.data;
	objectives.push_back(objective);
	return static_cast<int>(objectives.size() - 1);
}

const Objective& ObjectivesManager::getObjective(int index) const {
	auto& currentScene = ScenesManager::instance().getCurrentScene();
	auto& objectives = currentScene.objectives.data;
	if (index < 0 || index >= static_cast<int>(objectives.size())) {
		throw std::out_of_range("Objective index out of range: " + std::to_string(index));
	}
	return objectives[index];
}

Objective& ObjectivesManager::getUnsafeObjective(int index) {
	auto& currentScene = ScenesManager::instance().getUnsafeCurrentScene();
	auto& objectives = currentScene.objectives.data;
	if (index < 0 || index >= static_cast<int>(objectives.size())) {
		throw std::out_of_range("Objective index out of range: " + std::to_string(index));
	}
	return objectives[index];
}

void ObjectivesManager::removeObjective(int index) {
	auto& currentScene = ScenesManager::instance().getUnsafeCurrentScene();
	auto& objectives = currentScene.objectives.data;
	if (index >= 0 && index < static_cast<int>(objectives.size())) {
		auto removedObjectiveID = uuids::to_string(objectives[index].id);
		objectives.erase(objectives.begin() + index);
		if (!objectives.empty()) {
			auto newIndex = std::clamp(index, 0, static_cast<int>(objectives.size()) - 1);
			this->onObjectivesRemoved(removedObjectiveID, uuids::to_string(objectives[newIndex].id));
		}
	}
}

void ObjectivesManager::moveObjective(int fromIndex, int toIndex) {
	auto& currentScene = ScenesManager::instance().getUnsafeCurrentScene();
	auto& objectives = currentScene.objectives.data;
	if (!utils::moveInVector(objectives, fromIndex, toIndex)) {
		LDYOM_ERROR("Failed to move objective from {} to {}", fromIndex, toIndex);
	}
}

void ObjectivesManager::registerCoreObjectives() {
	using namespace objectives;
	this->registerObjectiveBuilder<test::Data>(test::builder());
	this->registerObjectiveBuilder<wait_signal::Data>(wait_signal::builder());
	this->registerObjectiveBuilder<kill_actor_gang::Data>(kill_actor_gang::builder());
	this->registerObjectiveBuilder<enter_to_vehicle::Data>(enter_to_vehicle::builder());
	this->registerObjectiveBuilder<destroy_vehicle::Data>(destroy_vehicle::builder());
	this->registerObjectiveBuilder<checkpoint::Data>(checkpoint::builder());
	this->registerObjectiveBuilder<countdown::Data>(countdown::builder());
	this->registerObjectiveBuilder<cutscene::Data>(cutscene::builder());
	this->registerObjectiveBuilder<clock::Data>(clock::builder());
	this->registerObjectiveBuilder<money_add::Data>(money_add::builder());
	this->registerObjectiveBuilder<money_subtract::Data>(money_subtract::builder());
	this->registerObjectiveBuilder<interact_with_object::Data>(interact_with_object::builder());
	this->registerObjectiveBuilder<ped_behaviour::Data>(ped_behaviour::builder());
	this->registerObjectiveBuilder<phone_call::Data>(phone_call::builder());
	this->registerObjectiveBuilder<pickup::Data>(pickup::builder());
	this->registerObjectiveBuilder<player_animation::Data>(player_animation::builder());
	this->registerObjectiveBuilder<player_disarm::Data>(player_disarm::builder());
	this->registerObjectiveBuilder<player_move_by_points::Data>(player_move_by_points::builder());
	this->registerObjectiveBuilder<player_random_spawn::Data>(player_random_spawn::builder());
	this->registerObjectiveBuilder<player_simple_task::Data>(player_simple_task::builder());
	this->registerObjectiveBuilder<player_teleport::Data>(player_teleport::builder());
	this->registerObjectiveBuilder<player_teleport_to_car::Data>(player_teleport_to_car::builder());
	this->registerObjectiveBuilder<player_vehicle_task::Data>(player_vehicle_task::builder());
	this->registerObjectiveBuilder<timelimit::Data>(timelimit::builder());
	this->registerObjectiveBuilder<timer_start::Data>(timer_start::builder());
	this->registerObjectiveBuilder<timeout_obj::Data>(timeout_obj::builder());
}
