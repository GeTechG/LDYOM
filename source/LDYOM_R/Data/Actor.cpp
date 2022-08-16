#include "Actor.h"

#include <CClothes.h>

#include "CWorld.h"
#include "utils.h"
#include "../shared/extensions/ScriptCommands.h"
#include "CStreaming.h"
#include "CTheScripts.h"
#include "ModelsService.h"

#include <boost/uuid/uuid_generators.hpp>

#include "ProjectsService.h"
#include "strUtils.h"

using namespace plugin;

extern bool restart;
void Actor::deleteEditorPed() {
	if (this->editorPed_.has_value() && !restart) {
		int pedRef = CPools::GetPedRef(this->editorPed_.value());
		Command<Commands::DELETE_CHAR>(pedRef);
		this->editorPed_ = std::nullopt;
	}
}

void Actor::spawnProjectEntity() {
	if (projectPed_.has_value())
		this->deleteProjectEntity();

	this->projectPed_ = spawnPed();

	if (this->group_ == 0)
	{
		int g;
		Command<Commands::GET_PLAYER_GROUP>(0, &g);
		Command<Commands::SET_GROUP_MEMBER>(g, CPools::GetPedRef(this->projectPed_.value()));
	}

	updateLocation();
}

void Actor::deleteProjectEntity() {
	if (this->projectPed_.has_value() && !restart) {
		int pedRef = CPools::GetPedRef(this->projectPed_.value());
		Command<Commands::DELETE_CHAR>(pedRef);
		this->projectPed_ = std::nullopt;
	}
}

CPed* Actor::spawnPed() {
	int model;

	std::array<unsigned, 18> textures{};
	std::array<unsigned, 10> models{};
	float muscular;
	float fats;
	auto playerModel = CWorld::Players[0].m_pPed->m_nModelIndex;
	{
		Command<Commands::SET_PLAYER_MODEL>(0, 0);
		const auto playerClothes = CWorld::Players[0].m_pPed->m_pPlayerData->m_pPedClothesDesc;
		std::memcpy(textures.data(), playerClothes->m_anTextureKeys, sizeof playerClothes->m_anTextureKeys);
		std::memcpy(models.data(), playerClothes->m_anModelKeys, sizeof playerClothes->m_anModelKeys);
		muscular = playerClothes->m_fFatStat;
		fats = playerClothes->m_fMuscleStat;
	}

	if (this->modelType_ == 0 && this->modelId_ == 0) {
		const auto playerClothes = CWorld::Players[0].m_pPed->m_pPlayerData->m_pPedClothesDesc;
		std::memcpy(playerClothes->m_anTextureKeys, this->clotherMAnTextureKeys_.data(), sizeof playerClothes->m_anTextureKeys);
		std::memcpy(playerClothes->m_anModelKeys, this->clotherMAnModelKeys_.data(), sizeof playerClothes->m_anModelKeys);
		playerClothes->m_fFatStat = this->fatStat;
		playerClothes->m_fMuscleStat = this->musculeStat;
		CClothes::RebuildPlayer(CWorld::Players[0].m_pPed, false);
	}

	if (this->modelType_ == 0) {
		model = ModelsService::validPedModel(this->modelId_);
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
		int weapon = ModelsService::validWeaponModel(weaponRaw);
		const int weaponModel = CWeaponInfo::GetWeaponInfo(static_cast<eWeaponType>(weapon), 1)->m_nModelId1;

		CStreaming::RequestModel(weaponModel, GAME_REQUIRED);
		CStreaming::LoadAllRequestedModels(false);

		ped->GiveWeapon(static_cast<eWeaponType>(weapon), ammo, false);

		CStreaming::SetMissionDoesntRequireModel(weaponModel);
	}
	if (!this->weapons_.empty()) {
		int currentWeapon = this->weapons_.at(this->defaultWeapon_).weapon;
		ped->SetCurrentWeapon(static_cast<eWeaponType>(currentWeapon));
	}

	ped->DisablePedSpeech(1);

	if (this->modelType_ == 0 && this->modelId_ == 0) {
		Command<Commands::SET_PLAYER_MODEL>(0, playerModel);
		const auto playerClothes = CWorld::Players[0].m_pPed->m_pPlayerData->m_pPedClothesDesc;
		std::memcpy(playerClothes->m_anTextureKeys, textures.data(), sizeof playerClothes->m_anTextureKeys);
		std::memcpy(playerClothes->m_anModelKeys, models.data(), sizeof playerClothes->m_anModelKeys);
		playerClothes->m_fFatStat = fats;
		playerClothes->m_fMuscleStat = muscular;
		CClothes::RebuildPlayer(CWorld::Players[0].m_pPed, false);
	}

	Command<Commands::SET_CHAR_STAY_IN_SAME_PLACE>(newPed, (int)this->stayInSamePlace_);
	Command<Commands::SET_CHAR_KINDA_STAY_IN_SAME_PLACE>(newPed, (int)this->kindaStayInSamePlace_);
	ped->m_nWeaponAccuracy = this->accuracy_;
	Command<Commands::SET_CHAR_SUFFERS_CRITICAL_HITS>(newPed, (int)this->headshot_);
	ped->m_nPedFlags.bDoesntDropWeaponsWhenDead = static_cast<unsigned>(!this->dropWeapons_);

	return ped;
}

