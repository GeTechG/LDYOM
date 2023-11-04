// ReSharper disable CppMemberFunctionMayBeStatic
#include "EditByPlayerService.h"

#include <CHud.h>
#include <CRadar.h>
#include <CSprite.h>
#include <CStreaming.h>
#include <CTheScripts.h>
#include <CWorld.h>
#include <extensions/ScriptCommands.h>

#include "CCamera.h"
#include "HotKeyService.h"
#include "KeyCodes.h"
#include "LuaEngine.h"
#include "MathUtils.h"
#include "ModelsService.h"
#include "plugin.h"
#include "Tasker.h"
#include "utils.h"
#include "WindowsRenderService.h"
#include "../Data/Actor.h"
#include "../Data/CutsceneObjective.h"
#include "../Data/Vehicle.h"
#include "../Windows/ObjectsWindow.h"
#include "extensions/KeyCheck.h"

class Vehicle;
using namespace std::chrono_literals;
using namespace plugin;

extern bool openWindowsMenu;

ktwait editByPlayerActorTask(Actor &actor) {
	Windows::WindowsRenderService::getInstance().setRenderWindows(false);

	TheCamera.Restore();
	Command<Commands::SET_PLAYER_CONTROL>(0, 1);

	Windows::WindowsRenderService::getInstance().addRender("editByPlayerOverlay", [&] {
		auto &local = Localization::getInstance();
		constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
		if (ImGui::Begin("##playerEditOverlay", nullptr, windowFlags)) {
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 16.5f);
			char acceptHotKey[32];
			ImHotKey::GetHotKeyLib(HotKeyService::getInstance().getHotKeyByName("accept")->functionKeys, acceptHotKey,
			                       sizeof acceptHotKey);
			char cancelHotKey[32];
			ImHotKey::GetHotKeyLib(HotKeyService::getInstance().getHotKeyByName("cancel")->functionKeys, cancelHotKey,
			                       sizeof cancelHotKey);
			ImGui::Text(local.get("info_overlay.accept_cancel").c_str(), acceptHotKey, cancelHotKey);
			ImGui::PopTextWrapPos();
		}
		ImGui::End();
	});

	actor.deleteEditorPed();

	int model;

	if (actor.getModelType() == 0) {
		model = ModelsService::validPedModel(actor.getModelId());
		CStreaming::RequestModel(model, GAME_REQUIRED);
	} else {
		model = 290 + actor.getSlot();
		const auto &specialsPed = ModelsService::getInstance().getSpecialsPed().at(actor.getModelId());
		CStreaming::RequestSpecialChar(actor.getSlot(), specialsPed.c_str(), GAME_REQUIRED);
	}
	CStreaming::LoadAllRequestedModels(false);

	Command<Commands::SET_PLAYER_MODEL>(0, model);

	while (true) {
		const auto hotKey = HotKeyService::getInstance().getHotKey();
		if (hotKey != nullptr && std::strcmp(hotKey->functionName, "accept") == 0) {
			const auto position = FindPlayerPed()->GetPosition();
			actor.getPosition()[0] = position.x;
			actor.getPosition()[1] = position.y;
			actor.getPosition()[2] = position.z;
			actor.getHeadingAngle() = DEG(FindPlayerPed()->GetHeading());

			break;
		}

		if (hotKey != nullptr && std::strcmp(hotKey->functionName, "cancel") == 0) {
			break;
		}

		co_await 1;
	}

	Command<Commands::SET_PLAYER_MODEL>(0, 0);

	actor.spawnEditorPed();

	Windows::WindowsRenderService::getInstance().setRenderWindows(true);

	Windows::WindowsRenderService::getInstance().removeRender("editByPlayerOverlay");

	Tasker::getInstance().removeTask("editByPlayerActor");
}

void EditByPlayerService::editByPlayerActor(Actor &actor) {
	Tasker::getInstance().addTask("editByPlayerActor", editByPlayerActorTask, actor);
}

