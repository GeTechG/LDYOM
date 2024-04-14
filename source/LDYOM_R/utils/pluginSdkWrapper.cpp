#include <CCamera.h>
#include <CSprite.h>
#include <CTimer.h>
#include <CWorld.h>
#include <plugin.h>
#include <sol.hpp>
#include <extensions/KeyCheck.h>

void pluginSdkWrapper(sol::state &state) {
	auto timeTable = state.create_table("time");
	timeTable.set("getSnTimeInMilliseconds", [] {
		return CTimer::m_snTimeInMilliseconds;
	});
	timeTable.set("getSnPreviousTimeInMilliseconds", [] {
		return CTimer::m_snPreviousTimeInMilliseconds;
	});
	state.set("distanceBetweenPoints",
	          [](const float x1, const float y1, const float z1, const float x2, const float y2, const float z2) {
		          return DistanceBetweenPoints(CVector(x1, y1, z1), CVector(x2, y2, z2));
	          });
	state.set("GetPedRef", CPools::GetPedRef);
	state.set("GetVehicleRef", [](sol::userdata ptr) {
		const auto vehicle = *static_cast<CVehicle**>(const_cast<void*>(ptr.pointer()));
		return CPools::GetVehicleRef(vehicle);
	});
	state.set("GetObjectRef", CPools::GetObjectRef);
	state.set("GetPed", CPools::GetPed);
	state.set("GetVehicle", CPools::GetVehicle);
	state.set("GetObject", CPools::GetObject);
	state.set("GetEntity", [](sol::userdata ptr) {
		const auto entity = *static_cast<CEntity**>(const_cast<void*>(ptr.pointer()));
		return entity;
	});
	state.set("findPlayerPed", []() {
		return static_cast<CPed*>(FindPlayerPed());
	});
	state.set("CalcScreenCoors",
	          [](const sol::table &posn, sol::table out, bool checkMaxVisible, bool checkMinVisible) {
		          RwV3d outRw{};
		          float w, h;
		          auto result = CSprite::CalcScreenCoors(RwV3d{posn[1], posn[2], posn[3]}, &outRw, &w, &h,
		                                                 checkMaxVisible, checkMinVisible);
		          out[1] = outRw.x;
		          out[2] = outRw.y;
		          return std::make_tuple(result, w, h);
	          });
	state["removePlayerFromWorld"] = []() {
		CWorld::Remove(FindPlayerPed());
	};
	state["addPlayerToWorld"] = []() {
		CWorld::Add(FindPlayerPed());
	};
	auto keyCheckTable = state.create_table("KeyCheck");
	keyCheckTable.set("Update", KeyCheck::Update);
	keyCheckTable.set("CheckJustDown", KeyCheck::CheckJustDown);
	keyCheckTable.set("KeyPressed", plugin::KeyPressed);
	state["CameraOp"]["setQuaternion"] = [](const sol::table &quat) {
		CQuaternion quaternion;
		quaternion.real = quat[1];
		quaternion.imag.x = quat[2];
		quaternion.imag.y = quat[3];
		quaternion.imag.z = quat[4];
		TheCamera.m_mCameraMatrix.SetRotate(quaternion);
	};
	state["CameraOp"]["getCoordinates"] = []() {
		return std::make_tuple(TheCamera.m_mCameraMatrix.pos.x, TheCamera.m_mCameraMatrix.pos.y,
		                       TheCamera.m_mCameraMatrix.pos.z);
	};
	state["CameraOp"]["getViewDirection"] = []() {
		return std::make_tuple(TheCamera.m_mViewMatrix.right.z, TheCamera.m_mViewMatrix.up.z,
		                       TheCamera.m_mViewMatrix.at.z);
	};
	state["CameraOp"]["getRightCamVector"] = []() {
		CVector rightCamVec;
		rightCamVec.Cross(TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vecFront,
		                  TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vecUp);
		return std::make_tuple(rightCamVec.x, rightCamVec.y, rightCamVec.z);
	};
	state["CameraOp"]["getUpCamVector"] = []() {
		return std::make_tuple(TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vecUp.x,
		                       TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vecUp.y,
		                       TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vecUp.z);
	};
	state["CameraOp"]["getFrontCamVector"] = []() {
		return std::make_tuple(TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vecFront.x,
		                       TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vecFront.y,
		                       TheCamera.m_aCams[TheCamera.m_nActiveCam].m_vecFront.z);
	};
}
