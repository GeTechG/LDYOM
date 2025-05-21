#pragma once
#include "component.h"
#include <functional>
#include <string>
#include <unordered_map>

struct ComponentBuilderData {
	std::string type;
	std::string category = "";
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

	void registerComponentBuilder(ComponentBuilderData data);
	std::shared_ptr<Component> createComponent(std::string_view type);

	std::unordered_map<std::string, ComponentBuilderData>& getComponentBuilders() { return m_componentsBuilders; }

	void registerCoreComponents();
};