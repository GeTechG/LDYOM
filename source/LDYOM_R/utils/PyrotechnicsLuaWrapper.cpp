#include <sol.hpp>
#include "../Data/Pyrotechnics.h"

#include "CFire.h"

void PyrotechnicsLuaWrap(sol::state& state) {
	auto clazz = state.new_usertype<Pyrotechnics>("Pyrotechnics");
	clazz["getUuid"] = &Pyrotechnics::getUuid;
	clazz["getEditorFire"] = &Pyrotechnics::getEditorFire;
	clazz["getProjectFire"] = &Pyrotechnics::getProjectFire;
	clazz["getType"] = &Pyrotechnics::getType;
	clazz["getTypeExplosion"] = &Pyrotechnics::getTypeExplosion;
	clazz["getSizeFire"] = &Pyrotechnics::getSizeFire;
	clazz["getPropagationFire"] = &Pyrotechnics::getPropagationFire;
	clazz["getProjectFireId"] = &Pyrotechnics::getProjectFireId;
	clazz["updateLocation"] = &Pyrotechnics::updateLocation;
	clazz["getName"] = &Pyrotechnics::getName;
	clazz["getPosition"] = &Pyrotechnics::getPosition;
	clazz["spawnEditorPyrotechnics"] = &Pyrotechnics::spawnEditorPyrotechnics;
	clazz["deleteEditorPyrotechnics"] = &Pyrotechnics::deleteEditorPyrotechnics;
	clazz["spawnProjectEntity"] = &Pyrotechnics::spawnProjectEntity;
	clazz["deleteProjectEntity"] = &Pyrotechnics::deleteProjectEntity;
}
