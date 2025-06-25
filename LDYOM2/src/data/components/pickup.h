#pragma once
#include "component.h"
#include <CPools.h>
#include <components_manager.h>
#include <fa_icons.h>
#include <imgui.h>
#include <localization.h>
#include <lua_define_type.h>
#include <memory>
#include <models_manager.h>
#include <optional>
#include <rocket.hpp>
#include <string>

namespace components {

class Pickup : public Component {
  private:
	void updatePosition();

	std::optional<rocket::scoped_connection> onSpawnedConnection;
	std::optional<rocket::scoped_connection> onDespawnedConnection;

  public:
	static constexpr auto TYPE = "pickup";
	static constexpr auto CATEGORY = "entities";

	static std::shared_ptr<Pickup> cast(std::shared_ptr<Component> component) {
		return std::dynamic_pointer_cast<Pickup>(component);
	}

	static Dependencies getDependencies() { return Dependencies{{}, true}; }

	// 0 - weapon, 1 - health, 2 - armor, 3 - police bribe, 4 - object
	int type = 0;
	int spawnType = 0;
	int weaponType = 1;
	int ammo = 0;
	int model = 1241;

	int dirty = false;
	int handle = -1;

	rocket::thread_safe_signal<void()> onSpawned;
	rocket::thread_safe_signal<void()> onDespawned;

	Pickup();
	~Pickup();

	[[nodiscard]] nlohmann::json to_json() const override;

	void from_json(const nlohmann::json& j) override;

	void editorRender() override;

	void onStart() override;
	void onUpdate(float deltaTime) override;
	void onReset() override;

	void spawn();
	void despawn();

	static bool isSpecialComponent() { return true; }
	static void sol_lua_register(sol::state_view lua_state);

	static std::shared_ptr<Component> make() { return std::make_shared<Pickup>(); }
};
} // namespace components