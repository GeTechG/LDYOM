#define IMGUI_DEFINE_MATH_OPERATORS
#include "tools_window.h"
#include <CGame.h>
#include <CPlayerPed.h>
#include <CPools.h>
#include <CStreaming.h>
#include <extensions/ScriptCommands.h>
#include <fa_icons.h>
#include <fmt/format.h>
#include <localization.h>
#include <plugin.h>
#include <popups/vehicle_selector.h>
#include <scenes_manager.h>
#include <task_manager.h>
#include <utils/imgui_configurate.h>
#include <utils/ktcoro_wait.hpp>

static int s_selectedEntityIndex = -1;
static CVehicle* s_spawnedVehicle = nullptr;

static void destroySpawnedVehicle() {
	if (!s_spawnedVehicle)
		return;

	auto player = FindPlayerPed();
	if (player && player->m_pVehicle == s_spawnedVehicle) {
		CVector pos = s_spawnedVehicle->GetPosition();
		plugin::Command<plugin::Commands::WARP_CHAR_FROM_CAR_TO_COORD>(player, pos.x, pos.y, pos.z + 1.0f);
	}

	plugin::Command<plugin::Commands::DELETE_CAR>(s_spawnedVehicle);
	s_spawnedVehicle = nullptr;
}

static ktwait monitorSpawnedVehicle() {
	// Give the warp task a moment to seat the player
	co_await 200;

	while (s_spawnedVehicle != nullptr) {
		auto player = FindPlayerPed();
		if (!player)
			break;

		// Player has left (or been removed from) the spawned vehicle
		if (player->m_pVehicle != s_spawnedVehicle) {
			destroySpawnedVehicle();
			break;
		}

		co_await 1;
	}

	co_return;
}

void ToolsWindow::renderContent(Window* window) {
	float windowWidth = ImGui::GetContentRegionAvail().x;

	// ── Teleport to Entity ──────────────────────────────────────────────
	ImGui::SeparatorText(_("tools.teleport_to_entity").c_str());

	auto& entities = ScenesManager::instance().getCurrentScene().entities;

	if (s_selectedEntityIndex >= static_cast<int>(entities.size()))
		s_selectedEntityIndex = -1;

	const char* previewText = (s_selectedEntityIndex >= 0) ? entities[s_selectedEntityIndex]->name.c_str() : "";

	ImGui::SetNextItemWidth(-1.0f);
	if (ImGui::BeginCombo("##tools_entity_select", previewText)) {
		for (int i = 0; i < static_cast<int>(entities.size()); ++i) {
			bool isSelected = (s_selectedEntityIndex == i);
			if (ImGui::Selectable(fmt::format("{}##{}", entities[i]->name, i).c_str(), isSelected))
				s_selectedEntityIndex = i;
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	ImGui::BeginDisabled(s_selectedEntityIndex < 0);
	if (ImGui::Button(fmt::format("{} {}", ICON_FA_LOCATION_CROSSHAIRS, _("tools.teleport")).c_str(),
	                  ImVec2(-1.f, 0.f))) {
		auto& entity = entities[s_selectedEntityIndex];
		auto player = FindPlayerPed();
		if (player) {
			float x = entity->position[0];
			float y = entity->position[1];
			float z = entity->position[2] + 1.0f;
			plugin::Command<plugin::Commands::SET_CHAR_COORDINATES_DONT_WARP_GANG_NO_OFFSET>(player, x, y, z);
			plugin::Command<plugin::Commands::SET_AREA_VISIBLE>(entity->areaId);
			plugin::Command<plugin::Commands::SET_CHAR_AREA_VISIBLE>(player, entity->areaId);
			plugin::Command<plugin::Commands::REQUEST_COLLISION>(x, y);
			plugin::Command<plugin::Commands::LOAD_SCENE>(x, y, z);
			plugin::Command<plugin::Commands::SET_CAMERA_BEHIND_PLAYER>();
			plugin::Command<plugin::Commands::RESTORE_CAMERA_JUMPCUT>();
		}
	}
	ImGui::EndDisabled();

	// ── Jetpack ─────────────────────────────────────────────────────────
	ImGui::SeparatorText(_("tools.jetpack").c_str());

	if (ImGui::Button(fmt::format("{} {}", ICON_FA_PARACHUTE_BOX, _("tools.give_jetpack")).c_str(),
	                  ImVec2(-1.f, 0.f))) {
		auto player = FindPlayerPed();
		if (player) {
			plugin::Command<plugin::Commands::TASK_JETPACK>(CPools::GetPedRef(player));
		}
	}

	// ── Spawn Vehicle ────────────────────────────────────────────────────
	ImGui::SeparatorText(_("tools.spawn_vehicle").c_str());

	if (s_spawnedVehicle != nullptr) {
		ImGui::TextDisabled(_("tools.spawn_vehicle_active").c_str());
		if (ImGui::Button(fmt::format("{} {}", ICON_FA_TRASH, _("tools.destroy_vehicle")).c_str(),
		                  ImVec2(-1.f, 0.f))) {
			TaskManager::instance().removeTask("tools_monitor_vehicle");
			destroySpawnedVehicle();
		}
	} else {
		if (ImGui::Button(fmt::format("{} {}", ICON_FA_CAR, _("tools.spawn_vehicle_btn")).c_str(),
		                  ImVec2(-1.f, 0.f))) {
			PopupVehicleSelector::showPopup();
		}
	}

	// Always render the popup so it can open
	PopupVehicleSelector::renderPopup([](int model) {
		auto player = FindPlayerPed();
		if (!player)
			return;

		CStreaming::RequestModel(model, GAME_REQUIRED);
		CStreaming::LoadAllRequestedModels(false);

		CVector pos = player->GetPosition();
		s_spawnedVehicle = nullptr;
		plugin::Command<plugin::Commands::CREATE_CAR>(model, pos.x + 2.0f, pos.y, pos.z, &s_spawnedVehicle);
		if (!s_spawnedVehicle)
			return;

		plugin::Command<plugin::Commands::SET_VEHICLE_AREA_VISIBLE>(s_spawnedVehicle, CGame::currArea);
		plugin::Command<plugin::Commands::TASK_WARP_CHAR_INTO_CAR_AS_DRIVER>(player, s_spawnedVehicle);

		TaskManager::instance().removeTask("tools_monitor_vehicle");
		TaskManager::instance().addTask("tools_monitor_vehicle", monitorSpawnedVehicle);
	});

	// ── Close ─────────────────────────────────────────────────────────────
	ImGui::Dummy(ImVec2(0.0f, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeight() -
	                              ImGui::GetStyle().FramePadding.y * 2));

	float buttonWidth = windowWidth * 0.3f;
	ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);
	if (ImGui::Button(_("tools.close").c_str(), ImVec2(buttonWidth, 0.f))) {
		window->close();
	}
}

ToolsWindow::ToolsWindow()
	: ModalPopupWindow(_("tools.title", ICON_FA_SCREWDRIVER_WRENCH)) {
	setSize(400, 320);
	setSizeMin(300, 240);
	setPosition(640, 360);
	setPivot(0.5, 0.5);
	setRenderCallback(ToolsWindow::renderContent);
}
