#include "RemoveWeaponsObjective.h"

#include "common.h"

RemoveWeaponsObjective::RemoveWeaponsObjective(void *_new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.remove_weapons_player"));
	this->name += suffix;
}

void RemoveWeaponsObjective::draw(Localization &local, std::vector<std::string> &listOverlay) {}

ktwait RemoveWeaponsObjective::execute(Scene *scene, Result &result, ktcoro_tasklist &tasklist) {
	FindPlayerPed()->ClearWeapons();
	co_return;
}
