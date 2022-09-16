#include "LuaWrapper.h"

#include <chrono>

#include "constants.h"
#include "DataLuaWrapper.h"
#include "FilesystemLuaWrapper.h"
#include "ImGuiLuaWrapper.h"
#include "ImNodesLuaWrapper.h"
#include "NodeEditorLuaWrapper.h"
#include "OpcodesLuaWrapper.h"
#include "strUtils.h"
#include "UtilsLuaWrapper.h"
#include "easylogging/easylogging++.h"
#include "Localization/Localization.h"
#include "ktcoro_wait.hpp"

void LuaWrapper::wrap(sol::state& state) {
	OpcodesLuaWrapper::wrap(state);
	FilesystemLuaWrapper::wrap(state);
	ImNodesLuaWrapper::wrap(state);
	ImGuiLuaWrapper::wrap(state);
	NodeEditorLuaWrapper::wrap(state);
	DataLuaWrapper::wrap(state);
	UtilsLuaWrapper::wrap(state);

	state.set_function("addLocalizationDirectory", [](const std::string path) {
		Localization::getInstance().addScriptsLocalization(path);
	});
	state.set_function("gxtEncode", [](const char* text) {
		auto cp1251Text = utf8ToCp1251(text);
		gxtEncode(cp1251Text);
		return cp1251Text;
	});
	state.set("LDYOM_VERSION", LDYOM_VERSION);

	auto loc = state.create_table("loc");
	loc.set_function("get", [](const std::string& key) {
		return Localization::getInstance().get(key);
	});
	loc.set_function("getArray", [](const std::string& key) {
		return &Localization::getInstance().getArray(key);
	});

	auto tasker = state.create_table("Tasker");
	tasker.set_function("addTask", [](ktcoro_tasklist& tasklist, sol::coroutine func) {
		tasklist.add_task([](sol::coroutine func) -> ktwait {
			while (func) {
				auto result = func();
				if (!result.valid()) {
					sol::error err = result;
					CLOG(ERROR, "lua") << err.what();
					break;
				}
				if (result.return_count() > 0 && result.get_type() == sol::type::number)
					co_await std::chrono:: milliseconds(result.get<int>());
			}
		}, func);
	});
}

