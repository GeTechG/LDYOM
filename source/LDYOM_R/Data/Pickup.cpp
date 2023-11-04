#include "Pickup.h"

#include <array>
#include <CStreaming.h>
#include <boost/uuid/random_generator.hpp>
#include <extensions/ScriptCommands.h>
#include <extensions/scripting/ScriptCommandNames.h>

#include "components.h"
#include "CPickups.h"
#include "LuaEngine.h"
#include "ModelsService.h"
#include "ProjectPlayerService.h"
#include "Scene.h"
#include "strUtils.h"
#include "utils.h"
#include "easylogging/easylogging++.h"

using namespace plugin;

std::optional<int> Pickup::spawnPickup(int pickupType) {
	if (this->type_ == 0) {
		auto weaponIds = ModelsService::getInstance().getWeaponIds();
		const int weaponModel = CWeaponInfo::GetWeaponInfo(static_cast<eWeaponType>(this->weapon_), 1)->m_nModelId1;
		if (weaponModel != -1) {
			CStreaming::RequestModel(weaponModel, 0);
			CStreaming::LoadAllRequestedModels(false);
		}
		const int newPickup = CPickups::GenerateNewOne_WeaponType(CVector(this->pos_[0], this->pos_[1], this->pos_[2]),
		                                                          static_cast<eWeaponType>(this->weapon_), pickupType,
		                                                          ammo_, false, nullptr);
		CStreaming::RemoveAllUnusedModels();
		return newPickup;
	}


	int modelId;
	switch (this->type_) {
	case 1:
		modelId = 1240;
		break;
	case 2:
		modelId = 1242;
		break;
	case 3:
		modelId = 1247;
		break;
	case 4:
		modelId = 1241;
		break;
	default:
		modelId = this->modelId_;
	}

	CStreaming::RequestModel(modelId, 0);
	CStreaming::LoadAllRequestedModels(false);

	const int newPickup = CPickups::GenerateNewOne(CVector(this->pos_[0], this->pos_[1], this->pos_[2]), modelId,
	                                               pickupType, this->ammo_, 0, false, nullptr);
	CStreaming::RemoveAllUnusedModels();
	return newPickup;
}

Pickup::Pickup(const char *name, const CVector &pos): ObjectiveDependent(nullptr),
                                                      uuid_(boost::uuids::random_generator()()),
                                                      modelId_(325) {
	strlcpy(this->name_.data(), name, sizeof this->name_);
	this->pos_[0] = pos.x;
	this->pos_[1] = pos.y;
	this->pos_[2] = pos.z;
}

Pickup::Pickup(const Pickup &other): ObjectiveDependent{other},
                                     INameable{other},
                                     IPositionable{other},
                                     IUuidable{other},
                                     uuid_{boost::uuids::random_generator()()},
                                     name_{other.name_},
                                     pos_{other.pos_},
                                     type_{other.type_},
                                     spawnType_{other.spawnType_},
                                     weapon_{other.weapon_},
                                     ammo_{other.ammo_},
                                     modelId_{other.modelId_} {
	strlcat(name_.data(), "C", sizeof name_);
}

Pickup& Pickup::operator=(const Pickup &other) {
	if (this == &other)
		return *this;
	ObjectiveDependent::operator =(other);
	INameable::operator =(other);
	IPositionable::operator =(other);
	IUuidable::operator =(other);
	uuid_ = other.uuid_;
	name_ = other.name_;
	pos_ = other.pos_;
	type_ = other.type_;
	spawnType_ = other.spawnType_;
	weapon_ = other.weapon_;
	ammo_ = other.ammo_;
	modelId_ = other.modelId_;
	return *this;
}

Pickup::~Pickup() {
	this->deleteEditorPickup();
	this->deleteProjectEntity();
}

boost::uuids::uuid& Pickup::getUuid() {
	return uuid_;
}

std::optional<CPickup*> Pickup::getEditorPickup() {
	if (this->editorPickup_.has_value()) {
		const auto actual = CPickups::GetActualPickupIndex(this->editorPickup_.value());
		return &CPickups::aPickUps[actual];
	}
	return std::nullopt;
}

