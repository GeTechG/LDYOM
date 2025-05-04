#pragma once
#include "scene.h"
#include "scene_info.h"
#include <memory>
#include <optional>
#include <string>
#include <vector>

class ScenesManager {
  private:
	std::vector<SceneInfo> m_scenesInfo;
	std::unique_ptr<Scene> m_currentScene;

	ScenesManager();

	static const std::string SCENE_FOLDER_NAME;

  public:
	static ScenesManager& instance();

	ScenesManager(const ScenesManager&) = delete;
	void operator=(const ScenesManager&) = delete;

	void loadScenesInfo();
	std::vector<SceneInfo>& getScenesInfo() { return m_scenesInfo; }
	Scene& getCurrentScene() { return *m_currentScene; }

	void loadScene(std::string_view sceneId);
	void removeScene(std::string_view sceneId);
	void rewriteSceneInfo(std::string_view sceneId);
	void saveCurrentScene();
};