#pragma once
#include "component.h"
#include <functional>
#include <lua_manager.h>
#include <string>
#include <unordered_map>

struct ComponentBuilderData {
	std::string type;
	std::string category = "";
	Dependencies dependencies;
	bool isSpecial = false;
	std::function<std::shared_ptr<Component>()> builder;
};

class ComponentsManager {
  private:
	std::unordered_map<std::string, ComponentBuilderData> m_componentsBuilders;

	ComponentsManager() = default;

  public:
	static ComponentsManager& instance();

	ComponentsManager(const ComponentsManager&) = delete;
	void operator=(const ComponentsManager&) = delete;

	template <class T> void registerComponentBuilder();
	std::shared_ptr<Component> createComponent(std::string_view type);

	std::unordered_map<std::string, ComponentBuilderData>& getComponentBuilders() { return m_componentsBuilders; }

	void registerCoreComponents();
};

template <class T> inline void ComponentsManager::registerComponentBuilder() {
	m_componentsBuilders[T::TYPE] = {.type = T::TYPE,
	                                 .category = T::CATEGORY,
	                                 .dependencies = T::getDependencies(),
	                                 .isSpecial = T::isSpecialComponent(),
	                                 .builder = T::make};
	{
		auto luaState = LuaManager::instance().getState();
		T::sol_lua_register(luaState.get());
	}
}
