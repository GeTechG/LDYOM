#include "DamageActorObjective.h"

#include "Actor.h"

DamageActorObjective::DamageActorObjective(void *const _new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.damage_actor"));
	this->name += suffix;
}

ktwait DamageActorObjective::execute(Scene *scene, Actor *actor, Result &result, ktcoro_tasklist &tasklist) {
	while (actor->getProjectPed().value()->m_pLastEntityDamage == nullptr) {
		co_await 1;
	}
	actor->getProjectPed().value()->m_pLastEntityDamage = nullptr;
}
