﻿#include "KillActorOrGroupObjective.h"

#include <CTheScripts.h>
#include <extensions/ScriptCommands.h>

#include "Scene.h"
#include "utils.h"

using namespace plugin;

KillActorOrGroupObjective::KillActorOrGroupObjective(void *_new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.kill_actor_or_group"));
	this->name += suffix;
}

void KillActorOrGroupObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {
	ActorObjective::draw(local, listOverlay);

	utils::ToggleButton(local.get("actor.group").c_str(), &this->killGroup_);
}

ktwait KillActorOrGroupObjective::execute(Scene *scene, Actor *actor, Result &result, ktcoro_tasklist &tasklist) {
	std::map<Actor*, int> actorsToKill;

	if (this->killGroup_) {
		this->removeProjectBlip();
		for (const auto &sceneActor : scene->getActors()) {
			if (sceneActor->getProjectPed().has_value()) {
				if (sceneActor->getProjectPed().value()->m_nPedType == actor->getProjectPed().value()->m_nPedType) {
					if (this->getColorBlip() > 0) {
						int blip = spawnBlip(sceneActor->getProjectPed().value());
						actorsToKill.emplace(sceneActor.get(), blip);
					} else {
						actorsToKill.emplace(sceneActor.get(), 0);
					}
				}
			}
		}
	} else {
		actorsToKill.emplace(actor, this->getProjectBlip().value_or(0));
	}

	unsigned countNeedKill = actorsToKill.size();

	while (true) {
		std::vector<Actor*> killedActors;
		for (const auto &[actorToKill, blip] : actorsToKill) {
			if (actorToKill->getProjectPed().has_value()) {
				if (CTheScripts::pActiveScripts->IsPedDead(actorToKill->getProjectPed().value())) {
					Command<Commands::REMOVE_BLIP>(blip);
					killedActors.push_back(actorToKill);
					--countNeedKill;
				}
			} else {
				setObjectiveError(result, *this, NotExists,
				                  fmt::format("The entity of the actor does not exist. Actor: {}",
				                              actorToKill->getName()));
				co_return;
			}
		}
		for (const auto &killedActor : killedActors)
			actorsToKill.erase(killedActor);

		if (countNeedKill == 0) {
			break;
		}

		co_await 1;
	}
}


bool& KillActorOrGroupObjective::isKillGroup() {
	return killGroup_;
}