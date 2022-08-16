#include "GetInVehicleObjective.h"

#include <extensions/ScriptCommands.h>
#include <common.h>

#include "strUtils.h"
#include "Vehicle.h"

using namespace plugin;

GetInVehicleObjective::GetInVehicleObjective(void* _new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.get_in_vehicle"));
	strlcat(this->name_.data(), suffix.c_str(), sizeof this->name_);
}

ktwait GetInVehicleObjective::execute(Scene* scene, Vehicle* vehicle, Result& result, ktcoro_tasklist& tasklist) {
	while (!Command<Commands::IS_CHAR_SITTING_IN_CAR>(static_cast<CPed*>(FindPlayerPed()), vehicle->getProjectVehicle().value())) {
		co_await 1;
	}
}
