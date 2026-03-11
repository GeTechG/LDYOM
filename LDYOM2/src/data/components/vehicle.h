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
#include <utils/vehicle_materials.h>

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

	// Game Colors Mode (uses GTA color palette)
	int primaryColorId = 0;
	int secondaryColorId = 0;
	int tertiaryColorId = 0;
	int quaternaryColorId = 0;

	// Custom Colors Mode
	bool isGameColorsMode = true;
	std::array<float, 4> primaryColor = {1.0f, 1.0f, 1.0f, 1.0f};
	std::array<float, 4> secondaryColor = {1.0f, 1.0f, 1.0f, 1.0f};
	bool extendedColor = false;
	std::vector<std::pair<unsigned char, std::array<float, 4>>> colors;

	std::array<int, 15> upgrades = {-1};
	int paintjob = -1;
	int componentTypeA = -1;
	int componentTypeB = -1;
	std::string numberplate = "";
	int numberplateCity = -1;
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

	void spawn() override;
	void despawn() override;
	int getVehicleRef() const { return this->handle ? CPools::GetVehicleRef(this->handle.get()) : -1; }

	// Color methods
	void recolorVehicle(bool recolor);
	void setEditorPrimaryColor();
	void setEditorSecondaryColor();
	bool isRecolorBanned() const;

	// Helper to check if model is in recolor ban list
	static bool isModelRecolorBanned(int model);

	static bool isSpecialComponent() { return true; }
	static void sol_lua_register(sol::state_view lua_state);

	static std::shared_ptr<Component> make() { return std::make_shared<Vehicle>(); }

  private:
	bool needToRecolor = false;
	// Helicopter models that cannot use custom colors
	static inline const std::vector<int> recolorBanList = {
		417, // MODEL_LEVIATHN
		425, // MODEL_HUNTER
		447, // MODEL_SEASPAR
		464, // MODEL_RCBARON
		465, // MODEL_RCRAIDER
		469, // MODEL_SPARROW
		487, // MODEL_MAVERICK
		488, // MODEL_VCNMAV
		497, // MODEL_POLMAV
		501, // MODEL_RCGOBLIN
		548, // MODEL_CARGOBOB
		563  // MODEL_RAINDANC
	};
};
} // namespace components