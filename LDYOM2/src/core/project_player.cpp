#include "project_player.h"
#include "localization.h"
#include "objectives_manager.h"
#include "projects_manager.h"
#include "scenes_manager.h"
#include <CCamera.h>
#include <CClock.h>
#include <CClothes.h>
#include <CHud.h>
#include <CMessages.h>
#include <CText.h>
#include <CTheScripts.h>
#include <CWeather.h>
#include <CWorld.h>
#include <common.h>
#include <counter_service.h>
#include <eFadeFlag.h>
#include <extensions/ScriptCommands.h>
#include <logger.h>
#include <task_manager.h>
#include <timer_service.h>
#include <utils/string_utils.h>

ProjectPlayer::~ProjectPlayer() { this->projectTasklist->clear_all_tasks(); }

ktwait ProjectPlayer::run() {
	TheCamera.Fade(0.5f, FADE_IN);
	co_await 500;

	instance().m_state.isPlaying = true;
	instance().transitionPlayingState(true);
	LDYOM_INFO("Project player started");
	TheCamera.Fade(0.5f, FADE_OUT);

	const auto& settings = ScenesManager::instance().getCurrentScene().settings;

	if (settings.isPrintSceneName) {
		// PRINT SCENE NAME
		static std::string sceneName;
		sceneName = utf8_to_cp1251(ScenesManager::instance().getCurrentScene().info.name);
		gxt_encode(sceneName);
		CMessages::AddBigMessage((char*)sceneName.c_str(), 1000, STYLE_BOTTOM_RIGHT);
	}

	if (settings.isSceneSettingsEnabled) {
		// SET RELATIONS
		for (const auto& relation : settings.groupRelations) {
			plugin::Command<plugin::Commands::SET_RELATIONSHIP>(relation.relationType, relation.ofPedType,
			                                                    relation.toPedType);
		}
		// SET TIME
		CClock::SetGameClock(settings.time[0], settings.time[1], 0);

		if (settings.limitCompletionTime && settings.completionTime > 0) {
			// SET COMPLETION TIME
			auto completionTime = settings.completionTime * 1000; // Convert to milliseconds
			TimerService::instance().addTimer(TheText.Get("RTIME"), true, completionTime);
			TaskManager::instance().addTask("scene_completion_timer", [completionTime]() -> ktwait {
				while (TimerService::instance().getTimerTime() > 0) {
					co_await 1000;
				}
				instance().failCurrentProject();
			});

		} else if (settings.isShowMissionTime) {
			TimerService::instance().addTimer(TheText.Get("BB_19"), false, 0);
		}

		// SET TRAFFIC
		plugin::Command<plugin::Commands::SET_PED_DENSITY_MULTIPLIER>(settings.trafficPed);
		plugin::Command<plugin::Commands::SET_CAR_DENSITY_MULTIPLIER>(settings.trafficCar);

		// SET WANTED
		CWorld::Players[0].m_PlayerData.m_pWanted->SetWantedLevelNoDrop(settings.wantedMin);
		CWanted::SetMaximumWantedLevel(settings.wantedMax);

		// SET WEATHER
		CWeather::ForceWeatherNow(static_cast<short>(settings.weather));

		// SET RIOT
		if (settings.riot) {
			plugin::Command<plugin::Commands::SET_LA_RIOTS>(true);
		} else {
			plugin::Command<plugin::Commands::SET_LA_RIOTS>(false);
		}
	}

	auto& objectives = ScenesManager::instance().getCurrentScene().objectives.data;

	for (int i = 0; i < static_cast<int>(objectives.size()); i++) {
		instance().m_state.currentObjectiveIndex = i;
		instance().onObjectiveStarted(i);
		auto& objective = ObjectivesManager::instance().getUnsafeObjective(i);
		co_await objective.execute();
		instance().onObjectiveCompleted(i);
	}

	co_await ProjectPlayer::playerLeaveAnyVehicle();

	instance().m_state.isPlaying = false;
	instance().transitionPlayingState(false);
	LDYOM_INFO("Project player finished");
}

ktwait ProjectPlayer::processStopCheat() {
	while (!plugin::Command<0x0ADC>("LDSTOP")) {
		co_await 1;
	}
	instance().stopCurrentProject();
}

