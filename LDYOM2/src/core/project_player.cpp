#define IMGUI_DEFINE_MATH_OPERATORS
#include "project_player.h"
#include <algorithm>
#include "global_vars_service.h"
#include "application.h"
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
#include <utils/objective_utils.h>
#include <utils/string_utils.h>
#include <uuid_wrap.h>
#include <window_manager.h>
#include <windows/node_editor.h>

ProjectPlayer::~ProjectPlayer() { this->projectTasklist->clear_all_tasks(); }

ktwait ProjectPlayer::run() {
	// Mission start: fade in briefly, then fade out (original LDYOM2 behavior for smooth transition)
	TheCamera.Fade(0.5f, FADE_IN);
	co_await 500;

	instance().m_state.isPlaying = true;
	instance().transitionPlayingState(true);
	LDYOM_INFO("Project player started");

	TheCamera.Fade(0.5f, FADE_OUT);
	instance().m_state.isFaded = true; // Mark as faded for first objective

	co_await runScenesLoop();
}

ktwait ProjectPlayer::runScenesLoop() {
	bool continueRunning = true;
	while (continueRunning) {
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

		instance().onSceneStarted(instance().m_state.currentSceneId);

		auto& objectives = ScenesManager::instance().getCurrentScene().objectives.data;

		int startIndex = 0;
		if (instance().m_state.pendingObjectiveJump.has_value()) {
			const std::string& targetId = *instance().m_state.pendingObjectiveJump;
			auto it = std::find_if(objectives.begin(), objectives.end(),
				[&](const auto& obj) { return uuids::to_string(obj.id) == targetId; });
			if (it != objectives.end()) {
				startIndex = static_cast<int>(std::distance(objectives.begin(), it));
			} else {
				LDYOM_WARN("pendingObjectiveJump '{}' not found in scene '{}', starting from 0", targetId, instance().m_state.currentSceneId);
			}
			instance().m_state.pendingObjectiveJump.reset();
		}

		for (int i = startIndex; i < static_cast<int>(objectives.size()); i++) {
			instance().m_state.currentObjectiveIndex = i;
			auto& objective = ObjectivesManager::instance().getUnsafeObjective(i);
			auto objectiveType = objective.type;

			auto shouldClearTasks =
				objective_utils::isLastInterruptingObjectiveOfType(objectives, i, "core.cutscene") ||
				objective_utils::isLastInterruptingObjectiveOfType(objectives, i, "core.player_animation");

			if (shouldClearTasks && objectiveType != "core.cutscene" && objectiveType != "core.player_animation") {
				auto player = FindPlayerPed();
				if (player) {
					plugin::Command<plugin::Commands::CLEAR_CHAR_TASKS>(player);
				}
			}

			if (!objective_utils::isLastInterruptingObjectiveOfType(objectives, i, "core.cutscene") &&
			    objectiveType == "core.cutscene") {
				plugin::Command<plugin::Commands::DO_FADE>(500, 0);
				co_await 600; // Wait 600ms for fade to complete
				ProjectPlayer::instance().setFaded(true);
			} else if (objectiveType != "core.mission_start") {
				// Centralized fade in before each objective (as in DYOM lines 20537-20541)
				// Only fade in if screen is currently black ($DYOM_faded == 1).
				// core.mission_start is exempt: it owns the fade-in so its fadeIn flag and
				// banner timing stay in sync. If mission_start is authored mid-project and
				// fadeIn=false, the screen will still be revealed instantly by mission_start.
				if (instance().m_state.isFaded) {
					plugin::Command<plugin::Commands::DO_FADE>(500, 1); // Fade IN from black
					instance().m_state.isFaded = false;
				}
			}

			instance().onObjectiveStarted(i);
			co_await objective.execute();
			instance().onObjectiveCompleted(i);

			if (instance().m_state.pendingSceneTransition.has_value()) {
				break;
			}

			if (instance().m_state.pendingObjectiveJump.has_value()) {
				std::string targetId = instance().m_state.pendingObjectiveJump.value();
				instance().m_state.pendingObjectiveJump.reset();
				int jumpIndex = -1;
				for (int j = 0; j < static_cast<int>(objectives.size()); j++) {
					if (uuids::to_string(objectives[j].id) == targetId) {
						jumpIndex = j;
						break;
					}
				}
				if (jumpIndex >= 0) {
					i = jumpIndex - 1; // -1 because loop will increment
				}
			}
		}

		instance().onSceneEnded(instance().m_state.currentSceneId);

		if (instance().m_state.pendingSceneTransition.has_value()) {
			auto pending = instance().m_state.pendingSceneTransition.value();
			instance().m_state.pendingSceneTransition.reset();
			instance().m_state.currentSceneId = pending.sceneId;

			if (!pending.instant) {
				if (!instance().m_state.isFaded) {
					plugin::Command<plugin::Commands::DO_FADE>(500, 0);
					co_await 600;
					instance().m_state.isFaded = true;
				}
			} else {
				instance().m_state.isFaded = false;
			}

			Application::instance().luaTaskManager().cancelAll();
			instance().projectTasklist->clear_all_tasks();
			ScenesManager::instance().resetCurrentScene();
			ScenesManager::instance().loadScene(pending.sceneId);
			LDYOM_INFO("Scene transition to: {}", pending.sceneId);
			co_await 1;
			// continueRunning stays true — loop repeats with new scene
		} else {
			continueRunning = false;
		}
	}

	co_await ProjectPlayer::playerLeaveAnyVehicle();

	// Final fade in at mission end (as in DYOM lines 22199-22203)
	if (instance().m_state.isFaded) {
		plugin::Command<plugin::Commands::DO_FADE>(500, 1); // Fade IN from black
		instance().m_state.isFaded = false;
	}

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
	ScenesManager::instance().saveCurrentScene();
	this->m_state.currentSceneId = sceneId.empty() ? currentProject.value()->startSceneId : std::string(sceneId);
	GlobalVarsService::instance().reset();

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
		if (ProjectPlayer::instance().m_state.isFaded) {
			plugin::Command<plugin::Commands::DO_FADE>(MISSION_FADE_TIME_MS, 1); // Fade IN from black
			co_await (MISSION_FADE_TIME_MS + 100);
			ProjectPlayer::instance().m_state.isFaded = false;
		}
		ProjectPlayer::instance().m_state.isPlaying = false;
		ProjectPlayer::instance().transitionPlayingState(false);
		LDYOM_INFO("Project player stopped");
	});
}

