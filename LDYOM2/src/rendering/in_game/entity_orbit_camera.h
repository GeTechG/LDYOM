#pragma once

#include <CVector.h>

class Entity;
class CPlayerPed;

/**
 * @brief Professional orbit camera for entity editing
 *
 * Provides 3D editor-style orbit camera (Blender, Unity, Unreal Engine):
 * - Activates automatically when entity is selected
 * - Works in background - UI remains visible and interactive
 * - RMB + drag to rotate (only when cursor outside UI)
 * - Mouse wheel to zoom (always active)
 * - Exits when entity deselected or window closed
 */
class EntityOrbitCamera {
  private:
	// Entity tracking
	static Entity* m_targetEntity;
	static int m_entityIndex;

	// Player management
	static CPlayerPed* m_playerPed;
	static int m_originalArea;

	// Orbit state (spherical coordinates)
	static float m_distance;         // 1.0 - 100.0 (zoom level)
	static float m_pitch;            // -89° to 89° (up/down angle)
	static float m_yaw;              // 0° to 360° (rotation around Z-axis)
	static CVector m_targetPosition; // Entity position (from entity.position)

	// Smooth interpolation targets
	static float m_targetDistance;
	static float m_targetPitch;
	static float m_targetYaw;

	// Input tracking
	static bool m_isRotating;
	static float m_lastMouseX;
	static float m_lastMouseY;

	// Constants
	static constexpr float ROTATION_SENSITIVITY = 0.2f;
	static constexpr float SMOOTHING_FACTOR = 0.15f; // 0.1 = very smooth, 0.5 = responsive, 1.0 = instant
	static constexpr float MIN_DISTANCE = 0.1f;
	static constexpr float MAX_DISTANCE = 100.0f;
	static constexpr float MIN_PITCH = -89.0f;
	static constexpr float MAX_PITCH = 89.0f;
	static constexpr int ORBIT_CAMERA_ID = 3;
	static constexpr float MOVEMENT_SPEED = 0.1f;

	// Internal methods
	static void render() noexcept;
	static void updateCamera() noexcept;
	static void handleInput() noexcept;
	static void teleportToEntityArea() noexcept;

  public:
	/**
	 * @brief Activate orbit camera for specified entity
	 * @param entity Target entity to orbit around
	 * @param entityIndex Index in entities manager
	 */
	static void activate(Entity* entity, int entityIndex) noexcept;

	/**
	 * @brief Deactivate orbit camera and restore normal state
	 * @param restorePlayer Whether to restore player visibility and controls
	 */
	static void deactivate(bool restorePlayer) noexcept;

	/**
	 * @brief Check if orbit camera is currently active
	 */
	static bool isActive() noexcept { return m_targetEntity != nullptr; }

	/**
	 * @brief Update entity index without deactivating camera (for drag and drop)
	 * @param newIndex New index of the entity in the list
	 */
	static void updateEntityIndex(int newIndex) noexcept { m_entityIndex = newIndex; }
};
