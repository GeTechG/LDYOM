#include <sol.hpp>
#include "../Data/Checkpoint.h"

void CheckpointLuaWrap(sol::state &state) {
	auto clazz = state.new_usertype<Checkpoint>("Checkpoint");
	clazz["getUuid"] = &Checkpoint::getUuid;
	clazz["getEditorCheckpoint"] = &Checkpoint::getEditorCheckpoint;
	clazz["getProjectCheckpoint"] = &Checkpoint::getProjectCheckpoint;
	clazz["getEditorSphere"] = &Checkpoint::getEditorSphere;
	clazz["getProjectSphere"] = &Checkpoint::getProjectSphere;
	clazz["updateLocation"] = &Checkpoint::updateLocation;
	clazz["getName"] = &Checkpoint::getName;
	clazz["getPosition"] = &Checkpoint::getPosition;
	clazz["getType"] = &Checkpoint::getType;
	clazz["getRadius"] = &Checkpoint::getRadius;
	clazz["getBlipColor"] = &Checkpoint::getBlipColor;
	clazz["getColor"] = &Checkpoint::getColor;
	clazz["getCheckpointType"] = &Checkpoint::getCheckpointType;
	clazz["getBlipType"] = &Checkpoint::getBlipType;
	clazz["getBlipSprite"] = &Checkpoint::getBlipSprite;
	clazz["getAngle"] = &Checkpoint::getAngle;
	clazz["spawnEditorCheckpoint"] = &Checkpoint::spawnEditorCheckpoint;
	clazz["deleteEditorCheckpoint"] = &Checkpoint::deleteEditorCheckpoint;
	clazz["spawnProjectEntity"] = &Checkpoint::spawnProjectEntity;
	clazz["deleteProjectEntity"] = &Checkpoint::deleteProjectEntity;
	clazz["spawnEditorBlip"] = &Checkpoint::spawnEditorBlip;
	clazz["deleteEditorBlip"] = &Checkpoint::deleteEditorBlip;
	clazz["spawnProjectBlip"] = &Checkpoint::spawnProjectBlip;
	clazz["deleteProjectBlip"] = &Checkpoint::deleteProjectBlip;
	clazz["drawSphere"] = &Checkpoint::drawSphere;
	clazz["existProjectEntity"] = &Checkpoint::existProjectEntity;
	clazz["getProjectEntityPosition"] = &Checkpoint::getProjectEntityPosition;
}
