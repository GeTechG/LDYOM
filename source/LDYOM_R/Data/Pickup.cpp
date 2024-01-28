#include "Pickup.h"

#include <array>
#include <CStreaming.h>
#include <boost/uuid/random_generator.hpp>
#include <extensions/ScriptCommands.h>
#include <extensions/scripting/ScriptCommandNames.h>

#include "components.h"
#include "CPickups.h"
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
	this->name_ = name;
	this->pos_[0] = pos.x;
	this->pos_[1] = pos.y;
	this->pos_[2] = pos.z;
}

Pickup Pickup::copy() const {
	Pickup pickup(*this);
	pickup.uuid_ = boost::uuids::random_generator()();
	pickup.name_ = this->name_;

	return pickup;
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

std::string& Pickup::getName() {
	return this->name_;
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
}

void Pickup::deleteProjectEntity() {
	if (this->projectPickup_.has_value() && !restart) {
		if (CPickups::GetActualPickupIndex(this->projectPickup_.value()) != -1) {
			Command<Commands::REMOVE_PICKUP>(this->projectPickup_.value());
		}
		this->projectPickup_ = std::nullopt;
	}
}