void ProjectPlayer::failCurrentProject() {
	if (!m_state.missionMode.active) {
		stopCurrentProject();
		return;
	}
	TaskManager::instance().removeTask("run_project_player");
	TaskManager::instance().addTask("mission_fail_sequence", missionFailSequence);
}

ktwait ProjectPlayer::missionFailSequence() {
	auto& missionMode = instance().m_state.missionMode;
	int localFailTextMode = missionMode.failTextMode;
	std::string localFailTextGxt = missionMode.failTextGxt;
	MissionFailAction localFailAction = missionMode.failAction;
	missionMode.active = false; // re-entrancy guard BEFORE any co_await

	plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(0, false);
	if (localFailTextMode == 0) {
		// Default: built-in M_FAIL GXT entry (SCM opcode style 1 == eMessageStyle::STYLE_MIDDLE after -1 inside handler)
		plugin::Command<plugin::Commands::PRINT_BIG>("M_FAIL", MISSION_FAIL_TEXT_TIME_MS, 1);
	} else {
		CMessages::AddBigMessage((char*)localFailTextGxt.c_str(), MISSION_FAIL_TEXT_TIME_MS, STYLE_MIDDLE);
	}
	co_await MISSION_FAIL_TEXT_TIME_MS;

	// 4.2 Fade-out before action
	bool fadeOut = std::visit([](const auto& a) { return a.fadeOut; }, localFailAction);
	if (fadeOut && !instance().isFaded()) {
		plugin::Command<plugin::Commands::DO_FADE>(MISSION_FADE_TIME_MS, 0);
		co_await (MISSION_FADE_TIME_MS + 100);
		instance().setFaded(true);
	}

	// 4.3
	plugin::Command<plugin::Commands::SET_PLAYER_CONTROL>(0, true);

	// 4.4 Dispatch fail action
	std::visit([](const auto& action) {
		using T = std::decay_t<decltype(action)>;
		if constexpr (std::is_same_v<T, mission_fail_actions::EndProject>) {
			ProjectPlayer::instance().stopCurrentProject();
		} else if constexpr (std::is_same_v<T, mission_fail_actions::RestartScene>) {
			navigationalArmContinue(instance().m_state.currentSceneId, std::nullopt);
		} else if constexpr (std::is_same_v<T, mission_fail_actions::GotoScene>) {
			navigationalArmContinue(action.sceneId, action.objectiveId);
		}
	}, localFailAction);
}

// 4.5 Helper for navigational fail arms (RestartScene / GotoScene)
void ProjectPlayer::navigationalArmContinue(const std::string& sceneId, const std::optional<std::string>& objectiveId) {
	if (!instance().isPlaying()) {
		return; // Guard against LDSTOP-during-fade-out edge case
	}

	auto& scenesInfo = ScenesManager::instance().getScenesInfo();
	auto it = std::find_if(scenesInfo.begin(), scenesInfo.end(),
	                       [&](const SceneInfo& si) { return si.id == sceneId; });
	if (it == scenesInfo.end()) {
		LDYOM_ERROR("navigational fail action references unknown scene id '{}'", sceneId);
		instance().stopCurrentProject();
		return;
	}

	// Clear lingering fail banner so the next scene's mission_start title slot is free.
	CMessages::ClearThisPrintBigNow(STYLE_MIDDLE);
	CMessages::ClearThisPrintBigNow(STYLE_BOTTOM_RIGHT);

	CTheScripts::OnAMissionFlag = 0;
	instance().clearMissionMode();
	Application::instance().luaTaskManager().cancelAll();
	instance().projectTasklist->clear_all_tasks();
	ScenesManager::instance().resetCurrentScene();
	ScenesManager::instance().loadScene(sceneId);
	instance().m_state.currentSceneId = sceneId;

	if (objectiveId) {
		instance().requestObjectiveJump(*objectiveId);
	}

	TaskManager::instance().addTask("run_project_player", runScenesLoop);
}

void ProjectPlayer::requestSceneTransition(std::string_view sceneId, bool instant) {
	m_state.pendingSceneTransition = PendingTransition{std::string(sceneId), instant};
}

void ProjectPlayer::requestObjectiveJump(std::string_view objectiveId) {
	m_state.pendingObjectiveJump = std::string(objectiveId);
}

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
		if (auto nodeEditor = WindowManager::instance().getWindowAs<NodeEditorWindow>("node_editor");
		    nodeEditor && *nodeEditor) {
			(*nodeEditor)->taskManager().cancelAll();
		}
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

		// Reset fade state for next mission
		instance().m_state.isFaded = false;
		TaskManager::instance().removeTask("mission_fail_sequence");
		CTheScripts::OnAMissionFlag = 0;
		instance().m_state.missionMode = {};
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