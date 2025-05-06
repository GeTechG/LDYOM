#pragma once
#include "project_info.h"
#include <optional>
#include <string>
#include <vector>

class ProjectsManager {
  public:
	static ProjectsManager& instance();

	ProjectsManager(const ProjectsManager&) = delete;
	void operator=(const ProjectsManager&) = delete;

	void initialize();
	std::vector<ProjectInfo>& getProjects();
	std::optional<ProjectInfo*> getCurrentProject();
	int getCurrentProjectIndex() const noexcept;

	bool createNewProject(std::string_view name, std::string_view author = "");
	bool loadProject(int index);
	void closeProject();

	void saveCurrentProject();

  private:
	std::vector<ProjectInfo> m_projects;
	int m_currentProjectIndex = -1;

	ProjectsManager() = default;

	static const std::string PROJECTS_DIR_PATH;
	static const std::string PROJECT_INFO_FILE_NAME;
};

inline std::string projectPath(std::string_view projectName) {
	auto currentProject = ProjectsManager::instance().getCurrentProject();
	if (!currentProject.has_value()) {
		return std::string();
	}
	return currentProject.value()->path + "/" + std::string(projectName);
}