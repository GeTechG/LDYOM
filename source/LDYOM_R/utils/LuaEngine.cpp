#include "LuaEngine.h"

#include <filesystem>

#include "FileWatcher.h"
#include "LuaLogger.h"
#include "LuaWrapper.h"
#include "Settings.h"
#include "easylogging/easylogging++.h"
#include "fmt/core.h"
#include "Localization/Localization.h"

const std::string SCRIPT_PATH = "LDYOM\\Scripts";

extern bool initServices;

void LuaEngine::resetState() {
	Localization::getInstance().clearScriptsLocalization();
	luaState_ = sol::state();

	luaState_.open_libraries(sol::lib::base, sol::lib::string, sol::lib::os, sol::lib::io, sol::lib::package,
	                         sol::lib::math, sol::lib::table, sol::lib::jit, sol::lib::ffi, sol::lib::coroutine);

	const std::string basePath = SCRIPT_PATH + "\\libs\\";
	luaState_.require_file("tl", basePath + "tl.lua");
	luaState_.safe_script("tl.loader()", errorHandler, "LuaEngine");
	luaState_.set_function("print", [](const sol::this_state l, const sol::object &obj, const sol::variadic_args args) {
		LuaLogger::getInstance().print(sol::state_view(l), obj, args);
	});

	std::string luaPaths = luaState_["package"]["path"];
	auto dirScriptsPath = std::filesystem::absolute(SCRIPT_PATH).string();
	luaState_["package"]["path"] =
		fmt::format("{}\\?.lua;{}\\?.tl;{}", dirScriptsPath, dirScriptsPath, luaPaths);

	//bind wrapper
	LuaWrapper::wrap(this->luaState_);

	const std::string scriptsPath = SCRIPT_PATH + "\\scripts";
	for (const auto &entry : std::filesystem::directory_iterator(scriptsPath)) {
		if (entry.is_directory()) {
			std::filesystem::path initFile;
			if (exists(entry.path() / "init.lua")) {
				initFile = entry.path() / "init.lua";
			}
			if (!initFile.empty()) {
				{
					luaPaths = luaState_["package"]["path"];
					auto scriptPath = absolute(entry.path()).string();
					luaState_["package"]["path"] = fmt::format("{}/?.lua;{}/?.tl;{}", scriptPath, scriptPath,
					                                           luaPaths);
				}
				luaState_.safe_script_file(initFile.string(), errorHandler);
			}
		}
	}
}

void LuaEngine::Init() {
	resetState();

	FileWatcher::addWatch(std::filesystem::path(SCRIPT_PATH) / "scripts",
	                      true,
	                      FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_DIR_NAME,
	                      [this] {
		                      this->resetState();
	                      });

	CLOG(INFO, "LDYOM") << "LuaEngine is init.";
}

sol::state& LuaEngine::getLuaState() {
	return luaState_;
}

sol::protected_function_result LuaEngine::errorHandler(sol::this_state, sol::protected_function_result pfr) {
	const sol::error error = pfr;
	CLOG(ERROR, "lua") << error.what();
	LuaLogger::getInstance().print(error.what());
	return pfr;
}
