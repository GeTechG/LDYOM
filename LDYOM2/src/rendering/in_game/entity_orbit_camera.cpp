#define IMGUI_DEFINE_MATH_OPERATORS
#include "entity_orbit_camera.h"
#include <CCamera.h>
#include <CPools.h>
#include <CTimer.h>
#include <CWorld.h>
#include <cmath>
#include <common.h>
#include <entities_manager.h>
#include <entity.h>
#include <extensions/ScriptCommands.h>
#include <imgui.h>
#include <logger.h>
#include <scenes_manager.h>
#include <utils/pad.h>
#include <window_manager.h>

// Static member initialization
Entity* EntityOrbitCamera::m_targetEntity = nullptr;
int EntityOrbitCamera::m_entityIndex = -1;
CPlayerPed* EntityOrbitCamera::m_playerPed = nullptr;
int EntityOrbitCamera::m_originalArea = -1;
float EntityOrbitCamera::m_distance = 10.0f;
float EntityOrbitCamera::m_pitch = 30.0f;
float EntityOrbitCamera::m_yaw = 0.0f;
CVector EntityOrbitCamera::m_targetPosition = {0.0f, 0.0f, 0.0f};
float EntityOrbitCamera::m_targetDistance = 10.0f;
float EntityOrbitCamera::m_targetPitch = 30.0f;
float EntityOrbitCamera::m_targetYaw = 0.0f;
bool EntityOrbitCamera::m_isRotating = false;
float EntityOrbitCamera::m_lastMouseX = 0.0f;
float EntityOrbitCamera::m_lastMouseY = 0.0f;
float EntityOrbitCamera::m_savedDistance = 10.0f;
float EntityOrbitCamera::m_savedPitch = 30.0f;
float EntityOrbitCamera::m_savedYaw = 0.0f;

void EntityOrbitCamera::activate(Entity* entity, int entityIndex) noexcept {
	if (!entity) {
		LDYOM_ERROR("EntityOrbitCamera::activate - entity is nullptr");
		return;
	}

	// Store entity reference
	m_targetEntity = entity;
	m_entityIndex = entityIndex;

	// Get initial target position from entity
	m_targetPosition = CVector(entity->position[0], entity->position[1], entity->position[2]);

	// Save player and hide
	m_playerPed = FindPlayerPed();
	if (m_playerPed) {
		m_originalArea = m_playerPed->m_nAreaCode;
		CWorld::Remove(m_playerPed);
	}

	// Initialize orbit state from saved values
	m_distance = m_savedDistance;
	m_pitch = m_savedPitch;
	m_yaw = m_savedYaw;
	m_targetDistance = m_savedDistance;
	m_targetPitch = m_savedPitch;
	m_targetYaw = m_savedYaw;
	m_isRotating = false;

	// Teleport to entity area if needed
	teleportToEntityArea();

	// Lock player controls
	addLockControl(ORBIT_CAMERA_ID);

	// Register render callback
	WindowManager::instance().addBackgroundRenderCallback("EntityOrbitCamera", render);

	LDYOM_INFO("EntityOrbitCamera activated for entity: {}", entity->name);
}

void EntityOrbitCamera::deactivate(bool restorePlayer) noexcept {
	if (!m_targetEntity) {
		return;
	}

	// Restore player
	if (restorePlayer && m_playerPed) {
		CWorld::Add(m_playerPed);

		// Restore original area if changed
		if (m_originalArea != -1 && m_targetEntity && m_originalArea != m_targetEntity->areaId) {
			int playerRef = CPools::GetPedRef(m_playerPed);
			plugin::Command<plugin::Commands::SET_AREA_VISIBLE>(m_originalArea);
			plugin::Command<plugin::Commands::SET_CHAR_AREA_VISIBLE>(playerRef, m_originalArea);
		}
	}

	// Unlock controls
	removeLockControl(ORBIT_CAMERA_ID);

	// Restore camera
	plugin::Command<plugin::Commands::RESTORE_CAMERA_JUMPCUT>();

	// Remove callback
	WindowManager::instance().removeBackgroundRenderCallback("EntityOrbitCamera");

	// Save current camera state for next activation
	m_savedDistance = m_distance;
	m_savedPitch = m_pitch;
	m_savedYaw = m_yaw;

	// Reset state
	m_targetEntity = nullptr;
	m_entityIndex = -1;
	m_playerPed = nullptr;
	m_originalArea = -1;
	m_isRotating = false;

	LDYOM_INFO("EntityOrbitCamera deactivated");
}

