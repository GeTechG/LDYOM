#include <sol.hpp>
#include "../Data/Vehicle.h"

void VehicleLuaWrap(sol::state& state) {
	auto clazz = state.new_usertype<Vehicle>("Vehicle");
	clazz["getUuid"] = &Vehicle::getUuid;
	clazz["getEditorVehicle"] = &Vehicle::getEditorVehicle;
	clazz["getProjectVehicle"] = &Vehicle::getProjectVehicle;
	clazz["getModelId"] = &Vehicle::getModelId;
	clazz["isShouldNotDie"] = &Vehicle::isShouldNotDie;
	clazz["getHealth"] = &Vehicle::getHealth;
	clazz["isBulletproof"] = &Vehicle::isBulletproof;
	clazz["isFireproof"] = &Vehicle::isFireproof;
	clazz["isExplosionproof"] = &Vehicle::isExplosionproof;
	clazz["isCollisionproof"] = &Vehicle::isCollisionproof;
	clazz["isMeleeproof"] = &Vehicle::isMeleeproof;
	clazz["isTiresVulnerability"] = &Vehicle::isTiresVulnerability;
	clazz["isExtendedColor"] = &Vehicle::isExtendedColor;
	clazz["isLocked"] = &Vehicle::isLocked;
	clazz["getColors"] = &Vehicle::getColors;
	clazz["getComponentTypeA"] = &Vehicle::getComponentTypeA;
	clazz["getComponentTypeB"] = &Vehicle::getComponentTypeB;
	clazz["getNumberplateCity"] = &Vehicle::getNumberplateCity;
	clazz["getPrimaryColor"] = &Vehicle::getPrimaryColor;
	clazz["getSecondaryColor"] = &Vehicle::getSecondaryColor;
	clazz["getNumberplate"] = &Vehicle::getNumberplate;
	clazz["updateLocation"] = &Vehicle::updateLocation;
	clazz["getName"] = &Vehicle::getName;
	clazz["getPosition"] = &Vehicle::getPosition;
	clazz["getHeadingAngle"] = &Vehicle::getHeadingAngle;
	clazz["setEditorPrimaryColor"] = &Vehicle::setEditorPrimaryColor;
	clazz["setEditorSecondaryColor"] = &Vehicle::setEditorSecondaryColor;
	clazz["spawnEditorVehicle"] = &Vehicle::spawnEditorVehicle;
	clazz["deleteEditorVehicle"] = &Vehicle::deleteEditorVehicle;
	clazz["spawnProjectEntity"] = &Vehicle::spawnProjectEntity;
	clazz["deleteProjectEntity"] = &Vehicle::deleteProjectEntity;
}
