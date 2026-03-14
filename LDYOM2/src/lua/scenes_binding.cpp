#include "scenes_binding.h"
#include "scenes_manager.h"
#include <component.h>
#include <entity.h>
#include <objective.h>
#include <scene_info.h>
#include <uuid_wrap.h>

void register_scenes_bindings(sol::state_view lua) {
	Entity::sol_lua_register(lua);

	// ── Objective usertype ────────────────────────────────────────────────────
	lua.new_usertype<Objective>(
		"Objective", sol::no_constructor,
		"name", &Objective::name,
		"type", sol::readonly(&Objective::type),
		"id", sol::property([](const Objective& o) { return uuids::to_string(o.id); }),
		"is_initialized", sol::readonly(&Objective::isInitialized));

	// ── SceneInfo usertype ────────────────────────────────────────────────────
	lua.new_usertype<SceneInfo>(
		"SceneInfo", sol::no_constructor,
		"name", &SceneInfo::name,
		"id", sol::readonly(&SceneInfo::id));

	// ── scenes table ──────────────────────────────────────────────────────────
	sol::table scenes = lua.create_named_table("scenes");

	// scenes.get_info() -> SceneInfo
	// Returns the SceneInfo (name, id) of the currently loaded scene.
	scenes.set_function("get_info", []() -> const SceneInfo& {
		return ScenesManager::instance().getCurrentScene().info;
	});

	// scenes.get_objectives() -> { Objective*, ... }
	// Returns all objectives in the currently loaded scene as Objective usertypes.
	scenes.set_function("get_objectives", [](sol::this_state s) -> sol::table {
		sol::state_view l(s);
		sol::table result = l.create_table();

		auto& objectives = ScenesManager::instance().getUnsafeCurrentScene().objectives.data;
		for (int i = 0; i < static_cast<int>(objectives.size()); ++i) {
			result[i + 1] = &objectives[i];
		}
		return result;
	});

	// scenes.get_entities() -> { Entity*, ... }
	// Returns all entities in the currently loaded scene as Entity usertypes.
	scenes.set_function("get_entities", [](sol::this_state s) -> sol::table {
		sol::state_view l(s);
		sol::table result = l.create_table();

		const auto& entities = ScenesManager::instance().getCurrentScene().entities;
		for (int i = 0; i < static_cast<int>(entities.size()); ++i) {
			result[i + 1] = entities[i].get();
		}
		return result;
	});

	// scenes.find_entity(id) -> Entity | nil
	// Finds and returns the entity with the given UUID string, or nil if not found.
	scenes.set_function("find_entity", [](const std::string& id) -> Entity* {
		auto& entities = ScenesManager::instance().getUnsafeCurrentScene().entities;
		for (const auto& entity : entities) {
			if (uuids::to_string(entity->id) == id)
				return entity.get();
		}
		return nullptr;
	});

	// scenes.find_entity_by_name(name) -> Entity | nil
	// Finds and returns the first entity with the given name, or nil if not found.
	scenes.set_function("find_entity_by_name", [](const std::string& name) -> Entity* {
		auto& entities = ScenesManager::instance().getUnsafeCurrentScene().entities;
		for (const auto& entity : entities) {
			if (entity->name == name)
				return entity.get();
		}
		return nullptr;
	});

	// scenes.spawn_entity(entity_or_id) — calls spawn() on all components.
	// Accepts either an Entity usertype or a UUID string.
	scenes.set_function("spawn_entity",
		sol::overload(
			[](Entity& entity) {
				for (const auto& comp : entity.getComponents())
					comp->spawn();
			},
			[](const std::string& id) {
				auto& entities = ScenesManager::instance().getUnsafeCurrentScene().entities;
				for (const auto& entity : entities) {
					if (uuids::to_string(entity->id) == id) {
						for (const auto& comp : entity->getComponents())
							comp->spawn();
						return;
					}
				}
			}));

	// scenes.despawn_entity(entity_or_id) — calls despawn() on all components.
	// Accepts either an Entity usertype or a UUID string.
	scenes.set_function("despawn_entity",
		sol::overload(
			[](Entity& entity) {
				for (const auto& comp : entity.getComponents())
					comp->despawn();
			},
			[](const std::string& id) {
				auto& entities = ScenesManager::instance().getUnsafeCurrentScene().entities;
				for (const auto& entity : entities) {
					if (uuids::to_string(entity->id) == id) {
						for (const auto& comp : entity->getComponents())
							comp->despawn();
						return;
					}
				}
			}));
}
