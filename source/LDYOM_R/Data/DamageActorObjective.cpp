#include "DamageActorObjective.h"

#include "Actor.h"
#include "strUtils.h"

DamageActorObjective::DamageActorObjective(void* const _new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.damage_actor"));
	strlcat(this->name_.data(), suffix.c_str(), sizeof this->name_);
}

ktwait DamageActorObjective::execute(Scene* scene, Actor* actor, Result& result, ktcoro_tasklist& tasklist) {
	while (actor->getProjectPed().value()->m_pLastEntityDamage == nullptr) {
		co_await 1;
	}
	actor->getProjectPed().value()->m_pLastEntityDamage = nullptr;
}
