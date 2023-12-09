#include "ProjectPlayerService.h"

#include <CCamera.h>
#include <CClock.h>
#include <CClothes.h>
#include <CHud.h>
#include <CTheScripts.h>
#include <CWeather.h>
#include <CWorld.h>

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "CFireManager.h"
#include "DiscordService.h"
#include "imgui_notify.h"
#include "LuaEngine.h"
#include "ProjectsService.h"
#include "Tasker.h"
#include "TimerService.h"
#include "utils.h"
#include "WindowsRenderService.h"
#include "../Data/Result.h"
#include "boost/functional/hash.hpp"
#include "easylogging/easylogging++.h"
#include "extensions/ScriptCommands.h"

namespace Windows {
	class AbstractWindow;
}

extern std::optional<Windows::AbstractWindow*> defaultWindow;
extern bool openWindowsMenu;

// ReSharper disable once CppMemberFunctionMayBeStatic
ktwait ProjectPlayerService::changeScene(Scene *scene, ktcoro_tasklist &tasklist, int startObjective) {
	std::unordered_map<boost::uuids::uuid, std::vector<ObjectiveDependent*>, boost::hash<boost::uuids::uuid>> spawnMap;
	std::unordered_map<boost::uuids::uuid, std::vector<ObjectiveDependent*>, boost::hash<boost::uuids::uuid>> deleteMap;

	auto addObjectiveDependedEntity = [&spawnMap, &deleteMap]<typename T>(std::vector<std::unique_ptr<T>> &entities) {
		for (const auto &entityT : entities) {
			auto entity = static_cast<ObjectiveDependent*>(entityT.get());
			if (entity->isUseObjective()) {
				spawnMap[entity->getSpawnObjectiveUuid()].emplace_back(entity);
				deleteMap[entity->getDeleteObjectiveUuid()].emplace_back(entity);
			}
		}
	};

	for (const auto &objective : scene->getObjectives()) {
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

	for (const auto &audio : scene->getAudio()) {
		if (audio->isUseObjective())
			audio->preloadProjectAudio();
	}

	//apply scene settings
	if (scene->isToggleSceneSettings()) {
		using namespace plugin;

		const auto &sceneSettings = scene->getSceneSettings();
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
						Command<Commands::SET_RELATIONSHIP>(sceneSettings.groupRelations[i][j], 24 + i, 0);
						Command<Commands::SET_RELATIONSHIP>(sceneSettings.groupRelations[i][j], 24 + i, 23);
						Command<Commands::SET_RELATIONSHIP>(sceneSettings.groupRelations[i][j], 23, 24 + i);
					} else {
						if (i != j - 1) {
							Command<Commands::SET_RELATIONSHIP>(sceneSettings.groupRelations[i][j], 24 + i, 24 + j - 1);
						}
					}
				}
			}
		}
	}


	{
		using namespace plugin;

		tasklist.add_task([](ProjectPlayerService *_this) -> ktwait {
			while (true) {
				if (Command<0x0ADC>("LDSTOP")) {
					break;
				}
				if (CWorld::Players[0].m_nPlayerState != PLAYERSTATE_PLAYING) {
					Command<Commands::SET_PLAYER_MODEL>(0, 0);
					while (CWorld::Players[0].m_nPlayerState != PLAYERSTATE_PLAYING)
						co_await 1;
					break;
				}
				co_await 1;
			}
			_this->stopCurrentScene();
		}, this);
	}

	/*const auto onStartSignals = LuaEngine::getInstance().getLuaState()["global_data"]["signals"]["onStartScene"].get<sol::table>();
	for (auto [_, func] : onStartSignals) {
		if (const auto result = func.as<sol::function>()(scene, tasklist); !result.valid()) {
			const sol::error err = result;
			CLOG(ERROR, "lua") << err.what();
		}
	}*/

	tasklist.add_task([](Scene *scene, ktcoro_tasklist &tasklist) -> ktwait {
		while (true) {
			/*const auto onMainLoop = LuaEngine::getInstance().getLuaState()["global_data"]["signals"]["onMainLoop"].get_or_create<sol::table>();
			for (auto [_, func] : onMainLoop) {
				if (const auto result = func.as<sol::function>()(scene, tasklist); !result.valid()) {
					const sol::error err = result;
					CLOG(ERROR, "lua") << err.what();
				}
			}*/
			co_await 1;
		}
	}, scene, tasklist);

	if (this->save_.has_value()) {
		using namespace plugin;

		Command<Commands::SET_PLAYER_MODEL>(0, this->save_.value()->playerModel);
		const auto playerClothes = CWorld::Players[0].m_pPed->m_pPlayerData->m_pPedClothesDesc;
		std::memcpy(playerClothes->m_anTextureKeys, this->save_.value()->clotherMAnTextureKeys_.data(),
		            sizeof playerClothes->m_anTextureKeys);
		std::memcpy(playerClothes->m_anModelKeys, this->save_.value()->clotherMAnModelKeys_.data(),
		            sizeof playerClothes->m_anModelKeys);
		playerClothes->m_fFatStat = this->save_.value()->fatStat;
		playerClothes->m_fMuscleStat = this->save_.value()->musculeStat;
		CClothes::RebuildPlayer(CWorld::Players[0].m_pPed, false);
		FindPlayerPed()->Teleport(CVector(this->save_.value()->playerPosition[0],
		                                  this->save_.value()->playerPosition[1],
		                                  this->save_.value()->playerPosition[2]), false);

		/*if (this->save_.value()->nodePinId != -1) {
			auto nodeEditorContext = LuaEngine::getInstance().getLuaState()["global_data"]["ed_contexts"][scene->getId()];
			nodeEditorContext["callNodes"](nodeEditorContext, scene, tasklist, this->save_.value()->nodePinId);
		}*/
	}

	for (int o = startObjective; o < static_cast<int>(scene->getObjectives().size()); ++o) {
		if (this->nextObjective.has_value()) {
			o = this->nextObjective.value();
			this->nextObjective = std::nullopt;
		}

		const auto &objective = scene->getObjectives().at(o);
		this->currentObjective_ = objective.get();

		for (const auto &dependent : spawnMap[objective->getUuid()]) {
			try {
				dependent->spawnProjectEntity();
			} catch (const std::exception &e) {
				CLOG(ERROR, "LDYOM") << "Failed spawn entity on objective \"" << objective->getName() << "\", error: "
					<< e.what();
				ImGui::InsertNotification({ImGuiToastType_Error, 1000, "Error, see log"});
			}
		}


		/*const auto onObjectiveStart = LuaEngine::getInstance().getLuaState()["global_data"]["signals"]["onObjectiveStart"].get_or_create<sol::table>();
		for (auto [_, func] : onObjectiveStart) {
			if (const auto result = func.as<sol::function>()(scene, tasklist, objective->getUuid()); !result.valid()) {
				const sol::error err = result;
				CLOG(ERROR, "lua") << err.what();
			}
		}*/

		Result result;
		co_await objective->execute(scene, result, tasklist);

		if (!result.isDone()) {
			CLOG(ERROR, "LDYOM") << result;
			ImGui::InsertNotification({ImGuiToastType_Error, 5000, result.str().c_str()});
			co_return;
		}

		if (o == static_cast<int>(scene->getObjectives().size() - 1)) {
			using namespace plugin;

			CPed *playerPed = FindPlayerPed();
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

		for (const auto &dependent : deleteMap[objective->getUuid()])
			dependent->deleteProjectEntity();

		/*const auto onObjectiveEnd = LuaEngine::getInstance().getLuaState()["global_data"]["signals"]["onObjectiveEnd"].get_or_create<sol::table>();
		for (auto [_, func] : onObjectiveEnd) {
			if (const auto result = func.as<sol::function>()(scene, tasklist, objective->getUuid()); !result.valid()) {
				const sol::error err = result;
				CLOG(ERROR, "lua") << err.what();
			}
		}*/
	}

	/*const auto onEndSignals = LuaEngine::getInstance().getLuaState()["global_data"]["signals"]["onEndScene"].get<sol::table>();
	for (auto [_, func] : onStartSignals) {
		if (const auto result = func.as<sol::function>()(scene, tasklist); !result.valid()) {
			const sol::error err = result;
			CLOG(ERROR, "lua") << err.what();
		}
	}*/
}

