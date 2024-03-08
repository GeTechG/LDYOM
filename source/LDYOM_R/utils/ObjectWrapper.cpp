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
	                           "getPosition", [](Object &object) {
		                           return std::make_tuple(object.getPosition()[0], object.getPosition()[1],
		                                                  object.getPosition()[2]);
	                           },
	                           "setPosition", [](Object &object, const float x, const float y, const float z) {
		                           object.getPosition()[0] = x;
		                           object.getPosition()[1] = y;
		                           object.getPosition()[2] = z;
	                           },
	                           "spawnEditorObject", &Object::spawnEditorObject,
	                           "deleteEditorObject", &Object::deleteEditorObject,
	                           "spawnProjectEntity", &Object::spawnProjectEntity,
	                           "deleteProjectEntity", &Object::deleteProjectEntity,
	                           "copy", &Object::copy
	);
}
