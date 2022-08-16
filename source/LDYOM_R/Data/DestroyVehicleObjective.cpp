#include "DestroyVehicleObjective.h"

#include <extensions/ScriptCommands.h>

#include "strUtils.h"
#include "Vehicle.h"

using namespace plugin;

DestroyVehicleObjective::DestroyVehicleObjective(void* _new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.destroy_vehicle"));
	strlcat(this->name_.data(), suffix.c_str(), sizeof this->name_);
}

ktwait DestroyVehicleObjective::execute(Scene* scene, Vehicle* vehicle, Result& result, ktcoro_tasklist& tasklist) {
	while (!Command<Commands::IS_CAR_DEAD>(vehicle->getProjectVehicle().value())) {
		co_await 1;
	}
}