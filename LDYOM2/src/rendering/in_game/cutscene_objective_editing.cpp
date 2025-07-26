#define IMGUI_DEFINE_MATH_OPERATORS
#include "cutscene_objective_editing.h"
#include <CCamera.h>
#include <CHud.h>
#include <CPools.h>
#include <CSprite.h>
#include <CStreaming.h>
#include <CTheScripts.h>
#include <CWorld.h>
#include <angles.h>
#include <common.h>
#include <extensions/ScriptCommands.h>
#include <glm/gtc/quaternion.hpp>
#include <imgui_hook/utils/imgui_configurate.h>
#include <localization.h>
#include <matrix_utils.h>
#include <models_manager.h>
#include <utils/task_manager.h>
#include <window_manager.h>

objectives::cutscene::Data CutsceneObjectiveEditing::m_data;
CObject* CutsceneObjectiveEditing::m_object = nullptr;
std::function<void(bool, objectives::cutscene::Data)> CutsceneObjectiveEditing::m_onCloseCallback = nullptr;
CPlayerPed* CutsceneObjectiveEditing::playerPed = nullptr;

struct CutsceneObjectiveEditingOptions {
	bool renderObject = true;
	bool freeCamera = false;
	bool grid = false;
} options;

// Статические переменные для отслеживания инверсии управления
static bool isWSInverted = false;
static glm::vec3 lastCameraUp(0.0f, 0.0f, 1.0f); // Последний up вектор камеры

