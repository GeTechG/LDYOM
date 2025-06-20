#pragma once
#include "component.h"
#include <components_manager.h>
#include <fa_icons.h>
#include <imgui.h>
#include <ktcoro_wait.hpp>
#include <localization.h>
#include <lua_define_type.h>
#include <memory>
#include <rocket.hpp>
#include <string>

namespace components {

class Actor;
class Vehicle;

class ActorDriveByPath : public Component {
  private:
	std::optional<rocket::scoped_connection> m_pedSpawnedConnection;

	std::shared_ptr<components::Vehicle> findTargetVehicle(std::shared_ptr<components::Actor> actor);
	std::shared_ptr<components::Vehicle> findNearestVehicle(std::shared_ptr<components::Actor> actor);
	std::shared_ptr<components::Vehicle> findVehicleById();
	void renderVehicleSelection(float availableWidth, float labelWidth);

	static ktwait enterVehicle(ActorDriveByPath* _this, std::shared_ptr<components::Actor> actor,
	                           std::shared_ptr<components::Vehicle> vehicle);

  public:
	static constexpr auto TYPE = "actor_drive_by_path";
	static constexpr auto CATEGORY = "actor";

	static std::shared_ptr<ActorDriveByPath> cast(std::shared_ptr<Component> component) {
		return std::dynamic_pointer_cast<ActorDriveByPath>(component);
	}

	static Dependencies getDependencies();

	bool nearestVehicle = false;
	std::string vehicleId = "";
	bool teleportToVehicle = false;
	int speedType = 0; // 0 - normal, 1 - fast, 2 - very fast
	std::vector<std::array<float, 3>> points;
	bool isLooped = false;
	bool leaveVehicle = false;

	ActorDriveByPath();

	void run();

	[[nodiscard]] nlohmann::json to_json() const override;

	void from_json(const nlohmann::json& j) override;

	void editorRender() override;

	void onStart() override;
	void onUpdate(float deltaTime) override;
	void onReset() override;

	static void sol_lua_register(sol::state_view lua_state);
	static std::shared_ptr<Component> make() { return std::make_shared<ActorDriveByPath>(); }
};
} // namespace components