#include <sol.hpp>

#include "CarrecPathsService.h"

void carrecPathServiceWrapper(sol::state &state) {
	auto ldyomTable = state["ld"].get_or_create<sol::table>();
	ldyomTable["carrecPathService"] = &CarrecPathsService::getInstance();
	state.new_usertype<CarrecPathsService>("CarrecPathsService", sol::no_constructor,
	                                       "savePaths", &CarrecPathsService::savePaths,
	                                       "loadPaths", &CarrecPathsService::loadPaths,
	                                       "createNewPath", &CarrecPathsService::createNewPath,
	                                       "getPaths", &CarrecPathsService::getPaths,
	                                       "startPlaybackRecordedCar", &CarrecPathsService::startPlaybackRecordedCar,
	                                       "stopPlaybackRecordedCar", &CarrecPathsService::stopPlaybackRecordedCar
	);
	state.new_usertype<CarrecPath>("CarrecPath", sol::no_constructor,
	                               "getPath", &CarrecPath::getPath,
	                               "getVehicle", &CarrecPath::getVehicle,
	                               "getName", &CarrecPath::getName,
	                               "getUuid", &CarrecPath::getUuid
	);
}
