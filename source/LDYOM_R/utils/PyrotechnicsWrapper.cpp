#include <CFire.h>
#include <sol.hpp>
#include "../Data/Pyrotechnics.h"


void pyrotechnicsWrapper(sol::state &state) {
	state.new_usertype<Pyrotechnics>("LDPyrotechnics", sol::no_constructor,
	                                 "copy", &Pyrotechnics::copy,
	                                 "getUuid", &Pyrotechnics::getUuid,
	                                 "getEditorFire", [](Pyrotechnics &p) -> CFire* {
		                                 return p.getEditorFire().value_or(nullptr);
	                                 },
	                                 "getProjectFire", [](Pyrotechnics &p) -> CFire* {
		                                 return p.getProjectFire().value_or(nullptr);
	                                 },
	                                 "getType", &Pyrotechnics::getType,
	                                 "getTypeExplosion", &Pyrotechnics::getTypeExplosion,
	                                 "getSizeFire", &Pyrotechnics::getSizeFire,
	                                 "getPropagationFire", &Pyrotechnics::getPropagationFire,
	                                 "getProjectFireId", [](Pyrotechnics &p) -> int {
		                                 return p.getProjectFireId().value_or(-1);
	                                 },
	                                 "updateLocation", &Pyrotechnics::updateLocation,
	                                 "getName", &Pyrotechnics::getName,
	                                 "getPosition", [](Pyrotechnics &pyrotechnics) {
		                                 return std::make_tuple(pyrotechnics.getPosition()[0],
		                                                        pyrotechnics.getPosition()[1],
		                                                        pyrotechnics.getPosition()[2]);
	                                 },
	                                 "setPosition",
	                                 [](Pyrotechnics &pyrotechnics, const float x, const float y, const float z) {
		                                 pyrotechnics.getPosition()[0] = x;
		                                 pyrotechnics.getPosition()[1] = y;
		                                 pyrotechnics.getPosition()[2] = z;
	                                 },
	                                 "spawnEditorPyrotechnics", &Pyrotechnics::spawnEditorPyrotechnics,
	                                 "deleteEditorPyrotechnics", &Pyrotechnics::deleteEditorPyrotechnics,
	                                 "spawnProjectEntity", &Pyrotechnics::spawnProjectEntity,
	                                 "deleteProjectEntity", &Pyrotechnics::deleteProjectEntity
	);
}