ktwait editByPlayerVehicleTask(Vehicle &vehicle) {
	Windows::WindowsRenderService::getInstance().setRenderWindows(false);

	Command<Commands::SET_PLAYER_CONTROL>(0, 1);

	Windows::WindowsRenderService::getInstance().addRender("editByPlayerOverlay", [&] {
		auto &local = Localization::getInstance();
		constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
		if (ImGui::Begin("##playerEditOverlay", nullptr, windowFlags)) {
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 16.5f);
			ImGui::Text(local.get("vehicle.edit_by_player_overlay").c_str());
			ImGui::PopTextWrapPos();
		}
		ImGui::End();
	});

	TheCamera.Restore();

	Command<Commands::FREEZE_CAR_POSITION_AND_DONT_LOAD_COLLISION>(
		CPools::GetVehicleRef(vehicle.getEditorVehicle().value()), 0);
	vehicle.getEditorVehicle().value()->m_bUsesCollision = 1;
	vehicle.getEditorVehicle().value()->m_nVehicleFlags.bEngineOn = 1;
	Command<Commands::WARP_CHAR_INTO_CAR>(CPools::GetPedRef(FindPlayerPed()),
	                                      CPools::GetVehicleRef(vehicle.getEditorVehicle().value()));

	while (true) {
		if (!Command<Commands::IS_CHAR_IN_CAR>(CPools::GetPedRef(FindPlayerPed()),
		                                       CPools::GetVehicleRef(vehicle.getEditorVehicle().value()))) {
			Command<Commands::FREEZE_CAR_POSITION_AND_DONT_LOAD_COLLISION>(
				CPools::GetVehicleRef(vehicle.getEditorVehicle().value()), 1);
			vehicle.getEditorVehicle().value()->m_nVehicleFlags.bEngineOn = 0;
			vehicle.getEditorVehicle().value()->m_bUsesCollision = 0;

			const auto position = vehicle.getEditorVehicle().value()->GetPosition();
			vehicle.getPosition()[0] = position.x;
			vehicle.getPosition()[1] = position.y;
			vehicle.getPosition()[2] = position.z;
			vehicle.getHeadingAngle() = DEG(vehicle.getEditorVehicle().value()->GetHeading());

			break;
		}

		co_await 1;
	}

	Windows::WindowsRenderService::getInstance().setRenderWindows(true);

	Windows::WindowsRenderService::getInstance().removeRender("editByPlayerOverlay");

	Command<Commands::SET_PLAYER_CONTROL>(0, 0);

	Tasker::getInstance().removeTask("editByPlayerVehicle");
}

void EditByPlayerService::editByPlayerVehicle(Vehicle &vehicle) {
	Tasker::getInstance().addTask("editByPlayerVehicle", editByPlayerVehicleTask, vehicle);
}

CVector rotate_vector_by_quaternion(const CVector &v, const CQuaternion &q) {
	// Extract the vector part of the quaternion
	const CVector u(q.imag.x, q.imag.y, q.imag.z);

	// Extract the scalar part of the quaternion
	const float s = q.real;

	// Do the math
	const float dotted1 = u.x * v.x + u.y * v.y + u.z * v.z;
	const float dotted2 = u.x * u.x + u.y * u.y + u.z * u.z;
	CVector crossed;
	crossed.Cross(u, v);
	return 2.f * dotted1 * u
		+ (s * s - dotted2) * v
		+ 2.0f * s * crossed;
}

