#include <sol.hpp>
#include "../Data/Object.h"

void objectWrapper(sol::state &state) {
	state.new_usertype<Object>("LDObject",
	                           sol::no_constructor,
	                           "getUuid", &Object::getUuid,
	                           "getEditorObject", &Object::getEditorObject,
	                           "getProjectObject", &Object::getProjectObject,
	                           "getRotations", &Object::getRotations,
	                           "getScale", &Object::getScale,
	                           "getModelId", &Object::getModelId,
	                           "updateLocation", &Object::updateLocation,
	                           "getName", &Object::getName,
	                           "getPosition", &Object::getPosition,
	                           "spawnEditorObject", &Object::spawnEditorObject,
	                           "deleteEditorObject", &Object::deleteEditorObject,
	                           "spawnProjectEntity", &Object::spawnProjectEntity,
	                           "deleteProjectEntity", &Object::deleteProjectEntity,
	                           "copy", &Object::copy
	);
}
