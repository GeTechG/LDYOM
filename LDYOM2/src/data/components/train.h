#pragma once
#include "component.h"
#include <CPools.h>
#include <CTrain.h>
#include <components_manager.h>
#include <fa_icons.h>
#include <imgui.h>
#include <localization.h>
#include <lua_define_type.h>
#include <memory>
#include <optional>
#include <rocket.hpp>

namespace components {

/**
 * Train component for spawning and managing mission trains in GTA SA
 * Supports different train types (freight, streak, tram) with customizable properties
 */
class Train : public Component {
  private:
	void updatePosition();

	std::optional<rocket::scoped_connection> onSpawnedConnection;
	std::optional<rocket::scoped_connection> onDespawnedConnection;

  public:
	enum DirtyFlags {
		None = 0,
		Speed = 1 << 1,
		Model = 1 << 2,
	};
	SOL_LUA_DEFINE_ENUM_NAMED(DirtyFlags, "TrainComponentDirtyFlags", None, Speed, Model);

	static constexpr auto TYPE = "train";
	static constexpr auto CATEGORY = "entities";

	static std::shared_ptr<Train> cast(std::shared_ptr<Component> component) {
		return std::dynamic_pointer_cast<Train>(component);
	}

	static Dependencies getDependencies() { return Dependencies{{}, true}; }

	// Train type ID (0-15, determines model and track)
	// 0,3,6,10,12,13 = Freight trains
	// 1,2,4,5,7,11 = Streak trains (brown streak)
	// 8,9,14 = Trams
	// 15 = Single streak car
	int trainType = 0;

	// Spawn train rotated 180 degrees
	bool rotate = false;

	// Train speed (current speed)
	float speed = 0.0f;

	// Train cruise speed (target speed)
	float cruiseSpeed = 0.0f;

	// Train health
	float health = 1000.0f;

	int dirty = DirtyFlags::None;
	std::shared_ptr<CTrain> handle;

	rocket::thread_safe_signal<void()> onSpawned;
	rocket::thread_safe_signal<void()> onDespawned;

	Train();

	[[nodiscard]] nlohmann::json to_json() const override;
	void from_json(const nlohmann::json& j) override;
	void editorRender() override;

	void onStart() override;
	void onUpdate(float deltaTime) override;
	void onReset() override;

	void spawn();
	void despawn();
	int getTrainRef() const { return this->handle ? CPools::GetVehicleRef(this->handle.get()) : -1; }

	static bool isSpecialComponent() { return true; }
	static void sol_lua_register(sol::state_view lua_state);
	static std::shared_ptr<Component> make() { return std::make_shared<Train>(); }
};

} // namespace components
