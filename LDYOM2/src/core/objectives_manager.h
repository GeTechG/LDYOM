#pragma once
#include <functional>
#include <lua_manager.h>
#include <objective.h>
#include <rocket.hpp>
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
	rocket::thread_safe_signal<void(std::string, std::string)> onObjectivesRemoved;

	static ObjectivesManager& instance();

	ObjectivesManager(const ObjectivesManager&) = delete;
	void operator=(const ObjectivesManager&) = delete;

	template <typename T> void registerObjectiveBuilder(ObjectiveBuilderData data);
	Objective createObjective(std::string_view type);
	void addNewObjective(std::string_view type);

	const Objective& getObjective(int index) const;
	Objective& getUnsafeObjective(int index);
	void removeObjective(int index);
	void moveObjective(int fromIndex, int toIndex);

	std::unordered_map<std::string, ObjectiveBuilderData>& getObjectiveBuilders() { return m_objectivesBuilders; }

	void registerCoreObjectives();
};

template <typename T> inline void ObjectivesManager::registerObjectiveBuilder(ObjectiveBuilderData data) {
	m_objectivesBuilders[data.type] = data;
	{
		auto luaState = LuaManager::instance().getState();
		T::sol_lua_register(luaState.get());
	}
}