ktwait positionalObjectTask(CEntity *entity, std::function<void(CMatrix &)> setMatrix, float *posO, CQuaternion &quat,
                            bool fastCreate) {
	Windows::WindowsRenderService::getInstance().setRenderWindows(false);

	Windows::WindowsRenderService::getInstance().addRender("editByPlayerOverlay", [&] {
		auto &local = Localization::getInstance();
		constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
		if (ImGui::Begin("##playerEditOverlay", nullptr, windowFlags)) {
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 16.5f);
			ImGui::Text(local.get("info_overlay.view_camera_mouse").c_str());
			ImGui::Text(local.get("info_overlay.move_camera").c_str());
			char toggleSurfaceRotate[32];
			ImHotKey::GetHotKeyLib(HotKeyService::getInstance().getHotKeyByName("guizmoRotate")->functionKeys,
			                       toggleSurfaceRotate, sizeof toggleSurfaceRotate);
			ImGui::Text(local.get("info_overlay.toggle_surface_rotate").c_str(), toggleSurfaceRotate);
			char acceptHotKey[32];
			ImHotKey::GetHotKeyLib(HotKeyService::getInstance().getHotKeyByName("accept")->functionKeys, acceptHotKey,
			                       sizeof acceptHotKey);
			char cancelHotKey[32];
			ImHotKey::GetHotKeyLib(HotKeyService::getInstance().getHotKeyByName("cancel")->functionKeys, cancelHotKey,
			                       sizeof cancelHotKey);
			ImGui::Text(local.get("info_overlay.accept_cancel").c_str(), acceptHotKey, cancelHotKey);
			ImGui::PopTextWrapPos();
		}
		ImGui::End();
	});


	CWorld::Remove(FindPlayerPed());
	Command<Commands::SET_PLAYER_CONTROL>(0, true);

	CVector posCam = TheCamera.m_vecGameCamPos;

	TheCamera.RestoreWithJumpCut();
	TheCamera.m_bCameraPersistPosition = true;
	TheCamera.VectorMoveLinear(&posCam, &posCam, 10, true);

	bool surfaceRotate = false;

	CVector pos;

	while (true) {
		CVector rot = TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vecFront;

		pos = TheCamera.GetPosition() + rot * 1000.f;

		CColPoint colPoint;
		CEntity *cEntity;
		CWorld::pIgnoreEntity = entity;
		CWorld::ProcessLineOfSight(TheCamera.GetPosition(), pos, colPoint, cEntity, true, true, false, true, true, true,
		                           true, true);
		CWorld::pIgnoreEntity = nullptr;

		CMatrix newMatrix;

		CQuaternion normalQuat;
		if (surfaceRotate) {
			auto up = CVector(0.f, 0.f, 1.f);
			normalQuat = MathUtils::lookRotationQuat(colPoint.m_vecNormal, up);
			newMatrix.SetRotate(normalQuat);
		} else {
			CQuaternion q = {{quat.imag.z, quat.imag.x, -quat.imag.y}, quat.real};
			newMatrix.SetRotate(q);
		}

		CVector newPos = colPoint.m_vecPoint;
		if (entity) {
			const auto objSize = entity->GetColModel()->m_boundBox.m_vecMax - entity->GetColModel()->m_boundBox.
				m_vecMin;
			newPos += colPoint.m_vecNormal * (objSize.z / 2.f);
		}

		newMatrix.SetTranslateOnly(newPos.x, newPos.y, newPos.z);
		setMatrix(newMatrix);

		const auto hotKey = HotKeyService::getInstance().getHotKey(true);
		if (hotKey != nullptr && std::strcmp(hotKey->functionName, "cancel") == 0) {
			newMatrix.SetRotate(quat);
			newMatrix.SetTranslateOnly(posO[0], posO[1], posO[1]);
			setMatrix(newMatrix);
			break;
		}

		if (hotKey != nullptr && std::strcmp(hotKey->functionName, "accept") == 0) {
			if (surfaceRotate) {
				quat.real = normalQuat.real;
				quat.imag.z = normalQuat.imag.x;
				quat.imag.x = normalQuat.imag.y;
				quat.imag.y = -normalQuat.imag.z;
			}

			posO[0] = newPos.x;
			posO[1] = newPos.y;
			posO[2] = newPos.z;

			if (fastCreate) {
				openWindowsMenu = true;
				Windows::WindowsRenderService::getInstance().getWindow<Windows::ObjectsWindow>()->selectElement(
					ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjects().size() - 1);
				Windows::WindowsRenderService::getInstance().getWindow<Windows::ObjectsWindow>()->open();
			}

			break;
		}

		CVector rightCamVec;
		rightCamVec.Cross(TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vecFront,
		                  TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vecUp);

		if (KeyPressed(VK_UP) || KeyPressed('W')) {
			posCam += TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vecFront;
			TheCamera.VectorMoveLinear(&posCam, &posCam, 10, true);
		}
		if (KeyPressed(VK_DOWN) || KeyPressed('S')) {
			posCam += TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vecFront * -1;
			TheCamera.VectorMoveLinear(&posCam, &posCam, 10, true);
			FindPlayerPed()->SetPosn(posCam);
		}

		if (KeyPressed(VK_LEFT) || KeyPressed('A')) {
			posCam += rightCamVec * -1;
			TheCamera.VectorMoveLinear(&posCam, &posCam, 10, true);
			FindPlayerPed()->SetPosn(posCam);
		}
		if (KeyPressed(VK_RIGHT) || KeyPressed('D')) {
			posCam += rightCamVec;
			TheCamera.VectorMoveLinear(&posCam, &posCam, 10, true);
			FindPlayerPed()->SetPosn(posCam);
		}

		if (KeyPressed(VK_Q)) {
			posCam += TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vecUp;
			TheCamera.VectorMoveLinear(&posCam, &posCam, 10, true);
			FindPlayerPed()->SetPosn(posCam);
		}
		if (KeyPressed(VK_E)) {
			posCam += TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vecUp * -1;
			TheCamera.VectorMoveLinear(&posCam, &posCam, 10, true);
			FindPlayerPed()->SetPosn(posCam);
		}

		static bool surfaceRotatePressed = false;
		if (hotKey != nullptr && std::strcmp(hotKey->functionName, "guizmoRotate") == 0 && !surfaceRotatePressed) {
			surfaceRotatePressed = true;
			surfaceRotate = !surfaceRotate;
		} else if (hotKey == nullptr) {
			surfaceRotatePressed = false;
		}

		co_await 1;
	}

	TheCamera.m_bCameraPersistPosition = false;
	TheCamera.RestoreWithJumpCut();
	const float z = CWorld::FindGroundZForCoord(posCam.x, posCam.y);
	FindPlayerPed()->SetPosn(posCam.x, posCam.y, z + 1.f);

	CWorld::Add(FindPlayerPed());
	Command<Commands::SET_PLAYER_CONTROL>(0, false);

	Windows::WindowsRenderService::getInstance().removeRender("editByPlayerOverlay");
	Windows::WindowsRenderService::getInstance().setRenderWindows(true);
}

void EditByPlayerService::positionalObject(CEntity *entity, std::function<void(CMatrix &)> setMatrix, float *posO,
                                           CQuaternion &quat, bool fastCreate) {
	Tasker::getInstance().addTask("createFastObject", positionalObjectTask, entity, setMatrix, posO, quat, fastCreate);
}


