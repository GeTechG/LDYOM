#include "ProjectsService.h"

#include "Logger.h"
#include <filesystem>

const std::string PROJECTS_PATH = "LDYOM\\Projects\\";

std::mutex ProjectsService::mutex_;

void ProjectsService::update() {
	this->projects_name_.clear();
	for (const auto &entry : std::filesystem::directory_iterator(PROJECTS_PATH)) {
		if (entry.is_directory()) {
			auto mission_file = entry.path() / "data.dat";
			if (exists(mission_file)) {
				projects_name_.emplace_back(entry.path().stem().string());
			}
		}
	}
}

void ProjectsService::Init() {
	update();
	Logger::getInstance().log("ProjectsManager is init.");
}

std::vector<std::string>& ProjectsService::getProjectsName() {
	return projects_name_;
}

ProjectData& ProjectsService::getCurrentProject() {
	return currentProject_;
}
