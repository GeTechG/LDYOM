#include "DestroyVehicleObjective.h"

#include <extensions/ScriptCommands.h>

#include "Vehicle.h"

using namespace plugin;

DestroyVehicleObjective::DestroyVehicleObjective(void *_new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.destroy_vehicle"));
	this->name += suffix;
}

ktwait DestroyVehicleObjective::execute(Scene *scene, Vehicle *vehicle, Result &result, ktcoro_tasklist &tasklist) {
	while (!Command<Commands::IS_CAR_DEAD>(vehicle->getProjectVehicle().value())) {
		co_await 1;
	}
}
