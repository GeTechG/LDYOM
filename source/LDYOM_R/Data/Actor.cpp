#include "Actor.h"

#include <CClothes.h>

#include "CStreaming.h"
#include "CTheScripts.h"
#include "CWorld.h"
#include "ModelsService.h"
#include "utils.h"
#include "../shared/extensions/ScriptCommands.h"

#include <boost/uuid/uuid_generators.hpp>

#include "LuaEngine.h"
#include "ProjectPlayerService.h"
#include "ProjectsService.h"
#include "strUtils.h"
#include "easylogging/easylogging++.h"

using namespace plugin;

extern bool restart;

void Actor::deleteEditorPed() {
	if (this->editorPed_.has_value() && !restart) {
		if (const auto ped = this->editorPed_.value(); CPools::ms_pPedPool->IsObjectValid(ped)) {
			const int pedRef = CPools::GetPedRef(this->editorPed_.value());
			Command<Commands::DELETE_CHAR>(pedRef);
		}
		this->editorPed_ = std::nullopt;
	}
}

void Actor::spawnProjectEntity() {
	if (projectPed_.has_value())
		this->deleteProjectEntity();

	this->projectPed_ = spawnPed();

	if (this->group == 0) {
		int g;
		Command<Commands::GET_PLAYER_GROUP>(0, &g);
		Command<Commands::SET_GROUP_MEMBER>(g, CPools::GetPedRef(this->projectPed_.value()));
	}
	this->projectPed_.value()->m_fMaxHealth = max(this->projectPed_.value()->m_fMaxHealth, this->health);
	this->projectPed_.value()->m_fHealth = static_cast<float>(this->health);

	updateLocation();

	auto scene = ProjectPlayerService::getInstance().getCurrentScene();
	auto tasklist = ProjectPlayerService::getInstance().getSceneTasklist();

	if (scene.has_value() && tasklist != nullptr) {
		/*const auto onActorSpawn = LuaEngine::getInstance().getLuaState()["global_data"]["signals"]["onActorSpawn"].
			get_or_create<sol::table>();
		for (auto [_, func] : onActorSpawn) {
			if (const auto result = func.as<sol::function>()(scene.value(), tasklist, this->uuid); !result.valid()) {
				const sol::error err = result;
				CLOG(ERROR, "lua") << err.what();
			}
		}*/
	}
}

void Actor::deleteProjectEntity() {
	if (this->projectPed_.has_value() && !restart) {
		if (const auto ped = this->projectPed_.value(); CPools::ms_pPedPool->IsObjectValid(ped)) {
			int pedRef = CPools::GetPedRef(this->projectPed_.value());
			Command<Commands::DELETE_CHAR>(pedRef);
			this->projectPed_ = std::nullopt;

			auto scene = ProjectPlayerService::getInstance().getCurrentScene();
			auto tasklist = ProjectPlayerService::getInstance().getSceneTasklist();

			if (scene.has_value() && tasklist != nullptr) {
				/*const auto onActorDelete = LuaEngine::getInstance().getLuaState()["global_data"]["signals"][
						"onActorDelete"]
					.get_or_create<sol::table>();
				for (auto [_, func] : onActorDelete) {
					if (const auto result = func.as<sol::function>()(scene.value(), tasklist, this->uuid); !result.
						valid()) {
						const sol::error err = result;
						CLOG(ERROR, "lua") << err.what();
					}
				}*/
			}
		}
	}
}

Actor Actor::copy() const {
	Actor copy(*this);
	copy.name = copy.name + " (copy)";
	copy.uuid = boost::uuids::random_generator()();
	copy.editorPed_ = std::nullopt;
	copy.projectPed_ = std::nullopt;
	return copy;
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

	if (this->modelType == 0 && this->modelId == 0 && this->dressUp) {
		const auto playerClothes = CWorld::Players[0].m_pPed->m_pPlayerData->m_pPedClothesDesc;
		std::memcpy(playerClothes->m_anTextureKeys, this->clotherMAnTextureKeys.data(),
		            sizeof playerClothes->m_anTextureKeys);
		std::memcpy(playerClothes->m_anModelKeys, this->clotherMAnModelKeys.data(),
		            sizeof playerClothes->m_anModelKeys);
		playerClothes->m_fFatStat = this->fatStat;
		playerClothes->m_fMuscleStat = this->musculeStat;
		CClothes::RebuildPlayer(CWorld::Players[0].m_pPed, false);
	}

	if (this->modelType == 0) {
		model = ModelsService::validPedModel(this->modelId);
		CStreaming::RequestModel(model, GAME_REQUIRED);
	} else {
		model = 290 + this->slot;
		const auto &specialsPed = ModelsService::getInstance().getSpecialsPed().at(this->modelId);
		CStreaming::RequestSpecialChar(this->slot, specialsPed.c_str(), GAME_REQUIRED);
	}
	CStreaming::LoadAllRequestedModels(false);

	int newPed;
	Command<Commands::CREATE_CHAR>(23 + this->group, model, this->pos[0], this->pos[1], this->pos[2], &newPed);
	CStreaming::SetMissionDoesntRequireModel(model);

	CPed *ped = CPools::GetPed(newPed);

	for (const auto &[weaponRaw, ammo] : this->weapons) {
		int weapon = ModelsService::validWeaponModel(weaponRaw);
		const int weaponModel = CWeaponInfo::GetWeaponInfo(static_cast<eWeaponType>(weapon), 1)->m_nModelId1;

		CStreaming::RequestModel(weaponModel, GAME_REQUIRED);
		CStreaming::LoadAllRequestedModels(false);

		ped->GiveWeapon(static_cast<eWeaponType>(weapon), ammo, false);

		CStreaming::SetMissionDoesntRequireModel(weaponModel);
	}
	if (!this->weapons.empty()) {
		int currentWeapon = this->weapons.at(this->defaultWeapon).weapon;
		ped->SetCurrentWeapon(static_cast<eWeaponType>(currentWeapon));
	}

	ped->DisablePedSpeech(1);

	if (this->modelType == 0 && this->modelId == 0 && this->dressUp) {
		Command<Commands::SET_PLAYER_MODEL>(0, playerModel);
		const auto playerClothes = CWorld::Players[0].m_pPed->m_pPlayerData->m_pPedClothesDesc;
		std::memcpy(playerClothes->m_anTextureKeys, textures.data(), sizeof playerClothes->m_anTextureKeys);
		std::memcpy(playerClothes->m_anModelKeys, models.data(), sizeof playerClothes->m_anModelKeys);
		playerClothes->m_fFatStat = fats;
		playerClothes->m_fMuscleStat = muscular;
		CClothes::RebuildPlayer(CWorld::Players[0].m_pPed, false);
	}

	Command<Commands::SET_CHAR_STAY_IN_SAME_PLACE>(newPed, static_cast<int>(this->stayInSamePlace));
	Command<Commands::SET_CHAR_KINDA_STAY_IN_SAME_PLACE>(newPed, static_cast<int>(this->kindaStayInSamePlace));
	ped->m_nWeaponAccuracy = this->accuracy;
	Command<Commands::SET_CHAR_SUFFERS_CRITICAL_HITS>(newPed, static_cast<int>(this->headshot));
	ped->m_nPedFlags.bDoesntDropWeaponsWhenDead = static_cast<unsigned>(!this->dropWeapons);

	Command<Commands::SET_CHAR_AREA_VISIBLE>(ped, this->interiorId);

	return ped;
}

