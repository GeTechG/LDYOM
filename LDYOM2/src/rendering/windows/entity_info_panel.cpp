#define IMGUI_DEFINE_MATH_OPERATORS
#include "entity_info_panel.h"
#include <algorithm>
#include <entity.h>
#include <fa_icons.h>
#include <imgui.h>
#include <in_game/entity_gizmo.h>
#include <in_game/entity_orbit_camera.h>
#include <localization.h>
#include <utils/imgui_configurate.h>
#include <utils/ui_scale.h>
#include <window_manager.h>

// Static member initialization
bool EntityInfoPanel::m_isVisible = false;
Entity* EntityInfoPanel::m_currentEntity = nullptr;

void EntityInfoPanel::show(Entity* entity) noexcept {
	m_currentEntity = entity;

	if (!m_isVisible) {
		m_isVisible = true;
		WindowManager::instance().addBackgroundRenderCallback("EntityInfoPanel", render);
	}
}

void EntityInfoPanel::hide() noexcept {
	if (m_isVisible) {
		m_isVisible = false;
		m_currentEntity = nullptr;
		WindowManager::instance().removeBackgroundRenderCallback("EntityInfoPanel");
	}
}

void EntityInfoPanel::render() noexcept {
	if (!m_isVisible) {
		return;
	}

	constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
	                                         ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
	                                         ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;

	const float uiScale = ImGuiConfigurate::getUiScale();
	const auto& io = ImGui::GetIO();

	// Calculate adaptive width based on screen aspect ratio
	const float aspectRatio = io.DisplaySize.x / io.DisplaySize.y;
	const float baseWidth = ui::em(20.0f);                                      // ~320px at 1em=16
	const float widthMultiplier = std::clamp(aspectRatio / 1.778f, 0.8f, 1.5f); // 1.778 = 16:9
	const float adaptiveWidth = baseWidth * widthMultiplier;

	// Position in top-left corner, next to entities panel (framework-level position uses uiScale)
	ImGui::SetNextWindowPos(ImVec2(210.0f, 10.0f) * uiScale, ImGuiCond_Always, ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowBgAlpha(0.85f); // Semi-transparent background

	// Set minimum window width constraint
	ImGui::SetNextWindowSizeConstraints(ImVec2(adaptiveWidth, 0), ImVec2(adaptiveWidth, FLT_MAX));

	// WindowPadding is a style var — auto-scaled by FontScaleDpi, pass raw.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 12));

	if (ImGui::Begin("##EntityInfoPanel", nullptr, windowFlags)) {

		// Render camera controls if orbit camera is active
		if (EntityOrbitCamera::isActive()) {
			renderCameraControls();
		}

		// Render gizmo controls if gizmo is active
		if (EntityGizmo::isActive()) {
			renderGizmoControls();
		}

		// Render entity-specific info if available
		if (m_currentEntity) {
			renderEntitySpecificInfo(m_currentEntity);
		}
	}
	ImGui::End();

	ImGui::PopStyleVar();
}

void EntityInfoPanel::renderCameraControls() noexcept {
	const float availWidth = ImGui::GetContentRegionAvail().x;

	// Header - use accent color from theme (ButtonHovered gives good visibility)
	auto headerColor = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
	headerColor.w = 1.0f; // Full opacity for header
	ImGui::PushStyleColor(ImGuiCol_Text, headerColor);
	ImGui::Text(ICON_FA_CAMERA " %s", _("entity_info.camera_controls").c_str());
	ImGui::PopStyleColor();

	// Free camera toggle
	bool freeMode = EntityOrbitCamera::isFreeMode();
	if (ImGui::Checkbox(_("entity_info.free_camera").c_str(), &freeMode)) {
		EntityOrbitCamera::toggleFreeMode();
	}
	ImGui::SameLine();
	ImGui::TextDisabled("(F)");

	ImGui::Separator();
	ImGui::Spacing();

	// Control instructions
	const float iconWidth = ui::em(2.0f);
	const float textOffset = iconWidth + ui::em(0.3f);

	if (freeMode) {
		// Free camera controls
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
		ImGui::Text(ICON_FA_COMPUTER_MOUSE);
		ImGui::PopStyleColor();
		ImGui::SameLine(textOffset);
		ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + availWidth - textOffset);
		ImGui::Text(_("entity_info.free_camera_scroll_speed").c_str());
		ImGui::PopTextWrapPos();

		ImGui::SameLine();
		ImGui::TextDisabled("[%.1f]", EntityOrbitCamera::getFreeSpeed());

		ImGui::Spacing();

		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
		ImGui::Text(ICON_FA_ARROW_POINTER);
		ImGui::PopStyleColor();
		ImGui::SameLine(textOffset);
		ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + availWidth - textOffset);
		ImGui::Text(_("entity_info.free_camera_rmb_rotate").c_str());
		ImGui::PopTextWrapPos();

		ImGui::Spacing();

		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
		ImGui::Text(ICON_FA_KEYBOARD);
		ImGui::PopStyleColor();
		ImGui::SameLine(textOffset);
		ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + availWidth - textOffset);
		ImGui::Text(_("entity_info.free_camera_wasd_move").c_str());
		ImGui::PopTextWrapPos();

		ImGui::Spacing();

		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
		ImGui::Text(ICON_FA_KEYBOARD);
		ImGui::PopStyleColor();
		ImGui::SameLine(textOffset);
		ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + availWidth - textOffset);
		ImGui::Text(_("entity_info.free_camera_qe_vertical").c_str());
		ImGui::PopTextWrapPos();
	} else {
		// Orbit camera controls
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
		ImGui::Text(ICON_FA_COMPUTER_MOUSE);
		ImGui::PopStyleColor();
		ImGui::SameLine(textOffset);
		ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + availWidth - textOffset);
		ImGui::Text(_("entity_info.mouse_wheel_zoom").c_str());
		ImGui::PopTextWrapPos();

		ImGui::Spacing();

		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
		ImGui::Text(ICON_FA_ARROW_POINTER);
		ImGui::PopStyleColor();
		ImGui::SameLine(textOffset);
		ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + availWidth - textOffset);
		ImGui::Text(_("entity_info.rmb_rotate").c_str());
		ImGui::PopTextWrapPos();

		ImGui::Spacing();

		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
		ImGui::Text(ICON_FA_KEYBOARD);
		ImGui::PopStyleColor();
		ImGui::SameLine(textOffset);
		ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + availWidth - textOffset);
		ImGui::Text(_("entity_info.wasd_move").c_str());
		ImGui::PopTextWrapPos();

		ImGui::Spacing();

		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
		ImGui::Text(ICON_FA_KEYBOARD);
		ImGui::PopStyleColor();
		ImGui::SameLine(textOffset);
		ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + availWidth - textOffset);
		ImGui::Text(_("entity_info.qe_vertical").c_str());
		ImGui::PopTextWrapPos();
	}
}

