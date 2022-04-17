#pragma once
#include <mutex>
#include "../Data/ProjectData.h"

class ProjectsService {
private:
	static std::mutex mutex_;

	std::vector<std::string> projects_name_;
	ProjectData currentProject_;

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

	std::vector<std::string>& getProjectsName();

	ProjectData& getCurrentProject();
};
