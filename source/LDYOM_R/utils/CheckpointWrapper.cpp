#include <sol.hpp>
#include "../Data/Checkpoint.h"

void checkpointWrapper(sol::state &state) {
	state.new_usertype<Checkpoint>("LDCheckpoint",
	                               sol::no_constructor,
	                               "getUuid", &Checkpoint::getUuid,
	                               "getEditorCheckpoint", &Checkpoint::getEditorCheckpoint,
	                               "getProjectCheckpoint", &Checkpoint::getProjectCheckpoint,
	                               "getEditorSphere", &Checkpoint::getEditorSphere,
	                               "getProjectSphere", &Checkpoint::getProjectSphere,
	                               "updateLocation", &Checkpoint::updateLocation,
	                               "getName", &Checkpoint::getName,
	                               "getPosition", &Checkpoint::getPosition,
	                               "getType", &Checkpoint::getType,
	                               "getRadius", &Checkpoint::getRadius,
	                               "getBlipColor", &Checkpoint::getBlipColor,
	                               "getColor", &Checkpoint::getColor,
	                               "getCheckpointType", &Checkpoint::getCheckpointType,
	                               "getBlipType", &Checkpoint::getBlipType,
	                               "getBlipSprite", &Checkpoint::getBlipSprite,
	                               "getAngle", &Checkpoint::getAngle,
	                               "spawnEditorCheckpoint", &Checkpoint::spawnEditorCheckpoint,
	                               "deleteEditorCheckpoint", &Checkpoint::deleteEditorCheckpoint,
	                               "spawnProjectEntity", &Checkpoint::spawnProjectEntity,
	                               "deleteProjectEntity", &Checkpoint::deleteProjectEntity,
	                               "spawnEditorBlip", &Checkpoint::spawnEditorBlip,
	                               "deleteEditorBlip", &Checkpoint::deleteEditorBlip,
	                               "spawnProjectBlip", &Checkpoint::spawnProjectBlip,
	                               "deleteProjectBlip", &Checkpoint::deleteProjectBlip,
	                               "drawSphere", &Checkpoint::drawSphere,
	                               "existProjectEntity", &Checkpoint::existProjectEntity,
	                               "getProjectEntityPosition", &Checkpoint::getProjectEntityPosition,
	                               "copy", &Checkpoint::copy
	);
}
