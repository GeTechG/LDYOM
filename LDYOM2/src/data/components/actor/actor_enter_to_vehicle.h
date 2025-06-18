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

class ActorEnterToVehicle : public Component {
  private:
	std::optional<rocket::scoped_connection> m_pedSpawnedConnection;

	std::shared_ptr<components::Vehicle> findTargetVehicle(std::shared_ptr<components::Actor> actor);
	std::shared_ptr<components::Vehicle> findNearestVehicle(std::shared_ptr<components::Actor> actor);
	std::shared_ptr<components::Vehicle> findVehicleById();
	void enterVehicle(std::shared_ptr<components::Actor> actor, std::shared_ptr<components::Vehicle> vehicle);
	void renderVehicleSelection(float availableWidth, float labelWidth);

  public:
	static constexpr auto TYPE = "actor_enter_to_vehicle";
	static constexpr auto CATEGORY = "actor";

	static std::shared_ptr<ActorEnterToVehicle> cast(std::shared_ptr<Component> component) {
		return std::dynamic_pointer_cast<ActorEnterToVehicle>(component);
	}

	static Dependencies getDependencies();

	bool nearestVehicle = false;
	std::string vehicleId = "";
	int seatIndex = 0; // 0 - driver, 1 - passenger, 2 - left rear, 3 - right rear
	bool teleportToVehicle = false;

	ActorEnterToVehicle();

	void run();

	[[nodiscard]] nlohmann::json to_json() const override;

	void from_json(const nlohmann::json& j) override;

	void editorRender() override;

	void onStart() override;
	void onUpdate(float deltaTime) override;
	void onReset() override;

	static void sol_lua_register(sol::state_view lua_state);
	static std::shared_ptr<Component> make() { return std::make_shared<ActorEnterToVehicle>(); }
};
} // namespace components