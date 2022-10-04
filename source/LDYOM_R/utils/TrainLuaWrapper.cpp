#include <sol.hpp>
#include "../Data/Train.h"

void TrainLuaWrap(sol::state& state) {
	auto clazz = state.new_usertype<Train>("Train");
	clazz["getUuid"] = &Train::getUuid;
	clazz["getEditorTrain"] = &Train::getEditorTrain;
	clazz["getProjectTrain"] = &Train::getProjectTrain;
	clazz["isRotate"] = &Train::isRotate;
	clazz["getHealth"] = &Train::getHealth;
	clazz["getTrainType"] = &Train::getTrainType;
	clazz["getSpeed"] = &Train::getSpeed;
	clazz["getCruiseSpeed"] = &Train::getCruiseSpeed;
	clazz["updateLocation"] = &Train::updateLocation;
	clazz["getName"] = &Train::getName;
	clazz["getPosition"] = &Train::getPosition;
	clazz["spawnEditorTrain"] = &Train::spawnEditorTrain;
	clazz["deleteEditorTrain"] = &Train::deleteEditorTrain;
	clazz["spawnProjectEntity"] = &Train::spawnProjectEntity;
	clazz["deleteProjectEntity"] = &Train::deleteProjectEntity;
}
