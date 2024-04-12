#include "CameraPathsWindow.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <CCamera.h>
#include <CSprite.h>
#include <CWorld.h>
#include <tweeny.h>
#include <extensions/ScriptCommands.h>
#include "EditByPlayerService.h"
#include "fa.h"
#include "imgui_internal.h"
#include "Settings.h"
#include "utilsRender.h"
#include "../Data/CCameraExtend.h"
#include "../Data/CMatrixUtils.h"
#include "fmt/core.h"
#include "Localization/Localization.h"

using namespace plugin;

std::string Windows::CameraPathsWindow::getNameList() {
	return fmt::format("{} {}", ICON_FA_CAMERA_MOVIE, Localization::getInstance().get("entities.camera_paths"));
}

std::string Windows::CameraPathsWindow::getNameOption() {
	return fmt::format("{} {}", ICON_FA_CAMERA_MOVIE, Localization::getInstance().get("entities.camera_path"));
}

int Windows::CameraPathsWindow::getListSize() {
	return static_cast<int>(ProjectsService::getInstance()
	                        .getCurrentProject()
	                        .getCurrentScene()
	                        ->getCameraPaths()
	                        .size());
}

void Windows::CameraPathsWindow::createNewElement() {
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->createNewCameraPath();
}

void Windows::CameraPathsWindow::createNewElementFrom(int i) {
	const auto currentScene = ProjectsService::getInstance().getCurrentProject().getCurrentScene();
	const auto &cameraPath = currentScene->getCameraPaths().at(i);
	currentScene->createNewCameraPathFrom(*cameraPath);
	currentScene->getObjects().back()->spawnEditorObject();
}

std::string& Windows::CameraPathsWindow::getElementName(int i) {
	return ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getCameraPaths().at(i)->getName();
}

void Windows::CameraPathsWindow::deleteElement(int i) {
	const auto begin = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getCameraPaths().begin();
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getCameraPaths().erase(begin + i);
	this->currentElement--;
}

enum EditorMouseMode {
	NONE,
	HOVER,
	DRAG,
};

extern float speedCameraMultiplier;

