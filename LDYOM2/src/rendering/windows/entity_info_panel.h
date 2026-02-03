#pragma once

class Entity;

/**
 * @brief Information overlay for entity editing
 *
 * Displays as an overlay (not a window):
 * - Camera controls information (when orbit camera is active)
 * - Entity-specific information based on component types
 * - Extensible design allows adding custom info for different entity types
 */
class EntityInfoPanel {
  private:
	static bool m_isVisible;
	static Entity* m_currentEntity;

	static void render() noexcept;
	static void renderCameraControls() noexcept;
	static void renderGizmoControls() noexcept;
	static void renderEntitySpecificInfo(Entity* entity) noexcept;

  public:
	/**
	 * @brief Show the info panel for specified entity
	 */
	static void show(Entity* entity) noexcept;

	/**
	 * @brief Hide the info panel
	 */
	static void hide() noexcept;

	/**
	 * @brief Check if panel is currently visible
	 */
	static bool isVisible() noexcept { return m_isVisible; }
};
