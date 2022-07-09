#pragma once
#include <ktcoro_wait.hpp>
#include <optional>

class Scene;

class ProjectPlayerService {
private:
	bool projectRunning = false;
	std::optional<Scene*> currentScene;
	std::optional<Scene*> nextScene;

	std::optional<ktwait*> currentSceneTask;

	ktwait changeScene(Scene* scene, int startObjective = 0);

	ProjectPlayerService() = default;
	ProjectPlayerService(const ProjectPlayerService& root) = delete;
	ProjectPlayerService& operator=(const ProjectPlayerService&) = delete;
public:
	static ProjectPlayerService& getInstance() {
		static ProjectPlayerService instance;
		return instance;
	}

	void setNextScene(Scene* _nextScene);

	ktwait startProject(int sceneIdx, int startObjective = 0);

	bool& isProjectRunning();
	void reset();
};

