#define IMGUI_DEFINE_MATH_OPERATORS
#include "object_paths_editing.h"
#include <CPools.h>
#include <CSprite.h>
#include <CStreaming.h>
#include <CWorld.h>
#include <angles.h>
#include <common.h>
#include <entity.h>
#include <extensions/ScriptCommands.h>
#include <glm/gtc/quaternion.hpp>
#include <imgui_hook/utils/imgui_configurate.h>
#include <localization.h>
#include <matrix_utils.h>
#include <models_manager.h>
#include <utils/manual_editing_session.h>
#include <utils/task_manager.h>
#include <window_manager.h>

std::vector<components::PathPoint> ObjectPathsEditing::m_points;
std::function<void(bool, const std::vector<components::PathPoint>&)> ObjectPathsEditing::m_onCloseCallback;
size_t ObjectPathsEditing::m_currentPointIndex = 0;
std::array<float, 3> ObjectPathsEditing::m_tempPosition = {0.f, 0.f, 0.f};
CQuaternion ObjectPathsEditing::m_tempRotation = {{{0.f, 0.f, 0.f}}, 1.f};
CObject* ObjectPathsEditing::m_object = nullptr;
CPlayerPed* ObjectPathsEditing::playerPed = nullptr;
std::unique_ptr<ManualEditingSession> ObjectPathsEditing::m_session = nullptr;

