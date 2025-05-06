#pragma once
#include "scene.h"
#include "scene_info.h"
#include <locked_ref.h>
#include <memory>
#include <optional>
#include <shared_mutex>
#include <string>
#include <vector>

class ScenesManager {
  private:
	std::vector<SceneInfo> m_scenesInfo;
	std::unique_ptr<Scene> m_currentScene;
	mutable std::shared_mutex m_sceneMutex;

	ScenesManager();

	static const std::string SCENE_FOLDER_NAME;

  public:
	static ScenesManager& instance();

	ScenesManager(const ScenesManager&) = delete;
	void operator=(const ScenesManager&) = delete;

	void loadScenesInfo();
	std::vector<SceneInfo>& getScenesInfo() { return m_scenesInfo; }
	const Scene& getCurrentScene() const { return *m_currentScene; }
	LockedRef<Scene, std::shared_mutex> getMutableCurrentScene() { return LockedRef(*m_currentScene, m_sceneMutex); }

	void loadScene(std::string_view sceneId);
	void removeScene(std::string_view sceneId);
	void saveCurrentScene();

	void rewriteSceneInfo(std::string_view sceneId);

  private:
	void loadSceneInternal(std::string_view sceneId);
};