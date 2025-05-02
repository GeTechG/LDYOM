#include "lua_manager.h"
#include "lua/log_bindings.h"
#include <filesystem>
#include <logger.h>

LuaManager& LuaManager::instance() {
	static LuaManager instance;
	return instance;
}

void LuaManager::initialize() {
	std::unique_lock lock(stateMutex);

	lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::coroutine, sol::lib::string, sol::lib::os,
	                   sol::lib::math, sol::lib::table, sol::lib::debug, sol::lib::jit);

	// Add error handler
	lua.set_exception_handler(
		[](lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description) {
			LDYOM_ERROR("Lua error: {}", description);
			return 0;
		});

	register_log_bindings(lua);

	LDYOM_INFO("Lua environment initialized");
}

void LuaManager::shutdown() {
	std::unique_lock lock(stateMutex);

	// Force garbage collection
	lua.collect_garbage();

	// Clear all global variables
	lua.set_panic(nullptr);
	lua = sol::state();

	LDYOM_INFO("Lua environment shutdown");
}

bool LuaManager::executeString(const std::string& code, std::optional<sol::environment> env) {
	std::unique_lock lock(stateMutex);

	try {
		auto result = lua.script(code);
		if (!result.valid()) {
			sol::error err = result;
			LDYOM_ERROR("Failed to execute Lua code: {}", err.what());
			return false;
		}
		return true;
	} catch (const sol::error& e) {
		LDYOM_ERROR("Lua execution error: {}", e.what());
		return false;
	}
}

bool LuaManager::executeFile(const std::string& filepath, std::optional<sol::environment> env) {
	std::unique_lock lock(stateMutex);
	return executeFileRaw(filepath, env);
}

bool LuaManager::executeFileRaw(const std::string& filepath, std::optional<sol::environment> env) {
	if (!std::filesystem::exists(filepath)) {
		LDYOM_ERROR("Lua file not found: {}", filepath);
		return false;
	}

	try {
		sol::environment scriptEnv(lua, sol::create, lua.globals());
		if (env) {
			scriptEnv = *env;
		}
		auto scriptDir = std::filesystem::path(filepath).parent_path().string();
		scriptEnv["package"]["path"] =
			fmt::format("{};{}/?.lua;", lua["package"]["path"].get<std::string>(), scriptDir);
		auto result = lua.script_file(filepath, scriptEnv);
		if (!result.valid()) {
			sol::error err = result;
			LDYOM_ERROR("Failed to execute Lua file: {}", err.what());
			return false;
		}
		return true;
	} catch (const sol::error& e) {
		LDYOM_ERROR("Lua file execution error: {}", e.what());
		return false;
	}
}
