#include <sol.hpp>
#include "../Data/Object.h"

void objectWrapper(sol::state &state) {
	state.new_usertype<Object>("LDObject",
	                           sol::no_constructor,
	                           "getUuid", &Object::getUuid,
	                           "getEditorObject", &Object::getEditorObject,
	                           "getProjectObject", &Object::getProjectObject,
	                           "getRotations", sol::property([](Object &object, sol::this_state l) {
		                                                         auto table = sol::state_view(l).create_table();
		                                                         table.add(object.getRotations().real);
		                                                         table.add(object.getRotations().imag.x);
		                                                         table.add(object.getRotations().imag.y);
		                                                         table.add(object.getRotations().imag.z);
		                                                         return table;
	                                                         }, [](Object &object,
	                                                               const sol::table &obj) {
		                                                         object.getRotations().real = obj.get<float>(1);
		                                                         object.getRotations().imag.x = obj.get<float>(2);
		                                                         object.getRotations().imag.y = obj.get<float>(3);
		                                                         object.getRotations().imag.z = obj.get<float>(4);
	                                                         }),
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