void ObjectPathsEditing::render() noexcept {
	constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
	                                         ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
	                                         ImGuiWindowFlags_NoNav;
	ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_Always);
	if (ImGui::Begin("##ObjectPathsEditor", nullptr, windowFlags)) {
		const auto textWidth = (SCL_PX).x * 300.f;
		ImGui::PushTextWrapPos(textWidth);
		ImGui::TextWrapped(_("object_paths_editing.info_add_point", "N").c_str());
		ImGui::TextWrapped(_("object_paths_editing.info_remove_point", "M").c_str());
		ImGui::TextWrapped(_("object_paths_editing.info_clear_all_points", "L").c_str());
		ImGui::TextWrapped(_("object_paths_editing.info_move_point_next", "]").c_str());
		ImGui::TextWrapped(_("object_paths_editing.info_move_point_prev", "[").c_str());
		ImGui::TextWrapped(_("object_paths_editing.info_update_point", "U").c_str());
		ImGui::TextWrapped(_("object_paths_editing.info_goto_point", "G").c_str());
		ImGui::Separator();
		ImGui::TextWrapped(_("object_paths_editing.info_move", "WASDQE").c_str());
		ImGui::TextWrapped(_("object_paths_editing.info_rotate", "Shift+WASDQE").c_str());
		ImGui::Separator();
		ImGui::TextWrapped(_("object_paths_editing.info_cancel_editing", "F").c_str());
		ImGui::TextWrapped(_("object_paths_editing.info_save", "Y").c_str());
		ImGui::Text(_("object_paths_editing.points_count", std::to_string(m_points.size())).c_str());
		if (!m_points.empty()) {
			ImGui::Text(_("object_paths_editing.current_point", std::to_string(m_currentPointIndex + 1)).c_str());
		}
		ImGui::PopTextWrapPos();
	}

	// Draw path visualization
	auto drawlist = ImGui::GetBackgroundDrawList();
	if (!m_points.empty()) {
		RwV3d lastPoint;
		float w, h;
		auto lastVisible = CSprite::CalcScreenCoors(
			RwV3d{m_points.at(0).position[0], m_points.at(0).position[1], m_points.at(0).position[2]}, &lastPoint, &w,
			&h, true, true);
		if (lastVisible)
			drawlist->AddCircleFilled(ImVec2(lastPoint.x, lastPoint.y), 5.f,
			                          m_currentPointIndex == 0 ? IM_COL32(255, 0, 0, 255) : IM_COL32_WHITE);
		for (size_t i = 1; i < m_points.size(); ++i) {
			RwV3d currentPoint;
			const auto currentVisible = CSprite::CalcScreenCoors(
				RwV3d{m_points.at(i).position[0], m_points.at(i).position[1], m_points.at(i).position[2]},
				&currentPoint, &w, &h, true, true);
			if (lastVisible && currentVisible)
				drawlist->AddLine(ImVec2(lastPoint.x, lastPoint.y), ImVec2(currentPoint.x, currentPoint.y),
				                  IM_COL32_WHITE);
			if (currentVisible)
				drawlist->AddCircleFilled(ImVec2(currentPoint.x, currentPoint.y), 5.f,
				                          m_currentPointIndex == i ? IM_COL32(255, 0, 0, 255) : IM_COL32_WHITE);
			lastPoint = currentPoint;
			lastVisible = currentVisible;
		}
	}

	// Handle rotation/position editing
	static CQuaternion lastQ;
	const glm::quat quaternion(m_tempRotation.real, m_tempRotation.imag.x, m_tempRotation.imag.y,
	                           m_tempRotation.imag.z);
	const glm::vec3 eulerAngles = glm::eulerAngles(quaternion);

	static std::array<float, 3> eularRot = {0, 0, 0};
	if (abs(m_tempRotation.real - lastQ.real) > FLT_EPSILON ||
	    abs(m_tempRotation.imag.x - lastQ.imag.x) > FLT_EPSILON ||
	    abs(m_tempRotation.imag.y - lastQ.imag.y) > FLT_EPSILON ||
	    abs(m_tempRotation.imag.z - lastQ.imag.z) > FLT_EPSILON) {
		eularRot[0] = glm::degrees(eulerAngles.x);
		eularRot[1] = glm::degrees(eulerAngles.y);
		eularRot[2] = glm::degrees(eulerAngles.z);
		lastQ = m_tempRotation;
	}

	bool isRotating = false;
	bool isMoving = false;

	// Rotation controls (Shift + WASDQE)
	if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
		if (ImGui::IsKeyDown(ImGuiKey_W)) {
			eularRot[0] += 0.5f;
			isRotating = true;
		}
		if (ImGui::IsKeyDown(ImGuiKey_S)) {
			eularRot[0] -= 0.5f;
			isRotating = true;
		}
		if (ImGui::IsKeyDown(ImGuiKey_A)) {
			eularRot[1] += 0.5f;
			isRotating = true;
		}
		if (ImGui::IsKeyDown(ImGuiKey_D)) {
			eularRot[1] -= 0.5f;
			isRotating = true;
		}
		if (ImGui::IsKeyDown(ImGuiKey_Q)) {
			eularRot[2] += 0.5f;
			isRotating = true;
		}
		if (ImGui::IsKeyDown(ImGuiKey_E)) {
			eularRot[2] -= 0.5f;
			isRotating = true;
		}
		if (isRotating) {
			m_tempRotation.Set(static_cast<float>(RAD(eularRot[1])), static_cast<float>(RAD(eularRot[2])),
			                   static_cast<float>(RAD(eularRot[0])));
		}
	} else {
		// Position controls (WASDQE)
		if (ImGui::IsKeyDown(ImGuiKey_W)) {
			m_tempPosition[0] += 0.1f;
			isMoving = true;
		}
		if (ImGui::IsKeyDown(ImGuiKey_S)) {
			m_tempPosition[0] -= 0.1f;
			isMoving = true;
		}
		if (ImGui::IsKeyDown(ImGuiKey_A)) {
			m_tempPosition[1] += 0.1f;
			isMoving = true;
		}
		if (ImGui::IsKeyDown(ImGuiKey_D)) {
			m_tempPosition[1] -= 0.1f;
			isMoving = true;
		}
		if (ImGui::IsKeyDown(ImGuiKey_Q)) {
			m_tempPosition[2] += 0.1f;
			isMoving = true;
		}
		if (ImGui::IsKeyDown(ImGuiKey_E)) {
			m_tempPosition[2] -= 0.1f;
			isMoving = true;
		}
	}

	// Update object transform if changed
	if (isRotating || isMoving) {
		if (m_object) {
			m_object->Teleport(CVector(m_tempPosition[0], m_tempPosition[1], m_tempPosition[2]), false);
			m_object->m_matrix->SetRotate(m_tempRotation);
			m_object->m_matrix->UpdateRW();
			m_object->UpdateRwMatrix();
			m_object->UpdateRwFrame();
		}
	}

	// Point management hotkeys
	if (ImGui::IsKeyReleased(ImGuiKey_N)) {
		// Add new point
		components::PathPoint newPoint;
		newPoint.position = m_tempPosition;
		newPoint.rotation = m_tempRotation;
		m_points.push_back(newPoint);
		m_currentPointIndex = m_points.size() - 1;
	}

	if (ImGui::IsKeyReleased(ImGuiKey_M)) {
		// Remove current point
		if (!m_points.empty() && m_currentPointIndex < m_points.size()) {
			m_points.erase(m_points.begin() + m_currentPointIndex);
			if (m_currentPointIndex >= m_points.size() && m_currentPointIndex > 0) {
				m_currentPointIndex = m_points.size() - 1;
			}
			if (!m_points.empty() && m_currentPointIndex < m_points.size()) {
				m_tempPosition = m_points[m_currentPointIndex].position;
				m_tempRotation = m_points[m_currentPointIndex].rotation;
				if (m_object) {
					m_object->Teleport(CVector(m_tempPosition[0], m_tempPosition[1], m_tempPosition[2]), false);
					m_object->m_matrix->SetRotate(m_tempRotation);
					m_object->m_matrix->UpdateRW();
					m_object->UpdateRwMatrix();
					m_object->UpdateRwFrame();
				}
			}
		}
	}

	if (ImGui::IsKeyReleased(ImGuiKey_L)) {
		// Clear all points
		m_points.clear();
		m_currentPointIndex = 0;
	}

	if (ImGui::IsKeyReleased(ImGuiKey_RightBracket)) {
		// Next point
		if (!m_points.empty() && m_currentPointIndex < m_points.size() - 1) {
			m_currentPointIndex++;
			m_tempPosition = m_points[m_currentPointIndex].position;
			m_tempRotation = m_points[m_currentPointIndex].rotation;
			if (m_object) {
				m_object->Teleport(CVector(m_tempPosition[0], m_tempPosition[1], m_tempPosition[2]), false);
				m_object->m_matrix->SetRotate(m_tempRotation);
				m_object->m_matrix->UpdateRW();
				m_object->UpdateRwMatrix();
				m_object->UpdateRwFrame();
			}
		}
	}

	if (ImGui::IsKeyReleased(ImGuiKey_LeftBracket)) {
		// Previous point
		if (!m_points.empty() && m_currentPointIndex > 0) {
			m_currentPointIndex--;
			m_tempPosition = m_points[m_currentPointIndex].position;
			m_tempRotation = m_points[m_currentPointIndex].rotation;
			if (m_object) {
				m_object->Teleport(CVector(m_tempPosition[0], m_tempPosition[1], m_tempPosition[2]), false);
				m_object->m_matrix->SetRotate(m_tempRotation);
				m_object->m_matrix->UpdateRW();
				m_object->UpdateRwMatrix();
				m_object->UpdateRwFrame();
			}
		}
	}

	if (ImGui::IsKeyReleased(ImGuiKey_U)) {
		// Update current point
		if (!m_points.empty() && m_currentPointIndex < m_points.size()) {
			m_points[m_currentPointIndex].position = m_tempPosition;
			m_points[m_currentPointIndex].rotation = m_tempRotation;
		}
	}

	if (ImGui::IsKeyReleased(ImGuiKey_G)) {
		// Go to current point
		if (!m_points.empty() && m_currentPointIndex < m_points.size()) {
			m_tempPosition = m_points[m_currentPointIndex].position;
			m_tempRotation = m_points[m_currentPointIndex].rotation;
			if (m_object) {
				m_object->Teleport(CVector(m_tempPosition[0], m_tempPosition[1], m_tempPosition[2]), false);
				m_object->m_matrix->SetRotate(m_tempRotation);
				m_object->m_matrix->UpdateRW();
				m_object->UpdateRwMatrix();
				m_object->UpdateRwFrame();
			}
		}
	}

	// Save/Cancel
	if (ImGui::IsKeyReleased(ImGuiKey_F)) {
		closePathEditor(false);
	}
	if (ImGui::IsKeyReleased(ImGuiKey_Y)) {
		closePathEditor(true);
	}

	ImGui::End();
}