ktwait editByPlayerCameraTask(float *pos, CQuaternion *rotation, bool widescreen, std::function<void()> callback) {
	Windows::WindowsRenderService::getInstance().setRenderWindows(false);

	static float multiplier = 1.f;

	Command<Commands::SET_PLAYER_CONTROL>(0, 1);

	Windows::WindowsRenderService::getInstance().addRender("editByPlayerOverlay", [&] {
		auto &local = Localization::getInstance();
		constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
		if (ImGui::Begin("##playerEditOverlay", nullptr, windowFlags)) {
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 16.5f);
			ImGui::Text(local.get("info_overlay.view_camera_mouse").c_str());
			ImGui::Text(local.get("info_overlay.move_camera").c_str());
			ImGui::Text(local.get("info_overlay.speed_move").c_str(), multiplier);
			char acceptHotKey[32];
			ImHotKey::GetHotKeyLib(HotKeyService::getInstance().getHotKeyByName("accept")->functionKeys, acceptHotKey,
			                       sizeof acceptHotKey);
			char cancelHotKey[32];
			ImHotKey::GetHotKeyLib(HotKeyService::getInstance().getHotKeyByName("cancel")->functionKeys, cancelHotKey,
			                       sizeof cancelHotKey);
			ImGui::Text(local.get("info_overlay.accept_cancel").c_str(), acceptHotKey, cancelHotKey);
			ImGui::PopTextWrapPos();

			if (ImGui::GetIO().MouseWheel < 0.f) {
				multiplier -= 0.01f;
				multiplier = max(multiplier, 0.f);
			} else if (ImGui::GetIO().MouseWheel > 0.f) {
				multiplier += 0.01f;
			}
		}
		ImGui::End();
	});

	CWorld::Remove(FindPlayerPed());

	CVector posCam = {pos[0], pos[1], pos[2]};

	TheCamera.RestoreWithJumpCut();
	TheCamera.m_bCameraPersistPosition = true;
	TheCamera.VectorMoveLinear(&posCam, &posCam, 10, true);
	if (widescreen)
		TheCamera.SetWideScreenOn();

	CTheScripts::bDisplayHud = false;
	CHud::bScriptDontDisplayRadar = true;

	while (true) {
		const auto hotKey = HotKeyService::getInstance().getHotKey(true);
		if (hotKey != nullptr && std::strcmp(hotKey->functionName, "cancel") == 0) {
			TheCamera.SetPosn(pos[0], pos[1], pos[2]);
			TheCamera.m_mCameraMatrix.SetRotate(*rotation);
			break;
		}

		if (hotKey != nullptr && std::strcmp(hotKey->functionName, "accept") == 0) {
			pos[0] = TheCamera.m_mCameraMatrix.pos.x;
			pos[1] = TheCamera.m_mCameraMatrix.pos.y;
			pos[2] = TheCamera.m_mCameraMatrix.pos.z;

			auto matInverse = TheCamera.m_mMatInverse;
			matInverse.Reorthogonalise();
			*rotation = MathUtils::lookRotationQuat({
				                                        TheCamera.m_mMatInverse.right.y, TheCamera.m_mMatInverse.up.y,
				                                        TheCamera.m_mMatInverse.at.y
			                                        }, {0.f, 0.f, 1.f});

			break;
		}

		CVector rightCamVec;
		rightCamVec.Cross(TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vecFront,
		                  TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vecUp);

		if (KeyPressed(VK_UP) || KeyPressed('W')) {
			posCam += TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vecFront * multiplier;
			TheCamera.VectorMoveLinear(&posCam, &posCam, 10, true);
		}
		if (KeyPressed(VK_DOWN) || KeyPressed('S')) {
			posCam += TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vecFront * -1 * multiplier;
			TheCamera.VectorMoveLinear(&posCam, &posCam, 10, true);
			FindPlayerPed()->SetPosn(posCam);
		}

		if (KeyPressed(VK_LEFT) || KeyPressed('A')) {
			posCam += rightCamVec * -1 * multiplier;
			TheCamera.VectorMoveLinear(&posCam, &posCam, 10, true);
			FindPlayerPed()->SetPosn(posCam);
		}
		if (KeyPressed(VK_RIGHT) || KeyPressed('D')) {
			posCam += rightCamVec * multiplier;
			TheCamera.VectorMoveLinear(&posCam, &posCam, 10, true);
			FindPlayerPed()->SetPosn(posCam);
		}

		if (KeyPressed(VK_Q)) {
			posCam += TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vecUp * multiplier;
			TheCamera.VectorMoveLinear(&posCam, &posCam, 10, true);
			FindPlayerPed()->SetPosn(posCam);
		}
		if (KeyPressed(VK_E)) {
			posCam += TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vecUp * -1 * multiplier;
			TheCamera.VectorMoveLinear(&posCam, &posCam, 10, true);
			FindPlayerPed()->SetPosn(posCam);
		}

		co_await 1;
	}

	TheCamera.m_bCameraPersistPosition = false;
	TheCamera.RestoreWithJumpCut();
	const float z = CWorld::FindGroundZForCoord(posCam.x, posCam.y);
	FindPlayerPed()->SetPosn(posCam.x, posCam.y, z + 1.f);
	if (widescreen)
		TheCamera.SetWideScreenOff();

	CWorld::Add(FindPlayerPed());

	CTheScripts::bDisplayHud = true;
	CHud::bScriptDontDisplayRadar = false;

	Windows::WindowsRenderService::getInstance().removeRender("editByPlayerOverlay");
	Windows::WindowsRenderService::getInstance().setRenderWindows(true);

	callback();
}

