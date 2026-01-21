#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "objective.h"
#include <CPlayerPed.h>
#include <components/vehicle.h>
#include <extensions/ScriptCommands.h>
#include <imgui.h>
#include <ktcoro_wait.hpp>
#include <localization.h>
#include <objectives_manager.h>
#include <project_player.h>
#include <utils/imgui_configurate.h>
#include <utils_entities_selections.h>

namespace objectives::player_vehicle_task {
constexpr const char* TYPE = "core.player_vehicle_task";

struct Data {
	bool nearestVehicle = false;
	std::string vehicleId = "";
	int seatIndex = 0; // 0 - driver, 1 - passenger, 2 - left rear, 3 - right rear
	int taskType = 0;  // 0 - enter, 1 - spawn, 2 - exit

	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectivePlayerVehicleTaskData");
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, nearestVehicle, vehicleId, seatIndex, taskType);
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Data, nearestVehicle, vehicleId, seatIndex, taskType);
};

inline std::string tr(const std::string& key) { return _(fmt::format("objectives.{}.{}", std::string(TYPE), key)); }

std::shared_ptr<components::Vehicle> findNearestVehicle(CPlayerPed* player) {
	auto entities = ProjectPlayer::instance().getEntities();

	std::vector<Entity*> vehicles;
	for (auto& entity : entities) {
		if (entity->hasComponent(components::Vehicle::TYPE)) {
			vehicles.push_back(entity);
		}
	}

	auto nearestVehicleIt = std::ranges::min_element(vehicles, [player](Entity* a, Entity* b) {
		auto aVehicle = components::Vehicle::cast(a->getComponent(components::Vehicle::TYPE));
		auto bVehicle = components::Vehicle::cast(b->getComponent(components::Vehicle::TYPE));

		if (!aVehicle || !bVehicle || !aVehicle->handle || !bVehicle->handle) {
			return false;
		}
		return DistanceBetweenPoints(aVehicle->handle->GetPosition(), player->GetPosition()) <
		       DistanceBetweenPoints(bVehicle->handle->GetPosition(), player->GetPosition());
	});

	return nearestVehicleIt != vehicles.end()
	           ? components::Vehicle::cast((*nearestVehicleIt)->getComponent(components::Vehicle::TYPE))
	           : nullptr;
}

std::shared_ptr<components::Vehicle> findVehicleById(const std::string& id) {
	auto vehicleUuid = uuids::uuid::from_string(id);
	auto entities = ProjectPlayer::instance().getEntities();
	auto vehicleIt = std::ranges::find_if(entities, [vehicleUuid](Entity* entity) {
		return entity->id == vehicleUuid && entity->hasComponent(components::Vehicle::TYPE);
	});

	return vehicleIt != entities.end()
	           ? components::Vehicle::cast((*vehicleIt)->getComponent(components::Vehicle::TYPE))
	           : nullptr;
}

void renderEditor(Data& data) {
	ImGui::Dummy(ImVec2((SCL_PX).x * 300.f, 0.f)); // Add some space at the top
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	ImGui::Text(tr("task_type").c_str());
	ImGui::SameLine(availableWidth * 0.6f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::SliderInt("##taskType", &data.taskType, 0, 2, tr(fmt::format("task_types.{}", data.taskType)).c_str());

	ImGui::Text(tr("seat_index").c_str());
	ImGui::SameLine(availableWidth * 0.6f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::SliderInt("##seatIndex", &data.seatIndex, 0, 3, tr(fmt::format("seat_indices.{}", data.seatIndex)).c_str());

	ImGui::Text(tr("nearest_vehicle").c_str());
	ImGui::SameLine(availableWidth * 0.6f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##nearestVehicle", &data.nearestVehicle);

	if (!data.nearestVehicle) {
		renderEntityByComponentSelection<components::Vehicle>(data.vehicleId, availableWidth, 0.6f);
	}
}

inline ktwait execute(Data& data) {
	auto player = FindPlayerPed();
	if (!player) {
		co_return;
	}

	// Find target vehicle
	std::shared_ptr<components::Vehicle> targetVehicle;
	if (data.nearestVehicle) {
		targetVehicle = findNearestVehicle(player);
	} else {
		targetVehicle = findVehicleById(data.vehicleId);
	}

	if (!targetVehicle || !targetVehicle->handle) {
		co_return;
	}

	const bool isDriver = (data.seatIndex == 0);
	const int passengerSeat = data.seatIndex - 1;

	// Same logic as ActorEnterSpawnExitVehicle component
	switch (data.taskType) {
		case 0: // Enter
			if (isDriver) {
				plugin::Command<plugin::Commands::TASK_ENTER_CAR_AS_DRIVER>(player, targetVehicle->getVehicleRef(),
				                                                            10000);
			} else {
				plugin::Command<plugin::Commands::TASK_ENTER_CAR_AS_PASSENGER>(player, targetVehicle->getVehicleRef(),
				                                                               10000, passengerSeat);
			}
			break;
		case 1: // Spawn (instant warp)
			if (isDriver) {
				plugin::Command<plugin::Commands::TASK_WARP_CHAR_INTO_CAR_AS_DRIVER>(player,
				                                                                     targetVehicle->getVehicleRef());
			} else {
				plugin::Command<plugin::Commands::TASK_WARP_CHAR_INTO_CAR_AS_PASSENGER>(
					player, targetVehicle->getVehicleRef(), passengerSeat);
			}
			break;
		case 2: // Exit
			// First warp into car, then exit after a delay
			if (isDriver) {
				plugin::Command<plugin::Commands::TASK_WARP_CHAR_INTO_CAR_AS_DRIVER>(player,
				                                                                     targetVehicle->getVehicleRef());
			} else {
				plugin::Command<plugin::Commands::TASK_WARP_CHAR_INTO_CAR_AS_PASSENGER>(
					player, targetVehicle->getVehicleRef(), passengerSeat);
			}
			co_await 1000;
			plugin::Command<plugin::Commands::TASK_LEAVE_CAR>(player, targetVehicle->getVehicleRef());
			break;
		default: break;
	}

	co_return;
}

Objective make() {
	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), Data{},
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute));
}

ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "player", .builder = make}; }
} // namespace objectives::player_vehicle_task