std::optional<CPickup*> Pickup::getProjectPickup() {
	if (this->projectPickup_.has_value()) {
		const auto actual = CPickups::GetActualPickupIndex(this->projectPickup_.value());
		return &CPickups::aPickUps[actual];
	}
	return std::nullopt;
}

int& Pickup::getModelId() {
	return modelId_;
}

int& Pickup::getType() {
	return type_;
}

int& Pickup::getSpawnType() {
	return spawnType_;
}

int& Pickup::getWeapon() {
	return weapon_;
}

int& Pickup::getAmmo() {
	return ammo_;
}

std::optional<int>& Pickup::getProjectPickupIndex() {
	return this->projectPickup_;
}

std::optional<int>& Pickup::getEditorPickupIndex() {
	return this->editorPickup_;
}

void Pickup::updateLocation() {
	if (const auto editorPickup = this->getEditorPickup(); editorPickup.has_value()) {
		editorPickup.value()->SetPosn(this->pos_[0], this->pos_[1], this->pos_[2]);
	}
	if (const auto projectPickup = this->getProjectPickup(); projectPickup.has_value()) {
		projectPickup.value()->SetPosn(this->pos_[0], this->pos_[1], this->pos_[2]);
	}
}

char* Pickup::getName() {
	return this->name_.data();
}

float* Pickup::getPosition() {
	return pos_.data();
}

void Pickup::spawnEditorPickup() {
	if (editorPickup_.has_value())
		this->deleteEditorPickup();

	this->editorPickup_ = spawnPickup(9);
}

extern bool restart;

void Pickup::deleteEditorPickup() {
	if (this->editorPickup_.has_value() && !restart) {
		if (CPickups::GetActualPickupIndex(this->editorPickup_.value()) != -1) {
			Command<Commands::REMOVE_PICKUP>(this->editorPickup_.value());
		}
		this->editorPickup_ = std::nullopt;
	}
}

void Pickup::spawnProjectEntity() {
	if (projectPickup_.has_value())
		this->deleteProjectEntity();

	int spawnType = 3;
	if (this->spawnType_ == 1)
		spawnType = 2;
	else if (this->spawnType_ == 2)
		spawnType = 15;

	this->projectPickup_ = spawnPickup(spawnType);

	updateLocation();

	auto scene = ProjectPlayerService::getInstance().getCurrentScene();
	auto tasklist = ProjectPlayerService::getInstance().getSceneTasklist();

	if (scene.has_value() && tasklist != nullptr) {
		const auto onPickupSpawn = LuaEngine::getInstance().getLuaState()["global_data"]["signals"]["onPickupSpawn"].
			get_or_create<sol::table>();
		for (auto func : onPickupSpawn | views::values) {
			if (const auto result = func.as<sol::function>()(scene.value(), tasklist, this->uuid_); !result.valid()) {
				const sol::error err = result;
				CLOG(ERROR, "lua") << err.what();
			}
		}
	}
}

void Pickup::deleteProjectEntity() {
	if (this->projectPickup_.has_value() && !restart) {
		if (CPickups::GetActualPickupIndex(this->projectPickup_.value()) != -1) {
			Command<Commands::REMOVE_PICKUP>(this->projectPickup_.value());
		}
		this->projectPickup_ = std::nullopt;

		auto scene = ProjectPlayerService::getInstance().getCurrentScene();
		auto tasklist = ProjectPlayerService::getInstance().getSceneTasklist();

		if (scene.has_value() && tasklist != nullptr) {
			const auto onPickupDelete = LuaEngine::getInstance().getLuaState()["global_data"]["signals"][
				"onPickupDelete"].get_or_create<sol::table>();
			for (auto func : onPickupDelete | views::values) {
				if (const auto result = func.as<sol::function>()(scene.value(), tasklist, this->uuid_); !result.
					valid()) {
					const sol::error err = result;
					CLOG(ERROR, "lua") << err.what();
				}
			}
		}
	}
}
