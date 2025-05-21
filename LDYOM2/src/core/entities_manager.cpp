#include "entities_manager.h"
#include "../utils/vector_utils.h"
#include "components_manager.h"
#include "localization.h"
#include "scenes_manager.h"
#include <stdexcept>

EntitiesManager& EntitiesManager::instance() {
	static EntitiesManager instance;
	return instance;
}

void EntitiesManager::registerEntityTemplate(const std::string& type, const EntityTemplate& templateData) {
	m_entityTemplates[type] = templateData;
}

const EntityTemplate* EntitiesManager::getEntityTemplate(const std::string& type) {
	auto it = m_entityTemplates.find(type);
	if (it != m_entityTemplates.end()) {
		return &it->second;
	}
	return nullptr;
}

Entity EntitiesManager::createEntityFromTemplate(const std::string& templateType) {
	auto templatePtr = getEntityTemplate(templateType);
	if (!templatePtr) {
		throw std::runtime_error("Entity template not found: " + templateType);
	}

	Entity entity = createEmptyEntity();

	// Add all components defined in the template
	for (const auto& componentType : *templatePtr) {
		entity.components.data.push_back(ComponentsManager::instance().createComponent(componentType));
	}

	return entity;
}

Entity EntitiesManager::createEmptyEntity() {
	Entity entity;
	entity.name = _("entities.new_entity");
	return entity;
}

void EntitiesManager::addNewEntityFromTemplate(const std::string& templateType) {
	Entity entity = createEntityFromTemplate(templateType);
	ScenesManager::instance().getMutableCurrentScene()->entities.push_back(entity);
}

void EntitiesManager::addNewEmptyEntity() {
	Entity entity = createEmptyEntity();
	ScenesManager::instance().getMutableCurrentScene()->entities.push_back(entity);
}

Entity& EntitiesManager::getUnsafeEntity(int index) {
	auto currentScene = ScenesManager::instance().getMutableCurrentScene();
	auto& entities = currentScene->entities;
	if (index < 0 || index >= static_cast<int>(entities.size())) {
		throw std::out_of_range("Entity index out of range: " + std::to_string(index));
	}
	return entities[index];
}

void EntitiesManager::removeEntity(int index) {
	auto currentScene = ScenesManager::instance().getMutableCurrentScene();
	auto& entities = currentScene->entities;
	if (index >= 0 && index < static_cast<int>(entities.size())) {
		entities.erase(entities.begin() + index);
	}
}

void EntitiesManager::moveEntity(int fromIndex, int toIndex) {
	auto currentScene = ScenesManager::instance().getMutableCurrentScene();
	auto& entities = currentScene->entities;
	if (!utils::moveInVector(entities, fromIndex, toIndex)) {
		LDYOM_ERROR("Failed to move entity from {} to {}", fromIndex, toIndex);
	}
}

void EntitiesManager::registerCoreEntityTemplates() { registerEntityTemplate("actor", {"actor"}); }