void Windows::CameraPathsWindow::drawOptions() {
	auto &local = Localization::getInstance();
	const auto fontScale = ImGui::GetFontSize() / 16.0f;

	static auto mouseMode = NONE;
	static auto currentMousePoint = -1;
	static auto previewCurrentPoint = false;

	auto angles = degrees(eulerAngles(quat_cast(CMatrixToGlmMat4(TheCameraExtend.matrix))));

	const auto &cameraPath = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getCameraPaths().at(
		currentElement);
	auto &positionCurve = cameraPath->getPositionCurve();
	bool recalcKnots = false;

	if (ImGui::DragFloat3(local.get("general.angle_rotation").c_str(), reinterpret_cast<float*>(&angles), 0.1, -180.f,
	                      180.f, "%.3f deg")) {
		glm::quat quat(radians(angles));
		TheCameraExtend.matrix.SetRotate({{quat.x, quat.y, quat.z}, quat.w});
		if (previewCurrentPoint) {
			cameraPath->getRotationsCurve()[currentPoint] = {{quat.x, quat.y, quat.z}, quat.w};
		}
	}

	ImGui::DragFloat3(local.get("general.position").c_str(), reinterpret_cast<float*>(&TheCameraExtend.matrix.pos),
	                  0.1f);

	static auto minDegree = 1;
	static auto maxDegree = 10;
	if (ImGui::DragScalar(local.get("camera_paths.degree").c_str(), ImGuiDataType_U32, &positionCurve.degree, 0.5,
	                      &minDegree, &maxDegree)) {
		recalcKnots = true;
	}

	auto editControlPointFunc = [&](int i) {
		if (ImGui::MenuItem(local.get("general.delete").c_str())) {
			positionCurve.control_points.erase(positionCurve.control_points.begin() + i);
			positionCurve.weights.erase(positionCurve.weights.begin() + i);
			cameraPath->getRotationsCurve().erase(cameraPath->getRotationsCurve().begin() + i);
			cameraPath->getRotationsEasing().erase(cameraPath->getRotationsEasing().begin() + i);
			recalcKnots = true;
			currentPoint = -1;
		}
		const bool isLastPoint = i == positionCurve.control_points.size() - 1;
		if (ImGui::MenuItem(local.get("general.duplicate").c_str(), nullptr, false, !isLastPoint)) {
			const auto point = positionCurve.control_points[i];
			const auto nextPoint = positionCurve.control_points[i + 1];
			positionCurve.control_points.insert(positionCurve.control_points.begin() + i + 1,
			                                    (point + nextPoint) / 2.f);
			positionCurve.weights.insert(positionCurve.weights.begin() + i + 1,
			                             (positionCurve.weights[i] + positionCurve.weights[i + 1]) / 2.f);
			CQuaternion newQuat;
			newQuat.Slerp(cameraPath->getRotationsCurve()[i], cameraPath->getRotationsCurve()[i + 1], 0.5f);
			cameraPath->getRotationsCurve().insert(cameraPath->getRotationsCurve().begin() + i + 1,
			                                       newQuat);
			cameraPath->getRotationsEasing().insert(cameraPath->getRotationsEasing().begin() + i + 1,
			                                        cameraPath->getRotationsEasing()[i]);
			recalcKnots = true;
		}
	};

	ImGui::DragFloat(local.get("general.time").c_str(), &cameraPath->getTime(), 0.001f);


	ImGui::Text(local.get("camera_paths.control_points").c_str());
	if (ImGui::BeginListBox("##controlPoints", ImVec2(fontScale * 300, 0))) {
		for (int i = 0; i < positionCurve.control_points.size(); ++i) {
			if (currentMousePoint == i) {
				ImGui::PushStyleColor(ImGuiCol_Header, ImGui::GetStyleColorVec4(ImGuiCol_HeaderHovered));
			}

			auto &point = positionCurve.control_points[i];
			auto name = fmt::format("{}, {}, {}", point.x, point.y, point.z);
			if (ImGui::Selectable(name.c_str(), currentPoint == i || currentMousePoint == i)) {
				currentPoint = i;
			}

			if (currentMousePoint == i) {
				ImGui::PopStyleColor();
			}

			if (ImGui::BeginPopupContextItem(fmt::format("##controlPoint{}", i).c_str())) {
				editControlPointFunc(i);
				ImGui::EndPopup();
			}
		}

		ImGui::EndListBox();
	}

	if (ImGui::Button(local.get("camera_paths.add_control_point").c_str())) {
		positionCurve.control_points.emplace_back(reinterpret_cast<glm::vec3&>(TheCameraExtend.matrix.pos));
		positionCurve.weights.emplace_back(1.f);
		auto q = quat_cast(CMatrixToGlmMat4(TheCameraExtend.matrix));
		cameraPath->getRotationsCurve().push_back(CQuaternion{{q.x, q.y, q.z}, q.w});
		cameraPath->getRotationsEasing().push_back(17);
		recalcKnots = true;
	}

	ImGui::SameLine();

	ImGui::BeginDisabled(currentPoint == -1);
	if (ImGui::Button(local.get("camera_paths.update_control_point").c_str())) {
		positionCurve.control_points[currentPoint] = reinterpret_cast<glm::vec3&>(TheCameraExtend.matrix.pos);
		auto q = quat_cast(CMatrixToGlmMat4(TheCameraExtend.matrix));
		cameraPath->getRotationsCurve()[currentPoint] = {{q.x, q.y, q.z}, q.w};
		recalcKnots = true;
	}
	ImGui::EndDisabled();

	utils::ToggleButton(local.get("camera_paths.catmull_rom_rotations").c_str(),
	                    &cameraPath->isCatmullRomRotations());

	if (currentPoint != -1) {
		ImGui::DragFloat(local.get("camera_paths.weight").c_str(), &positionCurve.weights[currentPoint], 0.01f, 0, 0);
		ImGui::BeginDisabled(cameraPath->isCatmullRomRotations());
		EasingCombo(local.get("camera_paths.rotation_easing").c_str(), &cameraPath->getRotationsEasing()[currentPoint]);
		ImGui::EndDisabled();
	}


	/////////////////////////////////////////

	/*std::string knotsValues;
	for (auto knot : positionCurve.knots) {
		knotsValues += fmt::format("{:.2f} ", knot);
	}
	ImGui::Text(knotsValues.c_str());*/

	utils::ToggleButton(local.get("camera_paths.custom_control_knots").c_str(),
	                    &cameraPath->isCustomControlKnots());

	if (cameraPath->isCustomControlKnots()) {
		ImGui::BeginChild("##knots", ImVec2(0, 100 * fontScale), ImGuiChildFlags_Border);
		for (int i = 0; i < positionCurve.knots.size(); ++i) {
			if (ImGui::DragFloat(fmt::format("##knots{}", i).c_str(), &positionCurve.knots[i], 0.01f, 0.f, 1.f)) {
				for (int j = 0; j < i; ++j) {
					if (positionCurve.knots[j] > positionCurve.knots[i]) {
						positionCurve.knots[j] = positionCurve.knots[i];
					}
				}
				for (int j = i + 1; j < positionCurve.knots.size(); ++j) {
					if (positionCurve.knots[j] < positionCurve.knots[i]) {
						positionCurve.knots[j] = positionCurve.knots[i];
					}
				}
			}
		}
		ImGui::EndChild();
	}

	utils::ToggleButton(local.get("camera_paths.preview_current_point").c_str(), &previewCurrentPoint);

	if (ImGui::Button(local.get("camera_paths.play").c_str())) {
		TheCameraExtend.playCameraPath(cameraPath.get());
	}


	if (mouseMode == HOVER) {
		currentMousePoint = -1;
		mouseMode = NONE;
	}

	// draw curve control points
	for (int i = 0; i < positionCurve.control_points.size(); ++i) {
		auto &point = positionCurve.control_points[i];
		ImVec2 screenPos;
		if (utils::getScreenPositionFromGamePosition(CVector(point.x, point.y, point.z), screenPos)) {
			if (i == currentPoint) {
				ImGui::GetBackgroundDrawList()->AddCircleFilled(screenPos, 5.f, IM_COL32(255, 255, 0, 255));
			} else {
				ImGui::GetBackgroundDrawList()->AddCircleFilled(screenPos, 5.f, IM_COL32(255, 255, 255, 255));
			}
			//hover
			if (mouseMode == NONE &&
				ImGui::IsMouseHoveringRect(screenPos - ImVec2(5, 5), screenPos + ImVec2(5, 5), false)) {
				currentMousePoint = i;
				mouseMode = HOVER;

				ImGui::BeginTooltip();
				ImGui::Text("%d", i);
				ImGui::EndTooltip();
			}

			if (i > 0) {
				const auto lastPoint = positionCurve.control_points[i - 1];
				ImVec2 lastScreenPos;
				if (utils::getScreenPositionFromGamePosition(CVector(lastPoint.x, lastPoint.y, lastPoint.z),
				                                             lastScreenPos)) {
					ImGui::GetBackgroundDrawList()->
						AddLine(screenPos, lastScreenPos, IM_COL32(255, 255, 255, 100), 1.f);
				}
			}
		}
	}

	switch (mouseMode) {
		case HOVER: {
			if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
				currentPoint = currentMousePoint;
				mouseMode = DRAG;
			} else if (ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
				currentPoint = currentMousePoint;
				ImGui::OpenPopup("##controlPointMenu");
			}
			break;
		}
		case DRAG: {
			if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
				mouseMode = NONE;
			} else if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
				auto &point = positionCurve.control_points[currentPoint];
				RwV3d outB;
				float w, h;
				CSprite::CalcScreenCoors({point.x, point.y, point.z}, &outB, &w, &h, true, true);
				auto &mouse = ImGui::GetIO().MousePos;
				RwV3d screen = {mouse.x, mouse.y, outB.z};
				RwV3d world;
				CalcWorldCoors(&screen, &world);
				point = glm::vec3(world.x, world.y, world.z);
			}
			break;
		}
	}

	if (ImGui::BeginPopup("##controlPointMenu")) {
		editControlPointFunc(currentPoint);
		ImGui::EndPopup();
	}

	unsigned allSteps = positionCurve.control_points.size() * 10;
	float step = 1.f / static_cast<float>(allSteps - 1);
	//draw curve
	if (cameraPath->isValid()) {
		float minDistance = FLT_MAX;
		float maxDistance = 0;
		for (const auto &controlPoint : positionCurve.control_points) {
			const auto distance = glm::distance(controlPoint, reinterpret_cast<glm::vec3&>(TheCameraExtend.matrix.pos));
			minDistance = std::min(minDistance, distance);
			maxDistance = std::max(maxDistance, distance);
		}
		for (unsigned i = 1; i < allSteps; i++) {
			const auto t = static_cast<float>(i) * step;
			const auto point = curvePoint(positionCurve, t);
			if (i > 0) {
				const auto lastPoint = curvePoint(positionCurve, t - step);
				ImVec2 p1, p2;
				auto isVisible =
					utils::getScreenPositionFromGamePosition(CVector(lastPoint.x, lastPoint.y, lastPoint.z), p1) &&
					utils::getScreenPositionFromGamePosition(CVector(point.x, point.y, point.z), p2);
				if (isVisible) {
					auto distance = DistanceBetweenPoints(CVector(point.x, point.y, point.z),
					                                      TheCameraExtend.matrix.pos);
					auto depth = 1 - (distance - minDistance) / (maxDistance - minDistance) / 2;
					ImGui::GetBackgroundDrawList()->AddLine(
						p1, p2, ImColor(depth, depth, depth, 1.f), 2.f);
				}
			}
		}
	} else {
		ImGui::PushTextWrapPos(fontScale * 300.f);
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f),
		                   fmt::format("{} {}", ICON_FA_EXCLAMATION_TRIANGLE,
		                               local.get("camera_paths.invalid_curve_warning")).c_str());
		ImGui::PopTextWrapPos();
	}

	if (const bool isWindow = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) || ImGui::IsAnyItemHovered();
		!isWindow) {
		const float mouseSensitive = CCamera::m_fMouseAccelHorzntal * 1000;
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
			const ImVec2 dt = ImGui::GetIO().MouseDelta * mouseSensitive * 0.2f;
			auto eular = eulerAngles(quat_cast(CMatrixToGlmMat4(TheCameraExtend.matrix)));
			eular.y += RAD(dt.y);
			eular.z += RAD(-dt.x);
			auto quat = glm::quat(eular);
			TheCameraExtend.matrix.SetRotate({{quat.x, quat.y, quat.z}, quat.w});
			if (previewCurrentPoint) {
				cameraPath->getRotationsCurve()[currentPoint] = {{quat.x, quat.y, quat.z}, quat.w};
			}
		}
		if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
			if (ImGui::IsKeyDown(ImGuiKey_W)) {
				TheCameraExtend.matrix.pos += TheCameraExtend.matrix.right * speedCameraMultiplier;
			} else if (ImGui::IsKeyDown(ImGuiKey_S)) {
				TheCameraExtend.matrix.pos += TheCameraExtend.matrix.right * speedCameraMultiplier * -1;
			}

			if (ImGui::IsKeyDown(ImGuiKey_A)) {
				TheCameraExtend.matrix.pos += TheCameraExtend.matrix.up * speedCameraMultiplier;
			} else if (ImGui::IsKeyDown(ImGuiKey_D)) {
				TheCameraExtend.matrix.pos += TheCameraExtend.matrix.up * speedCameraMultiplier * -1;
			}

			if (ImGui::IsKeyDown(ImGuiKey_Q)) {
				TheCameraExtend.matrix.pos += TheCameraExtend.matrix.at * speedCameraMultiplier;
			} else if (ImGui::IsKeyDown(ImGuiKey_E)) {
				TheCameraExtend.matrix.pos += TheCameraExtend.matrix.at * speedCameraMultiplier * -1;
			}

			if (ImGui::GetIO().MouseWheel < 0.f) {
				speedCameraMultiplier -= 0.01f;
				speedCameraMultiplier = std::max(speedCameraMultiplier, 0.f);
				Settings::getInstance().set("camera.editByPlayerSpeed", speedCameraMultiplier);
				Settings::getInstance().Save();
			} else if (ImGui::GetIO().MouseWheel > 0.f) {
				speedCameraMultiplier += 0.01f;
				Settings::getInstance().set("camera.editByPlayerSpeed", speedCameraMultiplier);
				Settings::getInstance().Save();
			}
		}
	}

	if (previewCurrentPoint && currentPoint != -1 && cameraPath->isValid() && !TheCameraExtend.isPlayingPath()) {
		unsigned allSteps = positionCurve.control_points.size();
		float step = 1.f / static_cast<float>(allSteps - 1);
		auto point = curvePoint(positionCurve, step * static_cast<float>(currentPoint));
		TheCameraExtend.matrix.pos = {point.x, point.y, point.z};
		auto q = cameraPath->getRotationsCurve()[currentPoint];
		TheCameraExtend.matrix.SetRotate(q);
	}

	if (recalcKnots) {
		positionCurve.knots.clear();
		const unsigned knotsSize = positionCurve.control_points.size() + positionCurve.degree + 1;
		positionCurve.knots.resize(knotsSize);
		for (unsigned i = 0; i < knotsSize; ++i) {
			if (i <= positionCurve.degree) {
				positionCurve.knots[i] = 0.f;
			} else if (i >= knotsSize - positionCurve.degree - 1) {
				positionCurve.knots[i] = 1.f;
			} else {
				const auto step = 1.f / static_cast<float>(knotsSize - (positionCurve.degree + 1) * 2 + 1);
				positionCurve.knots[i] = static_cast<float>(i - positionCurve.degree) * step;
			}
		}
	}

	std::array<char, 500> speedMove;
	ImFormatString(speedMove.data(), sizeof speedMove, local.get("camera_paths.control").c_str(),
	               speedCameraMultiplier);
	this->listOverlays.emplace_back(speedMove.data());
}

void Windows::CameraPathsWindow::close() {
	ListWindow::close();
	TheCameraExtend.setExtendMode(false);
	Command<Commands::FREEZE_CHAR_POSITION_AND_DONT_LOAD_COLLISION>(FindPlayerPed(), false);
	CWorld::Add(FindPlayerPed());
	TheCamera.Restore();
}

void Windows::CameraPathsWindow::open() {
	ListWindow::open();
	speedCameraMultiplier = Settings::getInstance().get<float>("camera.editByPlayerSpeed").value_or(1.f);
	TheCameraExtend.setExtendMode(true);
	Command<Commands::FREEZE_CHAR_POSITION_AND_DONT_LOAD_COLLISION>(FindPlayerPed(), true);
	CWorld::Remove(FindPlayerPed());
}
