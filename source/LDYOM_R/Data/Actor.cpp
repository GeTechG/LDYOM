#include "Actor.h"
#include "CWorld.h"
#include "utils.h"
#include "../shared/extensions/ScriptCommands.h"
#include "CStreaming.h"
#include "CTheScripts.h"
#include "ModelsService.h"

using namespace plugin;

void Actor::deleteEditorPed() {
	if (this->editorPed_.has_value()) {
		Command<Commands::DELETE_CHAR>(CPools::GetPedRef(this->editorPed_.value()));
		this->editorPed_ = std::nullopt;
	}
}

void Actor::deleteProjectPed() {
	if (this->projectPed_.has_value()) {
		Command<Commands::DELETE_CHAR>(CPools::GetPedRef(this->projectPed_.value()));
		this->projectPed_ = std::nullopt;
	}
}

int validPedModel(const int model) {
	int returnModel = 0;
	for (const int pedModel : ModelsService::getInstance().getPedModels()) {
		if (pedModel <= model)
			returnModel = pedModel;
	}
	return returnModel;
}

int validWeaponModel(const int model) {
	int returnModel = 0;
	for (const int weaponModel : ModelsService::getInstance().getWeaponIds()) {
		if (weaponModel <= model)
			returnModel = weaponModel;
	}
	return returnModel;
}

CPed* Actor::spawnPed() {
	int model;

	if (this->modelType_ == 0) {
		model = validPedModel(this->modelId_);
		CStreaming::RequestModel(model, GAME_REQUIRED);
	} else {
		model = 290 + this->slot_;
		const auto & specialsPed = ModelsService::getInstance().getSpecialsPed().at(this->modelId_);
		CStreaming::RequestSpecialChar(this->slot_, specialsPed.c_str(), GAME_REQUIRED);
	}
	CStreaming::LoadAllRequestedModels(false);

	int newPed;
	Command<Commands::CREATE_CHAR>(23 + this->group_, model, this->pos_[0], this->pos_[1], this->pos_[2], &newPed);
	CStreaming::SetMissionDoesntRequireModel(model);

	CPed* ped = CPools::GetPed(newPed);

	for (const auto &[weaponRaw, ammo] : this->weapons_) {
		int weapon = validWeaponModel(weaponRaw);
		const int weaponModel = CWeaponInfo::GetWeaponInfo(static_cast<eWeaponType>(weapon), 1)->m_nModelId1;

		CStreaming::RequestModel(weaponModel, GAME_REQUIRED);
		CStreaming::LoadAllRequestedModels(false);

		ped->GiveWeapon(static_cast<eWeaponType>(weapon), ammo, false);

		if (this->getModelType() == 0)
			CStreaming::SetMissionDoesntRequireModel(weaponModel);
		else
			CStreaming::SetMissionDoesntRequireSpecialChar(this->slot_);
	}
	if (!this->weapons_.empty()) {
		int currentWeapon = this->weapons_.at(this->defaultWeapon_).weapon;
		ped->SetCurrentWeapon(static_cast<eWeaponType>(currentWeapon));
	}

	return ped;
}

Actor::Actor(const char* name, const CVector& pos, float headingAngle): headingAngle_(headingAngle), group_(0),
                                                                        modelType_(0), slot_(0),
                                                                        modelId_(0), defaultWeapon_(0) {
	strcpy(this->name_, name);
	this->pos_[0] = pos.x;
	this->pos_[1] = pos.y;
	this->pos_[2] = pos.z;
}

Actor::Actor(const Actor& other): INameable{other},
                                  IPositionable{other},
                                  editorPed_{other.editorPed_},
                                  projectPed_{other.projectPed_},
                                  headingAngle_{other.headingAngle_},
                                  group_{other.group_}, modelType_(other.modelType_), slot_(other.slot_),
                                  modelId_{other.modelId_},
                                  defaultWeapon_(other.defaultWeapon_) {
	strcpy(this->name_, other.name_);
	memcpy(this->pos_, other.pos_, sizeof this->pos_);
}

Actor& Actor::operator=(Actor other) {
	using std::swap;
	swap(*this, other);
	return *this;
}

Actor::~Actor() {
	this->deleteEditorPed();
	this->deleteProjectPed();
}

char* Actor::getName() {
	return this->name_;
}

float* Actor::getPosition() {
	return this->pos_;
}

std::optional<CPed*>& Actor::getEditorPed() {
	return editorPed_;
}

std::optional<CPed*>& Actor::getProjectPed() {
	return projectPed_;
}

float& Actor::getHeadingAngle() {
	return headingAngle_;
}

int& Actor::getGroup() {
	return group_;
}

int& Actor::getModelId() {
	return modelId_;
}

std::vector<Weapon>& Actor::getWeapons() {
	return weapons_;
}

int& Actor::getDefaultWeapon() {
	return defaultWeapon_;
}

unsigned char& Actor::getModelType() {
	return modelType_;
}

int& Actor::getSlot() {
	return slot_;
}

void Actor::updateLocation() const {
	if (this->editorPed_.has_value()) {
		this->editorPed_.value()->SetPosn(this->pos_[0], this->pos_[1], this->pos_[2]);
		this->editorPed_.value()->SetHeading(RAD(this->headingAngle_));
	}
	if (this->projectPed_.has_value()) {
		this->projectPed_.value()->SetPosn(this->pos_[0], this->pos_[1], this->pos_[2]);
		this->projectPed_.value()->SetHeading(RAD(this->headingAngle_));
	}
}

void Actor::spawnEditorPed() {
	if (editorPed_.has_value())
		this->deleteEditorPed();

	this->editorPed_ = spawnPed();
	this->editorPed_.value()->m_bUsesCollision = 0;
	this->editorPed_.value()->m_nPhysicalFlags.bBulletProof = 1;
	this->editorPed_.value()->m_nPhysicalFlags.bCollisionProof = 1;
	this->editorPed_.value()->m_nPhysicalFlags.bExplosionProof = 1;
	this->editorPed_.value()->m_nPhysicalFlags.bFireProof = 1;
	this->editorPed_.value()->m_nPhysicalFlags.bMeeleProof = 1;
	Command<Commands::FREEZE_CHAR_POSITION_AND_DONT_LOAD_COLLISION>(CPools::GetPedRef(this->editorPed_.value()), 1);

	updateLocation();
}

void Actor::spawnProjectPed() {
	if (projectPed_.has_value())
		this->deleteProjectPed();

	this->projectPed_ = spawnPed();

	updateLocation();
}