void ProjectPlayerService::setNextScene(Scene *nextScene) {
	this->nextScene = std::make_optional(nextScene);
}

void ProjectPlayerService::setNextObjective(int objective) {
	this->nextObjective = objective;
}

void ProjectPlayerService::setSave(const std::optional<SaveData*> &save) {
	save_ = save;
}

ktwait ProjectPlayerService::startProject(int sceneIdx, int startObjective) {
	using namespace plugin;

	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->unloadEditorScene();
	const auto savedWindow = defaultWindow;
	defaultWindow = std::nullopt;
	Windows::WindowsRenderService::getInstance().closeAllWindows();
	Command<Commands::SET_CHAR_PROOFS>(static_cast<CPed*>(FindPlayerPed()), 0, 0, 0, 0, 0);
	this->projectRunning = true;

	const auto projectName = ProjectsService::getInstance().getCurrentProject().getProjectInfo()->name;
	DiscordService::getInstance().updateActivity(projectName, DiscordActivityType::PLAYING);

	const auto startScene = ProjectsService::getInstance().getCurrentProject().getScenes().at(sceneIdx).get();
	setNextScene(startScene);
	setNextObjective(startObjective);

	if (this->save_.has_value()) {
		auto nextScene = ProjectsService::getInstance().getCurrentProject().getScenes().at(this->save_.value()->sceneId)
		                                               .get();
		setNextScene(nextScene);

		auto objectiveIndex = utils::indexByUuid(nextScene->getObjectives(),
		                                         boost::lexical_cast<boost::uuids::uuid>(
			                                         this->save_.value()->objectiveUuid));
		if (objectiveIndex != -1) {
			objectiveIndex = min(objectiveIndex + 1, nextScene->getObjectives().size() - 1);
			setNextObjective(objectiveIndex);
		}

		/*auto& luaState = LuaEngine::getInstance().getLuaState();

		const sol::table luaData = luaState["bitser"]["loads"](luaState["base64"]["decode"](this->save_.value()->luaData));

		for (auto pairLua : luaState["global_data"]["signals"]["loadGame"].get_or_create<sol::table>()) {
			if (auto resultLua = pairLua.second.as<sol::function>()(nextScene->getId(), luaData); !resultLua.valid()) {
				sol::error err = resultLua;
				CLOG(ERROR, "lua") << err.what();
			}
		}*/
	}

	/*const auto onStartSignals = LuaEngine::getInstance().getLuaState()["global_data"]["signals"]["onStartProject"].get<sol::table>();
	for (auto [_, func] : onStartSignals) {
		if (auto result = func.as<sol::function>()(); !result.valid()) {
			sol::error err = result;
			CLOG(ERROR, "lua") << err.what();
		}
	}*/

	while (this->nextScene.has_value()) {
		const auto scene = this->nextScene.value();
		this->nextScene = std::nullopt;
		this->currentScene = scene;

		ktcoro_tasklist ts;
		this->sceneTasklist = &ts;
		Tasker::getInstance().getInstance().addTask("sceneProcces", [](ktcoro_tasklist &ts) -> ktwait {
			while (true) {
				ts.process();
				co_await 1;
			}
		}, ts);
		const auto startObjectiveScene = this->nextObjective.value_or(0);
		this->nextObjective = std::nullopt;
		auto taskScene = changeScene(scene, ts, startObjectiveScene);
		this->currentSceneTask.emplace(std::move(taskScene));
		co_await ktwait(this->currentSceneTask.value().coro_handle);

		{
			using namespace plugin;

			CPed *playerPed = FindPlayerPed();
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

		Tasker::getInstance().getInstance().removeTask("sceneProcces");

		scene->unloadProjectScene();
	}

	for (auto &fire : gFireManager.m_aFires) {
		fire.Extinguish();
	}

	TimerService::getInstance().removeTimer();
	CTheScripts::ScriptSpace[CTheScripts::OnAMissionFlag] = 0;

	Command<Commands::SET_CHAR_PROOFS>(static_cast<CPed*>(FindPlayerPed()), 1, 1, 1, 1, 1);
	Command<Commands::SET_LA_RIOTS>(0);
	Command<Commands::SET_PED_DENSITY_MULTIPLIER>(0.f);
	Command<Commands::SET_CAR_DENSITY_MULTIPLIER>(0.f);
	CWorld::Players[0].m_PlayerData.m_pWanted->SetWantedLevelNoDrop(0);
	CWanted::SetMaximumWantedLevel(0);
	FindPlayerPed()->SetWantedLevel(0);
	FindPlayerPed()->ClearWeapons();
	FindPlayerPed()->m_fHealth = 100.f;
	FindPlayerPed()->m_fMaxHealth = 100.f;
	FindPlayerPed()->m_pIntelligence->ClearTasks(true, true);
	CWorld::Players[0].m_nMoney = 0;
	Command<Commands::SET_PLAYER_MODEL>(0, 0);
	CClothes::RebuildPlayer(CWorld::Players[0].m_pPed, false);
	CTheScripts::bDisplayHud = true;
	CHud::bScriptDontDisplayRadar = false;
	TheCamera.RestoreWithJumpCut();
	Command<Commands::DO_FADE>(0, 1);
	CWorld::ClearExcitingStuffFromArea(FindPlayerPed()->GetPosition(), 999999.f, true);

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 9; j++) {
			if (j == 0) {
				Command<Commands::SET_RELATIONSHIP>(0, 24 + i, 0);
				Command<Commands::SET_RELATIONSHIP>(0, 24 + i, 23);
				Command<Commands::SET_RELATIONSHIP>(0, 23, 24 + i);
			} else {
				if (i != j - 1) {
					Command<Commands::SET_RELATIONSHIP>(0, 24 + i, 24 + j - 1);
				}
			}
		}
	}
	this->save_ = std::nullopt;

	ProjectsService::getInstance().getCurrentProject().getCurrentScene()->loadEditorScene();
	defaultWindow = savedWindow;
	openWindowsMenu = false;

	this->projectRunning = false;

	DiscordService::getInstance().updateActivity(projectName, DiscordActivityType::CREATING);
}

bool& ProjectPlayerService::isProjectRunning() {
	return projectRunning;
}

void ProjectPlayerService::reset() {
	this->projectRunning = false;
}

void ProjectPlayerService::stopCurrentScene() {
	Tasker::getInstance().getKtcoroTaskList().remove_task(this->currentSceneTask.value());
}

std::optional<Scene*>& ProjectPlayerService::getCurrentScene() {
	return currentScene;
}

ktcoro_tasklist*& ProjectPlayerService::getSceneTasklist() {
	return sceneTasklist;
}

std::optional<BaseObjective*>& ProjectPlayerService::getCurrentObjective() {
	return currentObjective_;
}
