#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "objective.h"
#include <CCamera.h>
#include <CHud.h>
#include <CMessages.h>
#include <CPools.h>
#include <CStreaming.h>
#include <CTheScripts.h>
#include <blip_color_utils.h>
#include <common.h>
#include <components/actor.h>
#include <extensions/ScriptCommands.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <ktcoro_wait.hpp>
#include <localization.h>
#include <memory>
#include <objectives_manager.h>
#include <project_player.h>
#include <scenes_manager.h>
#include <string_utils.h>
#include <utils/imgui_configurate.h>
#include <utils/objective_utils.h>
#include <utils_entities_selections.h>
#include <window_manager.h>

namespace objectives::cutscene {
constexpr const char* TYPE = "core.cutscene";

struct Data {
	int behaviour = 0;
	std::array<float, 3> cameraPosition;
	std::array<float, 3> targetPosition;
	std::string actorUuid;
	bool slowMotion = false;
	bool fadeInOut = false;
	bool shaking = false;
	bool wideScreen = false;
	std::string text;
	float textTime = 1.f;

	std::string gameText;

	static void sol_lua_register(sol::state_view lua_state) {
		auto ut = lua_state.new_usertype<Data>("ObjectiveCutsceneData");
		SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Data, behaviour, cameraPosition, targetPosition, actorUuid,
		                 slowMotion, fadeInOut, shaking, wideScreen, text, textTime);
	}
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Data, behaviour, cameraPosition, targetPosition, actorUuid, slowMotion, fadeInOut,
	                               shaking, wideScreen, text, textTime);
};

inline std::string tr(const std::string& key) { return _(fmt::format("objectives.{}.{}", std::string(TYPE), key)); }

void renderEditor(Data& data);

