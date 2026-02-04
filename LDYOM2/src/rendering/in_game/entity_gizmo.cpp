#define IMGUI_DEFINE_MATH_OPERATORS
#include "entity_gizmo.h"
#include <imgui.h>

#include <CCamera.h>
#include <ImGuizmo.h>
#include <d3d9.h>
#include <entity.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

extern void* gD3DDevice; // imgui_hook.cpp — D3D9 device stored at init

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

	// Use the camera position and look target that will be applied THIS frame,
	// avoiding the one-frame lag from reading TheCamera before the commands are issued.
	glm::vec3 glmEye(eye.x, eye.y, eye.z);
	glm::vec3 glmCenter(center.x, center.y, center.z);

	glm::vec3 viewDir = glmCenter - glmEye;
	if (glm::length(viewDir) < 0.0001f) {
		return;
	}

	glm::mat4 viewMatrix = glm::lookAt(glmEye, glmCenter, glm::vec3(0.0f, 0.0f, 1.0f));

	// Extract actual projection from D3D9 so the gizmo aligns with game rendering.
	// GTA's aspect and vFOV differ from display aspect / FindCamFOV.
	float vFOV  = glm::radians(TheCamera.FindCamFOV());   // fallback
	float aspect = io.DisplaySize.x / io.DisplaySize.y;   // fallback
	if (gD3DDevice) {
		auto* d3d = reinterpret_cast<IDirect3DDevice9*>(gD3DDevice);
		D3DMATRIX d3dProj;
		if (SUCCEEDED(d3d->GetTransform(D3DTS_PROJECTION, &d3dProj))) {
			float p00 = d3dProj.m[0][0]; // f / aspect
			float p11 = d3dProj.m[1][1]; // f = 1 / tan(vFOV/2)
			if (p00 > 0.01f && p11 > 0.01f) {
				aspect = p11 / p00;
				vFOV   = 2.0f * std::atan(1.0f / p11);
			}
		}
	}
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

	// --- DEBUG: three projection dots ---
	// Magenta = current (FOV as vertical)  |  Green = FOV converted h→v  |  Cyan = D3D9 actual
	{
		float camFOV = TheCamera.FindCamFOV();

		// --- Magenta: current projection (FOV treated as vertical) ---
		glm::vec4 clipMag = projMatrix * viewMatrix * glm::vec4(pos, 1.0f);
		float sxMag = -1.0f, syMag = -1.0f;
		if (clipMag.w > 0.0f) {
			glm::vec3 ndc = glm::vec3(clipMag) / clipMag.w;
			sxMag = (ndc.x * 0.5f + 0.5f) * io.DisplaySize.x;
			syMag = (-ndc.y * 0.5f + 0.5f) * io.DisplaySize.y;
			ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2(sxMag, syMag), 6.0f, IM_COL32(255, 0, 255, 255));
		}

		// --- Green: horizontal FOV → vertical FOV conversion ---
		float vFOV = 2.0f * std::atan(std::tan(glm::radians(camFOV) * 0.5f) / aspect);
		glm::mat4 projHtoV = glm::perspective(vFOV, aspect, 0.1f, 400.0f);
		glm::vec4 clipGrn = projHtoV * viewMatrix * glm::vec4(pos, 1.0f);
		float sxGrn = -1.0f, syGrn = -1.0f;
		if (clipGrn.w > 0.0f) {
			glm::vec3 ndc = glm::vec3(clipGrn) / clipGrn.w;
			sxGrn = (ndc.x * 0.5f + 0.5f) * io.DisplaySize.x;
			syGrn = (-ndc.y * 0.5f + 0.5f) * io.DisplaySize.y;
			ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2(sxGrn, syGrn), 6.0f, IM_COL32(0, 255, 0, 255));
		}

		// --- Yellow: D3D9 proj scaling (m[0][0], m[1][1]) → extract aspect+vFOV → our view ---
		// D3D9 view is already overwritten by this point but proj still has the camera values.
		// m[0][0] = f/aspect, m[1][1] = f where f = 1/tan(vFOV/2).  So aspect = m[1][1]/m[0][0].
		float sxYel = -1.0f, syYel = -1.0f;
		float d3dProj00 = 0, d3dProj11 = 0;
		if (gD3DDevice) {
			auto* d3d = reinterpret_cast<IDirect3DDevice9*>(gD3DDevice);
			D3DMATRIX d3dProj;
			if (SUCCEEDED(d3d->GetTransform(D3DTS_PROJECTION, &d3dProj))) {
				d3dProj00 = d3dProj.m[0][0];
				d3dProj11 = d3dProj.m[1][1];

				if (d3dProj00 > 0.01f && d3dProj11 > 0.01f) {
					float gtaAspect = d3dProj11 / d3dProj00;
					float gtaVFOV   = 2.0f * std::atan(1.0f / d3dProj11); // radians
					glm::mat4 projGTA = glm::perspective(gtaVFOV, gtaAspect, 0.1f, 400.0f);

					glm::vec4 clipY = projGTA * viewMatrix * glm::vec4(pos, 1.0f);
					if (clipY.w > 0.0f) {
						glm::vec3 ndc = glm::vec3(clipY) / clipY.w;
						sxYel = (ndc.x * 0.5f + 0.5f) * io.DisplaySize.x;
						syYel = (-ndc.y * 0.5f + 0.5f) * io.DisplaySize.y;
						ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2(sxYel, syYel), 8.0f,
						                                                IM_COL32(255, 255, 0, 255));
					}
				}
			}
		}

		ImGui::SetNextWindowPos(ImVec2(10, 250));
		ImGui::SetNextWindowSize(ImVec2(460, 200));
		if (ImGui::Begin("##GizmoDebug", nullptr,
		                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar)) {
			ImGui::Text("FindCamFOV: %.2f  disp-asp: %.3f  display: %.0fx%.0f",
			            camFOV, aspect, io.DisplaySize.x, io.DisplaySize.y);
			ImGui::Text("Eye: (%.2f, %.2f, %.2f)", eye.x, eye.y, eye.z);
			ImGui::Text("Entity: (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z);
			ImGui::Separator();
			ImGui::Text("Magenta (vFOV=70, asp=disp):      (%.1f, %.1f)", sxMag, syMag);
			ImGui::Text("Green   (hFOV->vFOV, asp=disp):   (%.1f, %.1f)", sxGrn, syGrn);
			ImGui::Text("Yellow  (D3D9 proj asp+FOV+ourV): (%.1f, %.1f)", sxYel, syYel);
			ImGui::Separator();
			ImGui::Text("D3D9 proj[0][0]=%.4f [1][1]=%.4f  → asp=%.3f vFOV=%.1f°",
			            d3dProj00, d3dProj11,
			            d3dProj00 > 0.01f ? d3dProj11 / d3dProj00 : 0.0f,
			            d3dProj11 > 0.01f ? 2.0f * std::atan(1.0f / d3dProj11) * 180.0f / std::numbers::pi : 0.0f);
		}
		ImGui::End();
	}
	// --- END DEBUG ---

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
