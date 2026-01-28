#include "manual_editing_session.h"
#include <entity.h>
#include <in_game/entity_orbit_camera.h>
#include <logger.h>
#include <window_manager.h>
#include <windows/entities.h>
#include <windows/entity_info_panel.h>

ManualEditingSession::ManualEditingSession(const Options& options) noexcept
	: m_options(options) {

	// Disable UI rendering if requested
	if (m_options.disableUI) {
		WindowManager::instance().disableWindowRendering(true);
	}

	// Deactivate orbit camera if requested
	if (m_options.disableCamera) {
		if (m_options.showInfoPanel) {
			EntityOrbitCamera::deactivateWithInfo(true);
		} else {
			EntityOrbitCamera::deactivate(true);
		}
	}

	LDYOM_INFO("ManualEditingSession: Started");
}

ManualEditingSession::~ManualEditingSession() noexcept {
	// Auto-complete if not already completed
	if (!m_completed) {

		complete();
	}
}

void ManualEditingSession::complete() noexcept {
	if (m_completed) {
		return; // Already completed
	}

	LDYOM_INFO("ManualEditingSession: Completing");

	// Re-enable UI rendering if we disabled it
	if (m_options.disableUI) {
		WindowManager::instance().disableWindowRendering(false);
	}

	// Reactivate orbit camera if we deactivated it and entity exists
	if (m_options.disableCamera && m_options.entity != nullptr) {
		if (m_options.showInfoPanel) {
			EntityOrbitCamera::activateWithInfo(m_options.entity);
		} else {
			// Get entity index from EntitiesWindow if needed
			auto entitiesWindow = WindowManager::instance().getWindowAs<EntitiesWindow>("entities");
			if (entitiesWindow.has_value()) {
				EntityOrbitCamera::activate(m_options.entity, entitiesWindow.value()->getSelectedEntityIndex());
			}
		}
	}

	// Invoke completion callback
	if (m_options.onComplete) {
		m_options.onComplete();
	}

	m_completed = true;
	LDYOM_INFO("ManualEditingSession: Completed");
}