void CutsceneObjectiveEditing::render() noexcept {
	constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
	                                         ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
	                                         ImGuiWindowFlags_NoNav;
	ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_Always);
	if (ImGui::Begin("##CutsceneObjectiveEditing", nullptr, windowFlags)) {
		const auto textWidth = (SCL_PX).x * 300.f;
		ImGui::PushTextWrapPos(textWidth);
		ImGui::TextWrapped(_("cutscene_objective_editing.info_cancel_editing", "F").c_str());
		ImGui::TextWrapped(_("cutscene_objective_editing.info_save", "Y").c_str());
		ImGui::TextWrapped(_("cutscene_objective_editing.render_object", "R").c_str());
		ImGui::TextWrapped(_("cutscene_objective_editing.free_camera", "C").c_str());
		ImGui::TextWrapped(_("cutscene_objective_editing.normalize_camera", "N").c_str());
		ImGui::TextWrapped(_("cutscene_objective_editing.move_camera").c_str());
		ImGui::TextWrapped(_("cutscene_objective_editing.rotate_camera").c_str());
		ImGui::TextWrapped(_("cutscene_objective_editing.move_target").c_str());
		ImGui::TextWrapped(_("cutscene_objective_editing.grid", "G").c_str());
		ImGui::Text("%.2f, %.2f, %.2f", m_data.cameraPosition[0], m_data.cameraPosition[1], m_data.cameraPosition[2]);
		ImGui::PopTextWrapPos();
	}

	if (ImGui::IsKeyReleased(ImGuiKey_R)) {
		options.renderObject = !options.renderObject;
		updateCutsceneObject();
	}

	if (ImGui::IsKeyReleased(ImGuiKey_N)) {
		auto direction = CVector(m_data.targetPosition[0] - m_data.cameraPosition[0],
		                         m_data.targetPosition[1] - m_data.cameraPosition[1],
		                         m_data.targetPosition[2] - m_data.cameraPosition[2]);
		direction.Normalise();
		m_data.targetPosition[0] = m_data.cameraPosition[0] + direction.x;
		m_data.targetPosition[1] = m_data.cameraPosition[1] + direction.y;
		m_data.targetPosition[2] = m_data.cameraPosition[2] + direction.z;
	}

	if (ImGui::IsKeyReleased(ImGuiKey_C)) {
		options.freeCamera = !options.freeCamera;
		plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(0, true);
		TheCamera.RestoreWithJumpCut();
		TheCamera.m_bCameraPersistPosition = options.freeCamera;
		TheCamera.m_vecMoveLinearPosnEnd =
			CVector(m_data.cameraPosition[0], m_data.cameraPosition[1], m_data.cameraPosition[2]);
		TheCamera.m_vecMoveLinearPosnStart = TheCamera.m_vecMoveLinearPosnEnd;
		TheCamera.m_vecMoveLinear = TheCamera.m_vecMoveLinearPosnEnd;
	}

	if (ImGui::IsKeyReleased(ImGuiKey_G)) {
		options.grid = !options.grid;
	}

	auto forward = TheCamera.GetForward();
	auto right = TheCamera.GetRight();
	auto up = TheCamera.GetUp();
	if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
		if (ImGui::IsKeyDown(ImGuiKey_W)) {
			m_data.targetPosition[0] += 0.1f;
		}
		if (ImGui::IsKeyDown(ImGuiKey_S)) {
			m_data.targetPosition[0] -= 0.1f;
		}
		if (ImGui::IsKeyDown(ImGuiKey_A)) {
			m_data.targetPosition[1] += 0.1f;
		}
		if (ImGui::IsKeyDown(ImGuiKey_D)) {
			m_data.targetPosition[1] -= 0.1f;
		}
		if (ImGui::IsKeyDown(ImGuiKey_Q)) {
			m_data.targetPosition[2] += 0.1f;
		}
		if (ImGui::IsKeyDown(ImGuiKey_E)) {
			m_data.targetPosition[2] -= 0.1f;
		}
	} else if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
		if (ImGui::IsKeyDown(ImGuiKey_W)) {
			m_data.targetPosition[0] += forward.x * 0.1f;
			m_data.targetPosition[1] += forward.y * 0.1f;
			m_data.targetPosition[2] += forward.z * 0.1f;
			m_data.cameraPosition[0] += forward.x * 0.1f;
			m_data.cameraPosition[1] += forward.y * 0.1f;
			m_data.cameraPosition[2] += forward.z * 0.1f;
		}
		if (ImGui::IsKeyDown(ImGuiKey_S)) {
			m_data.targetPosition[0] -= forward.x * 0.1f;
			m_data.targetPosition[1] -= forward.y * 0.1f;
			m_data.targetPosition[2] -= forward.z * 0.1f;
			m_data.cameraPosition[0] -= forward.x * 0.1f;
			m_data.cameraPosition[1] -= forward.y * 0.1f;
			m_data.cameraPosition[2] -= forward.z * 0.1f;
		}
		if (ImGui::IsKeyDown(ImGuiKey_A)) {
			m_data.targetPosition[0] += right.x * 0.1f;
			m_data.targetPosition[1] += right.y * 0.1f;
			m_data.targetPosition[2] += right.z * 0.1f;
			m_data.cameraPosition[0] += right.x * 0.1f;
			m_data.cameraPosition[1] += right.y * 0.1f;
			m_data.cameraPosition[2] += right.z * 0.1f;
		}
		if (ImGui::IsKeyDown(ImGuiKey_D)) {
			m_data.targetPosition[0] -= right.x * 0.1f;
			m_data.targetPosition[1] -= right.y * 0.1f;
			m_data.targetPosition[2] -= right.z * 0.1f;
			m_data.cameraPosition[0] -= right.x * 0.1f;
			m_data.cameraPosition[1] -= right.y * 0.1f;
			m_data.cameraPosition[2] -= right.z * 0.1f;
		}
		if (ImGui::IsKeyDown(ImGuiKey_Q)) {
			m_data.targetPosition[0] += up.x * 0.1f;
			m_data.targetPosition[1] += up.y * 0.1f;
			m_data.targetPosition[2] += up.z * 0.1f;
			m_data.cameraPosition[0] += up.x * 0.1f;
			m_data.cameraPosition[1] += up.y * 0.1f;
			m_data.cameraPosition[2] += up.z * 0.1f;
		}
		if (ImGui::IsKeyDown(ImGuiKey_E)) {
			m_data.targetPosition[0] -= up.x * 0.1f;
			m_data.targetPosition[1] -= up.y * 0.1f;
			m_data.targetPosition[2] -= up.z * 0.1f;
			m_data.cameraPosition[0] -= up.x * 0.1f;
			m_data.cameraPosition[1] -= up.y * 0.1f;
			m_data.cameraPosition[2] -= up.z * 0.1f;
		}
	} else {
		auto direction = CVector(m_data.targetPosition[0] - m_data.cameraPosition[0],
		                         m_data.targetPosition[1] - m_data.cameraPosition[1],
		                         m_data.targetPosition[2] - m_data.cameraPosition[2]);
		direction.Normalise();

		float distance = sqrt(pow(m_data.targetPosition[0] - m_data.cameraPosition[0], 2) +
		                      pow(m_data.targetPosition[1] - m_data.cameraPosition[1], 2) +
		                      pow(m_data.targetPosition[2] - m_data.cameraPosition[2], 2));
		const float rotationSpeed = distance * 0.002f;

		glm::vec3 glmDirection(direction.x, direction.y, direction.z);
		glm::vec3 rotatedDirection = glmDirection;

		glm::vec3 worldUp(0.0f, 0.0f, 1.0f);
		glm::vec3 rightAxis = glm::normalize(glm::cross(glmDirection, worldUp));
		if (glm::length(rightAxis) < 0.001f) {
			rightAxis = glm::vec3(1.0f, 0.0f, 0.0f);
		}
		glm::vec3 currentCameraUp = glm::normalize(glm::cross(rightAxis, glmDirection));

		float upDotProduct = glm::dot(currentCameraUp, lastCameraUp);
		if (upDotProduct < 0.0f) {
			isWSInverted = !isWSInverted;
		}

		float wsRotationMultiplier = isWSInverted ? -1.0f : 1.0f;

		if (ImGui::IsKeyDown(ImGuiKey_W)) {
			glm::vec3 worldUp(0.0f, 0.0f, 1.0f);
			glm::vec3 rightAxis = glm::normalize(glm::cross(glmDirection, worldUp));
			if (glm::length(rightAxis) < 0.001f) {
				rightAxis = glm::vec3(1.0f, 0.0f, 0.0f);
			}

			glm::quat rotation = glm::angleAxis(rotationSpeed * wsRotationMultiplier, rightAxis);
			rotatedDirection = rotation * glmDirection;
		}
		if (ImGui::IsKeyDown(ImGuiKey_S)) {
			glm::vec3 worldUp(0.0f, 0.0f, 1.0f);
			glm::vec3 rightAxis = glm::normalize(glm::cross(glmDirection, worldUp));
			if (glm::length(rightAxis) < 0.001f) {
				rightAxis = glm::vec3(1.0f, 0.0f, 0.0f);
			}

			glm::quat rotation = glm::angleAxis(-rotationSpeed * wsRotationMultiplier, rightAxis);
			rotatedDirection = rotation * glmDirection;
		}
		if (ImGui::IsKeyDown(ImGuiKey_A)) {
			glm::vec3 upAxis(0.0f, 0.0f, 1.0f);
			glm::quat rotation = glm::angleAxis(rotationSpeed, upAxis);
			rotatedDirection = rotation * glmDirection;
		}
		if (ImGui::IsKeyDown(ImGuiKey_D)) {
			glm::vec3 upAxis(0.0f, 0.0f, 1.0f);
			glm::quat rotation = glm::angleAxis(-rotationSpeed, upAxis);
			rotatedDirection = rotation * glmDirection;
		}
		if (ImGui::IsKeyDown(ImGuiKey_Q)) {
			distance += 0.2f;
		}
		if (ImGui::IsKeyDown(ImGuiKey_E)) {
			distance -= 0.2f;
			if (distance < 0.1f) {
				distance = 0.1f;
			}
		}

		rotatedDirection = glm::normalize(rotatedDirection);

		lastCameraUp = currentCameraUp;

		m_data.targetPosition[0] = m_data.cameraPosition[0] + rotatedDirection.x * distance;
		m_data.targetPosition[1] = m_data.cameraPosition[1] + rotatedDirection.y * distance;
		m_data.targetPosition[2] = m_data.cameraPosition[2] + rotatedDirection.z * distance;
	}

	if (options.freeCamera) {
		TheCamera.m_vecMoveLinearPosnEnd =
			CVector(m_data.cameraPosition[0], m_data.cameraPosition[1], m_data.cameraPosition[2]);
		TheCamera.m_vecMoveLinearPosnStart = TheCamera.m_vecMoveLinearPosnEnd;
		TheCamera.m_vecMoveLinear = TheCamera.m_vecMoveLinearPosnEnd;
		playerPed->SetPosn(TheCamera.m_vecMoveLinearPosnEnd);
		m_data.targetPosition[0] = TheCamera.m_vecMoveLinearPosnEnd.x + forward.x;
		m_data.targetPosition[1] = TheCamera.m_vecMoveLinearPosnEnd.y + forward.y;
		m_data.targetPosition[2] = TheCamera.m_vecMoveLinearPosnEnd.z + forward.z;
	} else {
		plugin::Command<plugin::Commands::SET_FIXED_CAMERA_POSITION>(m_data.cameraPosition[0], m_data.cameraPosition[1],
		                                                             m_data.cameraPosition[2], 0.f, 0.f, 0.f);
		plugin::Command<plugin::Commands::POINT_CAMERA_AT_POINT>(m_data.targetPosition[0], m_data.targetPosition[1],
		                                                         m_data.targetPosition[2], 2);
	}
	if (m_object) {
		m_object->SetPosn(CVector(m_data.targetPosition[0], m_data.targetPosition[1], m_data.targetPosition[2]));
		m_object->UpdateRwMatrix();
		m_object->UpdateRwFrame();
	}

	if (ImGui::IsKeyReleased(ImGuiKey_F)) {
		closeCutsceneEditor(false);
	}
	if (ImGui::IsKeyReleased(ImGuiKey_Y)) {
		closeCutsceneEditor(true);
	}

	if (options.grid) {
		auto* backgroundDrawList = ImGui::GetBackgroundDrawList();
		const auto* viewport = ImGui::GetMainViewport();
		const auto& viewportPos = viewport->Pos;
		const auto& viewportSize = viewport->Size;

		constexpr ImU32 gridColor = IM_COL32(255, 255, 255, 100);
		constexpr float thickness = 1.0f;
		constexpr float gridPositions[] = {1.0f / 3.0f, 2.0f / 3.0f};

		for (const float pos : gridPositions) {
			const float x = viewportPos.x + viewportSize.x * pos;
			backgroundDrawList->AddLine(ImVec2(x, viewportPos.y), ImVec2(x, viewportPos.y + viewportSize.y), gridColor,
			                            thickness);
		}

		for (const float pos : gridPositions) {
			const float y = viewportPos.y + viewportSize.y * pos;
			backgroundDrawList->AddLine(ImVec2(viewportPos.x, y), ImVec2(viewportPos.x + viewportSize.x, y), gridColor,
			                            thickness);
		}
	}

	ImGui::End();
}

