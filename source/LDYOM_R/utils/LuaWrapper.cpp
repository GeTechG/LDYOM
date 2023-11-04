#include "LuaWrapper.h"

#include <chrono>
#include <CTimer.h>

#include "constants.h"
#include "DataLuaWrapper.h"
#include "EditByPlayerService.h"
#include "FilesystemLuaWrapper.h"
#include "ImGuiLuaWrapper.h"
#include "ktcoro_wait.hpp"
#include "ModelsService.h"
#include "OpcodesLuaWrapper.h"
#include "ProjectPlayerService.h"
#include "strUtils.h"
#include "utils.h"
#include "UtilsLuaWrapper.h"
#include "WindowsRenderService.h"
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
#include "../Data/SaveObjective.h"
#include "../Data/Train.h"
#include "../Data/Vehicle.h"
#include "../Data/VisualEffect.h"
#include "../Data/WaitSignalObjective.h"

void LuaWrapper::wrap(sol::state &state) {
	OpcodesLuaWrapper::wrap(state);
	FilesystemLuaWrapper::wrap(state);
	ImGuiLuaWrapper::wrap(state);
	DataLuaWrapper::wrap(state);
	UtilsLuaWrapper::wrap(state);

	state.set_function("addLocalizationDirectory", [](const std::string path) {
		Localization::getInstance().addScriptsLocalization(path);
	});
	state.set_function("gxtEncode", [](const char *text) {
		auto cp1251Text = utf8ToCp1251(text);
		gxtEncode(cp1251Text);
		return cp1251Text;
	});
	state.set("LDYOM_VERSION", LDYOM_VERSION);

	state.set("UuidCombo", [](const char *label, sol::object currentItem_, int currentElement, int size,
	                          const std::function<const char*(int)> &getName,
	                          const std::function<boost::uuids::uuid&(int)> &getUuid) {
		boost::uuids::uuid currentItem{};
		memcpy(currentItem.data, currentItem_.pointer(), sizeof currentItem.data);
		const auto result = utils::Combo(label, &currentItem, currentElement, size, getName, getUuid);
		memcpy(const_cast<void*>(currentItem_.pointer()), currentItem.data, sizeof currentItem.data);
		return result;
	});
	state.set("IncorrectHighlight", IncorrectHighlight);
	auto indexByUuuidGeneric = []<typename T>(const std::vector<std::unique_ptr<T>> &vector, sol::object uuid) {
		boost::uuids::uuid uuid_{};
		memcpy(uuid_.data, uuid.pointer(), sizeof uuid_.data);
		return utils::indexByUuid(vector, uuid_);
	};
	state.set("indexByUuid", sol::overload(
		          [&indexByUuuidGeneric](const std::vector<std::unique_ptr<Actor>> &vector, sol::object uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<Audio>> &vector, sol::object uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<Checkpoint>> &vector, sol::object uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<Object>> &vector, sol::object uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<BaseObjective>> &vector,
		                                    sol::object uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<Particle>> &vector, sol::object uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<Pickup>> &vector, sol::object uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<Pyrotechnics>> &vector,
		                                    sol::object uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<Train>> &vector, sol::object uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<Vehicle>> &vector, sol::object uuid) {
			          return indexByUuuidGeneric(vector, uuid);
		          }, [&indexByUuuidGeneric](const std::vector<std::unique_ptr<VisualEffect>> &vector,
		                                    sol::object uuid) {
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
	state.set("UuidEquals", [](const sol::object &uuidObject, const boost::uuids::uuid &uuidCompare) {
		boost::uuids::uuid uuid{};
		memcpy(uuid.data, uuidObject.pointer(), sizeof uuid.data);
		return uuid == uuidCompare;
	});
	state.set("setNextObjective", [](int next_objective) {
		ProjectPlayerService::getInstance().setNextObjective(next_objective);
	});
	state.set("saveGame", SaveObjective::saveGame);
	state.set("editByPlayerActorLuaPath", [](sol::table path) {
		EditByPlayerService::getInstance().editByPlayerActorLuaPath(path);
	});
	state.set("CTimer_snTimeInMilliseconds", []() {
		return CTimer::m_snTimeInMilliseconds;
	});
	state.set("DistanceBetweenPoints", [](float x1, float y1, float z1, float x2, float y2, float z2) {
		return DistanceBetweenPoints(CVector(x1, y1, z1), CVector(x2, y2, z2));
	});
	state.set("emitSignal", WaitSignalObjective::emitSignal);
	state.set("editByPlayerVehicleLuaPath", [](sol::table path, int model) {
		EditByPlayerService::getInstance().editByPlayerVehicleLuaPath(path, model);
	});
	state.set("ModelsService_getPacksNames", []() {
		return &ModelsService::getInstance().getPacksNames();
	});
	state.set("ModelsService_getAnimations", []() {
		return &ModelsService::getInstance().getAnimations();
	});
	state.set("getCurrentObjective", []() {
		return ProjectPlayerService::getInstance().getCurrentObjective();
	});

	auto loc = state.create_table("loc");
	loc.set_function("get", [](const std::string &key) {
		return Localization::getInstance().get(key);
	});
	loc.set_function("getArray", [](const std::string &key) {
		return &Localization::getInstance().getArray(key);
	});

	auto tasker = state.create_table("Tasker");
	tasker.set_function("addTask", [](ktcoro_tasklist &tasklist, sol::coroutine func) {
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
