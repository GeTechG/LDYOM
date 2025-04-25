#include <sol.hpp>
#include "../Data/Actor.h"

void actorWrapper(sol::state &state) {
	state.new_usertype<Actor>("LDActor",
	                          sol::no_constructor,
	                          "getName", &Actor::getName,
	                          "getPosition", [](Actor &actor) {
		                          return std::make_tuple(actor.getPosition()[0], actor.getPosition()[1],
		                                                 actor.getPosition()[2]);
	                          },
	                          "setPosition", [](Actor &actor, const float x, const float y, const float z) {
		                          actor.getPosition()[0] = x;
		                          actor.getPosition()[1] = y;
		                          actor.getPosition()[2] = z;
	                          },
	                          "getEditorPed", &Actor::getEditorPed,
	                          "getProjectPed", &Actor::getProjectPed,
	                          "getHeadingAngle", &Actor::getHeadingAngle,
	                          "getGroup", &Actor::getGroup,
	                          "getModelId", &Actor::getModelId,
	                          "getWeapons", &Actor::getWeapons,
	                          "getDefaultWeapon", &Actor::getDefaultWeapon,
	                          "getModelType", &Actor::getModelType,
	                          "getSlot", &Actor::getSlot,
	                          "getAccuracy", &Actor::getAccuracy,
	                          "getHealth", &Actor::getHealth,
	                          "isRandomSpawn", &Actor::isRandomSpawn,
	                          "isShouldNotDie", &Actor::isShouldNotDie,
	                          "isStayInSamePlace", &Actor::isStayInSamePlace,
	                          "isKindaStayInSamePlace", &Actor::isKindaStayInSamePlace,
	                          "isHeadshot", &Actor::isHeadshot,
	                          "isDropWeapons", &Actor::isDropWeapons,
	                          "isDressUp", &Actor::isDressUp,
	                          "getClotherMAnModelKeys", &Actor::getClotherMAnModelKeys,
	                          "getClotherMAnTextureKeys", &Actor::getClotherMAnTextureKeys,
	                          "getFatStat", &Actor::getFatStat,
	                          "getMusculeStat", &Actor::getMusculeStat,
	                          "getInteriorId", &Actor::getInteriorId,
	                          "isShowHealthBarCounter", &Actor::isShowHealthBarCounter,
	                          "getHealthBarCounterText", &Actor::getHealthBarCounterText,
	                          "getUuid", &Actor::getUuid,
	                          "getSignalDeleteActor", &Actor::getSignalDeleteActor,
	                          "getProjectHealthBarCounter", &Actor::getProjectHealthBarCounter,
	                          "updateLocation", &Actor::updateLocation,
	                          "spawnEditorPed", &Actor::spawnEditorPed,
	                          "deleteEditorPed", &Actor::deleteEditorPed,
	                          "spawnProjectEntity", &Actor::spawnProjectEntity,
	                          "deleteProjectEntity", &Actor::deleteProjectEntity,
	                          "copy", &Actor::copy
	);
}
