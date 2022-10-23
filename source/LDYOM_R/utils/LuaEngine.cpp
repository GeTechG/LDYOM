#include "LuaEngine.h"

#include <filesystem>

#include "FileWatcher.h"
#include "LuaLogger.h"
#include "LuaWrapper.h"
#include "NodeEditorUtils.h"
#include "Settings.h"
#include "easylogging/easylogging++.h"
#include "fmt/core.h"
#include "Localization/Localization.h"

const std::string SCRIPT_PATH = "LDYOM\\Scripts";

extern bool initServices;

void LuaEngine::resetState() {
	Localization::getInstance().clearScriptsLocalization();
	luaState_ = sol::state();

	luaState_.open_libraries(sol::lib::base, sol::lib::string, sol::lib::os, sol::lib::io, sol::lib::package, sol::lib::math, sol::lib::table, sol::lib::jit, sol::lib::ffi, sol::lib::coroutine);

	const std::string basePath = SCRIPT_PATH + "\\libs\\";
	//luaState_.require_file("class", basePath + "middleclass.lua");
	luaState_.require_file("bitser", basePath + "bitser.lua");
	luaState_.require_file("base64", basePath + "base64.lua");
	//luaState_.require_file("tl", basePath + "tl.lua");
	luaState_.set_function("print", [](sol::this_state L, const sol::object& obj, const sol::variadic_args args) {
		LuaLogger::getInstance().print(sol::state_view(L), obj, args);
	});

	std::string luaPath = luaState_["package"]["path"];
	luaState_["package"]["path"] = fmt::format("{}\\?.lua;{}", std::filesystem::absolute(SCRIPT_PATH).string(), luaPath);

	//bind wrapper
	LuaWrapper::wrap(this->luaState_);

	luaState_.create_table("global_data");
	auto signals = luaState_["global_data"].get<sol::table>()
		.create_named("signals");
	signals.create_named("onNewScene");
	signals.create_named("onDeleteScene");
	signals.create_named("init");
	signals.create_named("onStartScene");
	signals.create_named("onChangeScene");
	signals.create_named("onStartProject");
	signals.create_named("saveScene");
	signals.create_named("loadScene");
	signals.create_named("saveProject");
	signals.create_named("loadProject");
	signals.create_named("onEndScene");
	signals.create_named("onActorSpawn");
	signals.create_named("onActorDelete");
	signals.create_named("onAudioSpawn");
	signals.create_named("onAudioDelete");
	signals.create_named("onCheckpointSpawn");
	signals.create_named("onCheckpointDelete");
	signals.create_named("onObjectSpawn");
	signals.create_named("onObjectDelete");
	signals.create_named("onObjectiveStart");
	signals.create_named("onObjectiveEnd");
	signals.create_named("onParticleSpawn");
	signals.create_named("onParticleDelete");
	signals.create_named("onPickupSpawn");
	signals.create_named("onPickupDelete");
	signals.create_named("onPickupSpawn");
	signals.create_named("onPickupDelete");
	signals.create_named("onTrainSpawn");
	signals.create_named("onTrainDelete");
	signals.create_named("onPyrotechnicsSpawn");
	signals.create_named("onPyrotechnicsDelete");
	signals.create_named("onVehicleSpawn");
	signals.create_named("onVehicleDelete");
	signals.create_named("onVisualEffectSpawn");
	signals.create_named("onVisualEffectDelete");
	signals.create_named("onMainLoop");
	signals.create_named("saveGame");
	signals.create_named("loadGame");

	const std::string scriptsPath = SCRIPT_PATH + "\\scripts";
	for (const auto &entry : std::filesystem::directory_iterator(scriptsPath)) {
		if (entry.is_directory()) {
			std::filesystem::path initFile;
			if (exists(entry.path() / "init.lua")) {
				initFile = entry.path() / "init.lua";
			} else if (exists(entry.path() / "init.tl")) {
				initFile = entry.path() / "init.tl";
			}
			if (!initFile.empty())
				luaState_.safe_script_file(initFile.string() , errorHandler);
		}
	}
	if (initServices) {
		const auto initFuncs = luaState_["global_data"]["signals"]["init"].get_or_create<sol::table>();
		for (auto pair : initFuncs) {
			if (auto result = pair.second.as<sol::function>()(); !result.valid()) {
				sol::error err = result;
				CLOG(ERROR, "lua") << err.what();
				LuaLogger::getInstance().print(err.what());
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
