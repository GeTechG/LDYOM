#include "projects_manager.h"
#include "data/project_info.h"
#include <filesystem>
#include <fstream>
#include <logger.h>
#include <nlohmann/json.hpp>
#include <paths.h>
#include <string_utils.h>

const std::string ProjectsManager::PROJECTS_DIR_PATH = "projects";
const std::string ProjectsManager::PROJECT_INFO_FILE_NAME = "project_info";

ProjectsManager& ProjectsManager::instance() {
	static ProjectsManager instance;
	return instance;
}

void ProjectsManager::initialize() {
	m_projects.clear();
	const std::filesystem::path projectsDir(LDYOM_PATH(PROJECTS_DIR_PATH));
	if (!std::filesystem::exists(projectsDir)) {
		std::filesystem::create_directories(projectsDir);
		LDYOM_INFO("Created projects directory: {}", projectsDir.string());
	}
	for (const auto& entry : std::filesystem::directory_iterator(projectsDir)) {
		if (entry.is_directory()) {
			const std::filesystem::path projectPath = entry.path();
			const std::filesystem::path infoFilePath = projectPath / (PROJECT_INFO_FILE_NAME + ".json");
			if (std::filesystem::exists(infoFilePath)) {
				try {
					std::ifstream file(infoFilePath);
					nlohmann::json jsonData;
					file >> jsonData;
					ProjectInfo projectInfo = jsonData;
					projectInfo.path = std::filesystem::path(projectPath.string()).make_preferred().string();
					m_projects.push_back(projectInfo);
				} catch (const std::exception& e) {
					LDYOM_ERROR("Failed to load project info from {}: {}", infoFilePath.string(), e.what());
				}
			}
		}
	}
}

std::vector<ProjectInfo>& ProjectsManager::getProjects() { return m_projects; }

std::optional<std::reference_wrapper<ProjectInfo>> ProjectsManager::getCurrentProject() {
	if (m_currentProjectIndex >= 0 && m_currentProjectIndex < static_cast<int>(m_projects.size())) {
		return std::ref(m_projects[m_currentProjectIndex]);
	}
	return std::nullopt;
}

int ProjectsManager::getCurrentProjectIndex() const noexcept { return m_currentProjectIndex; }

bool ProjectsManager::createNewProject(std::string_view name, std::string_view author) {
	try {
		ProjectInfo newProjectInfo{.name = std::string(name),
		                           .author = std::string(author),
		                           .timestamp = std::time(nullptr)};
		std::filesystem::path projectDir(LDYOM_PATH(PROJECTS_DIR_PATH) + "/" + to_snake_case(newProjectInfo.name));
		std::filesystem::create_directories(projectDir);
		nlohmann::json jsonData = newProjectInfo;
		std::ofstream file(projectDir / (PROJECT_INFO_FILE_NAME + ".json"));
		file << jsonData.dump(4);
		file.close();
		m_projects.push_back(newProjectInfo);
		m_currentProjectIndex = static_cast<int>(m_projects.size()) - 1;
		LDYOM_INFO("Created new project: {}", newProjectInfo.name);
	} catch (const std::exception& e) {
		LDYOM_ERROR("Failed to create new project '{}': {}", name, e.what());
		return false;
	}
	return true;
}

void ProjectsManager::openProject(int index) { m_currentProjectIndex = index; }

void ProjectsManager::closeProject() {
	if (m_currentProjectIndex >= 0 && m_currentProjectIndex < static_cast<int>(m_projects.size())) {
		m_currentProjectIndex = -1;
	}
}
