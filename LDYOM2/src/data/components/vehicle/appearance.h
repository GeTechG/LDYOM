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
 * Vehicle appearance component for managing visual and physical properties
 * Stores hydraulics angle, lights state, door positions, dirt level, and mass
 */
class VehicleAppearance : public Component {
  private:
	std::optional<rocket::scoped_connection> m_vehicleSpawnedConnection;

	void applyAppearanceToVehicle();

  public:
	enum DirtyFlags {
		None = 0,
		ApplyAppearance = 1 << 0,
	};

	SOL_LUA_DEFINE_ENUM_NAMED(DirtyFlags, "AppearanceComponentDirtyFlags", None, ApplyAppearance);

	static constexpr auto TYPE = "vehicle_appearance";
	static constexpr auto CATEGORY = "vehicle";

	static std::shared_ptr<VehicleAppearance> cast(std::shared_ptr<Component> component) {
		return std::dynamic_pointer_cast<VehicleAppearance>(component);
	}

	static Dependencies getDependencies();

	int dirty = None;

	// Hydraulics/extra parts angle (for lowriders, etc.)
	float extraPartsAngle = 0.0f;

	// Lights state (headlights on/off)
	bool isLightsOn = false;

	// Door open ratios (0.0 = closed, 1.0 = fully open)
	// Order: bonnet, boot, front_left, front_right, rear_left, rear_right
	std::array<float, 6> openDoorsRation = {0.0f};

	// Dirt/mud level (0.0 = clean, higher values = dirtier)
	float dirtyLevel = 0.0f;

	// Heavy flag (affects physics/handling)
	bool heavy = false;

	VehicleAppearance();

	void applyAppearance();

	[[nodiscard]] nlohmann::json to_json() const override;
	void from_json(const nlohmann::json& j) override;
	void editorRender() override;

	void onStart() override;
	void onUpdate(float deltaTime) override;
	void onReset() override;
	void onDestroy() override;

	static void sol_lua_register(sol::state_view lua_state);
	static std::shared_ptr<Component> make() { return std::make_shared<VehicleAppearance>(); }
};

} // namespace components
