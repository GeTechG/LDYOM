#pragma once
#include <entity.h>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

typedef std::vector<std::string> EntityTemplate;

class EntitiesManager {
  private:
	std::unordered_map<std::string, EntityTemplate> m_entityTemplates;

	EntitiesManager() = default;

  public:
	static EntitiesManager& instance();

	EntitiesManager(const EntitiesManager&) = delete;
	void operator=(const EntitiesManager&) = delete;

	// Template management
	void registerEntityTemplate(const std::string& type, const EntityTemplate& componentTypes);
	const std::vector<std::string>* getEntityTemplate(const std::string& type);

	// Entity creation and manipulation
	std::unique_ptr<Entity> createEntityFromTemplate(const std::string& templateType);
	std::unique_ptr<Entity> createEmptyEntity();
	int addNewEntityFromTemplate(const std::string& templateType);
	int addNewEntityFromTemplateWithEditorDefaults(const std::string& templateType, std::optional<std::string> defaultSpawnObjectiveUuid = std::nullopt);
	int addNewEmptyEntity();

	// Entity management within the current scene
	Entity& getUnsafeEntity(int index);
	void removeEntity(int index);
	int duplicateEntity(int index);
	void moveEntity(int fromIndex, int toIndex);
	std::vector<Entity*> getEntitiesWithComponent(const std::string_view type);

	// Get all entity templates
	const std::unordered_map<std::string, EntityTemplate>& getEntityTemplates() const { return m_entityTemplates; }

	// Register the predefined entity templates
	void registerCoreEntityTemplates();
};