#pragma once
#include <mutex>
#include "../Data/ProjectData.h"

class ProjectsService {
private:
	static std::mutex mutex_;

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
		std::lock_guard lock(mutex_);
		static ProjectsService instance;
		return instance;
	}

	void Init();
	void update();

	void createNewProject();

	void saveCurrentProject();

	void loadProject(int projectIdx);

	void deleteProject(int projectIdx) const;

	std::vector<std::unique_ptr<ProjectInfo>>& getProjectsInfos();
	boost::signals2::signal<void()>& onUpdate();

	ProjectData& getCurrentProject();
};
