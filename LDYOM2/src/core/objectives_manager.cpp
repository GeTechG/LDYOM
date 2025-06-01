#include "objectives_manager.h"
#include "scenes_manager.h"
#include <objectives/test.h>
#include <objectives/wait_signal.h>
#include <stdexcept>
#include <vector_utils.h>

ObjectivesManager& ObjectivesManager::instance() {
	static ObjectivesManager instance;
	return instance;
}

void ObjectivesManager::registerObjectiveBuilder(ObjectiveBuilderData data) { m_objectivesBuilders[data.type] = data; }

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
		objectives.erase(objectives.begin() + index);
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
	this->registerObjectiveBuilder(test_objective::builder());
	this->registerObjectiveBuilder(wait_signal_objective::builder());
}