void ObjectPathsEditing::openPathEditor(
	Entity* entity, CObject* object, const std::vector<components::PathPoint>& points,
	std::function<void(bool, const std::vector<components::PathPoint>&)> onClose) noexcept {

	// Create RAII session that handles UI/camera automatically
	m_session = std::make_unique<ManualEditingSession>(ManualEditingSession::Options{
		.entity = entity,
		.disableUI = true,
		.disableCamera = true,
		.showInfoPanel = true,
		.onComplete = nullptr // We'll call it manually in closePathEditor
	});

	m_object = object;
	m_points = points;
	m_onCloseCallback = std::move(onClose);
	m_currentPointIndex = 0;
	playerPed = FindPlayerPed();
	CWorld::Remove(playerPed);

	if (!m_points.empty()) {
		m_tempPosition = m_points[0].position;
		m_tempRotation = m_points[0].rotation;
	} else {
		const auto p = playerPed->GetPosition();
		m_tempPosition = {p.x, p.y, p.z};
		m_tempRotation = {{{0.f, 0.f, 0.f}}, 1.f};
	}

	if (object) {
		object->Teleport(CVector(m_tempPosition[0], m_tempPosition[1], m_tempPosition[2]), false);
		object->m_matrix->SetRotate(m_tempRotation);
		object->m_matrix->UpdateRW();
		object->UpdateRwMatrix();
		object->UpdateRwFrame();
	}

	WindowManager::instance().addBackgroundRenderCallback("ObjectPathsEditor", render);
}

void ObjectPathsEditing::closePathEditor(bool saveChanges) noexcept {
	CWorld::Add(playerPed);

	// Call user callback before session cleanup
	if (m_onCloseCallback) {
		m_onCloseCallback(saveChanges, m_points);
		m_onCloseCallback = nullptr;
	}

	WindowManager::instance().removeBackgroundRenderCallback("ObjectPathsEditor");

	// Destroy session (triggers RAII cleanup: UI/camera restoration)
	m_session.reset();
}
