#pragma once
#include <functional>
#include <objective.h>
#include <string>
#include <unordered_map>

struct ObjectiveBuilderData {
	std::string type;
	std::string category = "";
	std::function<Objective()> builder;
};

class ObjectivesManager {
  private:
	std::unordered_map<std::string, ObjectiveBuilderData> m_objectivesBuilders;

	ObjectivesManager() = default;

  public:
	static ObjectivesManager& instance();

	ObjectivesManager(const ObjectivesManager&) = delete;
	void operator=(const ObjectivesManager&) = delete;

	void registerObjectiveBuilder(ObjectiveBuilderData data);
	void createNewObjective(std::string_view type);

	std::unordered_map<std::string, ObjectiveBuilderData>& getObjectiveTypes() { return m_objectivesBuilders; }

	void registerCoreObjectives();
};