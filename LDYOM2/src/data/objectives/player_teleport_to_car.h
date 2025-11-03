#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "objective.h"
#include <CPlayerPed.h>
#include <CPools.h>
#include <CStreaming.h>
#include <common.h>
#include <components/vehicle.h>
#include <extensions/ScriptCommands.h>
#include <fa_icons.h>
#include <imgui.h>
#include <ktcoro_wait.hpp>
#include <localization.h>
#include <objectives_manager.h>
#include <project_player.h>
#include <scenes_manager.h>
#include <utils/imgui_configurate.h>
#include <utils_entities_selections.h>

namespace objectives::player_teleport_to_car {
constexpr const char* TYPE = "core.player_teleport_to_car";

struct Data {
	std::string vehicleUuid;
	int seatIndex = 0;        // 0 = driver, 1-3 = passenger seats
	bool enableDriveby = false;
	int markerColor = 0;      // 0 = no marker, 1-6 = colored markers

	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectivePlayerTeleportToCarData");
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, vehicleUuid, seatIndex, enableDriveby, markerColor);
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Data, vehicleUuid, seatIndex, enableDriveby, markerColor);
};

inline std::string tr(const std::string& key) { return _(fmt::format("objectives.{}.{}", std::string(TYPE), key)); }

void renderEditor(Data& data) {
	ImGui::Dummy(ImVec2((SCL_PX).x * 300.f, 0.f));
	const auto availableWidth = ImGui::GetContentRegionAvail().x;

	// Vehicle selection
	renderEntityByComponentSelection<components::Vehicle>(data.vehicleUuid, availableWidth, 0.55f);

	// Seat selection
	ImGui::Text(tr("seat").c_str());
	ImGui::SameLine(availableWidth * 0.55f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::BeginCombo("##seat", _(fmt::format("seat.{}", data.seatIndex)).c_str())) {
		for (int i = 0; i < 4; ++i) {
			if (ImGui::Selectable(_(fmt::format("seat.{}", i)).c_str(), data.seatIndex == i)) {
				data.seatIndex = i;
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// Marker/Blip color
	ImGui::Text(_("color_blip").c_str());
	ImGui::SameLine(availableWidth * 0.55f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::BeginCombo("##color_blip", _(fmt::format("colors_blip.{}", data.markerColor)).c_str())) {
		for (int i = 0; i < 7; ++i) {
			if (ImGui::Selectable(_(fmt::format("colors_blip.{}", i)).c_str(), data.markerColor == i)) {
				data.markerColor = i;
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// Driveby mode
	ImGui::Text(tr("enable_driveby").c_str());
	ImGui::SameLine(availableWidth * 0.55f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##enable_driveby", &data.enableDriveby);
	ImGui::SameLine();
	ImGui::TextDisabled("?");
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip(tr("driveby_tooltip").c_str());
	}
}

inline ktwait execute(Data& data) {
	auto player = FindPlayerPed();
	if (!player) {
		co_return;
	}

	// Find the target vehicle
	std::shared_ptr<components::Vehicle> targetVehicle;
	auto entities = ProjectPlayer::instance().getEntities();
	auto vehicleUuid = uuids::uuid::from_string(data.vehicleUuid);

	for (auto& entity : entities) {
		auto vehicle = components::Vehicle::cast(entity->getComponent(components::Vehicle::TYPE));
		if (!vehicle)
			continue;
		if (entity->id == vehicleUuid && vehicle->handle) {
			targetVehicle = vehicle;
			break;
		}
	}

	if (!targetVehicle || !targetVehicle->handle) {
		co_return;
	}

	auto vehicleRef = targetVehicle->getVehicleRef();
	auto vehiclePos = targetVehicle->handle->GetPosition();

	// Add marker/blip to vehicle if specified (similar to enter_to_vehicle objective)
	int blip = -1;
	if (data.markerColor > 0) {
		blip = addBlipToVehicle(vehicleRef, data.markerColor);
	}

	// Get current interior - DYOM captures this when creating the objective
	// We use the current area visible as vehicles inherit the interior they're in
	int interior = 0;
	plugin::Command<plugin::Commands::GET_AREA_VISIBLE>(&interior);

	// Set interior for both area and player (opcodes 04BB: SET_AREA_VISIBLE and 0860: SET_CHAR_AREA_VISIBLE)
	// Based on DYOM lines 21610-21611
	plugin::Command<plugin::Commands::SET_AREA_VISIBLE>(interior);
	plugin::Command<plugin::Commands::SET_CHAR_AREA_VISIBLE>(player, interior);

	// If player is in a car, remove them first (opcode 0362: WARP_CHAR_FROM_CAR_TO_COORD)
	if (plugin::Command<plugin::Commands::IS_CHAR_IN_ANY_CAR>(player)) {
		plugin::Command<plugin::Commands::WARP_CHAR_FROM_CAR_TO_COORD>(player, vehiclePos.x, vehiclePos.y,
		                                                               vehiclePos.z);
	}

	// Put player into vehicle
	// Based on DYOM logic (lines 21630-21636)
	if (data.seatIndex == 0) {
		// Driver seat (opcode 072A: WARP_CHAR_INTO_CAR)
		plugin::Command<plugin::Commands::WARP_CHAR_INTO_CAR>(player, vehicleRef);
	} else {
		// Passenger seat (opcode 072B: WARP_CHAR_INTO_CAR_AS_PASSENGER)
		// DYOM uses seat-1 because seat 0 is driver, so passenger seats are 0-2
		int passengerSeat = data.seatIndex - 1;
		plugin::Command<plugin::Commands::WARP_CHAR_INTO_CAR_AS_PASSENGER>(player, vehicleRef, passengerSeat);
	}

	// Enable/disable driveby mode (opcodes 0713: TASK_DRIVE_BY and 07CC: SET_PLAYER_CAN_DO_DRIVE_BY)
	if (data.enableDriveby) {
		// Enable driveby shooting mode (opcode 0713)
		plugin::Command<plugin::Commands::TASK_DRIVE_BY>(player, -1, -1, 0.0f, 0.0f, 0.0f, 300.0f, 8, 1, 100);
		// Prevent player from exiting vehicle (opcode 07CC)
		plugin::Command<plugin::Commands::SET_PLAYER_ENTER_CAR_BUTTON>(0, false);
	} else {
		// Allow normal enter/exit behavior
		plugin::Command<plugin::Commands::SET_PLAYER_ENTER_CAR_BUTTON>(0, true);
	}

	// Refresh renderer at position (opcode 04E4: REQUEST_COLLISION and 03CB: LOAD_SCENE)
	plugin::Command<plugin::Commands::REQUEST_COLLISION>(vehiclePos.x, vehiclePos.y);
	plugin::Command<plugin::Commands::LOAD_SCENE>(vehiclePos.x, vehiclePos.y, vehiclePos.z);

	// Reset camera (opcodes 0373: SET_CAMERA_BEHIND_PLAYER and 02EB: RESTORE_CAMERA_JUMPCUT)
	plugin::Command<plugin::Commands::SET_CAMERA_BEHIND_PLAYER>();
	plugin::Command<plugin::Commands::RESTORE_CAMERA_JUMPCUT>();

	// Enable player control
	plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(0, true);

	co_return;
}

Objective make() {
	Data initialData{};

	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), initialData,
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute));
}

ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "vehicle", .builder = make}; }
} // namespace objectives::player_teleport_to_car
