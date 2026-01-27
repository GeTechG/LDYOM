#include "projects_manager.h"
#include "data/project_info.h"
#include "scenes_manager.h"
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
	const std::filesystem::path projectsDir(LDYOM_PATH(PROJECTS_DIR_PATH));
	if (!exists(projectsDir)) {
		create_directories(projectsDir);
		LDYOM_INFO("Created projects directory: {}", projectsDir.string());
	}

	loadProjectsFromDirectory();

	LDYOM_INFO("ProjectsManager initialized with {} projects", m_projects.size());
}

void ProjectsManager::shutdown() {
	closeProject();
	m_projects.clear();
	LDYOM_INFO("ProjectsManager shutdown");
}

std::vector<ProjectInfo>& ProjectsManager::getProjects() { return m_projects; }

std::optional<ProjectInfo*> ProjectsManager::getCurrentProject() {
	if (m_currentProjectIndex >= 0 && m_currentProjectIndex < static_cast<int>(m_projects.size())) {
		return &m_projects[m_currentProjectIndex];
	}
	return std::nullopt;
}

int ProjectsManager::getCurrentProjectIndex() const noexcept { return m_currentProjectIndex; }

bool ProjectsManager::createNewProject(std::string_view name, std::string_view author) {
	try {
		std::filesystem::path projectDir(LDYOM_PATH(PROJECTS_DIR_PATH) + "/" + to_snake_case(name));

		ProjectInfo newProjectInfo{.name = std::string(name),
		                           .path = projectDir.string(),
		                           .author = std::string(author),
		                           .timestamp = std::time(nullptr)};
		create_directories(projectDir);
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

bool ProjectsManager::loadProject(int index) {
	if (index < 0 || index >= static_cast<int>(m_projects.size())) {
		LDYOM_ERROR("Invalid project index: {}", index);
		return false;
	}
	m_currentProjectIndex = index;
	ScenesManager::instance().loadScenesInfo();
	auto currentProject = getCurrentProject();
	if (!currentProject.has_value()) {
		LDYOM_ERROR("No current project set, cannot load project info");
		return false;
	}
	auto sceneToLoad = currentProject.value()->startSceneId;
	auto& scenesInfo = ScenesManager::instance().getScenesInfo();

	// Check if start scene exists
	if (!std::ranges::any_of(scenesInfo, [sceneToLoad](const SceneInfo& info) { return info.id == sceneToLoad; })) {
		// If no scenes exist at all, create and save a default scene
		if (scenesInfo.empty()) {
			LDYOM_INFO("No scenes found in project, creating default scene");
			ScenesManager::instance().saveCurrentScene();
			ScenesManager::instance().loadScenesInfo();
			sceneToLoad = ScenesManager::instance().getScenesInfo()[0].id;
		} else {
			// If scenes exist but start scene not found, use first available scene
			sceneToLoad = scenesInfo[0].id;
			LDYOM_INFO("Scene {} not found, loading first scene instead", currentProject.value()->startSceneId);
		}
		currentProject.value()->startSceneId = sceneToLoad;
		saveCurrentProject();
	}
	ScenesManager::instance().loadScene(sceneToLoad);
	LDYOM_INFO("Loaded project: {}", currentProject.value()->name);
	return true;
}

void ProjectsManager::closeProject() {
	if (m_currentProjectIndex >= 0 && m_currentProjectIndex < static_cast<int>(m_projects.size())) {
		ScenesManager::instance().unloadCurrentScene();
		m_currentProjectIndex = -1;
	}
}

void ProjectsManager::saveCurrentProject() {
	auto currentProject = getCurrentProject();
	if (!currentProject.has_value()) {
		LDYOM_ERROR("No current project set, cannot save project info");
		return;
	}
	std::filesystem::path projectPath = currentProject.value()->path;
	std::filesystem::path infoFilePath = projectPath / (PROJECT_INFO_FILE_NAME + ".json");
	std::ofstream file(infoFilePath);
	if (!file) {
		LDYOM_ERROR("Failed to open project info file for writing: {}", infoFilePath.string());
		return;
	}
	nlohmann::json jsonData = *currentProject.value();
	file << jsonData.dump(4);
	file.close();
	LDYOM_INFO("Saved project info: {}", currentProject.value()->name);
}

void ProjectsManager::refreshProjects() {
	loadProjectsFromDirectory();
	LDYOM_INFO("Projects list refreshed: {} projects", m_projects.size());
}

void ProjectsManager::loadProjectsFromDirectory() {
	m_projects.clear();
	const std::filesystem::path projectsDir(LDYOM_PATH(PROJECTS_DIR_PATH));

	for (const auto& entry : std::filesystem::directory_iterator(projectsDir)) {
		if (entry.is_directory()) {
			const std::filesystem::path projectPath = entry.path();
			const std::filesystem::path infoFilePath = projectPath / (PROJECT_INFO_FILE_NAME + ".json");
			if (exists(infoFilePath)) {
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

bool ProjectsManager::removeProject(int index) {
	if (index < 0 || index >= static_cast<int>(m_projects.size())) {
		LDYOM_ERROR("Invalid project index: {}", index);
		return false;
	}

	try {
		const auto& project = m_projects[index];
		std::filesystem::path projectPath(project.path);

		if (std::filesystem::exists(projectPath)) {
			std::filesystem::remove_all(projectPath);
			LDYOM_INFO("Removed project directory: {}", projectPath.string());
		}

		if (m_currentProjectIndex == index) {
			closeProject();
		} else if (m_currentProjectIndex > index) {
			m_currentProjectIndex--;
		}

		m_projects.erase(m_projects.begin() + index);
		LDYOM_INFO("Removed project at index: {}", index);
		return true;
	} catch (const std::exception& e) {
		LDYOM_ERROR("Failed to remove project at index {}: {}", index, e.what());
		return false;
	}
}

bool ProjectsManager::renameProject(int index, std::string_view newName) {
	if (index < 0 || index >= static_cast<int>(m_projects.size())) {
		LDYOM_ERROR("Invalid project index: {}", index);
		return false;
	}

	if (newName.empty()) {
		LDYOM_ERROR("New project name cannot be empty");
		return false;
	}

	try {
		auto& project = m_projects[index];
		std::filesystem::path oldPath(project.path);
		std::filesystem::path parentPath = oldPath.parent_path();
		std::filesystem::path newPath = parentPath / to_snake_case(newName);

		if (std::filesystem::exists(newPath) && oldPath != newPath) {
			LDYOM_ERROR("Project with name '{}' already exists", newName);
			return false;
		}

		if (oldPath != newPath) {
			std::filesystem::rename(oldPath, newPath);
			project.path = newPath.string();
		}

		project.name = std::string(newName);
		project.timestamp = std::time(nullptr);

		if (m_currentProjectIndex == index) {
			saveCurrentProject();
		} else {
			std::filesystem::path infoFilePath = newPath / (PROJECT_INFO_FILE_NAME + ".json");
			std::ofstream file(infoFilePath);
			if (!file) {
				LDYOM_ERROR("Failed to open project info file for writing: {}", infoFilePath.string());
				return false;
			}
			nlohmann::json jsonData = project;
			file << jsonData.dump(4);
			file.close();
		}

		LDYOM_INFO("Renamed project at index {} to '{}'", index, newName);
		return true;
	} catch (const std::exception& e) {
		LDYOM_ERROR("Failed to rename project at index {}: {}", index, e.what());
		return false;
	}
}

