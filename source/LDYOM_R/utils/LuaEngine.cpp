#include "LuaEngine.h"

#include "Logger.h"
#include <filesystem>

const std::string SCRIPT_PATH = "LDYOM\\Scripts";


void LuaEngine::resetState() {
	lua = sol::state();

	lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::table, sol::lib::jit, sol::lib::ffi);

	const std::string basePath = SCRIPT_PATH + "\\libs\\";
	lua.require_file("class", basePath + "middleclass.lua");
	lua.require_file("bitser", basePath + "bitser.lua");
	lua.set_function("log", [](std::string text) { Logger::getInstance().log(text); });

	const std::string scriptsPath = SCRIPT_PATH + "\\scripts";
	for (const auto &entry : std::filesystem::directory_iterator(scriptsPath)) {
		if (entry.is_directory()) {
			auto init_file = entry.path() / "init.lua";
			if (exists(init_file)) {
				auto result = lua.script_file(init_file.string());
				if (!result.valid()) {
					const sol::error error = result;
					Logger::getInstance().log(sol::error(error).what());
				}
			}
		}
	}
}

void LuaEngine::Init() {
	resetState();

	Logger::getInstance().log("LuaEngine is init.");
}
