#pragma once
#include "component.h"
#include <CPools.h>
#include <CVehicle.h>
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

class Vehicle : public Component {
  private:
	void updateDirection();
	void updatePosition();

	std::optional<rocket::scoped_connection> onSpawnedConnection;
	std::optional<rocket::scoped_connection> onDespawnedConnection;

  public:
	enum DirtyFlags {
		None = 0,
		Position = 1 << 0,
		Direction = 1 << 1,
		Model = 1 << 2,
	};
	SOL_LUA_DEFINE_ENUM_NAMED(DirtyFlags, "VehicleComponentDirtyFlags", None, Position, Direction, Model);

	static constexpr auto TYPE = "vehicle";
	static constexpr auto CATEGORY = "entities";

	static std::shared_ptr<Vehicle> cast(std::shared_ptr<Component> component) {
		return std::dynamic_pointer_cast<Vehicle>(component);
	}

	static Dependencies getDependencies() { return Dependencies{{}, true}; }

	float initialDirection = 0.0f;
	int model = 400;
	int primaryColorId = 0;
	int secondaryColorId = 0;
	int tertiaryColorId = 0;
	int quaternaryColorId = 0;
	float health = 1000.0f;
	bool bulletproof = false;
	bool fireproof = false;
	bool explosionproof = false;
	bool collisionproof = false;
	bool meleeproof = false;
	bool tiresVulnerability = false;
	bool mustSurvive = false;
	bool locked = false;

	int dirty = DirtyFlags::None;
	std::shared_ptr<CVehicle> handle;

	rocket::thread_safe_signal<void()> onSpawned;
	rocket::thread_safe_signal<void()> onDespawned;

	Vehicle();

	[[nodiscard]] nlohmann::json to_json() const override;

	void from_json(const nlohmann::json& j) override;

	void editorRender() override;

	void onStart() override;
	void onUpdate(float deltaTime) override;
	void onReset() override;

	void spawn();
	void despawn();
	int getVehicleRef() const { return this->handle ? CPools::GetVehicleRef(this->handle.get()) : -1; }

	static bool isSpecialComponent() { return true; }
	static void sol_lua_register(sol::state_view lua_state);

	static std::shared_ptr<Component> make() { return std::make_shared<Vehicle>(); }
};
} // namespace components