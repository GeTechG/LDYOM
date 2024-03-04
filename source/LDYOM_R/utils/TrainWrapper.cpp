#include <sol.hpp>
#include "../Data/Train.h"

void trainWrapper(sol::state &state) {
	state.new_usertype<Train>("LDTrain",
	                          sol::no_constructor,
	                          "getUuid", &Train::getUuid,
	                          "getEditorTrain", &Train::getEditorTrain,
	                          "getProjectTrain", &Train::getProjectTrain,
	                          "isRotate", &Train::isRotate,
	                          "getHealth", &Train::getHealth,
	                          "getTrainType", &Train::getTrainType,
	                          "getSpeed", &Train::getSpeed,
	                          "getCruiseSpeed", &Train::getCruiseSpeed,
	                          "updateLocation", &Train::updateLocation,
	                          "getName", &Train::getName,
	                          "getPosition", &Train::getPosition,
	                          "spawnEditorTrain", &Train::spawnEditorTrain,
	                          "deleteEditorTrain", &Train::deleteEditorTrain,
	                          "spawnProjectEntity", &Train::spawnProjectEntity,
	                          "deleteProjectEntity", &Train::deleteProjectEntity,
	                          "copy", &Train::copy
	);
}
