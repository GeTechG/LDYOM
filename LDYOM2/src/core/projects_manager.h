#pragma once
#include <optional>
#include <string>
#include <vector>

struct ProjectInfo;

class ProjectsManager {
  public:
	static ProjectsManager& instance();

	ProjectsManager(const ProjectsManager&) = delete;
	void operator=(const ProjectsManager&) = delete;

	void initialize();
	std::vector<ProjectInfo>& getProjects();
	std::optional<std::reference_wrapper<ProjectInfo>> getCurrentProject();
	int getCurrentProjectIndex() const noexcept;

	bool createNewProject(std::string_view name, std::string_view author = "");
	void openProject(int index);
	void closeProject();

  private:
	std::vector<ProjectInfo> m_projects;
	int m_currentProjectIndex = -1;

	ProjectsManager() = default;

	static const std::string PROJECTS_DIR_PATH;
	static const std::string PROJECT_INFO_FILE_NAME;
};

#define proj ::ProjectsManager::instance().getCurrentProject();