#include "scenes_binding.h"
#include "scenes_manager.h"
#include <component.h>
#include <entity.h>
#include <uuid_wrap.h>

void register_scenes_bindings(sol::state_view lua) {
	Entity::sol_lua_register(lua);

	sol::table scenes = lua.create_named_table("scenes");

	// scenes.get_objectives() -> { {index=0, name="...", type="..."}, ... }
	// Returns all objectives in the currently loaded scene.
	scenes.set_function("get_objectives", [](sol::this_state s) -> sol::table {
		sol::state_view l(s);
		sol::table result = l.create_table();

		const auto& objectives = ScenesManager::instance().getCurrentScene().objectives.data;
		for (int i = 0; i < static_cast<int>(objectives.size()); ++i) {
			sol::table entry = l.create_table();
			entry["index"] = i;
			entry["name"] = objectives[i].name;
			entry["type"] = objectives[i].type;
			result[i + 1] = entry;
		}
		return result;
	});

	// scenes.get_entities() -> { Entity, ... }
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

	// scenes.spawn_entity(id) — calls spawn() on all components of entity with given UUID
	scenes.set_function("spawn_entity", [](const std::string& id) {
		auto& entities = ScenesManager::instance().getUnsafeCurrentScene().entities;
		for (const auto& entity : entities) {
			if (uuids::to_string(entity->id) == id) {
				for (const auto& comp : entity->getComponents())
					comp->spawn();
				return;
			}
		}
	});

	// scenes.despawn_entity(id) — calls despawn() on all components of entity with given UUID
	scenes.set_function("despawn_entity", [](const std::string& id) {
		auto& entities = ScenesManager::instance().getUnsafeCurrentScene().entities;
		for (const auto& entity : entities) {
			if (uuids::to_string(entity->id) == id) {
				for (const auto& comp : entity->getComponents())
					comp->despawn();
				return;
			}
		}
	});
}
