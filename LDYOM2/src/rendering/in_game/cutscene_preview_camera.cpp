#include "cutscene_preview_camera.h"
#include <CCamera.h>
#include <CPools.h>
#include <CWorld.h>
#include <common.h>
#include <extensions/ScriptCommands.h>
#include <logger.h>
#include <utils/pad.h>
#include <window_manager.h>

// Static member initialization
bool CutscenePreviewCamera::m_isActive = false;
CVector CutscenePreviewCamera::m_cameraPosition = {0.0f, 0.0f, 0.0f};
CVector CutscenePreviewCamera::m_targetPosition = {0.0f, 0.0f, 0.0f};
int CutscenePreviewCamera::m_cameraAreaId = 0;
CPlayerPed* CutscenePreviewCamera::m_playerPed = nullptr;
int CutscenePreviewCamera::m_originalArea = -1;
CVector CutscenePreviewCamera::m_savedPlayerPosition = {0.0f, 0.0f, 0.0f};

void CutscenePreviewCamera::activate(const std::array<float, 3>& cameraPos,
                                     const std::array<float, 3>& targetPos, int areaId) noexcept {
	// Store camera and target positions
	m_cameraPosition = CVector(cameraPos[0], cameraPos[1], cameraPos[2]);
	m_targetPosition = CVector(targetPos[0], targetPos[1], targetPos[2]);
	m_cameraAreaId = areaId;

	// Save player position and area (only on first activation)
	if (!m_isActive) {
		m_playerPed = FindPlayerPed();
		if (m_playerPed) {
			m_originalArea = m_playerPed->m_nAreaCode;
			m_savedPlayerPosition = m_playerPed->GetPosition();

			// Remove player from world (hide from rendering)
			CWorld::Remove(m_playerPed);
		}

		// Lock player controls
		addLockControl(PREVIEW_CAMERA_ID);

		// Register render callback
		WindowManager::instance().addBackgroundRenderCallback("CutscenePreviewCamera", render);

		LDYOM_INFO("CutscenePreviewCamera activated");
	}

	// Setup player position and area for LOD rendering
	setupPlayerForArea(areaId);

	// Mark as active
	m_isActive = true;
}

void CutscenePreviewCamera::deactivate() noexcept {
	if (!m_isActive) {
		return;
	}

	// Restore player
	if (m_playerPed) {
		CWorld::Add(m_playerPed);

		// Restore original area if changed
		if (m_originalArea != -1 && m_originalArea != m_cameraAreaId) {
			int playerRef = CPools::GetPedRef(m_playerPed);
			plugin::Command<plugin::Commands::SET_AREA_VISIBLE>(m_originalArea);
			plugin::Command<plugin::Commands::SET_CHAR_AREA_VISIBLE>(playerRef, m_originalArea);
		}

		// Restore saved position
		m_playerPed->SetPosn(m_savedPlayerPosition);
	}

	// Unlock controls
	removeLockControl(PREVIEW_CAMERA_ID);

	// Restore camera
	plugin::Command<plugin::Commands::RESTORE_CAMERA_JUMPCUT>();

	// Remove callback
	WindowManager::instance().removeBackgroundRenderCallback("CutscenePreviewCamera");

	// Reset state
	m_isActive = false;
	m_cameraPosition = {0.0f, 0.0f, 0.0f};
	m_targetPosition = {0.0f, 0.0f, 0.0f};
	m_cameraAreaId = 0;
	m_playerPed = nullptr;
	m_originalArea = -1;
	m_savedPlayerPosition = {0.0f, 0.0f, 0.0f};

	LDYOM_INFO("CutscenePreviewCamera deactivated");
}

void CutscenePreviewCamera::render() noexcept {
	if (!m_isActive) {
		return;
	}

	// Update camera
	updateCamera();
}

void CutscenePreviewCamera::updateCamera() noexcept {
	if (!m_isActive) {
		return;
	}

	// Move player to camera position for proper LOD rendering
	if (m_playerPed) {
		m_playerPed->SetPosn(m_cameraPosition);
	}

	// Apply fixed camera position and point at target
	plugin::Command<plugin::Commands::SET_FIXED_CAMERA_POSITION>(m_cameraPosition.x, m_cameraPosition.y,
	                                                             m_cameraPosition.z, 0.0f, 0.0f, 0.0f);
	plugin::Command<plugin::Commands::POINT_CAMERA_AT_POINT>(m_targetPosition.x, m_targetPosition.y, m_targetPosition.z,
	                                                         2);
}

void CutscenePreviewCamera::setupPlayerForArea(int areaId) noexcept {
	if (!m_playerPed) {
		return;
	}

	int playerArea = m_playerPed->m_nAreaCode;

	if (areaId != playerArea) {
		LDYOM_INFO("CutscenePreviewCamera: switching area from {} to {}", playerArea, areaId);

		// Get player reference for area commands
		int playerRef = CPools::GetPedRef(m_playerPed);

		// Switch area (instant, no fade for faster preview)
		plugin::Command<plugin::Commands::SET_AREA_VISIBLE>(areaId);
		plugin::Command<plugin::Commands::SET_CHAR_AREA_VISIBLE>(playerRef, areaId);
		plugin::Command<plugin::Commands::REQUEST_COLLISION>(m_cameraPosition.x, m_cameraPosition.y);
		plugin::Command<plugin::Commands::LOAD_SCENE>(m_cameraPosition.x, m_cameraPosition.y, m_cameraPosition.z);
	}

	// Teleport player to camera position for LOD rendering
	m_playerPed->SetPosn(m_cameraPosition);
}