void EntityOrbitCamera::render() noexcept {
	// Validate entity still exists
	auto& entities = ScenesManager::instance().getCurrentScene().entities;
	if (!m_targetEntity || m_entityIndex < 0 || m_entityIndex >= static_cast<int>(entities.size()) ||
	    entities[m_entityIndex].get() != m_targetEntity) {
		LDYOM_WARN("EntityOrbitCamera: target entity no longer valid, deactivating");
		deactivate(true);
		return;
	}

	// Handle input
	handleInput();

	// Update and apply camera
	updateCamera();
}

void EntityOrbitCamera::updateCamera() noexcept {
	if (!m_targetEntity) {
		return;
	}

	// Update target position from entity (always use entity.position as source of truth)
	m_targetPosition = CVector(m_targetEntity->position[0], m_targetEntity->position[1], m_targetEntity->position[2]);

	// Calculate frame-independent delta time
	float deltaTime = ImGui::GetIO().DeltaTime;

	// Frame-independent exponential smoothing (consistent at any FPS)
	// SMOOTHING_FACTOR is calibrated for 60 FPS, scale by deltaTime * 60
	float smoothing = 1.0f - std::pow(1.0f - SMOOTHING_FACTOR, deltaTime * 60.0f);

	// Smooth interpolation for camera parameters
	m_distance += (m_targetDistance - m_distance) * smoothing;
	m_pitch += (m_targetPitch - m_pitch) * smoothing;

	// Handle yaw wrap-around (shortest path interpolation)
	float yawDiff = m_targetYaw - m_yaw;
	if (yawDiff > 180.0f)
		yawDiff -= 360.0f;
	else if (yawDiff < -180.0f)
		yawDiff += 360.0f;
	m_yaw += yawDiff * smoothing;

	// Normalize yaw to [0, 360)
	while (m_yaw < 0.0f)
		m_yaw += 360.0f;
	while (m_yaw >= 360.0f)
		m_yaw -= 360.0f;

	// Convert spherical to cartesian coordinates
	float pitchRad = m_pitch * static_cast<float>(std::numbers::pi) / 180.0f;
	float yawRad = m_yaw * static_cast<float>(std::numbers::pi) / 180.0f;

	float cosPitch = std::cos(pitchRad);
	float sinPitch = std::sin(pitchRad);
	float cosYaw = std::cos(yawRad);
	float sinYaw = std::sin(yawRad);

	// Calculate camera position
	CVector cameraPos;
	cameraPos.x = m_targetPosition.x + m_distance * cosPitch * cosYaw;
	cameraPos.y = m_targetPosition.y + m_distance * cosPitch * sinYaw;
	cameraPos.z = m_targetPosition.z + m_distance * sinPitch;

	// Apply to GTA camera
	plugin::Command<plugin::Commands::SET_FIXED_CAMERA_POSITION>(cameraPos.x, cameraPos.y, cameraPos.z, 0.0f, 0.0f,
	                                                             0.0f);
	plugin::Command<plugin::Commands::POINT_CAMERA_AT_POINT>(m_targetPosition.x, m_targetPosition.y, m_targetPosition.z,
	                                                         2);
}

