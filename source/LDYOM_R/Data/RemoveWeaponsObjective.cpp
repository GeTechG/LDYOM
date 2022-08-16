#include "RemoveWeaponsObjective.h"

#include "strUtils.h"
#include "common.h"

RemoveWeaponsObjective::RemoveWeaponsObjective(void* _new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.remove_weapons_player"));
	strlcat(this->name_.data(), suffix.c_str(), sizeof this->name_);
}

void RemoveWeaponsObjective::draw(Localization& local) {
}

ktwait RemoveWeaponsObjective::execute(Scene* scene, Result& result, ktcoro_tasklist& tasklist) {
	FindPlayerPed()->ClearWeapons();
	co_return;
}
