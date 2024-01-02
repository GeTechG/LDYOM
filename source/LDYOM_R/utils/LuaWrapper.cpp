#include "LuaWrapper.h"

#include <chrono>
#include <CPed.h>
#include <CPools.h>
#include <CTimer.h>

#include "constants.h"
#include "FilesystemLuaWrapper.h"
#include "ktcoro_wait.hpp"
#include "strUtils.h"
#include "utils.h"
#include "UtilsLuaWrapper.h"
#include "../Windows/utilsRender.h"
#include "boost/uuid/uuid.hpp"
#include "easylogging/easylogging++.h"
#include "Localization/Localization.h"

#include <ImGuiLuaWrapper.h>

extern void indexUuidWrapper(sol::state &state);

void LuaWrapper::wrap(sol::state &state) {
	FilesystemLuaWrapper::wrap(state);
	UtilsLuaWrapper::wrap(state);
	imguiLuaWrapper(state);
	indexUuidWrapper(state);


	state.set_function("addLocalizationDirectory", [](const std::string &path) {
		Localization::getInstance().addScriptsLocalization(path);
	});
	state.set_function("gxtEncode", [](const std::string &text) {
		auto cp1251Text = utf8ToCp1251(text);
		gxtEncode(cp1251Text);
		return cp1251Text;
	});
	state.set("LDYOM_VERSION", LDYOM_VERSION);
	state.set("CPedToHandle", [](CPed *ped) {
		return CPools::GetPedRef(ped);
	});
	state.set("CVehicleToHandle", [](CVehicle *vehicle) {
		return CPools::GetVehicleRef(vehicle);
	});
	state.set("CObjectToHandle", [](CObject *object) {
		return CPools::GetObjectRef(object);
	});
	state.set("CTimer_snTimeInMilliseconds", []() {
		return CTimer::m_snTimeInMilliseconds;
	});
	state.set("DistanceBetweenPoints", [](float x1, float y1, float z1, float x2, float y2, float z2) {
		return DistanceBetweenPoints(CVector(x1, y1, z1), CVector(x2, y2, z2));
	});

	auto loc = state.create_table("loc");
	loc.set_function("get", [](const std::string &key) {
		return Localization::getInstance().get(key);
	});
	loc.set_function("getArray", [](const std::string &key) {
		return &Localization::getInstance().getArray(key);
	});

	auto tasker = state.create_table("Tasker");
	tasker.set_function("addTask", [](ktcoro_tasklist &tasklist, const sol::coroutine &func) {
		tasklist.add_task([](sol::coroutine func) -> ktwait {
			while (func) {
				auto result = func();
				if (!result.valid()) {
					sol::error err = result;
					CLOG(ERROR, "lua") << err.what();
					break;
				}
				if (result.return_count() > 0 && result.get_type() == sol::type::number)
					co_await std::chrono::milliseconds(result.get<int>());
			}
		}, func);
	});
}
