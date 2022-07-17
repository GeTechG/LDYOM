#pragma once
#include "../Data/ProjectData.h"

class ProjectsService {
private:
	std::vector<std::unique_ptr<ProjectInfo>> projectsInfos_;
	ProjectData currentProject_;
	boost::signals2::signal<void()> onUpdate_;

	std::optional<std::filesystem::path> currentDirectory_;

	ProjectsService() = default;
	~ProjectsService() = default;
	ProjectsService(const ProjectsService&) = delete;
	ProjectsService& operator=(ProjectsService&) = delete;
public:
	static ProjectsService& getInstance() {
		static ProjectsService instance;
		return instance;
	}

	void Init();
	void update();
	void Reset();

	void createNewProject();

	void saveCurrentProject();

	void loadProject(int projectIdx);

	void deleteProject(int projectIdx) const;

	std::vector<std::unique_ptr<ProjectInfo>>& getProjectsInfos();
	boost::signals2::signal<void()>& onUpdate();

	ProjectData& getCurrentProject();
};
