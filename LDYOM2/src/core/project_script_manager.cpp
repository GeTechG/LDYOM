#include "project_script_manager.h"
#include "lua_manager.h"
#include <filesystem>
#include <logger.h>

ProjectScriptManager& ProjectScriptManager::instance() {
	static ProjectScriptManager inst;
	return inst;
}

void ProjectScriptManager::initialize() {
	LuaManager::instance().registerFunction("register_project_script", [this](sol::table t) {
		if (t["on_load"].valid())
			m_pendingOnLoad = t["on_load"];
		if (t["on_unload"].valid())
			m_pendingOnUnload = t["on_unload"];
	});
}

void ProjectScriptManager::loadProjectScript(const std::filesystem::path& scriptPath) {
	m_pendingOnLoad = sol::protected_function{};
	m_pendingOnUnload = sol::protected_function{};
	m_onUnload = sol::protected_function{};
	m_scriptLoaded = false;

	if (!std::filesystem::exists(scriptPath)) {
		return;
	}

	LDYOM_INFO("ProjectScriptManager: loading '{}'", scriptPath.string());

	try {
		// executeFileRaw does not acquire the lock — it must be called while NOT holding getState()
		if (!LuaManager::instance().executeFileRaw(scriptPath.string(), std::nullopt)) {
			LDYOM_ERROR("ProjectScriptManager: failed to execute script");
			return;
		}

		// m_pendingOnLoad / m_pendingOnUnload are now filled (if the script called register_project_script)
		{
			auto guard = LuaManager::instance().getState();
			if (m_pendingOnLoad.valid()) {
				auto result = m_pendingOnLoad();
				if (!result.valid()) {
					sol::error err = result;
					LDYOM_ERROR("ProjectScriptManager: on_load failed: {}", err.what());
				}
			}
		}

		m_onUnload = std::move(m_pendingOnUnload);
		m_scriptLoaded = true;
		LDYOM_INFO("ProjectScriptManager: script loaded successfully");
	} catch (const std::exception& e) {
		LDYOM_ERROR("ProjectScriptManager: exception during load: {}", e.what());
	}
}

void ProjectScriptManager::unloadProjectScript() {
	if (!m_scriptLoaded) {
		return;
	}

	if (m_onUnload.valid()) {
		try {
			auto guard = LuaManager::instance().getState();
			auto result = m_onUnload();
			if (!result.valid()) {
				sol::error err = result;
				LDYOM_ERROR("ProjectScriptManager: on_unload failed: {}", err.what());
			} else {
				LDYOM_INFO("ProjectScriptManager: script unloaded");
			}
		} catch (const std::exception& e) {
			LDYOM_ERROR("ProjectScriptManager: exception during unload: {}", e.what());
		}
	}

	m_onUnload = sol::protected_function{};
	m_scriptLoaded = false;
}
