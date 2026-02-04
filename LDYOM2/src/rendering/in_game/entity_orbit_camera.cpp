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
#include <in_game/entity_gizmo.h>
#include <windows/entities.h>
#include <windows/entity_info_panel.h>

// Static member initialization
Entity* EntityOrbitCamera::m_targetEntity = nullptr;
int EntityOrbitCamera::m_entityIndex = -1;
CPlayerPed* EntityOrbitCamera::m_playerPed = nullptr;
int EntityOrbitCamera::m_originalArea = -1;
CVector EntityOrbitCamera::m_savedPlayerPosition = {0.0f, 0.0f, 0.0f};
float EntityOrbitCamera::m_distance = 10.0f;
float EntityOrbitCamera::m_pitch = 30.0f;
float EntityOrbitCamera::m_yaw = 0.0f;
CVector EntityOrbitCamera::m_targetPosition = {0.0f, 0.0f, 0.0f};
CVector EntityOrbitCamera::m_cameraPos = {0.0f, 0.0f, 0.0f};
float EntityOrbitCamera::m_targetDistance = 10.0f;
float EntityOrbitCamera::m_targetPitch = 30.0f;
float EntityOrbitCamera::m_targetYaw = 0.0f;
bool EntityOrbitCamera::m_isRotating = false;
float EntityOrbitCamera::m_lastMouseX = 0.0f;
float EntityOrbitCamera::m_lastMouseY = 0.0f;
float EntityOrbitCamera::m_savedDistance = 10.0f;
float EntityOrbitCamera::m_savedPitch = 30.0f;
float EntityOrbitCamera::m_savedYaw = 0.0f;
bool EntityOrbitCamera::m_freeMode = false;
CVector EntityOrbitCamera::m_freeCameraPos = {0.0f, 0.0f, 0.0f};
float EntityOrbitCamera::m_freeYaw = 0.0f;
float EntityOrbitCamera::m_freePitch = 0.0f;
float EntityOrbitCamera::m_freeSpeed = 5.0f;

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

	// Save player position and area
	m_playerPed = FindPlayerPed();
	if (m_playerPed) {
		m_originalArea = m_playerPed->m_nAreaCode;
		m_savedPlayerPosition = m_playerPed->GetPosition(); // Save current position

		// Teleport player to entity position (needed for GTA rendering LOD)
		m_playerPed->SetPosn(m_targetPosition);

		// Remove player from world (hide from rendering)
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

	// Activate gizmo for the selected entity
	EntityGizmo::activate(entity);

	LDYOM_INFO("EntityOrbitCamera activated for entity: {}", entity->name);
}

void EntityOrbitCamera::activateWithInfo(Entity* entity) noexcept {
	auto entitiesWindow = WindowManager::instance().getWindowAs<EntitiesWindow>("entities");
	if (entitiesWindow.has_value()) {
		activate(entity, entitiesWindow.value()->getSelectedEntityIndex());
	}
	EntityInfoPanel::show(entity);
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

		// Restore saved position
		m_playerPed->SetPosn(m_savedPlayerPosition);
	}

	// Deactivate gizmo before cleanup
	EntityGizmo::deactivate();

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
	m_savedPlayerPosition = {0.0f, 0.0f, 0.0f};
	m_isRotating = false;
	m_freeMode = false;

	LDYOM_INFO("EntityOrbitCamera deactivated");
}

void EntityOrbitCamera::deactivateWithInfo(bool restorePlayer) noexcept {
	deactivate(restorePlayer);
	EntityInfoPanel::hide();
}

