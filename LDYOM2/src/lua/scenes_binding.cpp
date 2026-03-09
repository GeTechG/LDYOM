#include "scenes_binding.h"
#include "scenes_manager.h"

void register_scenes_bindings(sol::state_view lua) {
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
			entry["name"]  = objectives[i].name;
			entry["type"]  = objectives[i].type;
			result[i + 1]  = entry;
		}
		return result;
	});
}
