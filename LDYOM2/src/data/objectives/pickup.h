#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "objective.h"
#include <CMessages.h>
#include <CText.h>
#include <blip_color_utils.h>
#include <common.h>
#include <components/pickup.h>
#include <counter_service.h>
#include <extensions/ScriptCommands.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_widgets/imgui_widgets.h>
#include <ktcoro_wait.hpp>
#include <localization.h>
#include <objectives_manager.h>
#include <project_player.h>
#include <string_utils.h>
#include <utils/imgui_configurate.h>
#include <utils_entities_selections.h>


namespace objectives::pickup {
constexpr const char* TYPE = "core.pickup";

struct Data {
	std::string pickupUuid;
	bool collectAll = false;  // Collect all pickups of the same type
	bool showCounter = false; // Show counter on screen
	std::string text;
	float textTime = 1.f;
	int colorBlip = 0;

	std::string gameText;

	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectivePickupData");
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, pickupUuid, collectAll, showCounter, text, textTime,
		                 colorBlip);
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Data, pickupUuid, collectAll, showCounter, text, textTime, colorBlip);
};

std::string tr(const std::string& key) { return _(fmt::format("objectives.{}.{}", std::string(TYPE), key)); }

void renderEditor(Data& data) {
	ImGui::Dummy(ImVec2((SCL_PX).x * 300.f, 0.f)); // Add some space at the top
	renderEntityByComponentSelection<components::Pickup>(data.pickupUuid, ImGui::GetContentRegionAvail().x, 0.45f);

	ImGui::Text(tr("collect_all").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##collect_all", &data.collectAll);

	ImGui::Text(tr("show_counter").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::Checkbox("##show_counter", &data.showCounter);

	ImGui::Text(_("text").c_str());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x * 0.45f);
	ImGui::SetNextItemWidth(-1.f);
	ImGui::InputTextWithPopup("##text", &data.text);

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
	std::shared_ptr<components::Pickup> targetPickup;
	auto entities = ProjectPlayer::instance().getEntities();
	auto pickupUuid = uuids::uuid::from_string(data.pickupUuid);

	// Find the target pickup entity
	for (auto& entity : entities) {
		auto pickup = components::Pickup::cast(entity->getComponent(components::Pickup::TYPE));
		if (!pickup)
			continue;
		if (entity->id == pickupUuid && pickup->handle) {
			targetPickup = pickup;
			break;
		}
	}

	if (!targetPickup) {
		co_return;
	}

	// Collect all pickups to track
	std::vector<std::shared_ptr<components::Pickup>> pickupsToCollect;

	if (data.collectAll) {
		// Find all pickups with the same model
		int targetModel = targetPickup->pickupType;
		for (auto& entity : entities) {
			auto pickup = components::Pickup::cast(entity->getComponent(components::Pickup::TYPE));
			if (!pickup || !pickup->handle)
				continue;
			if (pickup->pickupType == targetModel) {
				pickupsToCollect.push_back(pickup);
			}
		}
	} else {
		// Just the target pickup
		pickupsToCollect.push_back(targetPickup);
	}

	// Create blips for all pickups
	std::vector<int> blips;
	if (data.colorBlip > 0) {
		blips.reserve(pickupsToCollect.size());
		for (auto& pickup : pickupsToCollect) {
			int blip = addBlipToPickup(pickup->handle, data.colorBlip);
			blips.push_back(blip);
		}
	}

	// Display text if provided
	if (!data.text.empty()) {
		auto cp1251Text = utf8_to_cp1251(data.text);
		gxt_encode(cp1251Text);
		data.gameText = cp1251Text;

		CMessages::AddMessage(const_cast<char*>(data.gameText.c_str()), static_cast<unsigned>(data.textTime * 1000.0f),
		                      0, false);
	}

	auto counterId = -1;

	// Show counter if enabled
	if (data.showCounter) {
		auto title = TheText.Get("TOGO");
		counterId = CounterService::instance().addNumberCounter(title, static_cast<int>(pickupsToCollect.size()));
	}

	// Wait until all pickups are collected
	while (!pickupsToCollect.empty()) {
		for (auto it = pickupsToCollect.begin(); it != pickupsToCollect.end();) {
			auto& pickup = *it;

			if (!pickup->handle || plugin::Command<plugin::Commands::HAS_PICKUP_BEEN_COLLECTED>(pickup->handle)) {
				// Remove blip if exists
				if (data.colorBlip > 0 && !blips.empty()) {
					size_t index = std::distance(pickupsToCollect.begin(), it);
					if (index < blips.size()) {
						plugin::Command<plugin::Commands::REMOVE_BLIP>(blips[index]);
						blips.erase(blips.begin() + index);
					}
				}
				it = pickupsToCollect.erase(it);

				// Update counter
				if (data.showCounter) {
					CounterService::instance().updateCounter(counterId, static_cast<int>(pickupsToCollect.size()));
				}
			} else {
				++it;
			}
		}

		co_await 10;
	}

	// Remove counter
	if (data.showCounter) {
		CounterService::instance().clearAllCounters();
	}

	// Clean up remaining blips
	for (auto& blip : blips) {
		plugin::Command<plugin::Commands::REMOVE_BLIP>(blip);
	}
}

Objective make() {
	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"), Data{},
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute));
}

ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "pickup", .builder = make}; }
} // namespace objectives::pickup
