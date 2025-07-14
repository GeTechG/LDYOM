#include "entities_manager.h"
#include "../utils/vector_utils.h"
#include "components_manager.h"
#include "localization.h"
#include "scenes_manager.h"
#include <extensions/ScriptCommands.h>
#include <plugin.h>
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

std::unique_ptr<Entity> EntitiesManager::createEntityFromTemplate(const std::string& templateType) {
	auto templatePtr = getEntityTemplate(templateType);
	if (!templatePtr) {
		throw std::runtime_error("Entity template not found: " + templateType);
	}

	std::unique_ptr<Entity> entity = createEmptyEntity();

	// Add all components defined in the template
	for (const auto& componentType : *templatePtr) {
		entity->addComponent(ComponentsManager::instance().createComponent(componentType));
	}

	return entity;
}

std::unique_ptr<Entity> EntitiesManager::createEmptyEntity() {
	auto entity = std::make_unique<Entity>();
	entity->name = _("entities.new_entity");
	auto position = FindPlayerPed()->GetPosition();
	entity->position = {position.x, position.y, position.z};
	plugin::Command<plugin::Commands::GET_AREA_VISIBLE>(&entity->areaId);
	return entity;
}

void EntitiesManager::addNewEntityFromTemplate(const std::string& templateType) {
	std::unique_ptr<Entity> entity = createEntityFromTemplate(templateType);
	ScenesManager::instance().getUnsafeCurrentScene().entities.emplace_back(std::move(entity));
}

void EntitiesManager::addNewEmptyEntity() {
	std::unique_ptr<Entity> entity = createEmptyEntity();
	ScenesManager::instance().getUnsafeCurrentScene().entities.emplace_back(std::move(entity));
}

Entity& EntitiesManager::getUnsafeEntity(int index) {
	auto& currentScene = ScenesManager::instance().getUnsafeCurrentScene();
	auto& entities = currentScene.entities;
	if (index < 0 || index >= static_cast<int>(entities.size())) {
		throw std::out_of_range("Entity index out of range: " + std::to_string(index));
	}
	return *entities[index];
}

void EntitiesManager::removeEntity(int index) {
	auto& currentScene = ScenesManager::instance().getUnsafeCurrentScene();
	auto& entities = currentScene.entities;
	if (index >= 0 && index < static_cast<int>(entities.size())) {
		entities.erase(entities.begin() + index);
	}
}

void EntitiesManager::moveEntity(int fromIndex, int toIndex) {
	auto& currentScene = ScenesManager::instance().getUnsafeCurrentScene();
	auto& entities = currentScene.entities;
	if (!utils::moveInVector(entities, fromIndex, toIndex)) {
		LDYOM_ERROR("Failed to move entity from {} to {}", fromIndex, toIndex);
	}
}

std::vector<Entity*> EntitiesManager::getEntitiesWithComponent(const std::string_view type) {
	auto& currentScene = ScenesManager::instance().getUnsafeCurrentScene();
	auto& entities = currentScene.entities;
	std::vector<Entity*> result;
	for (auto& entity : entities) {
		if (entity->hasComponent(type)) {
			result.push_back(entity.get());
		}
	}
	return result;
}

void EntitiesManager::registerCoreEntityTemplates() {
	registerEntityTemplate("actor", {"actor", "objective_specific"});
	registerEntityTemplate("vehicle", {"vehicle", "objective_specific"});
	registerEntityTemplate("object", {"object", "objective_specific"});
	registerEntityTemplate("pickup", {"pickup", "objective_specific"});
	registerEntityTemplate("firework", {"firework", "objective_specific"});
	registerEntityTemplate("particle", {"particle", "objective_specific"});
	registerEntityTemplate("checkpoint", {"checkpoint", "objective_specific"});
}