void CutsceneObjectiveEditing::updateCutsceneObject() noexcept {
	if (options.renderObject && !CutsceneObjectiveEditing::m_object) {
		CStreaming::RequestModel(2114, GAME_REQUIRED);
		CStreaming::LoadAllRequestedModels(false);

		int newObject;
		plugin::Command<plugin::Commands::CREATE_OBJECT_NO_OFFSET>(
			2114, m_data.targetPosition[0], m_data.targetPosition[1], m_data.targetPosition[2], &newObject);
		m_object = CPools::GetObject(newObject);

		CStreaming::SetMissionDoesntRequireModel(2114);
	} else if (!options.renderObject && CutsceneObjectiveEditing::m_object) {
		auto ref = CPools::GetObjectRef(m_object);
		plugin::Command<plugin::Commands::DELETE_OBJECT>(ref);
		m_object = nullptr;
	}
}

void CutsceneObjectiveEditing::openCutsceneEditor(
	objectives::cutscene::Data& data, std::function<void(bool, objectives::cutscene::Data)> onClose) noexcept {
	m_data = data;
	m_onCloseCallback = std::move(onClose);
	playerPed = FindPlayerPed();
	CWorld::Remove(playerPed);

	isWSInverted = false;
	CVector initialDirection(data.targetPosition[0] - data.cameraPosition[0],
	                         data.targetPosition[1] - data.cameraPosition[1],
	                         data.targetPosition[2] - data.cameraPosition[2]);
	initialDirection.Normalise();

	glm::vec3 glmInitialDirection(initialDirection.x, initialDirection.y, initialDirection.z);
	glm::vec3 worldUp(0.0f, 0.0f, 1.0f);
	glm::vec3 initialRightAxis = glm::normalize(glm::cross(glmInitialDirection, worldUp));
	if (glm::length(initialRightAxis) < 0.001f) {
		initialRightAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	lastCameraUp = glm::normalize(glm::cross(initialRightAxis, glmInitialDirection));

	TheCamera.m_bWideScreenOn = data.wideScreen;
	CTheScripts::bDisplayHud = false;
	CHud::bScriptDontDisplayRadar = true;

	plugin::Command<plugin::Commands::SET_FIXED_CAMERA_POSITION>(data.cameraPosition[0], data.cameraPosition[1],
	                                                             data.cameraPosition[2], 0.f, 0.f, 0.f);
	plugin::Command<plugin::Commands::POINT_CAMERA_AT_POINT>(data.targetPosition[0], data.targetPosition[1],
	                                                         data.targetPosition[2], 2);
	updateCutsceneObject();

	WindowManager::instance().addBackgroundRenderCallback("ObjectTransformEditor", render);
}

void CutsceneObjectiveEditing::closeCutsceneEditor(bool saveChanges) noexcept {
	CWorld::Add(playerPed);
	if (m_onCloseCallback) {
		m_onCloseCallback(saveChanges, m_data);
		m_onCloseCallback = nullptr;
	}

	TheCamera.m_bCameraPersistPosition = false;
	TheCamera.m_bWideScreenOn = false;
	CTheScripts::bDisplayHud = true;
	CHud::bScriptDontDisplayRadar = false;
	plugin::Command<plugin::Commands::RESTORE_CAMERA_JUMPCUT>();
	if (m_object) {
		auto ref = CPools::GetObjectRef(m_object);
		plugin::Command<plugin::Commands::DELETE_OBJECT>(ref);
		m_object = nullptr;
	}

	WindowManager::instance().removeBackgroundRenderCallback("ObjectTransformEditor");
}