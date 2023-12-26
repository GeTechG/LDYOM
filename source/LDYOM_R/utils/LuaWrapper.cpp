#include "LuaWrapper.h"

#include <chrono>
#include <CTimer.h>
#include <utility>

#include "constants.h"
#include "DataLuaWrapper.h"
#include "FilesystemLuaWrapper.h"
#include "ktcoro_wait.hpp"
#include "strUtils.h"
#include "utils.h"
#include "UtilsLuaWrapper.h"
#include "../Windows/utilsRender.h"
#include "boost/uuid/uuid.hpp"
#include "easylogging/easylogging++.h"
#include "Localization/Localization.h"

#include "../Data/Actor.h"
#include "../Data/Audio.h"
#include "../Data/BaseObjective.h"
#include "../Data/Checkpoint.h"
#include "../Data/Object.h"
#include "../Data/Particle.h"
#include "../Data/Pickup.h"
#include "../Data/Pyrotechnics.h"
#include "../Data/Train.h"
#include "../Data/Vehicle.h"
#include "../Data/VisualEffect.h"

void LuaWrapper::wrap(sol::state &state) {
	FilesystemLuaWrapper::wrap(state);
	DataLuaWrapper::wrap(state);
	UtilsLuaWrapper::wrap(state);

	state.set_function("addLocalizationDirectory", [](const std::string &path) {
		Localization::getInstance().addScriptsLocalization(path);
	});
	state.set_function("gxtEncode", [](const std::string &text) {
		auto cp1251Text = utf8ToCp1251(text);
		gxtEncode(cp1251Text);
		return cp1251Text;
	});
	state.set("LDYOM_VERSION", LDYOM_VERSION);

	state.set("IncorrectHighlight", IncorrectHighlight);
	auto indexByUuuidGeneric = []<typename T>(const std::vector<std::unique_ptr<T>> &vector, const sol::object &uuid) {
		boost::uuids::uuid uuid_{};
		memcpy(uuid_.data, uuid.pointer(), sizeof uuid_.data);
		return utils::indexByUuid(vector, uuid_);
	};
	state.set("indexByUuid", sol::overload(
		          [&indexByUuuidGeneric](const std::vector<std::unique_ptr<Actor>> &vector, const sol::object &uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<Audio>> &vector,
		                                    const sol::object &uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<Checkpoint>> &vector,
		                                    const sol::object &uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<Object>> &vector,
		                                    const sol::object &uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<BaseObjective>> &vector,
		                                    const sol::object &uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<Particle>> &vector,
		                                    const sol::object &uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<Pickup>> &vector,
		                                    const sol::object &uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<Pyrotechnics>> &vector,
		                                    const sol::object &uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<Train>> &vector,
		                                    const sol::object &uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<Vehicle>> &vector,
		                                    const sol::object &uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<VisualEffect>> &vector,
		                                    const sol::object &uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }));
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
