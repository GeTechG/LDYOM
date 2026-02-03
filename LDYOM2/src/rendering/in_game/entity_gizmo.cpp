#define IMGUI_DEFINE_MATH_OPERATORS
#include "entity_gizmo.h"
#include <imgui.h>

#include <ImGuizmo.h>
#include <entity.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <CCamera.h>
#include <in_game/entity_orbit_camera.h>


// Static member initialization
Entity* EntityGizmo::m_entity = nullptr;
GizmoOperation EntityGizmo::m_currentOperation = GizmoOperation::TRANSLATE;
GizmoMode EntityGizmo::m_currentMode = GizmoMode::LOCAL;

void EntityGizmo::activate(Entity* entity) noexcept {
	m_entity = entity;
	m_currentOperation = GizmoOperation::TRANSLATE;
}

void EntityGizmo::deactivate() noexcept { m_entity = nullptr; }

void EntityGizmo::render() noexcept {
	if (!m_entity) {
		return;
	}

	ImGuizmo::BeginFrame();

	// Keyboard shortcuts for mode switching (only when not typing)
	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantTextInput) {
		if (ImGui::IsKeyPressed(ImGuiKey_T))
			m_currentOperation = GizmoOperation::TRANSLATE;
		if (ImGui::IsKeyPressed(ImGuiKey_R))
			m_currentOperation = GizmoOperation::ROTATE;
		if (ImGui::IsKeyPressed(ImGuiKey_S))
			m_currentOperation = GizmoOperation::SCALE;
		if (ImGui::IsKeyPressed(ImGuiKey_G))
			m_currentMode = (m_currentMode == GizmoMode::LOCAL) ? GizmoMode::GLOBAL : GizmoMode::LOCAL;
	}

	// Draw on top of everything
	ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

	// Build view matrix from orbit camera state
	CVector camPos = EntityOrbitCamera::getCameraPosition();
	CVector target = EntityOrbitCamera::getTargetPosition();

	glm::vec3 eye(camPos.x, camPos.y, camPos.z);
	glm::vec3 center(target.x, target.y, target.z);
	glm::vec3 up(0.0f, 0.0f, 1.0f);

	// If eye and center are identical or up is parallel to view, use fallback up
	glm::vec3 viewDir = center - eye;
	if (glm::length(viewDir) < 0.0001f) {
		return; // Camera coincides with target, nothing to render
	}
	glm::vec3 viewDirNorm = glm::normalize(viewDir);
	if (glm::abs(glm::dot(viewDirNorm, up)) > 0.9999f) {
		up = glm::vec3(0.0f, 1.0f, 0.0f); // Fallback when looking straight up/down
	}

	glm::mat4 viewMatrix = glm::lookAt(eye, center, up);
	float aspect = io.DisplaySize.x / io.DisplaySize.y;
	glm::mat4 projMatrix = glm::perspective(glm::radians(TheCamera.FindCamFOV()), aspect, 0.1f, 400.0f);

	// Build model matrix from entity transform
	glm::vec3 pos(m_entity->position[0], m_entity->position[1], m_entity->position[2]);
	glm::quat rot(m_entity->rotation.real, m_entity->rotation.imag.x, m_entity->rotation.imag.y,
	              m_entity->rotation.imag.z); // glm::quat(w, x, y, z)
	glm::vec3 scl(m_entity->scale[0], m_entity->scale[1], m_entity->scale[2]);

	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), pos);
	modelMatrix *= glm::mat4_cast(rot);
	modelMatrix *= glm::scale(glm::mat4(1.0f), scl);

	// Map our enum to ImGuizmo operation
	ImGuizmo::OPERATION op;
	switch (m_currentOperation) {
		case GizmoOperation::TRANSLATE: op = ImGuizmo::TRANSLATE; break;
		case GizmoOperation::ROTATE: op = ImGuizmo::ROTATE; break;
		case GizmoOperation::SCALE: op = ImGuizmo::SCALE; break;
	}

	ImGuizmo::MODE mode = (m_currentMode == GizmoMode::LOCAL) ? ImGuizmo::LOCAL : ImGuizmo::WORLD;
	bool changed = ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projMatrix), op, mode,
	                                    glm::value_ptr(modelMatrix));

	if (changed) {
		// Translation: column 3
		glm::vec3 newPos = modelMatrix[3];

		// Scale: length of each column (0, 1, 2)
		glm::vec3 newScale(glm::length(modelMatrix[0]), glm::length(modelMatrix[1]), glm::length(modelMatrix[2]));

		// Rotation: normalize the 3x3 sub-matrix columns, then quat_cast
		glm::mat3 rotMat;
		rotMat[0] = modelMatrix[0] / newScale.x;
		rotMat[1] = modelMatrix[1] / newScale.y;
		rotMat[2] = modelMatrix[2] / newScale.z;
		glm::quat newRot = glm::quat_cast(rotMat);

		m_entity->position[0] = newPos.x;
		m_entity->position[1] = newPos.y;
		m_entity->position[2] = newPos.z;

		m_entity->rotation.real = newRot.w;
		m_entity->rotation.imag.x = newRot.x;
		m_entity->rotation.imag.y = newRot.y;
		m_entity->rotation.imag.z = newRot.z;

		m_entity->scale[0] = newScale.x;
		m_entity->scale[1] = newScale.y;
		m_entity->scale[2] = newScale.z;

		m_entity->updateSetTransformCallbacks();
	}
}
