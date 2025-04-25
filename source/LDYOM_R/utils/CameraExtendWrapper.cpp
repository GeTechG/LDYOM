#include <sol.hpp>

#include "../Data/CCameraExtend.h"
#include "../Data/CutsceneMutex.h"

class CutsceneMutexGuardLuaWrap {
private:
	CutsceneMutexGuard *mutex;

public:
	CutsceneMutexGuardLuaWrap() {
		mutex = new CutsceneMutexGuard();
	}

	~CutsceneMutexGuardLuaWrap() {
		delete mutex;
	}

	void unlock() {
		delete mutex;
		mutex = nullptr;
	}
};

void cameraExtendWrapper(sol::state &state) {
	auto ldTable = state["ld"].get<sol::table>();
	ldTable["TheCameraExtend"] = &TheCameraExtend;
	state.new_usertype<CCameraExtend>("CCameraExtend",
	                                  "setExtendMode", &CCameraExtend::setExtendMode,
	                                  "attachToEntity", &CCameraExtend::attachToEntity,
	                                  "playCameraPath", &CCameraExtend::playCameraPath,
	                                  "stopCameraPath", &CCameraExtend::stopCameraPath,
	                                  "isPlayingPath", &CCameraExtend::isPlayingPath,
	                                  "getPathProgress", &CCameraExtend::getPathProgress
	);
	auto cameraMutex = ldTable.create("CameraMutex");
	cameraMutex["isLocked"] = CutsceneMutex::isLocked;
	ldTable["createCutsceneGuard"] = []() {
		return std::make_shared<CutsceneMutexGuardLuaWrap>();
	};
	ldTable["destroyCutsceneGuard"] = [](const std::shared_ptr<CutsceneMutexGuardLuaWrap> &guard) {
		guard->unlock();
	};
}
