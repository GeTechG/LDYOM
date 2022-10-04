#include <sol.hpp>
#include "../Data/Actor.h"

void ActorLuaWrap(sol::state& state) {
	auto clazz = state.new_usertype<Actor>("Actor");
	clazz["getName"] = &Actor::getName;
	clazz["getPosition"] = &Actor::getPosition;
	clazz["getEditorPed"] = &Actor::getEditorPed;
	clazz["getProjectPed"] = &Actor::getProjectPed;
	clazz["getHeadingAngle"] = &Actor::getHeadingAngle;
	clazz["getGroup"] = &Actor::getGroup;
	clazz["getModelId"] = &Actor::getModelId;
	clazz["getWeapons"] = &Actor::getWeapons;
	clazz["getDefaultWeapon"] = &Actor::getDefaultWeapon;
	clazz["getModelType"] = &Actor::getModelType;
	clazz["getSlot"] = &Actor::getSlot;
	clazz["getAccuracy"] = &Actor::getAccuracy;
	clazz["getHealth"] = &Actor::getHealth;
	clazz["isRandomSpawn"] = &Actor::isRandomSpawn;
	clazz["isShouldNotDie"] = &Actor::isShouldNotDie;
	clazz["isStayInSamePlace"] = &Actor::isStayInSamePlace;
	clazz["isKindaStayInSamePlace"] = &Actor::isKindaStayInSamePlace;
	clazz["isHeadshot"] = &Actor::isHeadshot;
	clazz["isDropWeapons"] = &Actor::isDropWeapons;
	clazz["isDressUp"] = &Actor::isDressUp;
	clazz["getClotherMAnModelKeys"] = &Actor::getClotherMAnModelKeys;
	clazz["getClotherMAnTextureKeys"] = &Actor::getClotherMAnTextureKeys;
	clazz["getFatStat"] = &Actor::getFatStat;
	clazz["getMusculeStat"] = &Actor::getMusculeStat;
	clazz["getUuid"] = &Actor::getUuid;
	clazz["getSignalDeleteActor"] = &Actor::getSignalDeleteActor;
	clazz["updateLocation"] = &Actor::updateLocation;
	clazz["spawnEditorPed"] = &Actor::spawnEditorPed;
	clazz["deleteEditorPed"] = &Actor::deleteEditorPed;
	clazz["spawnProjectEntity"] = &Actor::spawnProjectEntity;
	clazz["deleteProjectEntity"] = &Actor::deleteProjectEntity;
}
