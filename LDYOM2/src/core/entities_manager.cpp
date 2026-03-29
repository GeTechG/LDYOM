#include "entities_manager.h"
#include "../utils/vector_utils.h"
#include "components_manager.h"
#include "localization.h"
#include "scenes_manager.h"
#include <extensions/ScriptCommands.h>
#include <plugin.h>
#include <stdexcept>
#include <uuid_wrap.h>

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

int EntitiesManager::addNewEntityFromTemplate(const std::string& templateType) {
	std::unique_ptr<Entity> entity = createEntityFromTemplate(templateType);
	auto& entities = ScenesManager::instance().getUnsafeCurrentScene().entities;
	entities.emplace_back(std::move(entity));
	return static_cast<int>(entities.size() - 1);
}

int EntitiesManager::addNewEmptyEntity() {
	std::unique_ptr<Entity> entity = createEmptyEntity();
	auto& entities = ScenesManager::instance().getUnsafeCurrentScene().entities;
	entities.emplace_back(std::move(entity));
	return static_cast<int>(entities.size() - 1);
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

int EntitiesManager::duplicateEntity(int index) {
	auto& entities = ScenesManager::instance().getUnsafeCurrentScene().entities;
	if (index < 0 || index >= static_cast<int>(entities.size())) {
		throw std::out_of_range("Entity index out of range: " + std::to_string(index));
	}
	nlohmann::json j;
	to_json(j, *entities[index]);
	auto newEntity = std::make_unique<Entity>();
	from_json(j, *newEntity);
	newEntity->id = uuids::uuid_system_generator{}();
	newEntity->name = entities[index]->name;
	int newIndex = index + 1;
	entities.insert(entities.begin() + newIndex, std::move(newEntity));
	return newIndex;
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
	registerEntityTemplate("train", {"train", "objective_specific"});
	registerEntityTemplate("object", {"object", "objective_specific"});
	registerEntityTemplate("pickup", {"pickup", "objective_specific"});
	registerEntityTemplate("firework", {"firework", "objective_specific"});
	registerEntityTemplate("particle", {"particle", "objective_specific"});
	registerEntityTemplate("checkpoint", {"checkpoint", "objective_specific"});
	registerEntityTemplate("audio", {"audio", "objective_specific"});
}