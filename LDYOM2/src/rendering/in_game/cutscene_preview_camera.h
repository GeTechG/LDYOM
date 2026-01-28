#pragma once

#include <CVector.h>

class CPlayerPed;

/**
 * @brief Preview camera for cutscene objectives
 *
 * Provides camera preview when cutscene objective is selected:
 * - Activates automatically when cutscene objective is selected
 * - Sets camera at cameraPosition looking at targetPosition
 * - Deactivates when different objective is selected
 * - Manages player visibility and area switching for proper LOD rendering
 */
class CutscenePreviewCamera {
  private:
	// Preview state
	static bool m_isActive;
	static CVector m_cameraPosition;
	static CVector m_targetPosition;
	static int m_cameraAreaId;

	// Player management
	static CPlayerPed* m_playerPed;
	static int m_originalArea;
	static CVector m_savedPlayerPosition;

	// Constants
	static constexpr int PREVIEW_CAMERA_ID = 4;

	// Internal methods
	static void render() noexcept;
	static void updateCamera() noexcept;
	static void setupPlayerForArea(int areaId) noexcept;

  public:
	/**
	 * @brief Activate cutscene preview camera
	 * @param cameraPos Camera position from cutscene data
	 * @param targetPos Target position from cutscene data
	 * @param areaId Area ID for the cutscene (default 0 = outside world)
	 */
	static void activate(const std::array<float, 3>& cameraPos, const std::array<float, 3>& targetPos,
	                     int areaId = 0) noexcept;

	/**
	 * @brief Deactivate cutscene preview camera
	 */
	static void deactivate() noexcept;

	/**
	 * @brief Check if preview camera is currently active
	 */
	static bool isActive() noexcept { return m_isActive; }
};
