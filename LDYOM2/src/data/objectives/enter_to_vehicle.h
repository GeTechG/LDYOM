#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "objective.h"
#include <CMessages.h>
#include <blip_color_utils.h>
#include <common.h>
#include <components/vehicle.h>
#include <extensions/ScriptCommands.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <ktcoro_wait.hpp>
#include <localization.h>
#include <objectives_manager.h>
#include <project_player.h>
#include <string_utils.h>
#include <utils/imgui_configurate.h>
#include <utils_entities_selections.h>

namespace objectives::enter_to_vehicle {
constexpr const char* TYPE = "core.enter_to_vehicle";

struct Data {
	std::string vehicleUuid;
	std::string text;
	float textTime = 1.f;
	int colorBlip = 0;

	std::string gameText;

	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectiveEnterToVehicleData");
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, vehicleUuid, text, textTime, colorBlip);
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Data, vehicleUuid, text, textTime, colorBlip);
};

std::string tr(const std::string& key) { return _(fmt::format("objectives.{}.{}", std::string(TYPE), key)); }

void renderEditor(Data& data) {
	ImGui::Dummy(ImVec2((SCL_PX).x * 300.f, 0.f)); // Add some space at the top
	renderEntityByComponentSelection<components::Vehicle>(data.vehicleUuid, ImGui::GetContentRegionAvail().x, 0.45f);
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
	std::shared_ptr<components::Vehicle> targetVehicle;
	auto entities = ProjectPlayer::instance().getEntities();
	auto vehicleUuid = uuids::uuid::from_string(data.vehicleUuid);
	for (auto& entity : entities) {
		auto vehicle = components::Vehicle::cast(entity->getComponent(components::Vehicle::TYPE));
		if (!vehicle)
			continue;
		if (entity->id == vehicleUuid && vehicle->handle) {
			targetVehicle = vehicle;
		}
	}

	if (!targetVehicle) {
		co_return;
	}

	int blip = -1;
	if (data.colorBlip > 0) {
		blip = addBlipToVehicle(targetVehicle->getVehicleRef(), data.colorBlip);
	}

	if (!data.text.empty()) {
		auto cp1251Text = utf8_to_cp1251(data.text);
		gxt_encode(cp1251Text);
		data.gameText = cp1251Text;

		CMessages::AddMessage(const_cast<char*>(data.gameText.c_str()), static_cast<unsigned>(data.textTime * 1000.0f),
		                      0, false);
	}

	while (!plugin::Command<plugin::Commands::IS_CHAR_IN_CAR>(CPools::GetPedRef(FindPlayerPed()),
	                                                          targetVehicle->getVehicleRef())) {
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

ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "vehicle", .builder = make}; }
} // namespace objectives::enter_to_vehicle