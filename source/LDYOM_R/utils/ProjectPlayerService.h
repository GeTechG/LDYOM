#pragma once
#include <ktcoro_wait.hpp>
#include <optional>

#include "SaveService.h"

class BaseObjective;
class Scene;

class ProjectPlayerService {
private:
	bool projectRunning = false;
	std::optional<Scene*> currentScene;
	std::optional<BaseObjective*> currentObjective_;
	std::optional<Scene*> nextScene;
	std::optional<int> nextObjective;
	std::optional<SaveData*> save_;

	ktcoro_tasklist* sceneTasklist = nullptr;
	std::optional<ktwait> currentSceneTask;

	ktwait changeScene(Scene* scene, ktcoro_tasklist& tasklist, int startObjective = 0);

	ProjectPlayerService() = default;
	ProjectPlayerService(const ProjectPlayerService& root) = delete;
	ProjectPlayerService& operator=(const ProjectPlayerService&) = delete;
public:
	static ProjectPlayerService& getInstance() {
		static ProjectPlayerService instance;
		return instance;
	}

	void setNextScene(Scene* nextScene);
	void setNextObjective(int objective);
	void setSave(const std::optional<SaveData*>& save);

	ktwait startProject(int sceneIdx, int startObjective = 0);

	bool& isProjectRunning();
	void reset();
	void stopCurrentScene();

	std::optional<Scene*>& getCurrentScene();
	ktcoro_tasklist*& getSceneTasklist();
	std::optional<BaseObjective*>& getCurrentObjective();
};

