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
	                               "getPosition", [](Checkpoint &checkpoint) {
		                               return std::make_tuple(checkpoint.getPosition()[0], checkpoint.getPosition()[1],
		                                                      checkpoint.getPosition()[2]);
	                               },
	                               "setPosition",
	                               [](Checkpoint &checkpoint, const float x, const float y, const float z) {
		                               checkpoint.getPosition()[0] = x;
		                               checkpoint.getPosition()[1] = y;
		                               checkpoint.getPosition()[2] = z;
	                               },
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
