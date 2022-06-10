#include "ProjectPlayerService.h"

#include "ProjectsService.h"
#include "Tasker.h"
#include "WindowsRenderService.h"
#include "boost/functional/hash.hpp"
#include "extensions/ScriptCommands.h"

namespace Windows {
	class AbstractWindow;
}

extern std::optional<Windows::AbstractWindow*> defaultWindow;

// ReSharper disable once CppMemberFunctionMayBeStatic
ktwait ProjectPlayerService::changeScene(Scene* scene, int startObjective) {
	std::unordered_map<boost::uuids::uuid, std::vector<ObjectiveDependent*>, boost::hash<boost::uuids::uuid>> spawnMap;
	std::unordered_map<boost::uuids::uuid, std::vector<ObjectiveDependent*>, boost::hash<boost::uuids::uuid>> deleteMap;

	for (const auto & objective : scene->getObjectives()) {
		spawnMap[objective->getUuid()] = std::vector<ObjectiveDependent*>();
		deleteMap[objective->getUuid()] = std::vector<ObjectiveDependent*>();
	}

	for (const auto & actor : scene->getActors()) {
		if (actor->isUseObjective()) {
			spawnMap[actor->getSpawnObjectiveUuid()].emplace_back(actor.get());
			deleteMap[actor->getDeleteObjectiveUuid()].emplace_back(actor.get());
		}
	}

	for (const auto& vehicle : scene->getVehicles()) {
		if (vehicle->isUseObjective()) {
			spawnMap[vehicle->getSpawnObjectiveUuid()].emplace_back(vehicle.get());
			deleteMap[vehicle->getDeleteObjectiveUuid()].emplace_back(vehicle.get());
		}
	}

	for (int o = startObjective; o < static_cast<int>(scene->getObjectives().size()); ++o) {
		const auto &objective = scene->getObjectives().at(o);

		for (const auto & dependent : spawnMap[objective->getUuid()])
			dependent->spawnProjectEntity();

		co_await objective->execute(scene);

		if (o == static_cast<int>(scene->getObjectives().size() - 1)) {
			using namespace plugin;

			CPed* playerPed = FindPlayerPed();
			auto playerInVehicle = Command<Commands::IS_CHAR_IN_ANY_CAR>(playerPed) || 
				Command<Commands::IS_CHAR_IN_ANY_BOAT>(playerPed) ||
				Command<Commands::IS_CHAR_IN_ANY_HELI>(playerPed) ||
				Command<Commands::IS_CHAR_IN_ANY_PLANE>(playerPed) ||
				Command<Commands::IS_CHAR_IN_ANY_TRAIN>(playerPed);
			if (playerInVehicle) {
				Command<Commands::TASK_LEAVE_ANY_CAR>(playerPed);
				while (playerInVehicle) {
					co_await 1;
					playerInVehicle = Command<Commands::IS_CHAR_IN_ANY_CAR>(playerPed) ||
						Command<Commands::IS_CHAR_IN_ANY_BOAT>(playerPed) ||
						Command<Commands::IS_CHAR_IN_ANY_HELI>(playerPed) ||
						Command<Commands::IS_CHAR_IN_ANY_PLANE>(playerPed) ||
						Command<Commands::IS_CHAR_IN_ANY_TRAIN>(playerPed);
				}
			}
		}

		for (const auto& dependent : deleteMap[objective->getUuid()])
			dependent->deleteProjectEntity();
	}
}

void ProjectPlayerService::setNextScene(Scene* _nextScene) {
	this->nextScene = std::make_optional(_nextScene);
}

ktwait ProjectPlayerService::startProject(int sceneIdx, int startObjective) {
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->unloadEditorScene();
	const auto savedWindow = defaultWindow;
	defaultWindow = std::nullopt;
	Windows::WindowsRenderService::getInstance().closeAllWindows();
	this->projectRunning = true;

	const auto startScene = ProjectsService::getInstance().getCurrentProject().getScenes().at(sceneIdx).get();
	setNextScene(startScene);

	while(this->nextScene.has_value()) {
		const auto scene = this->nextScene.value();
		this->nextScene = std::nullopt;
		auto taskScene = changeScene(startScene, startObjective);
		this->currentSceneTask = &taskScene;
		co_await std::move(taskScene);

		{
			using namespace plugin;

			CPed* playerPed = FindPlayerPed();
			auto playerInVehicle = Command<Commands::IS_CHAR_IN_ANY_CAR>(playerPed) ||
				Command<Commands::IS_CHAR_IN_ANY_BOAT>(playerPed) ||
				Command<Commands::IS_CHAR_IN_ANY_HELI>(playerPed) ||
				Command<Commands::IS_CHAR_IN_ANY_PLANE>(playerPed) ||
				Command<Commands::IS_CHAR_IN_ANY_TRAIN>(playerPed);
			if (playerInVehicle) {
				Command<Commands::TASK_LEAVE_ANY_CAR>(playerPed);
				while (playerInVehicle) {
					co_await 1;
					playerInVehicle = Command<Commands::IS_CHAR_IN_ANY_CAR>(playerPed) ||
						Command<Commands::IS_CHAR_IN_ANY_BOAT>(playerPed) ||
						Command<Commands::IS_CHAR_IN_ANY_HELI>(playerPed) ||
						Command<Commands::IS_CHAR_IN_ANY_PLANE>(playerPed) ||
						Command<Commands::IS_CHAR_IN_ANY_TRAIN>(playerPed);
				}
			}
		}

		scene->unloadProjectScene();
	}

	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->loadEditorScene();
	defaultWindow = savedWindow;
	defaultWindow.value()->open();
	this->projectRunning = false;
}

bool& ProjectPlayerService::isProjectRunning() {
	return projectRunning;
}