void EntityOrbitCamera::toggleFreeMode() noexcept {
	if (!m_targetEntity) {
		return;
	}

	if (!m_freeMode) {
		// Orbit → Free: capture current camera position and derive look direction
		m_freeCameraPos = m_cameraPos;

		// Look direction = normalize(target - camera)
		float dx = m_targetPosition.x - m_cameraPos.x;
		float dy = m_targetPosition.y - m_cameraPos.y;
		float dz = m_targetPosition.z - m_cameraPos.z;
		float len = std::sqrt(dx * dx + dy * dy + dz * dz);
		if (len > 0.001f) {
			dx /= len;
			dy /= len;
			dz /= len;
		}
		m_freeYaw   = std::atan2(dy, dx) * 180.0f / static_cast<float>(std::numbers::pi);
		m_freePitch = std::asin(std::clamp(dz, -1.0f, 1.0f)) * 180.0f / static_cast<float>(std::numbers::pi);

		// Initialize speed from current orbit distance for smooth transition
		m_freeSpeed = std::clamp(MOVEMENT_SPEED * m_distance, FREE_SPEED_MIN, FREE_SPEED_MAX);

		m_freeMode = true;
		LDYOM_INFO("EntityOrbitCamera: switched to free camera mode");
	} else {
		// Free → Orbit: derive orbit parameters from current free camera position
		m_targetPosition =
		    CVector(m_targetEntity->position[0], m_targetEntity->position[1], m_targetEntity->position[2]);

		float dx = m_freeCameraPos.x - m_targetPosition.x;
		float dy = m_freeCameraPos.y - m_targetPosition.y;
		float dz = m_freeCameraPos.z - m_targetPosition.z;
		float dist = std::sqrt(dx * dx + dy * dy + dz * dz);
		if (dist < MIN_DISTANCE) {
			dist = MIN_DISTANCE;
		}

		m_distance       = dist;
		m_targetDistance  = dist;
		m_yaw            = std::atan2(dy, dx) * 180.0f / static_cast<float>(std::numbers::pi);
		m_pitch          = std::asin(std::clamp(dz / dist, -1.0f, 1.0f)) * 180.0f / static_cast<float>(std::numbers::pi);
		m_targetYaw      = m_yaw;
		m_targetPitch    = m_pitch;

		// Normalize yaw to [0, 360)
		while (m_yaw < 0.0f)
			m_yaw += 360.0f;
		while (m_yaw >= 360.0f)
			m_yaw -= 360.0f;
		m_targetYaw = m_yaw;

		m_freeMode = false;
		LDYOM_INFO("EntityOrbitCamera: switched back to orbit mode");
	}
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

	if (m_freeMode) {
		// Free camera: position is updated directly via input
		if (m_playerPed) {
			m_playerPed->SetPosn(m_freeCameraPos);
		}
		m_cameraPos = m_freeCameraPos;

		// Compute look target from free camera angles
		float pitchRad = m_freePitch * static_cast<float>(std::numbers::pi) / 180.0f;
		float yawRad   = m_freeYaw * static_cast<float>(std::numbers::pi) / 180.0f;
		CVector lookTarget;
		lookTarget.x = m_freeCameraPos.x + std::cos(pitchRad) * std::cos(yawRad) * 10.0f;
		lookTarget.y = m_freeCameraPos.y + std::cos(pitchRad) * std::sin(yawRad) * 10.0f;
		lookTarget.z = m_freeCameraPos.z + std::sin(pitchRad) * 10.0f;

		EntityGizmo::render(m_freeCameraPos, lookTarget);

		plugin::Command<plugin::Commands::SET_FIXED_CAMERA_POSITION>(m_freeCameraPos.x, m_freeCameraPos.y,
		                                                             m_freeCameraPos.z, 0.0f, 0.0f, 0.0f);
		plugin::Command<plugin::Commands::POINT_CAMERA_AT_POINT>(lookTarget.x, lookTarget.y, lookTarget.z, 2);
		return;
	}

	// Update target position from entity (always use entity.position as source of truth)
	m_targetPosition = CVector(m_targetEntity->position[0], m_targetEntity->position[1], m_targetEntity->position[2]);

	// Move player to entity position for proper LOD rendering
	if (m_playerPed) {
		m_playerPed->SetPosn(m_targetPosition);
	}

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
	m_cameraPos = cameraPos;

	// Render gizmo overlay using the camera state that will be applied this frame
	EntityGizmo::render(cameraPos, m_targetPosition);

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
		// Mouse wheel: zoom in orbit mode, adjust speed in free mode
		if (io.MouseWheel != 0.0f) {
			if (m_freeMode) {
				m_freeSpeed *= (1.0f + io.MouseWheel * FREE_SPEED_SCROLL_FACTOR);
				m_freeSpeed = std::clamp(m_freeSpeed, FREE_SPEED_MIN, FREE_SPEED_MAX);
			} else {
				m_targetDistance -= io.MouseWheel * (m_targetDistance * 0.1f);
				m_targetDistance = std::clamp(m_targetDistance, MIN_DISTANCE, MAX_DISTANCE);
			}
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

				if (m_freeMode) {
					// Free mode: RMB rotates the look direction (FPS-style)
					m_freeYaw -= deltaX * ROTATION_SENSITIVITY;
					m_freePitch -= deltaY * ROTATION_SENSITIVITY;
					m_freePitch = std::clamp(m_freePitch, MIN_PITCH, MAX_PITCH);
					while (m_freeYaw < 0.0f)
						m_freeYaw += 360.0f;
					while (m_freeYaw >= 360.0f)
						m_freeYaw -= 360.0f;
				} else {
					// Orbit mode: RMB orbits around target
					m_targetYaw -= deltaX * ROTATION_SENSITIVITY;
					m_targetPitch += deltaY * ROTATION_SENSITIVITY;
					m_targetPitch = std::clamp(m_targetPitch, MIN_PITCH, MAX_PITCH);
					while (m_targetYaw < 0.0f)
						m_targetYaw += 360.0f;
					while (m_targetYaw >= 360.0f)
						m_targetYaw -= 360.0f;
				}

				m_lastMouseX = io.MousePos.x;
				m_lastMouseY = io.MousePos.y;
			}
		} else {
			m_isRotating = false;
		}
	} else {
		m_isRotating = false;
	}

	// Toggle free camera mode
	if (!io.WantTextInput && ImGui::IsKeyPressed(ImGuiKey_Tab)) {
		toggleFreeMode();
	}

	// WASD + QE: camera movement in free mode, entity movement in orbit mode
	if (!io.WantTextInput && m_targetEntity) {
		if (m_freeMode) {
			// Free camera: WASD moves camera along view direction, QE moves up/down
			float deltaTime = io.DeltaTime;
			float speed     = m_freeSpeed * deltaTime;

			float pitchRad = m_freePitch * static_cast<float>(std::numbers::pi) / 180.0f;
			float yawRad   = m_freeYaw * static_cast<float>(std::numbers::pi) / 180.0f;

			// Forward vector (3D, follows pitch)
			float forwardX = std::cos(pitchRad) * std::cos(yawRad);
			float forwardY = std::cos(pitchRad) * std::sin(yawRad);
			float forwardZ = std::sin(pitchRad);

			// Right vector (horizontal)
			float rightX = std::sin(yawRad);
			float rightY = -std::cos(yawRad);

			if (ImGui::IsKeyDown(ImGuiKey_W)) {
				m_freeCameraPos.x += forwardX * speed;
				m_freeCameraPos.y += forwardY * speed;
				m_freeCameraPos.z += forwardZ * speed;
			}
			if (ImGui::IsKeyDown(ImGuiKey_S)) {
				m_freeCameraPos.x -= forwardX * speed;
				m_freeCameraPos.y -= forwardY * speed;
				m_freeCameraPos.z -= forwardZ * speed;
			}
			if (ImGui::IsKeyDown(ImGuiKey_A)) {
				m_freeCameraPos.x -= rightX * speed;
				m_freeCameraPos.y -= rightY * speed;
			}
			if (ImGui::IsKeyDown(ImGuiKey_D)) {
				m_freeCameraPos.x += rightX * speed;
				m_freeCameraPos.y += rightY * speed;
			}
			if (ImGui::IsKeyDown(ImGuiKey_Q)) {
				m_freeCameraPos.z += speed;
			}
			if (ImGui::IsKeyDown(ImGuiKey_E)) {
				m_freeCameraPos.z -= speed;
			}
		} else {
			// Orbit mode: WASD moves the entity relative to camera view
			bool isMoving = false;

			float dynamicSpeed = MOVEMENT_SPEED * m_distance * io.DeltaTime;

			float yawRad = m_yaw * static_cast<float>(std::numbers::pi) / 180.0f;

			// Forward vector (from camera to target, projected on XY plane)
			float forwardX = -std::cos(yawRad);
			float forwardY = -std::sin(yawRad);

			// Right vector (perpendicular to forward on XY plane)
			float rightX = -std::sin(yawRad);
			float rightY = std::cos(yawRad);

			if (ImGui::IsKeyDown(ImGuiKey_W)) {
				m_targetEntity->position[0] += forwardX * dynamicSpeed;
				m_targetEntity->position[1] += forwardY * dynamicSpeed;
				isMoving = true;
			}
			if (ImGui::IsKeyDown(ImGuiKey_S)) {
				m_targetEntity->position[0] -= forwardX * dynamicSpeed;
				m_targetEntity->position[1] -= forwardY * dynamicSpeed;
				isMoving = true;
			}
			if (ImGui::IsKeyDown(ImGuiKey_A)) {
				m_targetEntity->position[0] -= rightX * dynamicSpeed;
				m_targetEntity->position[1] -= rightY * dynamicSpeed;
				isMoving = true;
			}
			if (ImGui::IsKeyDown(ImGuiKey_D)) {
				m_targetEntity->position[0] += rightX * dynamicSpeed;
				m_targetEntity->position[1] += rightY * dynamicSpeed;
				isMoving = true;
			}
			if (ImGui::IsKeyDown(ImGuiKey_Q)) {
				m_targetEntity->position[2] += dynamicSpeed;
				isMoving = true;
			}
			if (ImGui::IsKeyDown(ImGuiKey_E)) {
				m_targetEntity->position[2] -= dynamicSpeed;
				isMoving = true;
			}

			if (isMoving) {
				m_targetEntity->updateSetTransformCallbacks();
			}
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
