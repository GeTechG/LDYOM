#include "vehicle_manual_editing.h"
#include <CCamera.h>
#include <CPlayerPed.h>
#include <CPools.h>
#include <common.h>
#include <components/vehicle.h>
#include <entity.h>
#include <extensions/ScriptCommands.h>
#include <fa_icons.h>
#include <imgui.h>
#include <localization.h>
#include <utils/manual_editing_session.h>
#include <window_manager.h>

using namespace plugin;

components::Vehicle* VehicleManualEditing::m_vehicle = nullptr;
CVehicle* VehicleManualEditing::m_vehicleHandle = nullptr;
std::function<void()> VehicleManualEditing::m_onCompleteCallback = nullptr;
std::unique_ptr<ManualEditingSession> VehicleManualEditing::m_session = nullptr;

void VehicleManualEditing::render() noexcept {
	// Draw overlay instructions
	constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
	                                         ImGuiWindowFlags_NoSavedSettings |
	                                         ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
	if (ImGui::Begin("##VehicleManualEditor", nullptr, windowFlags)) {
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 16.5f);
		ImGui::TextWrapped("%s %s", ICON_FA_CIRCLE_INFO, _("components.vehicle.edit_manually_instruction").c_str());
		ImGui::PopTextWrapPos();
	}
	ImGui::End();

	// Check if player exited vehicle
	if (m_vehicleHandle && !Command<Commands::IS_CHAR_IN_CAR>(CPools::GetPedRef(FindPlayerPed()),
	                                                          CPools::GetVehicleRef(m_vehicleHandle))) {
		closeVehicleEditor(true);
	}
}

void VehicleManualEditing::openVehicleEditor(Entity* entity, components::Vehicle* vehicle,
                                             std::function<void()> onComplete) noexcept {
	// Get vehicle handle
	auto vehicleHandle = vehicle->handle.get();
	if (!vehicleHandle) {
		return; // Vehicle not spawned
	}

	// Store state
	m_vehicle = vehicle;
	m_vehicleHandle = vehicleHandle;
	m_onCompleteCallback = onComplete;

	// Create RAII session that handles UI/camera automatically
	m_session = std::make_unique<ManualEditingSession>(ManualEditingSession::Options{
		.entity = entity,
		.disableUI = true,
		.disableCamera = true,
		.showInfoPanel = true,
		.onComplete = nullptr // We'll call it manually in closeVehicleEditor
	});

	// Enable player control
	Command<Commands::SET_PLAYER_CONTROL>(0, 1);

	// Restore camera
	TheCamera.Restore();

	// Unfreeze vehicle and enable collision
	Command<Commands::FREEZE_CAR_POSITION_AND_DONT_LOAD_COLLISION>(CPools::GetVehicleRef(vehicleHandle), 0);
	vehicleHandle->m_bUsesCollision = 1;
	vehicleHandle->m_nVehicleFlags.bEngineOn = 1;

	// Warp player into vehicle
	Command<Commands::WARP_CHAR_INTO_CAR>(CPools::GetPedRef(FindPlayerPed()), CPools::GetVehicleRef(vehicleHandle));

	// Register render callback to check for exit
	WindowManager::instance().addBackgroundRenderCallback("VehicleManualEditor", render);
}

void VehicleManualEditing::closeVehicleEditor(bool saveChanges) noexcept {
	if (!m_vehicle || !m_vehicleHandle) {
		return;
	}

	if (saveChanges) {
		// Freeze vehicle back
		Command<Commands::FREEZE_CAR_POSITION_AND_DONT_LOAD_COLLISION>(CPools::GetVehicleRef(m_vehicleHandle), 1);
		m_vehicleHandle->m_nVehicleFlags.bEngineOn = 0;
		m_vehicleHandle->m_bUsesCollision = 0;

		// Save new position and direction
		const auto position = m_vehicleHandle->GetPosition();
		m_vehicle->entity->position = {position.x, position.y, position.z};
		m_vehicle->initialDirection = m_vehicleHandle->GetHeading();

		// Mark as dirty to update
		m_vehicle->dirty |= components::Vehicle::DirtyFlags::Position | components::Vehicle::DirtyFlags::Direction;
	}

	// Disable player control
	Command<Commands::SET_PLAYER_CONTROL>(0, 0);

	// Remove render callback
	WindowManager::instance().removeBackgroundRenderCallback("VehicleManualEditor");

	// Call user callback before session cleanup
	if (m_onCompleteCallback) {
		m_onCompleteCallback();
		m_onCompleteCallback = nullptr;
	}

	// Destroy session (triggers RAII cleanup: UI/camera restoration)
	m_session.reset();

	// Reset state
	m_vehicle = nullptr;
	m_vehicleHandle = nullptr;
}
