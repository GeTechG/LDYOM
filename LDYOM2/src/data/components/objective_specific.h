#pragma once
#include "component.h"
#include <components_manager.h>
#include <fa_icons.h>
#include <imgui.h>
#include <localization.h>
#include <lua_define_type.h>
#include <rocket.hpp>
#include <string>

namespace components {

class ObjectiveSpecific : public Component {
  private:
	std::optional<rocket::scoped_connection> spawnObjectiveConnection;
	std::optional<rocket::scoped_connection> despawnObjectiveConnection;

  public:
	static constexpr auto TYPE = "objective_specific";
	static constexpr auto CATEGORY = "entities";

	static std::shared_ptr<ObjectiveSpecific> cast(std::shared_ptr<Component> component) {
		return std::dynamic_pointer_cast<ObjectiveSpecific>(component);
	}

	static Dependencies getDependencies();

	std::string spawnObjective = "";
	std::string despawnObjective = "";

	rocket::thread_safe_signal<void()> onSpawned;
	rocket::thread_safe_signal<void()> onDespawned;

	ObjectiveSpecific();

	[[nodiscard]] nlohmann::json to_json() const override;

	void from_json(const nlohmann::json& j) override;

	void editorRender() override;

	void onStart() override;
	void onUpdate(float deltaTime) override;
	void onReset() override;

	static void sol_lua_register(sol::state_view lua_state);
	static std::shared_ptr<Component> make() { return std::make_shared<ObjectiveSpecific>(); }
};
} // namespace components