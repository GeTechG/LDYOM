#include "ProjectPlayerService.h"

#include <CClock.h>
#include <CWeather.h>
#include <CWorld.h>

#include "ProjectsService.h"
#include "Tasker.h"
#include "WindowsRenderService.h"
#include "boost/functional/hash.hpp"
#include "extensions/ScriptCommands.h"
#include "CFireManager.h"
#include "imgui_notify.h"
#include "../Data/Result.h"
#include "../easylogging/easylogging++.h"

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
	addObjectiveDependedEntity(scene->getPyrotechnics());
	addObjectiveDependedEntity(scene->getAudio());
	addObjectiveDependedEntity(scene->getVisualEffects());
	addObjectiveDependedEntity(scene->getCheckpoints());

	for (const auto & audio : scene->getAudio()) {
		if (audio->isUseObjective()) 
			audio->preloadProjectAudio();
	}

	//apply scene settings
	if (scene->isToggleSceneSettings()) {
		using namespace plugin;

		const auto & sceneSettings = scene->getSceneSettings();
		CWeather::ForceWeatherNow(static_cast<short>(sceneSettings.weather));
		CClock::SetGameClock(sceneSettings.time[0], sceneSettings.time[1], 0);
		Command<Commands::SET_LA_RIOTS>(sceneSettings.riot);
		Command<Commands::SET_PED_DENSITY_MULTIPLIER>(sceneSettings.trafficPed);
		Command<Commands::SET_CAR_DENSITY_MULTIPLIER>(sceneSettings.trafficCar);
		CWorld::Players[0].m_PlayerData.m_pWanted->SetWantedLevelNoDrop(sceneSettings.wantedMin);
		CWanted::SetMaximumWantedLevel(sceneSettings.wantedMax);

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 9; j++) {
				if (sceneSettings.groupRelations[i][j] != -1) {
					if (j == 0) {
						Command<Commands::SET_RELATIONSHIP>((int)sceneSettings.groupRelations[i][j], 24 + i, 0);
						Command<Commands::SET_RELATIONSHIP>((int)sceneSettings.groupRelations[i][j], 24 + i, 23);
						Command<Commands::SET_RELATIONSHIP>((int)sceneSettings.groupRelations[i][j], 23, 24 + i);
					} else {
						if (i != j - 1) {
							Command<Commands::SET_RELATIONSHIP>((int)sceneSettings.groupRelations[i][j], 24 + i, 24 + j - 1);
						}
					}
				}
			}
		}
	}

	for (int o = startObjective; o < static_cast<int>(scene->getObjectives().size()); ++o) {
		const auto &objective = scene->getObjectives().at(o);

		for (const auto & dependent : spawnMap[objective->getUuid()]) {
			try {
				dependent->spawnProjectEntity();
			} catch (const std::exception& e) {
				CLOG(ERROR, "LDYOM") << "Failed spawn entity on objective \"" << objective->getName() << "\", error: " <<  e.what();
				ImGui::InsertNotification({ ImGuiToastType_Error, 1000, "Error, see log" });
			}
		}

		Result result;
		co_await objective->execute(scene, result);

		if (!result.isDone()) {
			CLOG(ERROR, "LDYOM") << result;
			ImGui::InsertNotification({ ImGuiToastType_Error, 5000, result.str().c_str()});
			co_return;
		}

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
	using namespace plugin;

	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->unloadEditorScene();
	const auto savedWindow = defaultWindow;
	defaultWindow = std::nullopt;
	Windows::WindowsRenderService::getInstance().closeAllWindows();
	Command<Commands::SET_CHAR_PROOFS>(static_cast<CPed*>(FindPlayerPed()), 0, 0, 0, 0, 0);
	this->projectRunning = true;

	const auto startScene = ProjectsService::getInstance().getCurrentProject().getScenes().at(sceneIdx).get();
	setNextScene(startScene);

	while(this->nextScene.has_value()) {
		const auto scene = this->nextScene.value();
		this->nextScene = std::nullopt;
		auto taskScene = changeScene(startScene, startObjective);
		this->currentSceneTask.emplace(std::move(taskScene));
		co_await ktwait(this->currentSceneTask.value().coro_handle);

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

	for (auto & fire : gFireManager.m_aFires) {
		fire.Extinguish();
	}
	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->loadEditorScene();
	defaultWindow = savedWindow;
	defaultWindow.value()->open();
	Command<Commands::SET_CHAR_PROOFS>(static_cast<CPed*>(FindPlayerPed()), 1, 1, 1, 1, 1);

	Command<Commands::SET_LA_RIOTS>(0);
	Command<Commands::SET_PED_DENSITY_MULTIPLIER>(0.f);
	Command<Commands::SET_CAR_DENSITY_MULTIPLIER>(0.f);
	CWorld::Players[0].m_PlayerData.m_pWanted->SetWantedLevelNoDrop(0);
	CWanted::SetMaximumWantedLevel(0);

	CWorld::ClearExcitingStuffFromArea(FindPlayerPed()->GetPosition(), 999999.f, true);

	this->projectRunning = false;
}

bool& ProjectPlayerService::isProjectRunning() {
	return projectRunning;
}

void ProjectPlayerService::reset() {
	this->projectRunning = false;
}