void EditByPlayerService::editByPlayerCamera(float *pos, CQuaternion *rotation, bool widescreen,
                                             std::function<void()> callback) {
	Tasker::getInstance().addTask("editByPlayerCamera", editByPlayerCameraTask, pos, rotation, widescreen, callback);
}

ktwait editByPlayerActorPathTask(std::vector<std::array<float, 3>> &path) {
	Windows::WindowsRenderService::getInstance().setRenderWindows(false);

	TheCamera.RestoreWithJumpCut();

	Command<Commands::SET_PLAYER_CONTROL>(0, 1);

	auto newPath = path;

	int currentIndexPoint = 0;

	Windows::WindowsRenderService::getInstance().addRender("editByPlayerOverlay", [&] {
		auto &local = Localization::getInstance();
		constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
		if (ImGui::Begin("##playerEditOverlay", nullptr, windowFlags)) {
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 16.5f);
			ImGui::Text(local.get("info_overlay.delete_all_points").c_str());
			ImGui::Text(local.get("info_overlay.move_point").c_str());
			ImGui::Text(local.get("info_overlay.select_point").c_str());
			ImGui::Text(local.get("info_overlay.add_point").c_str());
			char acceptHotKey[32];
			ImHotKey::GetHotKeyLib(HotKeyService::getInstance().getHotKeyByName("accept")->functionKeys, acceptHotKey,
			                       sizeof acceptHotKey);
			char cancelHotKey[32];
			ImHotKey::GetHotKeyLib(HotKeyService::getInstance().getHotKeyByName("cancel")->functionKeys, cancelHotKey,
			                       sizeof cancelHotKey);
			ImGui::Text(local.get("info_overlay.accept_cancel").c_str(), acceptHotKey, cancelHotKey);
			ImGui::PopTextWrapPos();

			const auto drawList = ImGui::GetBackgroundDrawList();

			if (!newPath.empty()) {
				RwV3d lastPoint;
				float w, h;
				auto lastVisible = CSprite::CalcScreenCoors(RwV3d{newPath.at(0)[0], newPath.at(0)[1], newPath.at(0)[2]},
				                                            &lastPoint, &w, &h, true, true);
				if (lastVisible)
					drawList->AddCircleFilled(ImVec2(lastPoint.x, lastPoint.y), 5.f,
					                          currentIndexPoint == 0 ? IM_COL32(255, 0, 0, 255) : IM_COL32_WHITE);
				for (int i = 1; i < newPath.size(); ++i) {
					RwV3d currentPoint;
					const auto currentVisible = CSprite::CalcScreenCoors(
						RwV3d{newPath.at(i)[0], newPath.at(i)[1], newPath.at(i)[2]}, &currentPoint, &w, &h, true, true);
					if (lastVisible && currentVisible)
						drawList->AddLine(ImVec2(lastPoint.x, lastPoint.y), ImVec2(currentPoint.x, currentPoint.y),
						                  IM_COL32_WHITE);
					if (currentVisible)
						drawList->AddCircleFilled(ImVec2(currentPoint.x, currentPoint.y), 5.f,
						                          currentIndexPoint == i ? IM_COL32(255, 0, 0, 255) : IM_COL32_WHITE);
					lastPoint = currentPoint;
					lastVisible = currentVisible;
				}
			}
		}
		ImGui::End();
	});

	while (true) {
		const auto hotKey = HotKeyService::getInstance().getHotKey(true);
		if (hotKey != nullptr && std::strcmp(hotKey->functionName, "cancel") == 0) {
			break;
		}

		if (hotKey != nullptr && std::strcmp(hotKey->functionName, "accept") == 0) {
			path.swap(newPath);
			break;
		}

		KeyCheck::Update();

		if (KeyCheck::CheckJustDown('L')) {
			newPath.clear();
		}

		if (KeyCheck::CheckJustDown('M')) {
			const auto position = FindPlayerPed()->GetPosition();
			newPath.at(currentIndexPoint) = {position.x, position.y, position.z};
		}

		if (KeyCheck::CheckJustDown('I')) {
			currentIndexPoint++;
			currentIndexPoint = min(currentIndexPoint, newPath.size() - 1);
		}

		if (KeyCheck::CheckJustDown('O')) {
			currentIndexPoint--;
			currentIndexPoint = max(currentIndexPoint, 0);
		}

		if (KeyCheck::CheckJustDown('P')) {
			const auto position = FindPlayerPed()->GetPosition();
			newPath.emplace_back(std::array{position.x, position.y, position.z});
			currentIndexPoint = newPath.size() - 1;
		}

		co_await 1;
	}


	Windows::WindowsRenderService::getInstance().removeRender("editByPlayerOverlay");
	Windows::WindowsRenderService::getInstance().setRenderWindows(true);
}

void EditByPlayerService::editByPlayerActorPath(std::vector<std::array<float, 3>> &path) {
	Tasker::getInstance().addTask("editByPlayerActorPathTask", editByPlayerActorPathTask, path);
}

