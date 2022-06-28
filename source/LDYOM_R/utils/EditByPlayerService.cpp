// ReSharper disable CppMemberFunctionMayBeStatic
#include "EditByPlayerService.h"

#include <CStreaming.h>
#include <CWorld.h>
#include <extensions/ScriptCommands.h>

#include "ModelsService.h"
#include "Tasker.h"
#include "WindowsRenderService.h"
#include "plugin.h"
#include "../Data/Actor.h"
#include "KeyCodes.h"
#include "utils.h"
#include "CCamera.h"
#include "HotKeyService.h"
#include "../Data/Vehicle.h"
#include "extensions/KeyCheck.h"
#include "../easylogging/easylogging++.h"
#include "../Windows/ObjectsWindow.h"

class Vehicle;
using namespace std::chrono_literals;
using namespace plugin;

extern bool openWindowsMenu;

ktwait editByPlayerActorTask(Actor& actor) {
	Windows::WindowsRenderService::getInstance().setRenderWindows(false);

	TheCamera.Restore();

	Windows::WindowsRenderService::getInstance().addRender("editByPlayerOverlay", [&] {
		auto& local = Localization::getInstance();
		constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
		if (ImGui::Begin("##playerEditOverlay", nullptr, windowFlags)) {
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 16.5f);
			char acceptHotKey[32];
			ImHotKey::GetHotKeyLib(HotKeyService::getInstance().getHotKeyByName("accept")->functionKeys, acceptHotKey, sizeof acceptHotKey);
			char cancelHotKey[32];
			ImHotKey::GetHotKeyLib(HotKeyService::getInstance().getHotKeyByName("cancel")->functionKeys, cancelHotKey, sizeof cancelHotKey);
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
	}
	else {
		model = 290 + actor.getSlot();
		const auto& specialsPed = ModelsService::getInstance().getSpecialsPed().at(actor.getModelId());
		CStreaming::RequestSpecialChar(actor.getSlot(), specialsPed.c_str(), GAME_REQUIRED);
	}
	CStreaming::LoadAllRequestedModels(false);

	Command<Commands::SET_PLAYER_MODEL>(0, model);

	while(true) {

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

void EditByPlayerService::editByPlayerActor(Actor& actor) {
	Tasker::getInstance().addTask("editByPlayerActor", editByPlayerActorTask, actor);
}

ktwait editByPlayerVehicleTask(Vehicle& vehicle) {
	Windows::WindowsRenderService::getInstance().setRenderWindows(false);

	Windows::WindowsRenderService::getInstance().addRender("editByPlayerOverlay", [&] {
		auto& local = Localization::getInstance();
		constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
		if (ImGui::Begin("##playerEditOverlay", nullptr, windowFlags)) {
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 16.5f);
			ImGui::Text(local.get("vehicle.edit_by_player_overlay").c_str());
			ImGui::PopTextWrapPos();
		}
		ImGui::End();
		});

	TheCamera.Restore();

	Command<Commands::FREEZE_CAR_POSITION_AND_DONT_LOAD_COLLISION>(CPools::GetVehicleRef(vehicle.getEditorVehicle().value()), 0);
	vehicle.getEditorVehicle().value()->m_bUsesCollision = 1;
	vehicle.getEditorVehicle().value()->m_nVehicleFlags.bEngineOn = 1;
	Command<Commands::WARP_CHAR_INTO_CAR>(CPools::GetPedRef(FindPlayerPed()), CPools::GetVehicleRef(vehicle.getEditorVehicle().value()));

	while(true) {


		if (!Command<Commands::IS_CHAR_IN_CAR>(CPools::GetPedRef(FindPlayerPed()), CPools::GetVehicleRef(vehicle.getEditorVehicle().value()))) {

			Command<Commands::FREEZE_CAR_POSITION_AND_DONT_LOAD_COLLISION>(CPools::GetVehicleRef(vehicle.getEditorVehicle().value()), 1);
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

	Tasker::getInstance().removeTask("editByPlayerVehicle");
}

void EditByPlayerService::editByPlayerVehicle(Vehicle& vehicle) {
	Tasker::getInstance().addTask("editByPlayerVehicle", editByPlayerVehicleTask, vehicle);
}

CVector rotate_vector_by_quaternion(const CVector& v, const CQuaternion& q) {
	// Extract the vector part of the quaternion
	CVector u(q.imag.x, q.imag.y, q.imag.z);

	// Extract the scalar part of the quaternion
	float s = q.real;

	// Do the math
	float dotted1 = u.x * v.x + u.y * v.y + u.z * v.z;
	float dotted2 = u.x * u.x + u.y * u.y + u.z * u.z;
	CVector crossed;
	crossed.Cross(u, v);
	return 2.f * dotted1 * u
		+ (s * s - dotted2) * v
		+ 2.0f * s * crossed;
}

ktwait positionalObjectTask(CEntity* entity, std::function<void(CMatrix&)> setMatrix, float* posO, CQuaternion& quat, bool fastCreate) {
	Windows::WindowsRenderService::getInstance().setRenderWindows(false);

	Windows::WindowsRenderService::getInstance().addRender("editByPlayerOverlay", [&] {
		auto& local = Localization::getInstance();
		constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
		if (ImGui::Begin("##playerEditOverlay", nullptr, windowFlags)) {
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 16.5f);
			ImGui::Text(local.get("info_overlay.view_camera_mouse").c_str());
			ImGui::Text(local.get("info_overlay.move_camera").c_str());
			char toggleSurfaceRotate[32];
			ImHotKey::GetHotKeyLib(HotKeyService::getInstance().getHotKeyByName("guizmoRotate")->functionKeys, toggleSurfaceRotate, sizeof toggleSurfaceRotate);
			ImGui::Text(local.get("info_overlay.toggle_surface_rotate").c_str(), toggleSurfaceRotate);
			char acceptHotKey[32];
			ImHotKey::GetHotKeyLib(HotKeyService::getInstance().getHotKeyByName("accept")->functionKeys, acceptHotKey, sizeof acceptHotKey);
			char cancelHotKey[32];
			ImHotKey::GetHotKeyLib(HotKeyService::getInstance().getHotKeyByName("cancel")->functionKeys, cancelHotKey, sizeof cancelHotKey);
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
		CEntity* cEntity;
		CWorld::pIgnoreEntity = entity;
		CWorld::ProcessLineOfSight(TheCamera.GetPosition(), pos, colPoint, cEntity, true, true, false, true, true, true, true, true);
		CWorld::pIgnoreEntity = nullptr;

		CMatrix newMatrix;

		CQuaternion normalQuat;
		if (surfaceRotate) {
			CVector up = CVector(0.f, 1.f, 0.f);
			normalQuat = utils::lookRotation(colPoint.m_vecNormal, up);
			newMatrix.SetRotate(normalQuat);
		} else {
			CQuaternion q = {{quat.imag.z, quat.imag.x, -quat.imag.y}, quat.real};
			newMatrix.SetRotate(q);
		}

		CVector newPos = colPoint.m_vecPoint;
		if (entity) {
			const auto objSize = entity->GetColModel()->m_boundBox.m_vecMax - entity->GetColModel()->m_boundBox.m_vecMin;
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
			if (surfaceRotate)
				quat = normalQuat;

			posO[0] = newPos.x;
			posO[1] = newPos.y;
			posO[2] = newPos.z;

			if (fastCreate) {
				openWindowsMenu = true;
				Windows::WindowsRenderService::getInstance().getWindow<Windows::ObjectsWindow>()->selectElement(ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjects().size() - 1);
				Windows::WindowsRenderService::getInstance().getWindow<Windows::ObjectsWindow>()->open();
			}

			break;
		}

		CVector rightCamVec;
		rightCamVec.Cross(TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vecFront, TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vecUp);

		if (KeyPressed(VK_UP)) {
			posCam += TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vecFront;
			TheCamera.VectorMoveLinear(&posCam, &posCam, 10, true);
		}
		if (KeyPressed(VK_DOWN)) {
			posCam += TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vecFront * -1;
			TheCamera.VectorMoveLinear(&posCam, &posCam, 10, true);
			FindPlayerPed()->SetPosn(posCam);
		}

		if (KeyPressed(VK_LEFT)) {
			posCam += rightCamVec * -1;
			TheCamera.VectorMoveLinear(&posCam, &posCam, 10, true);
			FindPlayerPed()->SetPosn(posCam);
		}
		if (KeyPressed(VK_RIGHT)) {
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

void EditByPlayerService::positionalObject(CEntity* entity, std::function<void(CMatrix&)> setMatrix, float* posO, CQuaternion& quat, bool fastCreate) {
	Tasker::getInstance().addTask("createFastObject", positionalObjectTask, entity, setMatrix, posO, quat, fastCreate);
}
