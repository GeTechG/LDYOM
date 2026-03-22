#include "lua_manager.h"
#include "application.h"
#include "ImGuiLuaWrapper.h"
#include "OpcodesLuaWrapper.h"
#include "lua/localization_binding.h"
#include "lua/log_bindings.h"
#include "lua/node_editor_binding.h"
#include "lua/carrec_paths_binding.h"
#include "lua/global_vars_binding.h"
#include "lua/project_player_binding.h"
#include "lua/scenes_binding.h"
#include <filesystem>
#include <filesystem_binding.h>
#include <paths.h>
#include <logger.h>
#include <lua_node.h>
#include <node_registry.h>

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

	const auto libsPath = LDYOM_PATH("addons/libs");
	lua["package"]["path"] =
		fmt::format("{};{}/?.lua", lua["package"]["path"].get<std::string>(), libsPath);

	register_log_bindings(lua);
	register_filesystem_bindings(lua);
	register_localization_bindings(lua);
	imguiLuaWrapper(lua);
	opcodesLuaWrapper(lua);
	NodeRegistry::sol_lua_register(lua);
	LuaNodeHandle::sol_lua_register(lua);
	LuaTaskManager::registerBindings(lua, Application::instance().luaTaskManager());
	register_project_player_bindings(lua);
	register_scenes_bindings(lua);
	register_node_editor_bindings(lua);
	register_global_vars_bindings(lua);
	register_carrec_paths_bindings(lua);

	LDYOM_INFO("Lua environment initialized");
}

void LuaManager::shutdown() {
	std::unique_lock lock(stateMutex);

	// Clear Lua event callbacks before destroying the state
	clear_project_player_lua_callbacks();

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
