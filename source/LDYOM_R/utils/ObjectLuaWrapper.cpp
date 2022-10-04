#include <sol.hpp>
#include "../Data/Object.h"

void ObjectLuaWrap(sol::state& state) {
	auto clazz = state.new_usertype<Object>("Object");
	clazz["getUuid"] = &Object::getUuid;
	clazz["getEditorObject"] = &Object::getEditorObject;
	clazz["getProjectObject"] = &Object::getProjectObject;
	clazz["getRotations"] = &Object::getRotations;
	clazz["getScale"] = &Object::getScale;
	clazz["getModelId"] = &Object::getModelId;
	clazz["updateLocation"] = &Object::updateLocation;
	clazz["getName"] = &Object::getName;
	clazz["getPosition"] = &Object::getPosition;
	clazz["spawnEditorObject"] = &Object::spawnEditorObject;
	clazz["deleteEditorObject"] = &Object::deleteEditorObject;
	clazz["spawnProjectEntity"] = &Object::spawnProjectEntity;
	clazz["deleteProjectEntity"] = &Object::deleteProjectEntity;
}
