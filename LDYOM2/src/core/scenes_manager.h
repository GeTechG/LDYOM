#pragma once
#include "scene.h"
#include "scene_info.h"
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
	bool m_isRestartGame = false;

	ScenesManager();

	static const std::string SCENE_FOLDER_NAME;

  public:
	static ScenesManager& instance();

	ScenesManager(const ScenesManager&) = delete;
	void operator=(const ScenesManager&) = delete;

	void loadScenesInfo();
	std::vector<SceneInfo>& getScenesInfo() { return m_scenesInfo; }
	const Scene& getCurrentScene() const { return *m_currentScene; }
	Scene& getUnsafeCurrentScene() { return *m_currentScene; }

	void onUpdate(float deltaTime);
	void resetCurrentScene();
	void unloadCurrentScene();

	void loadScene(std::string_view sceneId);
	void removeScene(std::string_view sceneId);
	void saveCurrentScene();

	void rewriteSceneInfo(std::string_view sceneId);

	bool isRestartGame() const { return m_isRestartGame; }
	void setRestartGame(bool value) { m_isRestartGame = value; }

  private:
	void loadSceneInternal(std::string_view sceneId);
};