void EntityInfoPanel::renderGizmoControls() noexcept {
	const float availWidth = ImGui::GetContentRegionAvail().x;

	ImGui::Spacing();

	// Header
	auto headerColor = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
	headerColor.w = 1.0f;
	ImGui::PushStyleColor(ImGuiCol_Text, headerColor);
	ImGui::Text(ICON_FA_CROSSHAIRS " %s", _("entity_info.gizmo_controls").c_str());
	ImGui::PopStyleColor();

	ImGui::Separator();
	ImGui::Spacing();

	const float iconWidth = ui::em(2.0f);
	const float textOffset = iconWidth + ui::em(0.3f);

	// Current mode indicator
	bool isLocal = (EntityGizmo::getCurrentMode() == GizmoMode::LOCAL);
	ImVec4 modeColor = isLocal ? ImVec4(0.45f, 0.85f, 0.45f, 1.0f) : ImVec4(0.85f, 0.75f, 0.35f, 1.0f);
	ImGui::PushStyleColor(ImGuiCol_Text, modeColor);
	ImGui::Text("%s: %s", _("entity_info.gizmo_mode").c_str(),
	            (isLocal ? _("entity_info.gizmo_mode_local") : _("entity_info.gizmo_mode_global")).c_str());
	ImGui::PopStyleColor();

	ImGui::Spacing();

	// T / R / S operation hints — highlight the active one
	GizmoOperation activeOp = EntityGizmo::getCurrentOperation();

	auto renderOpHint = [&](char label, GizmoOperation op, const char* locKey) {
		bool active = (activeOp == op);
		if (active) {
			ImGui::PushStyleColor(ImGuiCol_Text, modeColor);
		}
		ImGui::Text("[%c]", label);
		if (active) {
			ImGui::PopStyleColor();
		}
		ImGui::SameLine();
		ImGui::Text("%s", _(locKey).c_str());
	};

	renderOpHint('1', GizmoOperation::TRANSLATE, "entity_info.gizmo_translate");
	renderOpHint('2', GizmoOperation::ROTATE, "entity_info.gizmo_rotate");
	renderOpHint('3', GizmoOperation::SCALE, "entity_info.gizmo_scale");

	ImGui::Spacing();

	// G — mode toggle hint
	ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
	ImGui::Text(ICON_FA_KEYBOARD);
	ImGui::PopStyleColor();
	ImGui::SameLine(textOffset);
	ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + availWidth - textOffset);
	ImGui::Text(_("entity_info.gizmo_mode_toggle").c_str());
	ImGui::PopTextWrapPos();
}

void EntityInfoPanel::renderEntitySpecificInfo(Entity* entity) noexcept {
	// This method can be extended to show entity-specific information
	// based on component types (actor, vehicle, object, etc.)

	if (!entity) {
		return;
	}

	// Example: Show info for specific entity types
	// Uncomment and customize as needed:

	/*
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	if (entity->hasComponent("actor")) {
	    // Use theme color with green tint for actors
	    auto actorColor = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);
	    actorColor.x *= 0.5f; actorColor.y *= 1.2f; // Greenish tint
	    ImGui::PushStyleColor(ImGuiCol_Text, actorColor);
	    ImGui::Text(ICON_FA_USER " %s", _("entity_info.actor_info").c_str());
	    ImGui::PopStyleColor();
	    // Add actor-specific controls/info here
	}

	if (entity->hasComponent("vehicle")) {
	    // Use theme color with red tint for vehicles
	    auto vehicleColor = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);
	    vehicleColor.x *= 1.2f; vehicleColor.y *= 0.5f; // Reddish tint
	    ImGui::PushStyleColor(ImGuiCol_Text, vehicleColor);
	    ImGui::Text(ICON_FA_CAR " %s", _("entity_info.vehicle_info").c_str());
	    ImGui::PopStyleColor();
	    // Add vehicle-specific controls/info here
	}

	if (entity->hasComponent("object")) {
	    // Use theme color with orange tint for objects
	    auto objectColor = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);
	    objectColor.y *= 0.9f; objectColor.z *= 0.5f; // Orangish tint
	    ImGui::PushStyleColor(ImGuiCol_Text, objectColor);
	    ImGui::Text(ICON_FA_CUBE " %s", _("entity_info.object_info").c_str());
	    ImGui::PopStyleColor();
	    // Add object-specific controls/info here
	}
	*/
}
