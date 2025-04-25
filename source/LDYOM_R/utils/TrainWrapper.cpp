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
	                          "getPosition", [](Train &train) {
		                          return std::make_tuple(train.getPosition()[0], train.getPosition()[1],
		                                                 train.getPosition()[2]);
	                          },
	                          "setPosition", [](Train &train, const float x, const float y, const float z) {
		                          train.getPosition()[0] = x;
		                          train.getPosition()[1] = y;
		                          train.getPosition()[2] = z;
	                          },
	                          "spawnEditorTrain", &Train::spawnEditorTrain,
	                          "deleteEditorTrain", &Train::deleteEditorTrain,
	                          "spawnProjectEntity", &Train::spawnProjectEntity,
	                          "deleteProjectEntity", &Train::deleteProjectEntity,
	                          "copy", &Train::copy
	);
}
