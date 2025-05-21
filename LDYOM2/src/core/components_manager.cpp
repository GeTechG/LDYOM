#include "components_manager.h"
#include <components/actor.h>

ComponentsManager& ComponentsManager::instance() {
	static ComponentsManager instance;
	return instance;
}

void ComponentsManager::registerComponentBuilder(ComponentBuilderData data) { m_componentsBuilders[data.type] = data; }

std::shared_ptr<Component> ComponentsManager::createComponent(std::string_view type) {
	auto it = m_componentsBuilders.find(std::string(type));
	if (it != m_componentsBuilders.end()) {
		auto& builderData = it->second;
		return builderData.builder();
	}
	throw std::runtime_error("Component type not registered: " + std::string(type));
}

void ComponentsManager::registerCoreComponents() { registerComponentBuilder(components::actorBuilder()); }