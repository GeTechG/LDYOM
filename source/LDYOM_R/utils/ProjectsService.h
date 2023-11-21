#pragma once
#include "../Data/ProjectData.h"

class ProjectsService {
private:
	std::vector<std::unique_ptr<ProjectInfo>> projectsInfos_;
	std::vector<std::unique_ptr<ProjectInfo>> productionProjectsInfos_;
	ProjectData currentProject_;
	boost::signals2::signal<void()> onUpdate_;

	std::optional<std::filesystem::path> currentDirectory_;

	ProjectsService() = default;
	~ProjectsService() = default;
	ProjectsService(const ProjectsService &) = delete;
	ProjectsService& operator=(ProjectsService &) = delete;

	void loadProjectData(const std::filesystem::path &projectDirectory);

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
	void loadProductionProject(int projectIdx);

	void deleteProject(int projectIdx) const;
	void deleteProductionProject(int projectIdx) const;

	void makeProjectProduction(int projectIdx) const;

	std::vector<std::unique_ptr<ProjectInfo>>& getProjectsInfos();
	std::vector<std::unique_ptr<ProjectInfo>>& getProductionProjectsInfos();
	boost::signals2::signal<void()>& onUpdate();

	ProjectData& getCurrentProject();
};
