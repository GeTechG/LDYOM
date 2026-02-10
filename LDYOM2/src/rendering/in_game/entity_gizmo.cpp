#define IMGUI_DEFINE_MATH_OPERATORS
#include "entity_gizmo.h"
#include <imgui.h>

#include <CCamera.h>
#include <CDraw.h>
#include <ImGuizmo.h>
#include <entity.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

// Static member initialization
Entity* EntityGizmo::m_entity = nullptr;
GizmoOperation EntityGizmo::m_currentOperation = GizmoOperation::TRANSLATE;
GizmoMode EntityGizmo::m_currentMode = GizmoMode::LOCAL;

void EntityGizmo::activate(Entity* entity) noexcept {
	m_entity = entity;
	m_currentOperation = GizmoOperation::TRANSLATE;
}

void EntityGizmo::deactivate() noexcept { m_entity = nullptr; }

void EntityGizmo::render(CVector eye, CVector center) noexcept {
	if (!m_entity) {
		return;
	}

	ImGuizmo::BeginFrame();

	// Keyboard shortcuts for mode switching (only when not typing)
	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantTextInput) {
		if (ImGui::IsKeyPressed(ImGuiKey_1))
			m_currentOperation = GizmoOperation::TRANSLATE;
		if (ImGui::IsKeyPressed(ImGuiKey_2))
			m_currentOperation = GizmoOperation::ROTATE;
		if (ImGui::IsKeyPressed(ImGuiKey_3))
			m_currentOperation = GizmoOperation::SCALE;
		if (ImGui::IsKeyPressed(ImGuiKey_G))
			m_currentMode = (m_currentMode == GizmoMode::LOCAL) ? GizmoMode::GLOBAL : GizmoMode::LOCAL;
	}

	// Draw on top of everything
	ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList());
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

	// Use the camera position and look target that will be applied THIS frame,
	// avoiding the one-frame lag from reading TheCamera before the commands are issued.
	glm::vec3 glmEye(eye.x, eye.y, eye.z);
	glm::vec3 glmCenter(center.x, center.y, center.z);

	glm::vec3 viewDir = glmCenter - glmEye;
	if (glm::length(viewDir) < 0.0001f) {
		return;
	}

	glm::mat4 viewMatrix = glm::lookAt(glmEye, glmCenter, glm::vec3(0.0f, 0.0f, 1.0f));

	// FindCamFOV() returns horizontal FOV; convert to vertical using the same
	// aspect ratio the game uses for its projection matrix.
	float aspect = CDraw::ms_fAspectRatio;
	float vFOV = 2.0f * std::atan(std::tan(glm::radians(TheCamera.FindCamFOV()) * 0.5f) / aspect);
	glm::mat4 projMatrix = glm::perspective(vFOV, aspect, 0.1f, 400.0f);

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