ktwait editByPlayerVehiclePathTask(std::vector<std::array<float, 3>> &path, int model) {
	Windows::WindowsRenderService::getInstance().setRenderWindows(false);

	TheCamera.RestoreWithJumpCut();

	Command<Commands::SET_PLAYER_CONTROL>(0, 1);

	auto newPath = path;

	int vehicleHandle;
	Command<Commands::CREATE_CAR>(model, FindPlayerCoors(0).x, FindPlayerCoors(0).y, FindPlayerCoors(0).z,
	                              &vehicleHandle);
	Command<Commands::TASK_WARP_CHAR_INTO_CAR_AS_DRIVER>(static_cast<CPed*>(FindPlayerPed()), vehicleHandle);

	co_await 100ms;

	int currentIndexPoint = 0;

	Windows::WindowsRenderService::getInstance().addRender("editByPlayerOverlay", [&] {
		auto &local = Localization::getInstance();
		constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
		if (ImGui::Begin("##playerEditOverlay", nullptr, windowFlags)) {
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 16.5f);
			ImGui::Text(local.get("info_overlay.delete_all_points").c_str());
			ImGui::Text(local.get("info_overlay.move_point").c_str());
			ImGui::Text(local.get("info_overlay.select_point").c_str());
			ImGui::Text(local.get("info_overlay.add_point").c_str());
			ImGui::Text(local.get("vehicle.edit_by_player_overlay").c_str());
			ImGui::PopTextWrapPos();

			const auto drawList = ImGui::GetBackgroundDrawList();

			if (!newPath.empty()) {
				RwV3d lastPoint;
				float w, h;
				auto lastVisible = CSprite::CalcScreenCoors(RwV3d{newPath.at(0)[0], newPath.at(0)[1], newPath.at(0)[2]},
				                                            &lastPoint, &w, &h, true, true);
				if (lastVisible)
					drawList->AddCircleFilled(ImVec2(lastPoint.x, lastPoint.y), 5.f,
					                          currentIndexPoint == 0 ? IM_COL32(255, 0, 0, 255) : IM_COL32_WHITE);
				for (int i = 1; i < newPath.size(); ++i) {
					RwV3d currentPoint;
					const auto currentVisible = CSprite::CalcScreenCoors(
						RwV3d{newPath.at(i)[0], newPath.at(i)[1], newPath.at(i)[2]}, &currentPoint, &w, &h, true, true);
					if (lastVisible && currentVisible)
						drawList->AddLine(ImVec2(lastPoint.x, lastPoint.y), ImVec2(currentPoint.x, currentPoint.y),
						                  IM_COL32_WHITE);
					if (currentVisible)
						drawList->AddCircleFilled(ImVec2(currentPoint.x, currentPoint.y), 5.f,
						                          currentIndexPoint == i ? IM_COL32(255, 0, 0, 255) : IM_COL32_WHITE);
					lastPoint = currentPoint;
					lastVisible = currentVisible;
				}
			}
		}
		ImGui::End();
	});

	while (Command<Commands::IS_CHAR_IN_CAR>(static_cast<CPed*>(FindPlayerPed()), vehicleHandle)) {
		KeyCheck::Update();

		if (KeyCheck::CheckJustDown('L')) {
			newPath.clear();
		}

		if (KeyCheck::CheckJustDown('M')) {
			const auto position = FindPlayerVehicle(0, false)->GetPosition();
			newPath.at(currentIndexPoint) = {position.x, position.y, position.z};
		}

		if (KeyCheck::CheckJustDown('I')) {
			currentIndexPoint++;
			currentIndexPoint = min(currentIndexPoint, newPath.size() - 1);
		}

		if (KeyCheck::CheckJustDown('O')) {
			currentIndexPoint--;
			currentIndexPoint = max(currentIndexPoint, 0);
		}

		if (KeyCheck::CheckJustDown('P')) {
			const auto position = FindPlayerVehicle(0, false)->GetPosition();
			newPath.emplace_back(std::array{position.x, position.y, position.z});
			currentIndexPoint = newPath.size() - 1;
		}

		co_await 1;
	}

	path.swap(newPath);

	Command<Commands::DELETE_CAR>(vehicleHandle);

	Windows::WindowsRenderService::getInstance().removeRender("editByPlayerOverlay");
	Windows::WindowsRenderService::getInstance().setRenderWindows(true);
}

void EditByPlayerService::editByPlayerVehiclePath(std::vector<std::array<float, 3>> &path, int model) {
	Tasker::getInstance().addTask("editByPlayerVehiclePathTask", editByPlayerVehiclePathTask, path, model);
}

