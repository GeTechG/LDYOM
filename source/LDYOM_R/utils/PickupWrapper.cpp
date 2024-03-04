#include <sol.hpp>
#include "../Data/Pickup.h"


void pickupWrapper(sol::state &state) {
	state.new_usertype<Pickup>("LDPickup",
	                           sol::no_constructor,
	                           "copy", &Pickup::copy,
	                           "getUuid", &Pickup::getUuid,
	                           "getEditorPickup", &Pickup::getEditorPickup,
	                           "getProjectPickup", &Pickup::getProjectPickup,
	                           "getModelId", &Pickup::getModelId,
	                           "getType", &Pickup::getType,
	                           "getSpawnType", &Pickup::getSpawnType,
	                           "getWeapon", &Pickup::getWeapon,
	                           "getAmmo", &Pickup::getAmmo,
	                           "getProjectPickupIndex", &Pickup::getProjectPickupIndex,
	                           "getEditorPickupIndex", &Pickup::getEditorPickupIndex,
	                           "updateLocation", &Pickup::updateLocation,
	                           "getName", &Pickup::getName,
	                           "getPosition", &Pickup::getPosition,
	                           "spawnEditorPickup", &Pickup::spawnEditorPickup,
	                           "deleteEditorPickup", &Pickup::deleteEditorPickup,
	                           "spawnProjectEntity", &Pickup::spawnProjectEntity,
	                           "deleteProjectEntity", &Pickup::deleteProjectEntity
	);
}
