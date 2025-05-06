#include "objectives_manager.h"
#include "scenes_manager.h"
#include <objectives/test.h>
#include <stdexcept>

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
	ScenesManager::instance().getMutableCurrentScene()->objectives.data.push_back(objective);
}

Objective& ObjectivesManager::getObjectiveMutable(int index) {
	auto currentScene = ScenesManager::instance().getMutableCurrentScene();
	auto& objectives = currentScene->objectives.data;
	if (index < 0 || index >= static_cast<int>(objectives.size())) {
		throw std::out_of_range("Objective index out of range: " + std::to_string(index));
	}
	return objectives[index];
}

void ObjectivesManager::removeObjective(int index) {
	auto currentScene = ScenesManager::instance().getMutableCurrentScene();
	auto& objectives = currentScene->objectives.data;
	if (index >= 0 && index < static_cast<int>(objectives.size())) {
		objectives.erase(objectives.begin() + index);
	}
}

void ObjectivesManager::moveObjective(int fromIndex, int toIndex) {
	auto currentScene = ScenesManager::instance().getMutableCurrentScene();
	auto& objectives = currentScene->objectives.data;
	if (fromIndex >= 0 && fromIndex < static_cast<int>(objectives.size()) && toIndex >= 0 &&
	    toIndex < static_cast<int>(objectives.size())) {
		if (fromIndex == toIndex)
			return;

		Objective movingObjective = std::move(objectives[fromIndex]);

		objectives.erase(objectives.begin() + fromIndex);

		int actualToIndex = (toIndex > fromIndex) ? toIndex - 1 : toIndex;
		objectives.insert(objectives.begin() + actualToIndex, std::move(movingObjective));
	}
}

void ObjectivesManager::registerCoreObjectives() { this->registerObjectiveBuilder(createTestObjectiveBuilder()); }