void EntityOrbitCamera::handleInput() noexcept {
	ImGuiIO& io = ImGui::GetIO();

	// RMB + drag for rotation (only when cursor is not over UI)
	if (!io.WantCaptureMouse) {
		// Mouse wheel zoom
		if (io.MouseWheel != 0.0f) {
			m_targetDistance -= io.MouseWheel * (m_targetDistance * 0.1f);
			m_targetDistance = std::clamp(m_targetDistance, MIN_DISTANCE, MAX_DISTANCE);
		}
		if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
			if (!m_isRotating) {
				// Start rotating
				m_isRotating = true;
				m_lastMouseX = io.MousePos.x;
				m_lastMouseY = io.MousePos.y;
			} else {
				// Continue rotating
				float deltaX = io.MousePos.x - m_lastMouseX;
				float deltaY = io.MousePos.y - m_lastMouseY;

				m_targetYaw -= deltaX * ROTATION_SENSITIVITY;
				m_targetPitch += deltaY * ROTATION_SENSITIVITY;

				// Clamp pitch to avoid gimbal lock
				m_targetPitch = std::clamp(m_targetPitch, MIN_PITCH, MAX_PITCH);

				// Normalize yaw to [0, 360)
				while (m_targetYaw < 0.0f)
					m_targetYaw += 360.0f;
				while (m_targetYaw >= 360.0f)
					m_targetYaw -= 360.0f;

				m_lastMouseX = io.MousePos.x;
				m_lastMouseY = io.MousePos.y;
			}
		} else {
			m_isRotating = false;
		}
	} else {
		m_isRotating = false;
	}

	// WASD + QE for entity position control (only when not typing in UI)
	if (!io.WantTextInput && m_targetEntity) {
		bool isMoving = false;

		// Calculate movement speed based on camera distance (zoom level)
		// Closer camera = smaller steps for precision, farther = larger steps for speed
		float dynamicSpeed = MOVEMENT_SPEED * m_distance * ImGui::GetIO().DeltaTime;

		// Calculate camera direction vectors on XY plane (ignore pitch for horizontal movement)
		float yawRad = m_yaw * static_cast<float>(std::numbers::pi) / 180.0f;

		// Forward vector (from camera to target, projected on XY plane)
		float forwardX = -std::cos(yawRad);
		float forwardY = -std::sin(yawRad);

		// Right vector (perpendicular to forward on XY plane)
		float rightX = -std::sin(yawRad);
		float rightY = std::cos(yawRad);

		// W - move away from camera (along forward direction)
		if (ImGui::IsKeyDown(ImGuiKey_W)) {
			m_targetEntity->position[0] += forwardX * dynamicSpeed;
			m_targetEntity->position[1] += forwardY * dynamicSpeed;
			isMoving = true;
		}

		// S - move toward camera (opposite to forward direction)
		if (ImGui::IsKeyDown(ImGuiKey_S)) {
			m_targetEntity->position[0] -= forwardX * dynamicSpeed;
			m_targetEntity->position[1] -= forwardY * dynamicSpeed;
			isMoving = true;
		}

		// A - move left relative to camera (opposite to right direction)
		if (ImGui::IsKeyDown(ImGuiKey_A)) {
			m_targetEntity->position[0] -= rightX * dynamicSpeed;
			m_targetEntity->position[1] -= rightY * dynamicSpeed;
			isMoving = true;
		}

		// D - move right relative to camera (along right direction)
		if (ImGui::IsKeyDown(ImGuiKey_D)) {
			m_targetEntity->position[0] += rightX * dynamicSpeed;
			m_targetEntity->position[1] += rightY * dynamicSpeed;
			isMoving = true;
		}

		// Q - move along Z axis (up)
		if (ImGui::IsKeyDown(ImGuiKey_Q)) {
			m_targetEntity->position[2] += dynamicSpeed;
			isMoving = true;
		}

		// E - move along Z axis (down)
		if (ImGui::IsKeyDown(ImGuiKey_E)) {
			m_targetEntity->position[2] -= dynamicSpeed;
			isMoving = true;
		}

		// Update entity transform callbacks if position changed
		if (isMoving) {
			m_targetEntity->updateSetTransformCallbacks();
		}
	}
}

void EntityOrbitCamera::teleportToEntityArea() noexcept {
	if (!m_targetEntity || !m_playerPed) {
		return;
	}

	int entityArea = m_targetEntity->areaId;
	int playerArea = m_playerPed->m_nAreaCode;

	if (entityArea != playerArea) {
		LDYOM_INFO("EntityOrbitCamera: switching area from {} to {}", playerArea, entityArea);

		// Get entity position for collision request
		CVector pos(m_targetEntity->position[0], m_targetEntity->position[1], m_targetEntity->position[2]);

		// Get player reference for area commands
		int playerRef = CPools::GetPedRef(m_playerPed);

		// Switch area (instant, no fade for faster editing)
		plugin::Command<plugin::Commands::SET_AREA_VISIBLE>(entityArea);
		plugin::Command<plugin::Commands::SET_CHAR_AREA_VISIBLE>(playerRef, entityArea);
		plugin::Command<plugin::Commands::REQUEST_COLLISION>(pos.x, pos.y);
		plugin::Command<plugin::Commands::LOAD_SCENE>(pos.x, pos.y, pos.z);
	}
}
