#include "objectives_manager.h"
#include "scenes_manager.h"
#include <objectives/test.h>
#include <stdexcept>

ObjectivesManager& ObjectivesManager::instance() {
	static ObjectivesManager instance;
	return instance;
}

void ObjectivesManager::registerObjectiveBuilder(ObjectiveBuilderData data) { m_objectivesBuilders[data.type] = data; }

void ObjectivesManager::createNewObjective(std::string_view type) {
	auto it = m_objectivesBuilders.find(std::string(type));
	if (it != m_objectivesBuilders.end()) {
		auto& builderData = it->second;
		Objective objective = builderData.builder();
		ScenesManager::instance().getCurrentScene().objectives.push_back(objective);
	} else {
		throw std::runtime_error("Objective type not registered: " + std::string(type));
	}
}

void ObjectivesManager::registerCoreObjectives() { this->registerObjectiveBuilder(createTestObjectiveBuilder()); }
