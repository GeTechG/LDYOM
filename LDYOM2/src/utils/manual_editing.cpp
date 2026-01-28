#include "manual_editing.h"
#include "../data/components/vehicle.h"
#include <CCamera.h>
#include <CPlayerPed.h>
#include <CPools.h>
#include <common.h>
#include <entity.h>
#include <extensions/ScriptCommands.h>
#include <fa_icons.h>
#include <imgui.h>
#include <localization.h>
#include <task_manager.h>
#include <window_manager.h>

using namespace plugin;

ktwait editVehicleManuallyTask(components::Vehicle* vehicle, std::function<void()> onComplete) {
	// RAII-style cleanup guard to ensure onComplete is always called
	struct CleanupGuard {
		std::function<void()> callback;
		bool completed = false;
		~CleanupGuard() {
			if (!completed && callback) {
				callback();
			}
		}
	} cleanupGuard{onComplete};

	// Disable ImGui windows rendering
	WindowManager::instance().disableWindowRendering(true);

	// Enable player control
	Command<Commands::SET_PLAYER_CONTROL>(0, 1);

	// Add overlay with instructions
	WindowManager::instance().addBackgroundRenderCallback("editVehicleManually", [] {
		constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
		                                         ImGuiWindowFlags_NoSavedSettings |
		                                         ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
		if (ImGui::Begin("##editVehicleOverlay", nullptr, windowFlags)) {
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 16.5f);
			ImGui::TextWrapped("%s %s", ICON_FA_CIRCLE_INFO, _("components.vehicle.edit_manually_instruction").c_str());
			ImGui::PopTextWrapPos();
		}
		ImGui::End();
	});

	// Restore camera
	TheCamera.Restore();

	// Get vehicle handle
	auto vehicleHandle = vehicle->handle.get();
	if (!vehicleHandle) {
		// If vehicle not spawned, clean up and exit
		WindowManager::instance().removeBackgroundRenderCallback("editVehicleManually");
		WindowManager::instance().disableWindowRendering(false);
		Command<Commands::SET_PLAYER_CONTROL>(0, 0);
		// cleanupGuard destructor will call onComplete()
		co_return;
	}

	// Unfreeze vehicle and enable collision
	Command<Commands::FREEZE_CAR_POSITION_AND_DONT_LOAD_COLLISION>(CPools::GetVehicleRef(vehicleHandle), 0);
	vehicleHandle->m_bUsesCollision = 1;
	vehicleHandle->m_nVehicleFlags.bEngineOn = 1;

	// Warp player into vehicle
	Command<Commands::WARP_CHAR_INTO_CAR>(CPools::GetPedRef(FindPlayerPed()), CPools::GetVehicleRef(vehicleHandle));

	// Wait until player exits the vehicle
	while (true) {
		if (!Command<Commands::IS_CHAR_IN_CAR>(CPools::GetPedRef(FindPlayerPed()),
		                                       CPools::GetVehicleRef(vehicleHandle))) {
			// Freeze vehicle back
			Command<Commands::FREEZE_CAR_POSITION_AND_DONT_LOAD_COLLISION>(CPools::GetVehicleRef(vehicleHandle), 1);
			vehicleHandle->m_nVehicleFlags.bEngineOn = 0;
			vehicleHandle->m_bUsesCollision = 0;

			// Save new position and direction
			const auto position = vehicleHandle->GetPosition();
			vehicle->entity->position = {position.x, position.y, position.z};
			vehicle->initialDirection = vehicleHandle->GetHeading();

			// Mark as dirty to update
			vehicle->dirty |= components::Vehicle::DirtyFlags::Position | components::Vehicle::DirtyFlags::Direction;

			break;
		}

		co_await 1;
	}

	// Restore UI
	WindowManager::instance().disableWindowRendering(false);
	WindowManager::instance().removeBackgroundRenderCallback("editVehicleManually");
	Command<Commands::SET_PLAYER_CONTROL>(0, 0);

	TaskManager::instance().removeTask("editVehicleManually");

	// Mark cleanup as completed and call onComplete
	cleanupGuard.completed = true;
	onComplete();
}

void manual_editing::editVehicleManually(components::Vehicle* vehicle, std::function<void()> onComplete) {
	TaskManager::instance().addTask("editVehicleManually", editVehicleManuallyTask, vehicle, onComplete);
}