ktwait editByPlayerActorPathLuaTask(sol::table path) {
	Windows::WindowsRenderService::getInstance().setRenderWindows(false);

	TheCamera.RestoreWithJumpCut();

	Command<Commands::SET_PLAYER_CONTROL>(0, 1);

	auto &luaState = LuaEngine::getInstance().getLuaState();
	auto newPath = luaState["bitser"]["loads"](luaState["bitser"]["dumps"](path)).get<sol::table>();

	int currentIndexPoint = 1;

	Windows::WindowsRenderService::getInstance().addRender("editByPlayerOverlay", [&] {
		auto &local = Localization::getInstance();
		constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
		if (ImGui::Begin("##playerEditOverlay", nullptr, windowFlags)) {
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 16.5f);
			ImGui::Text(local.get("info_overlay.delete_all_points").c_str());
			ImGui::Text(local.get("info_overlay.move_point").c_str());
			ImGui::Text(local.get("info_overlay.select_point").c_str());
			ImGui::Text(local.get("info_overlay.add_point").c_str());
			char acceptHotKey[32];
			ImHotKey::GetHotKeyLib(HotKeyService::getInstance().getHotKeyByName("accept")->functionKeys, acceptHotKey,
			                       sizeof acceptHotKey);
			char cancelHotKey[32];
			ImHotKey::GetHotKeyLib(HotKeyService::getInstance().getHotKeyByName("cancel")->functionKeys, cancelHotKey,
			                       sizeof cancelHotKey);
			ImGui::Text(local.get("info_overlay.accept_cancel").c_str(), acceptHotKey, cancelHotKey);
			ImGui::PopTextWrapPos();

			const auto drawList = ImGui::GetBackgroundDrawList();

			if (!newPath.empty()) {
				RwV3d lastPoint;
				float w, h;
				const auto pointer = static_cast<float*>(const_cast<void*>(newPath[1].get<sol::object>().pointer()));
				auto lastVisible = CSprite::CalcScreenCoors(RwV3d{pointer[0], pointer[1], pointer[2]}, &lastPoint, &w,
				                                            &h, true, true);
				if (lastVisible)
					drawList->AddCircleFilled(ImVec2(lastPoint.x, lastPoint.y), 5.f,
					                          currentIndexPoint == 1 ? IM_COL32(255, 0, 0, 255) : IM_COL32_WHITE);
				for (int i = 2; i <= newPath.size(); ++i) {
					RwV3d currentPoint;
					const auto currentPointer = static_cast<float*>(const_cast<void*>(newPath[i].get<sol::object>().
						pointer()));
					const auto currentVisible = CSprite::CalcScreenCoors(
						RwV3d{currentPointer[0], currentPointer[1], currentPointer[2]}, &currentPoint, &w, &h, true,
						true);
					if (lastVisible && currentVisible)
						drawList->AddLine(ImVec2(lastPoint.x, lastPoint.y), ImVec2(currentPoint.x, currentPoint.y),
						                  IM_COL32_WHITE);
					if (currentVisible)
						drawList->AddCircleFilled(ImVec2(currentPoint.x, currentPoint.y), 5.f,
						                          currentIndexPoint == i ? IM_COL32(255, 0, 0, 255) : IM_COL32_WHITE);
					lastPoint = currentPoint;
					lastVisible = currentVisible;
				}
			}
		}
		ImGui::End();
	});

	while (true) {
		const auto hotKey = HotKeyService::getInstance().getHotKey(true);
		if (hotKey != nullptr && std::strcmp(hotKey->functionName, "cancel") == 0) {
			break;
		}

		if (hotKey != nullptr && std::strcmp(hotKey->functionName, "accept") == 0) {
			path.clear();
			for (auto pair : newPath) {
				path.set(pair.first, pair.second);
			}
			break;
		}

		KeyCheck::Update();

		if (KeyCheck::CheckJustDown('L')) {
			newPath.clear();
		}

		if (KeyCheck::CheckJustDown('M')) {
			const auto position = FindPlayerPed()->GetPosition();
			const auto curr = static_cast<float*>(const_cast<void*>(newPath[currentIndexPoint].get<sol::object>().
				pointer()));
			curr[0] = position.x;
			curr[1] = position.y;
			curr[2] = position.z;
		}

		if (KeyCheck::CheckJustDown('I')) {
			currentIndexPoint++;
			currentIndexPoint = min(currentIndexPoint, newPath.size());
		}

		if (KeyCheck::CheckJustDown('O')) {
			currentIndexPoint--;
			currentIndexPoint = max(currentIndexPoint, 1);
		}

		if (KeyCheck::CheckJustDown('P')) {
			const auto position = FindPlayerPed()->GetPosition();
			newPath.add(LuaEngine::getInstance().getLuaState()
			                                    .do_string("return ffi.new(\"float[3]\", {0})").get<sol::object>());
			auto newPosObj = newPath[newPath.size()].get<sol::object>();
			auto newPos = static_cast<float*>(const_cast<void*>(newPosObj.pointer()));
			newPos[0] = position.x;
			newPos[1] = position.y;
			newPos[2] = position.z;
			currentIndexPoint = newPath.size();
		}

		co_await 1;
	}

	Windows::WindowsRenderService::getInstance().removeRender("editByPlayerOverlay");
	Windows::WindowsRenderService::getInstance().setRenderWindows(true);
}

void EditByPlayerService::editByPlayerActorLuaPath(sol::table path) {
	Tasker::getInstance().addTask("editByPlayerActorPathLuaTask", editByPlayerActorPathLuaTask, path);
}