Actor::Actor(const char *name, const CVector &pos, float headingAngle): ObjectiveDependent(nullptr),
                                                                        uuid(boost::uuids::random_generator()()),
                                                                        pos{pos.x, pos.y, pos.z},
                                                                        headingAngle(headingAngle),
                                                                        accuracy(50),
                                                                        health(100),
                                                                        headshot(true) {
	this->name = name;

	const auto playerClothes = CWorld::Players[0].m_pPed->m_pPlayerData->m_pPedClothesDesc;
	std::memcpy(this->clotherMAnTextureKeys.data(), playerClothes->m_anTextureKeys,
	            sizeof playerClothes->m_anTextureKeys);
	std::memcpy(this->clotherMAnModelKeys.data(), playerClothes->m_anModelKeys, sizeof playerClothes->m_anModelKeys);
	this->fatStat = playerClothes->m_fFatStat;
	this->musculeStat = playerClothes->m_fMuscleStat;
}

Actor::~Actor() {
	this->deleteEditorPed();
	this->deleteProjectEntity();
	this->signalDeleteActor();
}

std::string& Actor::getName() {
	return this->name;
}

float* Actor::getPosition() {
	return this->pos.data();
}

std::optional<CPed*>& Actor::getEditorPed() {
	return editorPed_;
}

std::optional<CPed*>& Actor::getProjectPed() {
	return projectPed_;
}

float& Actor::getHeadingAngle() {
	return headingAngle;
}

int& Actor::getGroup() {
	return group;
}

int& Actor::getModelId() {
	return modelId;
}

std::vector<Weapon>& Actor::getWeapons() {
	return weapons;
}

int& Actor::getDefaultWeapon() {
	return defaultWeapon;
}

unsigned char& Actor::getModelType() {
	return modelType;
}

int& Actor::getSlot() {
	return slot;
}

int& Actor::getAccuracy() {
	return accuracy;
}

int& Actor::getHealth() {
	return health;
}

bool& Actor::isRandomSpawn() {
	return randomSpawn;
}

bool& Actor::isShouldNotDie() {
	return shouldNotDie;
}

bool& Actor::isStayInSamePlace() {
	return stayInSamePlace;
}

bool& Actor::isKindaStayInSamePlace() {
	return kindaStayInSamePlace;
}

bool& Actor::isHeadshot() {
	return headshot;
}

bool& Actor::isDropWeapons() {
	return dropWeapons;
}

bool& Actor::isDressUp() {
	return dressUp;
}

std::array<unsigned, 10>& Actor::getClotherMAnModelKeys() {
	return clotherMAnModelKeys;
}

std::array<unsigned, 18>& Actor::getClotherMAnTextureKeys() {
	return clotherMAnTextureKeys;
}

float& Actor::getFatStat() {
	return fatStat;
}

float& Actor::getMusculeStat() {
	return musculeStat;
}

int& Actor::getInteriorId() { return interiorId; }

boost::uuids::uuid& Actor::getUuid() {
	return uuid;
}

boost::signals2::signal<void()>& Actor::getSignalDeleteActor() {
	return signalDeleteActor;
}

void Actor::updateLocation() const {
	if (this->editorPed_.has_value()) {
		this->editorPed_.value()->SetPosn(this->pos[0], this->pos[1], this->pos[2]);
		Command<Commands::SET_CHAR_HEADING>(this->editorPed_.value(), this->headingAngle);
	}
	if (this->projectPed_.has_value()) {
		this->projectPed_.value()->SetPosn(this->pos[0], this->pos[1], this->pos[2]);
		Command<Commands::SET_CHAR_HEADING>(this->projectPed_.value(), this->headingAngle);
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
