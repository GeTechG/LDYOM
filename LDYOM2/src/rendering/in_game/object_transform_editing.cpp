#define IMGUI_DEFINE_MATH_OPERATORS
#include "object_transform_editing.h"
#include <CCamera.h>
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
#include <utils/ui_scale.h>
#include <localization.h>
#include <matrix_utils.h>
#include <models_manager.h>
#include <utils/manual_editing_session.h>
#include <utils/task_manager.h>
#include <window_manager.h>

std::array<float, 3> ObjectTransformEditing::m_position = {0.f, 0.f, 0.f};
CQuaternion ObjectTransformEditing::m_rotation = {{{0.f, 0.f, 0.f}}, 0.f};
std::array<float, 3> ObjectTransformEditing::m_scale = {1.f, 1.f, 1.f};
CObject* ObjectTransformEditing::m_object = nullptr;
std::function<void(ObjectTransformEditingCallbackData)> ObjectTransformEditing::m_onCloseCallback = nullptr;
CPlayerPed* ObjectTransformEditing::playerPed = nullptr;
std::unique_ptr<ManualEditingSession> ObjectTransformEditing::m_session = nullptr;

void ObjectTransformEditing::render() noexcept {
	constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
	                                         ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
	                                         ImGuiWindowFlags_NoNav;
	ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_Always);
	if (ImGui::Begin("##ObjectTransformEditor", nullptr, windowFlags)) {
		ImGui::PushTextWrapPos(ui::em(18.75f));
		ImGui::TextWrapped(_("object_transform_editing.info_cancel_editing", "F").c_str());
		ImGui::TextWrapped(_("object_transform_editing.info_save", "Y").c_str());
		ImGui::PopTextWrapPos();
	}

	static CQuaternion lastQ;
	const glm::quat quaternion(m_rotation.real, m_rotation.imag.x, m_rotation.imag.y, m_rotation.imag.z);
	const glm::vec3 eulerAngles = glm::eulerAngles(quaternion);

	// rotations
	static std::array<float, 3> eularRot = {0, 0, 0};
	if (abs(m_rotation.real - lastQ.real) > FLT_EPSILON || abs(m_rotation.imag.x - lastQ.imag.x) > FLT_EPSILON ||
	    abs(m_rotation.imag.y - lastQ.imag.y) > FLT_EPSILON || abs(m_rotation.imag.z - lastQ.imag.z) > FLT_EPSILON) {
		eularRot[0] = glm::degrees(eulerAngles.x);
		eularRot[1] = glm::degrees(eulerAngles.y);
		eularRot[2] = glm::degrees(eulerAngles.z);
		lastQ = m_rotation;
	}

	bool isRotating = false;
	bool isMoving = false;

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
			m_rotation.Set(static_cast<float>(RAD(eularRot[1])), static_cast<float>(RAD(eularRot[2])),
			               static_cast<float>(RAD(eularRot[0])));
		}
	} else {
		if (ImGui::IsKeyDown(ImGuiKey_W)) {
			m_position[0] += 0.1f;
			isMoving = true;
		}

		if (ImGui::IsKeyDown(ImGuiKey_S)) {
			m_position[0] -= 0.1f;
			isMoving = true;
		}

		if (ImGui::IsKeyDown(ImGuiKey_A)) {
			m_position[1] += 0.1f;
			isMoving = true;
		}

		if (ImGui::IsKeyDown(ImGuiKey_D)) {
			m_position[1] -= 0.1f;
			isMoving = true;
		}

		if (ImGui::IsKeyDown(ImGuiKey_Q)) {
			m_position[2] += 0.1f;
			isMoving = true;
		}

		if (ImGui::IsKeyDown(ImGuiKey_E)) {
			m_position[2] -= 0.1f;
			isMoving = true;
		}
	}

	if (isRotating || isMoving) {
		m_object->Teleport(CVector(m_position[0], m_position[1], m_position[2]), false);
		m_object->m_matrix->SetRotate(m_rotation);
		scaleMatrix(*m_object->m_matrix, m_scale);
		m_object->m_matrix->UpdateRW();
		m_object->UpdateRwMatrix();
		m_object->UpdateRwFrame();
	}

	if (ImGui::IsKeyReleased(ImGuiKey_F)) {
		closeTransformEditing(false);
	}
	if (ImGui::IsKeyReleased(ImGuiKey_Y)) {
		closeTransformEditing(true);
	}

	ImGui::End();
}

void ObjectTransformEditing::openTransformEditing(
	Entity* entity, CObject* object, std::array<float, 3> position, CQuaternion rotation, std::array<float, 3> scale,
	std::function<void(ObjectTransformEditingCallbackData)> onClose) noexcept {

	// Create RAII session that handles UI/camera automatically
	m_session = std::make_unique<ManualEditingSession>(ManualEditingSession::Options{
		.entity = entity,
		.disableUI = true,
		.disableCamera = true,
		.showInfoPanel = true,
		.onComplete = nullptr // We'll call our callback manually in closeTransformEditing
	});

	m_object = object;
	m_position = position;
	m_rotation = rotation;
	m_scale = scale;
	m_onCloseCallback = std::move(onClose);
	playerPed = FindPlayerPed();
	CWorld::Remove(playerPed);
	if (object) {
		object->Teleport(CVector(m_position[0], m_position[1], m_position[2]), false);
		object->m_matrix->SetRotate(m_rotation);
		scaleMatrix(*object->m_matrix, m_scale);
		object->m_matrix->UpdateRW();
		object->UpdateRwMatrix();
		object->UpdateRwFrame();
	}
	WindowManager::instance().addBackgroundRenderCallback("ObjectTransformEditor", render);
}

void ObjectTransformEditing::closeTransformEditing(bool saveChanges) noexcept {
	CWorld::Add(playerPed);

	// Call user callback before session cleanup
	if (m_onCloseCallback) {
		m_onCloseCallback({saveChanges, m_position, m_rotation, m_scale});
		m_onCloseCallback = nullptr;
	}

	WindowManager::instance().removeBackgroundRenderCallback("ObjectTransformEditor");

	// Destroy session (triggers RAII cleanup: UI/camera restoration)
	m_session.reset();
}