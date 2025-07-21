#include "objectives_manager.h"
#include "scenes_manager.h"
#include <objectives/checkpoint.h>
#include <objectives/countdown.h>
#include <objectives/destroy_vehicle.h>
#include <objectives/enter_to_vehicle.h>
#include <objectives/kill_actor_gang.h>
#include <objectives/test.h>
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

void ObjectivesManager::addNewObjective(std::string_view type) {
	Objective objective = this->createObjective(type);
	ScenesManager::instance().getUnsafeCurrentScene().objectives.data.push_back(objective);
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
}
