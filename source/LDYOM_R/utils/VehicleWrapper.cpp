#include <sol.hpp>
#include "../Data/Vehicle.h"

void vehicleWrapper(sol::state &state) {
	state.new_usertype<Vehicle>("LDVehicle",
	                            sol::no_constructor,
	                            "getUuid", &Vehicle::getUuid,
	                            "getEditorVehicle", &Vehicle::getEditorVehicle,
	                            "getProjectVehicle", &Vehicle::getProjectVehicle,
	                            "getModelId", &Vehicle::getModelId,
	                            "isShouldNotDie", &Vehicle::isShouldNotDie,
	                            "getHealth", &Vehicle::getHealth,
	                            "isBulletproof", &Vehicle::isBulletproof,
	                            "isFireproof", &Vehicle::isFireproof,
	                            "isExplosionproof", &Vehicle::isExplosionproof,
	                            "isCollisionproof", &Vehicle::isCollisionproof,
	                            "isMeleeproof", &Vehicle::isMeleeproof,
	                            "isTiresVulnerability", &Vehicle::isTiresVulnerability,
	                            "isExtendedColor", &Vehicle::isExtendedColor,
	                            "isLocked", &Vehicle::isLocked,
	                            "getColors", &Vehicle::getColors,
	                            "getComponentTypeA", &Vehicle::getComponentTypeA,
	                            "getComponentTypeB", &Vehicle::getComponentTypeB,
	                            "getNumberplateCity", &Vehicle::getNumberplateCity,
	                            "getPrimaryColor", &Vehicle::getPrimaryColor,
	                            "getSecondaryColor", &Vehicle::getSecondaryColor,
	                            "getNumberplate", &Vehicle::getNumberplate,
	                            "updateLocation", &Vehicle::updateLocation,
	                            "getName", &Vehicle::getName,
	                            "getPosition", [](Vehicle &vehicle) {
		                            return std::make_tuple(vehicle.getPosition()[0], vehicle.getPosition()[1],
		                                                   vehicle.getPosition()[2]);
	                            },
	                            "setPosition", [](Vehicle &vehicle, const float x, const float y, const float z) {
		                            vehicle.getPosition()[0] = x;
		                            vehicle.getPosition()[1] = y;
		                            vehicle.getPosition()[2] = z;
	                            },
	                            "getHeadingAngle", &Vehicle::getHeadingAngle,
	                            "setEditorPrimaryColor", &Vehicle::setEditorPrimaryColor,
	                            "setEditorSecondaryColor", &Vehicle::setEditorSecondaryColor,
	                            "spawnEditorVehicle", &Vehicle::spawnEditorVehicle,
	                            "deleteEditorVehicle", &Vehicle::deleteEditorVehicle,
	                            "spawnProjectEntity", &Vehicle::spawnProjectEntity,
	                            "deleteProjectEntity", &Vehicle::deleteProjectEntity,
	                            "copy", &Vehicle::copy
	);
}