Actor::Actor(const char* name, const CVector& pos, float headingAngle): ObjectiveDependent(nullptr),
																		uuid_(boost::uuids::random_generator()()),
                                                                        pos_{pos.x, pos.y, pos.z}, headingAngle_(headingAngle),
                                                                        accuracy_(50),
																		health_(100),
                                                                        headshot_(true) {
	strcpy(this->name_.data(), name);

	const auto playerClothes = CWorld::Players[0].m_pPed->m_pPlayerData->m_pPedClothesDesc;
	std::memcpy(this->clotherMAnTextureKeys_.data(), playerClothes->m_anTextureKeys, sizeof playerClothes->m_anTextureKeys);
	std::memcpy(this->clotherMAnModelKeys_.data(), playerClothes->m_anModelKeys, sizeof playerClothes->m_anModelKeys);
	this->fatStat = playerClothes->m_fFatStat;
	this->musculeStat = playerClothes->m_fMuscleStat;
}

Actor::Actor(const Actor& other): ObjectiveDependent{other},
                                  INameable{other},
                                  IPositionable{other},
                                  IUuidable{other},
                                  uuid_{ boost::uuids::random_generator()() },
                                  name_{other.name_},
                                  pos_{other.pos_},
                                  headingAngle_{other.headingAngle_},
                                  group_{other.group_},
                                  modelType_{other.modelType_},
                                  slot_{other.slot_},
                                  modelId_{other.modelId_},
                                  weapons_{other.weapons_},
                                  defaultWeapon_{other.defaultWeapon_},
                                  accuracy_{other.accuracy_},
                                  health_{other.health_},
                                  randomSpawn_{other.randomSpawn_},
                                  shouldNotDie_{other.shouldNotDie_},
                                  stayInSamePlace_{other.stayInSamePlace_},
                                  kindaStayInSamePlace_{other.kindaStayInSamePlace_},
                                  headshot_{other.headshot_},
                                  dropWeapons_{other.dropWeapons_},
                                  clotherMAnModelKeys_{other.clotherMAnModelKeys_},
                                  clotherMAnTextureKeys_{other.clotherMAnTextureKeys_},
								  fatStat{other.fatStat},
								  musculeStat{other.musculeStat} {
	strlcat(this->name_.data(), "C", sizeof this->name_);
}

Actor& Actor::operator=(const Actor& other) {
	if (this == &other)
		return *this;
	ObjectiveDependent::operator =(other);
	INameable::operator =(other);
	IPositionable::operator =(other);
	IUuidable::operator =(other);
	uuid_ = other.uuid_;
	name_ = other.name_;
	pos_ = other.pos_;
	headingAngle_ = other.headingAngle_;
	group_ = other.group_;
	modelType_ = other.modelType_;
	slot_ = other.slot_;
	modelId_ = other.modelId_;
	weapons_ = other.weapons_;
	defaultWeapon_ = other.defaultWeapon_;
	accuracy_ = other.accuracy_;
	health_ = other.health_;
	randomSpawn_ = other.randomSpawn_;
	shouldNotDie_ = other.shouldNotDie_;
	stayInSamePlace_ = other.stayInSamePlace_;
	kindaStayInSamePlace_ = other.kindaStayInSamePlace_;
	headshot_ = other.headshot_;
	dropWeapons_ = other.dropWeapons_;
	clotherMAnTextureKeys_ = other.clotherMAnTextureKeys_;
	clotherMAnModelKeys_ = other.clotherMAnModelKeys_;
	fatStat = other.fatStat;
	musculeStat = other.musculeStat;
	return *this;
}

Actor::~Actor() {
	this->deleteEditorPed();
	this->deleteProjectEntity();
	this->signalDeleteActor();
}

char* Actor::getName() {
	return this->name_.data();
}

float* Actor::getPosition() {
	return this->pos_.data();
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

int& Actor::getAccuracy() {
	return accuracy_;
}

int& Actor::getHealth() {
	return health_;
}

bool& Actor::isRandomSpawn() {
	return randomSpawn_;
}

bool& Actor::isShouldNotDie() {
	return shouldNotDie_;
}

bool& Actor::isStayInSamePlace() {
	return stayInSamePlace_;
}

bool& Actor::isKindaStayInSamePlace() {
	return kindaStayInSamePlace_;
}

bool& Actor::isHeadshot() {
	return headshot_;
}

bool& Actor::isDropWeapons() {
	return dropWeapons_;
}

std::array<unsigned, 10>& Actor::getClotherMAnModelKeys() {
	return clotherMAnModelKeys_;
}

std::array<unsigned, 18>& Actor::getClotherMAnTextureKeys() {
	return clotherMAnTextureKeys_;
}

float& Actor::getFatStat() {
	return fatStat;
}

float& Actor::getMusculeStat() {
	return musculeStat;
}

boost::uuids::uuid& Actor::getUuid() {
	return uuid_;
}

boost::signals2::signal<void()>& Actor::getSignalDeleteActor() {
	return signalDeleteActor;
}

void Actor::updateLocation() const {
	if (this->editorPed_.has_value()) {
		this->editorPed_.value()->SetPosn(this->pos_[0], this->pos_[1], this->pos_[2]);
		Command<Commands::SET_CHAR_HEADING>(this->editorPed_.value(), this->headingAngle_);
	}
	if (this->projectPed_.has_value()) {
		this->projectPed_.value()->SetPosn(this->pos_[0], this->pos_[1], this->pos_[2]);
		Command<Commands::SET_CHAR_HEADING>(this->projectPed_.value(), this->headingAngle_);
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
	this->editorPed_.value()->m_nPhysicalFlags.bMeleeProof = 1;
	Command<Commands::FREEZE_CHAR_POSITION_AND_DONT_LOAD_COLLISION>(CPools::GetPedRef(this->editorPed_.value()), 1);

	updateLocation();
}

