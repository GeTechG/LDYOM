﻿#include "CollectPickupObjective.h"

#include <CPickups.h>

#include "Pickup.h"
#include "strUtils.h"

CollectPickupObjective::CollectPickupObjective(void* _new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.collect_pickup"));
	strlcat(this->name_.data(), suffix.c_str(), sizeof this->name_);
}

ktwait CollectPickupObjective::execute(Scene* scene, Pickup* pickup, Result& result, ktcoro_tasklist& tasklist) {
	while (!CPickups::IsPickUpPickedUp(pickup->getProjectPickupIndex().value())) {
		co_await 1;
	}
}