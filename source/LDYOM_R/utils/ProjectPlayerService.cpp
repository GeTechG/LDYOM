﻿#include "ProjectPlayerService.h"

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

	auto addObjectiveDependedEntity = [&spawnMap, &deleteMap]<typename T>(std::vector<std::unique_ptr<T>>& entities) {
		for (const auto& entityT : entities) {
			auto entity = static_cast<ObjectiveDependent*>(entityT.get());
			if (entity->isUseObjective()) {
				spawnMap[entity->getSpawnObjectiveUuid()].emplace_back(entity);
				deleteMap[entity->getDeleteObjectiveUuid()].emplace_back(entity);
			}
		}
	};

	for (const auto & objective : scene->getObjectives()) {
		spawnMap[objective->getUuid()] = std::vector<ObjectiveDependent*>();
		deleteMap[objective->getUuid()] = std::vector<ObjectiveDependent*>();
	}

	addObjectiveDependedEntity(scene->getActors());
	addObjectiveDependedEntity(scene->getVehicles());
	addObjectiveDependedEntity(scene->getObjects());
	addObjectiveDependedEntity(scene->getParticles());
	addObjectiveDependedEntity(scene->getTrains());
	addObjectiveDependedEntity(scene->getPickups());

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
