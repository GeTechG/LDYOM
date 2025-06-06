#include "components_manager.h"
#include <components/actor.h>
#include <components/health_bar.h>
#include <components/objective_specific.h>
#include <components/weapons.h>

ComponentsManager& ComponentsManager::instance() {
	static ComponentsManager instance;
	return instance;
}

std::shared_ptr<Component> ComponentsManager::createComponent(std::string_view type) {
	auto it = m_componentsBuilders.find(std::string(type));
	if (it != m_componentsBuilders.end()) {
		auto& builderData = it->second;
		return builderData.builder();
	}
	throw std::runtime_error("Component type not registered: " + std::string(type));
}

void ComponentsManager::registerCoreComponents() {
	registerComponentBuilder<components::Actor>();
	registerComponentBuilder<components::ObjectiveSpecific>();
	registerComponentBuilder<components::HealthBar>();
	registerComponentBuilder<components::Weapons>();
}