inline ktwait execute(Data& data) {
	static auto duration = 0;
	static auto isSkipped = false;

	auto currentObjectiveIndex = ProjectPlayer::instance().getCurrentObjectiveIndex();
	auto& objectives = ScenesManager::instance().getUnsafeCurrentScene().objectives.data;

	auto jumpcut = true;
	auto doFadeOutAfter = false;

	// Do fade out then fade in if this is NOT continuing from previous cutscene
	// (as in DYOM: fade out at lines 19983-19987, then fade in at 20537-20541)
	if (data.fadeInOut &&
	    !objective_utils::isLastInterruptingObjectiveOfType(objectives, currentObjectiveIndex, TYPE)) {
		// First: Fade OUT to black
		plugin::Command<plugin::Commands::DO_FADE>(500, 0);
		co_await 600; // Wait 600ms for fade to complete
		ProjectPlayer::instance().setFaded(true);

		// Then immediately: Fade IN from black (so cutscene is visible!)
		plugin::Command<plugin::Commands::DO_FADE>(500, 1);
		ProjectPlayer::instance().setFaded(false);
	}

	// Determine if we need fade out AFTER cutscene (as in DYOM lines 19917-19927)
	// If next objective is NOT a cutscene, fade out at end
	if (data.fadeInOut &&
	    !objective_utils::isNextInterruptingObjectiveOfType(objectives, currentObjectiveIndex, TYPE)) {
		doFadeOutAfter = true;
	}

	// Skip jumpcut if next interrupting objective is also a cutscene
	if (objective_utils::isNextInterruptingObjectiveOfType(objectives, currentObjectiveIndex, TYPE)) {
		jumpcut = false;
	}

	std::shared_ptr<components::Actor> targetActor;
	if (data.behaviour > 2 && data.behaviour < 6) {
		auto entities = ProjectPlayer::instance().getEntities();
		auto actorUuid = uuids::uuid::from_string(data.actorUuid);
		for (auto& entity : entities) {
			auto actor = components::Actor::cast(entity->getComponent(components::Actor::TYPE));
			if (!actor)
				continue;
			if (entity->id == actorUuid && actor->ped) {
				targetActor = actor;
			}
		}
	}

	TheCamera.m_bWideScreenOn = data.wideScreen;
	CTheScripts::bDisplayHud = false;
	CHud::bScriptDontDisplayRadar = true;
	plugin::Command<plugin::Commands::SET_TIME_SCALE>(data.slowMotion ? 0.2f : 1.0f);

	auto time = int(data.textTime * 1000.0f);
	CPed* targetPed = nullptr;
	std::shared_ptr<CPed> cameraPed;

	if (data.behaviour > 2 && data.behaviour < 6) {
		if (targetActor && targetActor->ped) {
			targetPed = &*targetActor->ped;
		}
	} else if (data.behaviour > 5) {
		targetPed = FindPlayerPed();
	}

	plugin::Command<plugin::Commands::CAMERA_PERSIST_TRACK>(false);
	plugin::Command<plugin::Commands::CAMERA_PERSIST_POS>(false);
	plugin::Command<plugin::Commands::CAMERA_RESET_NEW_SCRIPTABLES>();

	if (data.shaking) {
		plugin::Command<plugin::Commands::CAMERA_SET_SHAKE_SIMULATION_SIMPLE>(5, 99000.0f, 50.0f);
	} else {
		plugin::Command<plugin::Commands::CAMERA_SET_SHAKE_SIMULATION_SIMPLE>(1, 0.0, 0.0);
	}

	switch (data.behaviour) {
		case 0:
			{
				plugin::Command<plugin::Commands::SET_FIXED_CAMERA_POSITION>(
					data.cameraPosition[0], data.cameraPosition[1], data.cameraPosition[2], 0.f, 0.f, 0.f);
				plugin::Command<plugin::Commands::POINT_CAMERA_AT_POINT>(data.targetPosition[0], data.targetPosition[1],
				                                                         data.targetPosition[2], 2);
				break;
			}
		case 1:
		case 2:
			{
				plugin::Command<plugin::Commands::CAMERA_PERSIST_TRACK>(true);
				plugin::Command<plugin::Commands::CAMERA_PERSIST_POS>(true);
				float activeX, activeY, activeZ;
				plugin::Command<plugin::Commands::GET_ACTIVE_CAMERA_COORDINATES>(&activeX, &activeY, &activeZ);
				plugin::Command<plugin::Commands::CAMERA_SET_VECTOR_MOVE>(
					activeX, activeY, activeZ, data.cameraPosition[0], data.cameraPosition[1], data.cameraPosition[2],
					time, data.behaviour - 1);
				float activeTargetX, activeTargetY, activeTargetZ;
				plugin::Command<plugin::Commands::GET_ACTIVE_CAMERA_POINT_AT>(&activeTargetX, &activeTargetY,
				                                                              &activeTargetZ);
				plugin::Command<plugin::Commands::CAMERA_SET_VECTOR_TRACK>(
					activeTargetX, activeTargetY, activeTargetZ, data.targetPosition[0], data.targetPosition[1],
					data.targetPosition[2], time, data.behaviour - 1);
				break;
			}
		case 3:
		case 6:
			{
				if (targetPed) {
					// Create invisible ped at camera position (as in original DYOM)
					CStreaming::RequestModel(0, GAME_REQUIRED);
					CStreaming::LoadAllRequestedModels(false);

					int newPed;
					plugin::Command<plugin::Commands::CREATE_CHAR>(4, 0, data.cameraPosition[0], data.cameraPosition[1],
					                                               data.cameraPosition[2], &newPed);

					CPed* ped = CPools::GetPed(newPed);
					cameraPed = std::shared_ptr<CPed>(ped, [](CPed* ped) {
						if (ped) {
							auto ref = CPools::GetPedRef(ped);
							plugin::Command<plugin::Commands::DELETE_CHAR>(ref);
						}
					});

					CStreaming::SetMissionDoesntRequireModel(0);

					// Setup invisible camera ped
					plugin::Command<plugin::Commands::FREEZE_CHAR_POSITION_AND_DONT_LOAD_COLLISION>(&*cameraPed, true);
					plugin::Command<plugin::Commands::SET_CHAR_PROOFS>(&*cameraPed, true, true, true, true, true);
					plugin::Command<plugin::Commands::SET_CHAR_COLLISION>(&*cameraPed, false);
					plugin::Command<plugin::Commands::SET_CHAR_VISIBLE>(&*cameraPed, false);

					// Attach camera to invisible ped looking at target actor
					plugin::Command<plugin::Commands::ATTACH_CAMERA_TO_CHAR_LOOK_AT_CHAR>(&*cameraPed, 0.0f, 0.0f,
					                                                                      -1.0f, targetPed, 0.0f, 2);
				}
				break;
			}
		case 4:
		case 7:
			{
				if (targetPed) {
					plugin::Command<plugin::Commands::ATTACH_CAMERA_TO_CHAR>(targetPed, 0.0f, 0.2f, 1.0f, 0.0f, 1.0f,
					                                                         1.0f, 0.0f, 2);
					plugin::Command<plugin::Commands::CAMERA_SET_SHAKE_SIMULATION_SIMPLE>(3, 1000000.0f, 1.0f);
				}
				break;
			}
		case 5:
		case 8:
			{
				if (targetPed) {
					plugin::Command<plugin::Commands::ATTACH_CAMERA_TO_CHAR>(targetPed, 0.0f, -4.0f, 1.0f, 0.0f, 0.0f,
					                                                         0.5f, 0.0f, 2);
				}
				break;
			}
		default: break;
	}

	duration = isSkipped ? 0 : time;

	if (!data.text.empty()) {
		auto cp1251Text = utf8_to_cp1251(data.text);
		gxt_encode(cp1251Text);
		data.gameText = cp1251Text;

		CMessages::AddMessage(const_cast<char*>(data.gameText.c_str()), duration, 0, false);
	}

	auto lastTime = CTimer::m_snTimeInMilliseconds;
	while (duration > 0) {
		auto currentTime = CTimer::m_snTimeInMilliseconds;
		duration -= currentTime - lastTime;
		if (plugin::Command<plugin::Commands::IS_BUTTON_PRESSED>(0, 16)) {
			duration = 0;
			isSkipped = true;
		}
		lastTime = currentTime;
		co_await 1;
	}

	// Fade out at END of cutscene if next objective is not a cutscene (DYOM lines 19917-19927)
	if (doFadeOutAfter && !ProjectPlayer::instance().isFaded()) {
		plugin::Command<plugin::Commands::DO_FADE>(500, 0); // Fade OUT to black
		co_await 600;                                       // Wait 600ms for fade to complete
		ProjectPlayer::instance().setFaded(true);           // Mark screen as black ($DYOM_faded = 1)
	}

	{
		cameraPed.reset();
		float activeX, activeY, activeZ;
		plugin::Command<plugin::Commands::GET_ACTIVE_CAMERA_COORDINATES>(&activeX, &activeY, &activeZ);
		plugin::Command<plugin::Commands::SET_FIXED_CAMERA_POSITION>(activeX, activeY, activeZ, 0.f, 0.f, 0.f);
		float activeTargetX, activeTargetY, activeTargetZ;
		plugin::Command<plugin::Commands::GET_ACTIVE_CAMERA_POINT_AT>(&activeTargetX, &activeTargetY, &activeTargetZ);
		plugin::Command<plugin::Commands::POINT_CAMERA_AT_POINT>(activeTargetX, activeTargetY, activeTargetZ, 2);
	}

	plugin::Command<plugin::Commands::CAMERA_PERSIST_POS>(false);
	plugin::Command<plugin::Commands::CAMERA_PERSIST_TRACK>(false);
	plugin::Command<plugin::Commands::CAMERA_RESET_NEW_SCRIPTABLES>();
	if (jumpcut) {
		isSkipped = false;
		plugin::Command<plugin::Commands::RESTORE_CAMERA_JUMPCUT>();
		TheCamera.m_bWideScreenOn = false;
		CTheScripts::bDisplayHud = true;
		CHud::bScriptDontDisplayRadar = false;
		plugin::Command<plugin::Commands::SET_TIME_SCALE>(1.0f);
	}
}

inline Objective make() {
	auto cameraPosition = TheCamera.GetPosition();
	auto targetPosition = TheCamera.GetPosition() + TheCamera.GetForward() * 10.f;

	return Objective(TYPE, _("objectives." + std::string(TYPE) + ".name"),
	                 Data{
						 .cameraPosition = {cameraPosition.x, cameraPosition.y, cameraPosition.z},
						 .targetPosition = {targetPosition.x, targetPosition.y, targetPosition.z},
					 },
	                 std::function<void(Data&)>(renderEditor), std::function<ktwait(Data&)>(execute));
}

inline ObjectiveBuilderData builder() { return ObjectiveBuilderData{.type = TYPE, .category = "", .builder = make}; }
} // namespace objectives::cutscene