ktwait ProjectPlayer::playerLeaveAnyVehicle() {
	using namespace plugin;

	CPed* playerPed = FindPlayerPed();
	auto playerInVehicle =
		Command<Commands::IS_CHAR_IN_ANY_CAR>(playerPed) || Command<Commands::IS_CHAR_IN_ANY_BOAT>(playerPed) ||
		Command<Commands::IS_CHAR_IN_ANY_HELI>(playerPed) || Command<Commands::IS_CHAR_IN_ANY_PLANE>(playerPed) ||
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

ProjectPlayer& ProjectPlayer::instance() {
	static ProjectPlayer instance;
	return instance;
}

void ProjectPlayer::startCurrentProject(std::string_view sceneId) {
	auto currentProject = ProjectsManager::instance().getCurrentProject();
	if (!currentProject.has_value()) {
		LDYOM_ERROR("No current project set, cannot start project player");
		return;
	}
	this->m_state.currentSceneId = sceneId.empty() ? currentProject.value()->startSceneId : std::string(sceneId);

	if (!TaskManager::instance().addTask("run_project_player", run)) {
		LDYOM_ERROR("Failed to add task for project player");
	}
}

void ProjectPlayer::stopCurrentProject() {
	if (!this->m_state.isPlaying) {
		LDYOM_ERROR("Project player is not running, cannot stop it");
		return;
	}
	TaskManager::instance().removeTask("run_project_player");

	TaskManager::instance().addTask("stopping_project_player", []() -> ktwait {
		co_await ProjectPlayer::playerLeaveAnyVehicle();
		ProjectPlayer::instance().m_state.isPlaying = false;
		ProjectPlayer::instance().transitionPlayingState(false);
		LDYOM_INFO("Project player stopped");
	});
}

void ProjectPlayer::failCurrentProject() { this->stopCurrentProject(); }

void ProjectPlayer::transitionPlayingState(bool toPlayMode) {
	if (toPlayMode) {
		TaskManager::instance().addTask("stop_cheat", processStopCheat);
		instance().projectTasklist->clear_all_tasks();
		TaskManager::instance().addTask("project_tasks", []() -> ktwait {
			while (instance().m_state.isPlaying) {
				instance().projectTasklist->process();
				co_await 1;
			}
		});
	} else {
		TaskManager::instance().removeTask("stop_cheat");
		TaskManager::instance().removeTask("project_tasks");
		TaskManager::instance().removeTask("scene_completion_timer");
		for (size_t i = PED_TYPE_PLAYER1; i <= PED_TYPE_MISSION8; i++) {
			for (size_t j = PED_TYPE_PLAYER1; j <= PED_TYPE_MISSION8; j++) {
				plugin::Command<plugin::Commands::SET_RELATIONSHIP>(TYPE_IGNORE, i, j);
			}
		}

		plugin::Command<plugin::Commands::SET_PED_DENSITY_MULTIPLIER>(0.f);
		plugin::Command<plugin::Commands::SET_CAR_DENSITY_MULTIPLIER>(0.f);

		CWorld::Players[0].m_PlayerData.m_pWanted->SetWantedLevelNoDrop(0);
		CWanted::SetMaximumWantedLevel(0);
		FindPlayerPed()->SetWantedLevel(0);

		plugin::Command<plugin::Commands::SET_LA_RIOTS>(false);

		FindPlayerPed()->GetPlayerInfoForThisPlayerPed()->m_bDoesNotGetTired = true;
		FindPlayerPed()->ClearWeapons();
		FindPlayerPed()->m_fHealth = 100.f;
		FindPlayerPed()->m_fMaxHealth = 100.f;
		FindPlayerPed()->m_pIntelligence->ClearTasks(true, true);
		FindPlayerPed()->DettachPedFromEntity();
		CWorld::Players[0].m_nMoney = 0;
		plugin::Command<plugin::Commands::SET_PLAYER_MODEL>(0, 0);
		CClothes::RebuildPlayer(CWorld::Players[0].m_pPed, false);

		CTheScripts::bDisplayHud = true;
		CHud::bScriptDontDisplayRadar = false;
		TimerService::instance().removeTimer();
		CounterService::instance().clearAllCounters();
		TheCamera.RestoreWithJumpCut();
		TheCamera.Fade(0, 1);
		plugin::Command<plugin::Commands::CLEAR_AREA>(0.0f, 0.0f, 0.0f, 10000.f, 1);
	}
	ScenesManager::instance().resetCurrentScene();
}

std::vector<Entity*> ProjectPlayer::getEntities() {
	std::vector<Entity*> entities;
	if (isPlaying()) {
		auto& currentScene = ScenesManager::instance().getCurrentScene();
		for (const auto& entity : currentScene.entities) {
			entities.push_back(entity.get());
		}
	}
	return entities;
}