#include "scenes_manager.h"
#include "localization.h"
#include "projects_manager.h"
#include <ePedType.h>
#include <eRelationshipType.h>
#include <fstream>
#include <logger.h>
#include <project_info.h>
#include <scene.h>

const std::string ScenesManager::SCENE_FOLDER_NAME = "scenes";

ScenesManager::ScenesManager() {
	this->m_currentScene = std::make_unique<Scene>(Scene{.info = SceneInfo{.name = _("scenes.default_scene_name")}});

	// Initialize default group relations using the new vector structure
	auto& relations = this->m_currentScene->settings.groupRelations;

	relations.emplace_back(SceneSettings::Relation{PED_TYPE_MISSION1, PED_TYPE_PLAYER1, eRelationshipType::TYPE_HATE});
	relations.emplace_back(SceneSettings::Relation{PED_TYPE_MISSION1, PED_TYPE_MISSION2, eRelationshipType::TYPE_HATE});
	relations.emplace_back(SceneSettings::Relation{PED_TYPE_MISSION1, PED_TYPE_MISSION3, eRelationshipType::TYPE_HATE});
	relations.emplace_back(SceneSettings::Relation{PED_TYPE_MISSION1, PED_TYPE_CIVMALE, eRelationshipType::TYPE_LIKE});

	relations.emplace_back(SceneSettings::Relation{PED_TYPE_MISSION2, PED_TYPE_PLAYER1, eRelationshipType::TYPE_LIKE});
	relations.emplace_back(SceneSettings::Relation{PED_TYPE_MISSION2, PED_TYPE_MISSION1, eRelationshipType::TYPE_HATE});
	relations.emplace_back(SceneSettings::Relation{PED_TYPE_MISSION2, PED_TYPE_MISSION3, eRelationshipType::TYPE_HATE});
	relations.emplace_back(SceneSettings::Relation{PED_TYPE_MISSION2, PED_TYPE_CIVMALE, eRelationshipType::TYPE_LIKE});

	relations.emplace_back(SceneSettings::Relation{PED_TYPE_MISSION3, PED_TYPE_PLAYER1, eRelationshipType::TYPE_HATE});
	relations.emplace_back(SceneSettings::Relation{PED_TYPE_MISSION3, PED_TYPE_MISSION1, eRelationshipType::TYPE_HATE});
	relations.emplace_back(SceneSettings::Relation{PED_TYPE_MISSION3, PED_TYPE_MISSION2, eRelationshipType::TYPE_HATE});
	relations.emplace_back(SceneSettings::Relation{PED_TYPE_MISSION3, PED_TYPE_CIVMALE, eRelationshipType::TYPE_LIKE});

	relations.emplace_back(SceneSettings::Relation{PED_TYPE_CIVMALE, PED_TYPE_PLAYER1, eRelationshipType::TYPE_LIKE});
	relations.emplace_back(SceneSettings::Relation{PED_TYPE_CIVMALE, PED_TYPE_MISSION1, eRelationshipType::TYPE_LIKE});
	relations.emplace_back(SceneSettings::Relation{PED_TYPE_CIVMALE, PED_TYPE_MISSION2, eRelationshipType::TYPE_LIKE});
	relations.emplace_back(SceneSettings::Relation{PED_TYPE_CIVMALE, PED_TYPE_MISSION3, eRelationshipType::TYPE_LIKE});

	relations.emplace_back(SceneSettings::Relation{PED_TYPE_GANG1, PED_TYPE_PLAYER1, eRelationshipType::TYPE_LIKE});
	relations.emplace_back(SceneSettings::Relation{PED_TYPE_GANG2, PED_TYPE_PLAYER1, eRelationshipType::TYPE_LIKE});
	relations.emplace_back(SceneSettings::Relation{PED_TYPE_GANG3, PED_TYPE_PLAYER1, eRelationshipType::TYPE_LIKE});
}

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
	std::unique_lock<std::shared_mutex> lock(m_sceneMutex);
	loadSceneInternal(sceneId);
}

void ScenesManager::loadSceneInternal(std::string_view sceneId) {
	if (m_currentScene->info.id == sceneId) {
		LDYOM_INFO("Scene with ID {} is already loaded", sceneId);
		return;
	}
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
			std::string sceneIdStr(sceneId);
			m_scenesInfo.erase(it);
			LDYOM_INFO("Removed scene: {}", sceneIdStr);
		} else {
			LDYOM_ERROR("Failed to remove scene file: {}", scenePath.string());
		}
	} else {
		LDYOM_ERROR("Scene with ID {} not found", sceneId);
	}
}

void ScenesManager::rewriteSceneInfo(std::string_view sceneId) {
	auto it = std::ranges::find_if(m_scenesInfo, [&sceneId](const SceneInfo& info) { return info.id == sceneId; });
	if (it != m_scenesInfo.end()) {
		// открыть файл сцены и перезаписать информацию
		std::filesystem::path scenePath(projectPath(SCENE_FOLDER_NAME) + "/" + it->id + ".json");
		std::ifstream file(scenePath);
		if (!file.is_open()) {
			LDYOM_ERROR("Failed to open scene file for rewriting: {}", scenePath.string());
			return;
		}
		nlohmann::json jsonData;
		file >> jsonData;
		file.close();
		jsonData["info"] = *it;
		std::ofstream outFile(scenePath);
		if (outFile.is_open()) {
			outFile << jsonData.dump(4);
			outFile.close();
			LDYOM_INFO("Rewrote scene info: {}", sceneId);
		} else {
			LDYOM_ERROR("Failed to open scene file for writing: {}", scenePath.string());
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

void ScenesManager::onUpdate(float deltaTime) {
	auto& currentScene = getUnsafeCurrentScene();
	for (auto& entity : currentScene.entities) {
		entity->onUpdate(deltaTime);
	}
}

void ScenesManager::resetCurrentScene() {
	auto& currentScene = getUnsafeCurrentScene();
	for (auto& entity : currentScene.entities) {
		entity->reset();
	}
}
