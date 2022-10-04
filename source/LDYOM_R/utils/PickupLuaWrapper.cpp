#include <sol.hpp>
#include "../Data/Pickup.h"

void PickupLuaWrap(sol::state& state) {
	auto clazz = state.new_usertype<Pickup>("Pickup");
	clazz["getUuid"] = &Pickup::getUuid;
	clazz["getEditorPickup"] = &Pickup::getEditorPickup;
	clazz["getProjectPickup"] = &Pickup::getProjectPickup;
	clazz["getModelId"] = &Pickup::getModelId;
	clazz["getType"] = &Pickup::getType;
	clazz["getSpawnType"] = &Pickup::getSpawnType;
	clazz["getWeapon"] = &Pickup::getWeapon;
	clazz["getAmmo"] = &Pickup::getAmmo;
	clazz["getProjectPickupIndex"] = &Pickup::getProjectPickupIndex;
	clazz["getEditorPickupIndex"] = &Pickup::getEditorPickupIndex;
	clazz["updateLocation"] = &Pickup::updateLocation;
	clazz["getName"] = &Pickup::getName;
	clazz["getPosition"] = &Pickup::getPosition;
	clazz["spawnEditorPickup"] = &Pickup::spawnEditorPickup;
	clazz["deleteEditorPickup"] = &Pickup::deleteEditorPickup;
	clazz["spawnProjectEntity"] = &Pickup::spawnProjectEntity;
	clazz["deleteProjectEntity"] = &Pickup::deleteProjectEntity;
}
