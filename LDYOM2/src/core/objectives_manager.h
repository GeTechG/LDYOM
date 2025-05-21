#pragma once
#include <functional>
#include <locked_ref.h>
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
	Objective createObjective(std::string_view type);
	void addNewObjective(std::string_view type);

	Objective& getUnsafeObjective(int index);
	void removeObjective(int index);
	void moveObjective(int fromIndex, int toIndex);

	std::unordered_map<std::string, ObjectiveBuilderData>& getObjectiveBuilders() { return m_objectivesBuilders; }

	void registerCoreObjectives();
};