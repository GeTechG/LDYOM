#include "components_manager.h"
#include <components/actor.h>
#include <components/actor/actor_animation.h>
#include <components/actor/actor_behaviour.h>
#include <components/actor/actor_drive_by_path.h>
#include <components/actor/actor_enter_spawn_exit_vehicle.h>
#include <components/actor/actor_move_by_points.h>
#include <components/actor/actor_random_spawn.h>
#include <components/actor/actor_simple_task.h>
#include <components/actor/weapons.h>
#include <components/health_bar.h>
#include <components/objective_specific.h>
#include <components/pickup.h>
#include <components/vehicle.h>


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
	registerComponentBuilder<components::ActorBehaviour>();
	registerComponentBuilder<components::ActorAnimation>();
	registerComponentBuilder<components::ActorMoveByPoints>();
	registerComponentBuilder<components::ActorEnterSpawnExitVehicle>();
	registerComponentBuilder<components::ActorDriveByPath>();
	registerComponentBuilder<components::ActorSimpleTask>();
	registerComponentBuilder<components::ActorRandomSpawn>();
	registerComponentBuilder<components::Vehicle>();
	registerComponentBuilder<components::Pickup>();
}