#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "enter_to_vehicle.h"
#include "objective.h"
#include <CMessages.h>
#include <blip_color_utils.h>
#include <components/checkpoint.h>
#include <extensions/ScriptCommands.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <ktcoro_wait.hpp>
#include <localization.h>
#include <objectives_manager.h>
#include <project_player.h>
#include <scenes_manager.h>
#include <string_utils.h>
#include <utils/imgui_configurate.h>
#include <utils_entities_selections.h>

namespace objectives::checkpoint {
constexpr const char* TYPE = "core.checkpoint";

struct Data {
	std::string checkpointUuid;
	std::string text;
	float textTime = 1.f;
	int colorBlip = 0;

	std::string gameText;

	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectiveCheckpointData");
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, checkpointUuid, text, textTime, colorBlip);
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Data, checkpointUuid, text, textTime, colorBlip)
};

std::string tr(const std::string& key) { return _(fmt::format("objectives.{}.{}", std::string(TYPE), key)); }

void renderEditor(Data& data) {
	ImGui::Dummy(ImVec2((SCL_PX).x * 300.f, 0.f)); // Add some space at the top

	renderEntityByComponentSelection<components::Checkpoint>(data.checkpointUuid, ImGui::GetContentRegionAvail().x,
	                                                         0.45f);

	ImGui::Text(_("text").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::InputText("##text", &data.text);

	ImGui::Text(_("text_time").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::DragFloat("##text_time", &data.textTime, 0.1f, 0.f, 10.f, "%.1f");

	ImGui::Text(_("color_blip").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	if (ImGui::BeginCombo("##color_blip", _(fmt::format("colors_blip.{}", data.colorBlip)).c_str())) {
		for (int i = 0; i < 7; ++i) {
			if (ImGui::Selectable(_(fmt::format("colors_blip.{}", i)).c_str(), data.colorBlip == i)) {
				data.colorBlip = i;
			}
		}
		ImGui::EndCombo();
	}
}

ktwait execute(Data& data) {
	std::shared_ptr<components::Checkpoint> targetCheckpoint;
	auto entities = ProjectPlayer::instance().getEntities();
	auto checkpointUuid = uuids::uuid::from_string(data.checkpointUuid);
	for (auto& entity : entities) {
		auto checkpoint = components::Checkpoint::cast(entity->getComponent(components::Checkpoint::TYPE));
		if (!checkpoint)
			continue;
		if (entity->id == checkpointUuid && checkpoint->handle) {
			targetCheckpoint = checkpoint;
		}
	}

	if (!targetCheckpoint) {
		co_return;
	}

	std::optional<objectives::enter_to_vehicle::Data*> enterToVehicleData;
	std::shared_ptr<components::Vehicle> targetVehicle;

	auto currentObjectiveIndex = ProjectPlayer::instance().getCurrentObjectiveIndex();
	auto& objectives = ScenesManager::instance().getUnsafeCurrentScene().objectives.data;

	for (int i = currentObjectiveIndex - 1; i >= 0; --i) {
		auto& objective = objectives[i];
		if (objective.type == objectives::enter_to_vehicle::TYPE) {
			enterToVehicleData = &objective.getValue<objectives::enter_to_vehicle::Data>();
			auto entities = ProjectPlayer::instance().getEntities();
			auto vehicleUuid = uuids::uuid::from_string(enterToVehicleData.value()->vehicleUuid);
			for (auto& entity : entities) {
				auto vehicle = components::Vehicle::cast(entity->getComponent(components::Vehicle::TYPE));
				if (!vehicle)
					continue;
				if (entity->id == vehicleUuid && vehicle->handle) {
					targetVehicle = vehicle;
				}
			}
			break;
		} else if (objective.type != objectives::checkpoint::TYPE) {
			break;
		}
	}

	if (!data.text.empty()) {
		auto cp1251Text = utf8_to_cp1251(data.text);
		gxt_encode(cp1251Text);
		data.gameText = cp1251Text;

		CMessages::AddMessage(const_cast<char*>(data.gameText.c_str()), static_cast<unsigned>(data.textTime * 1000.0f),
		                      0, false);
	}

	int blip = -1;
	if (data.colorBlip > 0) {
		blip = addBlipToCoord(targetCheckpoint->entity->position[0], targetCheckpoint->entity->position[1],
		                      targetCheckpoint->entity->position[2], data.colorBlip);
	}

	while (true) {
		auto playerHandle = CPools::GetPedRef(FindPlayerPed());
		if (targetVehicle) {
			if (!plugin::Command<plugin::Commands::IS_CHAR_IN_CAR>(playerHandle, targetVehicle->getVehicleRef())) {
				plugin::Command<plugin::Commands::PRINT_NOW>("GOCAR", 2000, 1);

				if (blip != -1) {
					plugin::Command<plugin::Commands::REMOVE_BLIP>(blip);
				}
				blip = -1;

				int carBlip = -1;
				if (enterToVehicleData) {
					if (enterToVehicleData.value()->colorBlip > 0) {
						carBlip =
							addBlipToVehicle(targetVehicle->getVehicleRef(), enterToVehicleData.value()->colorBlip);
					}
				}
				while (
					!plugin::Command<plugin::Commands::IS_CHAR_IN_CAR>(playerHandle, targetVehicle->getVehicleRef())) {
					co_await 10;
				}

				if (carBlip != -1) {
					plugin::Command<plugin::Commands::REMOVE_BLIP>(carBlip);
				}

				if (data.colorBlip > 0) {
					blip = addBlipToCoord(targetCheckpoint->entity->position[0], targetCheckpoint->entity->position[1],
					                      targetCheckpoint->entity->position[2], data.colorBlip);
				}
			} else {
				if (targetCheckpoint->typeCheckpoint == 0 || targetCheckpoint->typeCheckpoint == 5) { // Sphere
					if (plugin::Command<plugin::Commands::LOCATE_CHAR_IN_CAR_3D>(
							playerHandle, targetCheckpoint->entity->position[0], targetCheckpoint->entity->position[1],
							targetCheckpoint->entity->position[2], targetCheckpoint->radius, targetCheckpoint->radius,
							targetCheckpoint->radius, false)) {
						break;
					}
				} else {
					if (plugin::Command<plugin::Commands::LOCATE_CHAR_IN_CAR_2D>(
							playerHandle, targetCheckpoint->entity->position[0], targetCheckpoint->entity->position[1],
							targetCheckpoint->radius, targetCheckpoint->radius, false)) {
						break;
					}
				}
			}
		} else {
			if (targetCheckpoint->typeCheckpoint == 0 || targetCheckpoint->typeCheckpoint == 5) { // Sphere
				if (plugin::Command<plugin::Commands::LOCATE_CHAR_ON_FOOT_3D>(
						playerHandle, targetCheckpoint->entity->position[0], targetCheckpoint->entity->position[1],
						targetCheckpoint->entity->position[2], targetCheckpoint->radius, targetCheckpoint->radius,
						targetCheckpoint->radius, false)) {
					break;
				}
			} else {
				if (plugin::Command<plugin::Commands::LOCATE_CHAR_ON_FOOT_2D>(
						playerHandle, targetCheckpoint->entity->position[0], targetCheckpoint->entity->position[1],
						targetCheckpoint->radius, targetCheckpoint->radius, false)) {
					break;
				}
			}
		}
		co_await 10;
	}

	if (blip != -1) {
		plugin::Command<plugin::Commands::REMOVE_BLIP>(blip);
	}
}

Objective make() {
	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), Data{},
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute));
}

ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "checkpoint", .builder = make}; }
} // namespace objectives::checkpoint