ktwait editByPlayerVehiclePathLuaTask(sol::table path, int model) {
	Windows::WindowsRenderService::getInstance().setRenderWindows(false);

	TheCamera.RestoreWithJumpCut();

	Command<Commands::SET_PLAYER_CONTROL>(0, 1);

	auto &luaState = LuaEngine::getInstance().getLuaState();
	auto newPath = luaState["bitser"]["loads"](luaState["bitser"]["dumps"](path)).get<sol::table>();

	int currentIndexPoint = 1;

	int vehicleHandle;
	Command<Commands::CREATE_CAR>(model, FindPlayerCoors(0).x, FindPlayerCoors(0).y, FindPlayerCoors(0).z,
	                              &vehicleHandle);
	Command<Commands::TASK_WARP_CHAR_INTO_CAR_AS_DRIVER>(static_cast<CPed*>(FindPlayerPed()), vehicleHandle);

	co_await 100ms;

	Windows::WindowsRenderService::getInstance().addRender("editByPlayerOverlay", [&] {
		auto &local = Localization::getInstance();
		constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
		if (ImGui::Begin("##playerEditOverlay", nullptr, windowFlags)) {
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 16.5f);
			ImGui::Text(local.get("info_overlay.delete_all_points").c_str());
			ImGui::Text(local.get("info_overlay.move_point").c_str());
			ImGui::Text(local.get("info_overlay.select_point").c_str());
			ImGui::Text(local.get("info_overlay.add_point").c_str());
			ImGui::Text(local.get("vehicle.edit_by_player_overlay").c_str());
			ImGui::PopTextWrapPos();

			const auto drawList = ImGui::GetBackgroundDrawList();

			if (!newPath.empty()) {
				RwV3d lastPoint;
				float w, h;
				const auto pointer = static_cast<float*>(const_cast<void*>(newPath[1].get<sol::object>().pointer()));
				auto lastVisible = CSprite::CalcScreenCoors(RwV3d{pointer[0], pointer[1], pointer[2]}, &lastPoint, &w,
				                                            &h, true, true);
				if (lastVisible)
					drawList->AddCircleFilled(ImVec2(lastPoint.x, lastPoint.y), 5.f,
					                          currentIndexPoint == 1 ? IM_COL32(255, 0, 0, 255) : IM_COL32_WHITE);
				for (int i = 2; i <= newPath.size(); ++i) {
					RwV3d currentPoint;
					const auto currentPointer = static_cast<float*>(const_cast<void*>(newPath[i].get<sol::object>().
						pointer()));
					const auto currentVisible = CSprite::CalcScreenCoors(
						RwV3d{currentPointer[0], currentPointer[1], currentPointer[2]}, &currentPoint, &w, &h, true,
						true);
					if (lastVisible && currentVisible)
						drawList->AddLine(ImVec2(lastPoint.x, lastPoint.y), ImVec2(currentPoint.x, currentPoint.y),
						                  IM_COL32_WHITE);
					if (currentVisible)
						drawList->AddCircleFilled(ImVec2(currentPoint.x, currentPoint.y), 5.f,
						                          currentIndexPoint == i ? IM_COL32(255, 0, 0, 255) : IM_COL32_WHITE);
					lastPoint = currentPoint;
					lastVisible = currentVisible;
				}
			}
		}
		ImGui::End();
	});

	while (Command<Commands::IS_CHAR_IN_CAR>(static_cast<CPed*>(FindPlayerPed()), vehicleHandle)) {
		KeyCheck::Update();

		if (KeyCheck::CheckJustDown('L')) {
			newPath.clear();
		}

		if (KeyCheck::CheckJustDown('M')) {
			const auto position = FindPlayerVehicle(0, false)->GetPosition();
			const auto curr = static_cast<float*>(const_cast<void*>(newPath[currentIndexPoint].get<sol::object>().
				pointer()));
			curr[0] = position.x;
			curr[1] = position.y;
			curr[2] = position.z;
		}

		if (KeyCheck::CheckJustDown('I')) {
			currentIndexPoint++;
			currentIndexPoint = min(currentIndexPoint, newPath.size());
		}

		if (KeyCheck::CheckJustDown('O')) {
			currentIndexPoint--;
			currentIndexPoint = max(currentIndexPoint, 1);
		}

		if (KeyCheck::CheckJustDown('P')) {
			const auto position = FindPlayerVehicle(0, false)->GetPosition();
			newPath.add(LuaEngine::getInstance().getLuaState()
			                                    .do_string("return ffi.new(\"float[3]\", {0})").get<sol::object>());
			auto newPosObj = newPath[newPath.size()].get<sol::object>();
			auto newPos = static_cast<float*>(const_cast<void*>(newPosObj.pointer()));
			newPos[0] = position.x;
			newPos[1] = position.y;
			newPos[2] = position.z;
			currentIndexPoint = newPath.size();
		}

		co_await 1;
	}

	path.clear();
	for (auto pair : newPath) {
		path.set(pair.first, pair.second);
	}

	Command<Commands::DELETE_CAR>(vehicleHandle);

	Windows::WindowsRenderService::getInstance().removeRender("editByPlayerOverlay");
	Windows::WindowsRenderService::getInstance().setRenderWindows(true);
}

void EditByPlayerService::editByPlayerVehicleLuaPath(sol::table path, int model) {
	Tasker::getInstance().addTask("editByPlayerVehicleLuaPathTask", editByPlayerVehiclePathLuaTask, path, model);
}
