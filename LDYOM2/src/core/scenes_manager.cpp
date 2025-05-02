#include "scenes_manager.h"
#include "localization.h"
#include "projects_manager.h"
#include <fstream>
#include <logger.h>
#include <project_info.h>
#include <scene.h>

const std::string ScenesManager::SCENE_FOLDER_NAME = "scenes";

ScenesManager::ScenesManager() {
	this->m_currentScene =
		std::make_unique<Scene>(Scene{.info = SceneInfo{.name = _("scenes_settings.default_scene_name")}});
};

ScenesManager& ScenesManager::instance() {
	static ScenesManager instance;
	return instance;
}

void ScenesManager::loadScenesInfo() {
	auto currentProject = ProjectsManager::instance().getCurrentProject();
	if (!currentProject.has_value()) {
		LDYOM_ERROR("No current project set, cannot load scenes info");
		return;
	}
	std::filesystem::path scenesDir(projectPath(SCENE_FOLDER_NAME));
	if (!std::filesystem::exists(scenesDir)) {
		std::filesystem::create_directories(scenesDir);
	}

	this->m_scenesInfo.clear();
	for (const auto& entry : std::filesystem::directory_iterator(scenesDir)) {
		if (entry.is_regular_file()) {
			try {
				std::ifstream file(entry.path());
				if (file.is_open()) {
					nlohmann::json jsonData;
					file >> jsonData;
					file.close();
					m_scenesInfo.push_back(jsonData["info"].get<SceneInfo>());
				} else {
					LDYOM_ERROR("Failed to open scene file: {}", entry.path().string());
				}
			} catch (const std::exception& e) {
				LDYOM_ERROR("Error loading scene info from {}: {}", entry.path().string(), e.what());
			}
		}
	}
	std::ranges::sort(m_scenesInfo, [](const SceneInfo& a, const SceneInfo& b) { return a.name < b.name; });
}

void ScenesManager::loadScene(std::string_view sceneId) {
	auto it = std::ranges::find_if(m_scenesInfo, [&sceneId](const SceneInfo& info) { return info.id == sceneId; });
	if (it != m_scenesInfo.end()) {
		std::filesystem::path scenePath(projectPath(SCENE_FOLDER_NAME) + "/" + it->id + ".json");
		std::ifstream file(scenePath);
		if (file.is_open()) {
			nlohmann::json jsonData;
			file >> jsonData;
			file.close();
			Scene newScene;
			try {
				newScene = jsonData.get<Scene>();
			} catch (const nlohmann::json::exception& e) {
				LDYOM_ERROR("Failed to parse scene JSON: {}", e.what());
				return;
			}
			m_currentScene = std::make_unique<Scene>(std::move(newScene));
			LDYOM_INFO("Loaded scene: {}", m_currentScene->info.name);
		} else {
			LDYOM_ERROR("Failed to open scene file: {}", it->name);
			return;
		}
	} else {
		LDYOM_ERROR("Scene with ID {} not found", sceneId);
	}
}

void ScenesManager::removeScene(std::string_view sceneId) {
	auto it = std::ranges::find_if(m_scenesInfo, [&sceneId](const SceneInfo& info) { return info.id == sceneId; });
	if (it != m_scenesInfo.end()) {
		std::filesystem::path scenePath(projectPath(SCENE_FOLDER_NAME) + "/" + it->id + ".json");
		if (std::filesystem::remove(scenePath)) {
			m_scenesInfo.erase(it);
			LDYOM_INFO("Removed scene: {}", sceneId);
		} else {
			LDYOM_ERROR("Failed to remove scene file: {}", scenePath.string());
		}
	} else {
		LDYOM_ERROR("Scene with ID {} not found", sceneId);
	}
}

void ScenesManager::saveCurrentScene() {
	if (!m_currentScene) {
		LDYOM_ERROR("No current scene to save");
		return;
	}

	auto currentProject = ProjectsManager::instance().getCurrentProject();
	if (!currentProject.has_value()) {
		LDYOM_ERROR("No current project set, cannot save scene");
		return;
	}

	std::filesystem::path scenesDir(projectPath(SCENE_FOLDER_NAME));
	if (!std::filesystem::exists(scenesDir)) {
		std::filesystem::create_directories(scenesDir);
	}

	if (m_currentScene->info.id.empty()) {
		m_currentScene->info.id = std::to_string(std::time(nullptr));
	}
	std::filesystem::path sceneFilePath =
		projectPath(SCENE_FOLDER_NAME) + "/" + this->m_currentScene.get()->info.id + ".json";
	std::ofstream file(sceneFilePath);
	if (file.is_open()) {
		nlohmann::json jsonData = *m_currentScene;
		file << jsonData.dump(4);
		file.close();
		auto sceneIndex = std::ranges::find_if(
			m_scenesInfo, [this](const SceneInfo& info) { return info.id == m_currentScene->info.id; });
		if (sceneIndex != m_scenesInfo.end()) {
			*sceneIndex = m_currentScene->info;
		} else {
			m_scenesInfo.push_back(m_currentScene->info);
		}
		LDYOM_INFO("Saved scene: {}", m_currentScene->info.name);
	} else {
		LDYOM_ERROR("Failed to open file for saving scene: {}", sceneFilePath.string());
	}
}
