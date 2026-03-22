#pragma once
#include <filesystem>
#include <sol/sol.hpp>

class ProjectScriptManager {
  public:
	static ProjectScriptManager& instance();
	ProjectScriptManager(const ProjectScriptManager&) = delete;
	ProjectScriptManager& operator=(const ProjectScriptManager&) = delete;

	void initialize();
	void loadProjectScript(const std::filesystem::path& scriptPath);
	void unloadProjectScript();

  private:
	ProjectScriptManager() = default;

	sol::protected_function m_onUnload;
	bool m_scriptLoaded = false;
	// Temporary storage filled by register_project_script() during script execution
	sol::protected_function m_pendingOnLoad;
	sol::protected_function m_pendingOnUnload;
};
