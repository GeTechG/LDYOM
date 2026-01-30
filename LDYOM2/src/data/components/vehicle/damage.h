#pragma once
#include "component.h"
#include <array>
#include <components_manager.h>
#include <fa_icons.h>
#include <imgui.h>
#include <localization.h>
#include <lua_define_type.h>
#include <memory>
#include <optional>
#include <rocket.hpp>


namespace components {

class Vehicle;

/**
 * Vehicle damage component for managing vehicle damage state
 * Stores damage values for wheels, doors, and panels
 */
class VehicleDamage : public Component {
  private:
	std::optional<rocket::scoped_connection> m_vehicleSpawnedConnection;

	void applyDamageToVehicle();

  public:
	enum DirtyFlags {
		None = 0,
		ApplyDamage = 1 << 0,
	};

	SOL_LUA_DEFINE_ENUM_NAMED(DirtyFlags, "DamageComponentDirtyFlags", None, ApplyDamage);

	static constexpr auto TYPE = "vehicle_damage";
	static constexpr auto CATEGORY = "vehicle";

	static std::shared_ptr<VehicleDamage> cast(std::shared_ptr<Component> component) {
		return std::dynamic_pointer_cast<VehicleDamage>(component);
	}

	static Dependencies getDependencies();

	int dirty = None;

	// Damage components array (17 components):
	// 0-3:   Wheels (front_left, front_right, rear_left, rear_right)
	// 4-9:   Doors (bonnet, boot, front_left, front_right, rear_left, rear_right)
	// 10-16: Panels (front_left, front_right, rear_left, rear_right, windscreen, front_bumper, rear_bumper)
	std::array<float, 17> damageComponents = {0.0f};

	VehicleDamage();

	void applyDamage();

	[[nodiscard]] nlohmann::json to_json() const override;
	void from_json(const nlohmann::json& j) override;
	void editorRender() override;

	void onStart() override;
	void onUpdate(float deltaTime) override;
	void onReset() override;

	static void sol_lua_register(sol::state_view lua_state);
	static std::shared_ptr<Component> make() { return std::make_shared<VehicleDamage>(); }
};

} // namespace components
