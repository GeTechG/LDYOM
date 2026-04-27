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

	// Keyboard shortcuts for mode switching (only when not typing). The !io.KeyAlt guard prevents Alt+digit chords
	// (used by EntitiesWindow for tab navigation) from also firing gizmo operations.
	ImGuiIO& io = ImGui::GetIO();
	if (!io.WantTextInput && !io.KeyAlt) {
		if (ImGui::IsKeyPressed(ImGuiKey_1))
			m_currentOperation = GizmoOperation::TRANSLATE;
		if (ImGui::IsKeyPressed(ImGuiKey_2))
			m_currentOperation = GizmoOperation::ROTATE;
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

	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), pos);
	modelMatrix *= glm::mat4_cast(rot);

	// Map our enum to ImGuizmo operation
	ImGuizmo::OPERATION op;
	switch (m_currentOperation) {
		case GizmoOperation::TRANSLATE: op = ImGuizmo::TRANSLATE; break;
		case GizmoOperation::ROTATE: op = ImGuizmo::ROTATE; break;
	}

	ImGuizmo::MODE mode = (m_currentMode == GizmoMode::LOCAL) ? ImGuizmo::LOCAL : ImGuizmo::WORLD;
	bool changed = ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projMatrix), op, mode,
	                                    glm::value_ptr(modelMatrix));

	if (changed) {
		glm::vec3 newPos = modelMatrix[3];
		glm::quat newRot = glm::quat_cast(glm::mat3(modelMatrix));

		m_entity->position[0] = newPos.x;
		m_entity->position[1] = newPos.y;
		m_entity->position[2] = newPos.z;

		m_entity->rotation.real = newRot.w;
		m_entity->rotation.imag.x = newRot.x;
		m_entity->rotation.imag.y = newRot.y;
		m_entity->rotation.imag.z = newRot.z;

		m_entity->updateSetTransformCallbacks();
	}
}
