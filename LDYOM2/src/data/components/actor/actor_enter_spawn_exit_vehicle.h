#pragma once
#include "component.h"
#include <components_manager.h>
#include <fa_icons.h>
#include <imgui.h>
#include <localization.h>
#include <lua_define_type.h>
#include <memory>
#include <rocket.hpp>
#include <string>

namespace components {

class Actor;
class Vehicle;

class ActorEnterSpawnExitVehicle : public Component {
  private:
	std::optional<rocket::scoped_connection> m_pedSpawnedConnection;

	std::shared_ptr<components::Vehicle> findTargetVehicle(std::shared_ptr<components::Actor> actor);
	std::shared_ptr<components::Vehicle> findNearestVehicle(std::shared_ptr<components::Actor> actor);
	std::shared_ptr<components::Vehicle> findVehicleById();
	void applyVehicleTask(std::shared_ptr<components::Actor> actor, std::shared_ptr<components::Vehicle> vehicle);

  public:
	static constexpr auto TYPE = "actor_enter_spawn_exit_vehicle";
	static constexpr auto CATEGORY = "actor";

	static std::shared_ptr<ActorEnterSpawnExitVehicle> cast(std::shared_ptr<Component> component) {
		return std::dynamic_pointer_cast<ActorEnterSpawnExitVehicle>(component);
	}

	static Dependencies getDependencies();

	bool nearestVehicle = false;
	std::string vehicleId = "";
	int seatIndex = 0; // 0 - driver, 1 - passenger, 2 - left rear, 3 - right rear
	int taskType = 0;  // 0 - enter, 1 - spawn, 2 - exit

	ActorEnterSpawnExitVehicle();

	void run();

	[[nodiscard]] nlohmann::json to_json() const override;

	void from_json(const nlohmann::json& j) override;

	void editorRender() override;

	void onStart() override;
	void onUpdate(float deltaTime) override;
	void onReset() override;

	static void sol_lua_register(sol::state_view lua_state);
	static std::shared_ptr<Component> make() { return std::make_shared<ActorEnterSpawnExitVehicle>(); }
};
} // namespace components