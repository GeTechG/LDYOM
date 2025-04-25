#pragma once
#include <array>

#include <optional>
#include "INameable.h"
#include "IPositionable.h"
#include "IUuidable.h"
#include "ObjectiveDependent.h"
#include "plugin.h"
#include "Weapon.h"
#include "boost/signals2.hpp"

class Actor final : public ObjectiveDependent, public INameable, public IPositionable, public IUuidable {
private:
	boost::uuids::uuid uuid{};

	std::optional<CPed*> editorPed_;
	std::optional<CPed*> projectPed_;

	std::string name{};
	std::array<float, 3> pos{};
	float headingAngle{};
	int group = 1;
	unsigned char modelType{};
	int slot{};
	int modelId{};
	std::vector<Weapon> weapons;
	int defaultWeapon{};
	int accuracy{};
	int health{};
	bool randomSpawn{};
	bool shouldNotDie{};
	bool stayInSamePlace{};
	bool kindaStayInSamePlace{};
	bool headshot{};
	bool dropWeapons{};
	bool dressUp = true;
	std::array<unsigned, 10> clotherMAnModelKeys{};
	std::array<unsigned, 18> clotherMAnTextureKeys{};
	float fatStat = 1.f;
	float musculeStat = 1.f;
	int interiorId = 0;
	bool showHealthBarCounter = false;
	std::string healthBarCounterText{};

	boost::signals2::signal<void()> signalDeleteActor;
	std::optional<int> projectHealthBarCounter;

	CPed* spawnPed();

public:
	Actor() = default;
	Actor(const char *name, const CVector &pos, float headingAngle);

	Actor(const Actor &other)
		: ObjectiveDependent(other),
		  INameable(other),
		  IPositionable(other),
		  IUuidable(other),
		  uuid(other.uuid),
		  editorPed_(other.editorPed_),
		  projectPed_(other.projectPed_),
		  name(other.name),
		  pos(other.pos),
		  headingAngle(other.headingAngle),
		  group(other.group),
		  modelType(other.modelType),
		  slot(other.slot),
		  modelId(other.modelId),
		  weapons(other.weapons),
		  defaultWeapon(other.defaultWeapon),
		  accuracy(other.accuracy),
		  health(other.health),
		  randomSpawn(other.randomSpawn),
		  shouldNotDie(other.shouldNotDie),
		  stayInSamePlace(other.stayInSamePlace),
		  kindaStayInSamePlace(other.kindaStayInSamePlace),
		  headshot(other.headshot),
		  dropWeapons(other.dropWeapons),
		  dressUp(other.dressUp),
		  clotherMAnModelKeys(other.clotherMAnModelKeys),
		  clotherMAnTextureKeys(other.clotherMAnTextureKeys),
		  fatStat(other.fatStat),
		  musculeStat(other.musculeStat),
		  interiorId(other.interiorId),
		  showHealthBarCounter(other.showHealthBarCounter),
		  healthBarCounterText(other.healthBarCounterText) {}


	Actor(Actor &&other) noexcept
		: ObjectiveDependent(other),
		  INameable(other),
		  IPositionable(other),
		  IUuidable(other),
		  uuid(other.uuid),
		  editorPed_(other.editorPed_),
		  projectPed_(other.projectPed_),
		  name(std::move(other.name)),
		  pos(other.pos),
		  headingAngle(other.headingAngle),
		  group(other.group),
		  modelType(other.modelType),
		  slot(other.slot),
		  modelId(other.modelId),
		  weapons(std::move(other.weapons)),
		  defaultWeapon(other.defaultWeapon),
		  accuracy(other.accuracy),
		  health(other.health),
		  randomSpawn(other.randomSpawn),
		  shouldNotDie(other.shouldNotDie),
		  stayInSamePlace(other.stayInSamePlace),
		  kindaStayInSamePlace(other.kindaStayInSamePlace),
		  headshot(other.headshot),
		  dropWeapons(other.dropWeapons),
		  dressUp(other.dressUp),
		  clotherMAnModelKeys(other.clotherMAnModelKeys),
		  clotherMAnTextureKeys(other.clotherMAnTextureKeys),
		  fatStat(other.fatStat),
		  musculeStat(other.musculeStat),
		  interiorId(other.interiorId),
		  showHealthBarCounter(other.showHealthBarCounter),
		  healthBarCounterText(other.healthBarCounterText) {}

	Actor& operator=(const Actor &other) {
		if (this == &other)
			return *this;
		ObjectiveDependent::operator =(other);
		INameable::operator =(other);
		IPositionable::operator =(other);
		IUuidable::operator =(other);
		uuid = other.uuid;
		editorPed_ = other.editorPed_;
		projectPed_ = other.projectPed_;
		name = other.name;
		pos = other.pos;
		headingAngle = other.headingAngle;
		group = other.group;
		modelType = other.modelType;
		slot = other.slot;
		modelId = other.modelId;
		weapons = other.weapons;
		defaultWeapon = other.defaultWeapon;
		accuracy = other.accuracy;
		health = other.health;
		randomSpawn = other.randomSpawn;
		shouldNotDie = other.shouldNotDie;
		stayInSamePlace = other.stayInSamePlace;
		kindaStayInSamePlace = other.kindaStayInSamePlace;
		headshot = other.headshot;
		dropWeapons = other.dropWeapons;
		dressUp = other.dressUp;
		clotherMAnModelKeys = other.clotherMAnModelKeys;
		clotherMAnTextureKeys = other.clotherMAnTextureKeys;
		fatStat = other.fatStat;
		musculeStat = other.musculeStat;
		interiorId = other.interiorId;
		showHealthBarCounter = other.showHealthBarCounter;
		healthBarCounterText = other.healthBarCounterText;
		return *this;
	}

	Actor& operator=(Actor &&other) noexcept {
		if (this == &other)
			return *this;
		ObjectiveDependent::operator =(other);
		INameable::operator =(other);
		IPositionable::operator =(other);
		IUuidable::operator =(other);
		uuid = other.uuid;
		editorPed_ = other.editorPed_;
		projectPed_ = other.projectPed_;
		name = std::move(other.name);
		pos = other.pos;
		headingAngle = other.headingAngle;
		group = other.group;
		modelType = other.modelType;
		slot = other.slot;
		modelId = other.modelId;
		weapons = std::move(other.weapons);
		defaultWeapon = other.defaultWeapon;
		accuracy = other.accuracy;
		health = other.health;
		randomSpawn = other.randomSpawn;
		shouldNotDie = other.shouldNotDie;
		stayInSamePlace = other.stayInSamePlace;
		kindaStayInSamePlace = other.kindaStayInSamePlace;
		headshot = other.headshot;
		dropWeapons = other.dropWeapons;
		dressUp = other.dressUp;
		clotherMAnModelKeys = other.clotherMAnModelKeys;
		clotherMAnTextureKeys = other.clotherMAnTextureKeys;
		fatStat = other.fatStat;
		musculeStat = other.musculeStat;
		interiorId = other.interiorId;
		showHealthBarCounter = other.showHealthBarCounter;
		healthBarCounterText = other.healthBarCounterText;
		return *this;
	}

	~Actor() override;

	std::string& getName() override;
	float* getPosition() override;

	std::optional<CPed*>& getEditorPed();
	std::optional<CPed*>& getProjectPed();

	float& getHeadingAngle();
	int& getGroup();
	int& getModelId();
	std::vector<Weapon>& getWeapons();
	int& getDefaultWeapon();
	unsigned char& getModelType();
	int& getSlot();
	int& getAccuracy();
	int& getHealth();
	bool& isRandomSpawn();
	bool& isShouldNotDie();
	bool& isStayInSamePlace();
	bool& isKindaStayInSamePlace();
	bool& isHeadshot();
	bool& isDropWeapons();
	bool& isDressUp();
	std::array<unsigned, 10>& getClotherMAnModelKeys();
	std::array<unsigned, 18>& getClotherMAnTextureKeys();
	float& getFatStat();
	float& getMusculeStat();
	int& getInteriorId();
	bool& isShowHealthBarCounter();
	std::string& getHealthBarCounterText();
	boost::uuids::uuid& getUuid() override;

	boost::signals2::signal<void()>& getSignalDeleteActor();
	std::optional<int>& getProjectHealthBarCounter();

	void updateLocation() const;

	void spawnEditorPed();
	void deleteEditorPed();

	void spawnProjectEntity() override;
	void deleteProjectEntity() override;

	Actor copy() const;
};

NLOHMANN_JSON_NAMESPACE_BEGIN
	template <>
	struct adl_serializer<Actor> {
		static void to_json(json &j, const Actor &obj) {
			auto &base = static_cast<const ObjectiveDependent&>(obj);
			j = base;
			auto &a = const_cast<Actor&>(obj);
			j["uuid"] = uuidToJson(a.getUuid());
			j["name"] = a.getName();
			j["pos"] = arrayPtrToJson(a.getPosition(), 3);
			j["headingAngle"] = a.getHeadingAngle();
			j["group"] = a.getGroup();
			j["modelType"] = a.getModelType();
			j["slot"] = a.getSlot();
			j["modelId"] = a.getModelId();
			j["weapons"] = a.getWeapons();
			j["defaultWeapon"] = a.getDefaultWeapon();
			j["accuracy"] = a.getAccuracy();
			j["health"] = a.getHealth();
			j["randomSpawn"] = a.isRandomSpawn();
			j["shouldNotDie"] = a.isShouldNotDie();
			j["stayInSamePlace"] = a.isStayInSamePlace();
			j["kindaStayInSamePlace"] = a.isKindaStayInSamePlace();
			j["headshot"] = a.isHeadshot();
			j["dropWeapons"] = a.isDropWeapons();
			j["dressUp"] = a.isDressUp();
			j["clotherMAnModelKeys"] = a.getClotherMAnModelKeys();
			j["clotherMAnTextureKeys"] = a.getClotherMAnTextureKeys();
			j["musculeStat"] = a.getMusculeStat();
			j["fatStat"] = a.getFatStat();
			j["interiorId"] = a.getInteriorId();
			j["showHealthBarCounter"] = a.isShowHealthBarCounter();
			j["healthBarCounterText"] = a.getHealthBarCounterText();
		}

		static void from_json(const json &j, Actor &obj) {
			auto &base = static_cast<ObjectiveDependent&>(obj);
			j.get_to(base);
			jsonToUuid(j.at("uuid"), obj.getUuid());
			j.at("name").get_to(obj.getName());
			jsonToArrayPtr(j.at("pos"), obj.getPosition(), 3);
			j.at("headingAngle").get_to(obj.getHeadingAngle());
			j.at("group").get_to(obj.getGroup());
			j.at("modelType").get_to(obj.getModelType());
			j.at("slot").get_to(obj.getSlot());
			j.at("modelId").get_to(obj.getModelId());
			j.at("weapons").get_to(obj.getWeapons());
			j.at("defaultWeapon").get_to(obj.getDefaultWeapon());
			j.at("accuracy").get_to(obj.getAccuracy());
			j.at("health").get_to(obj.getHealth());
			j.at("randomSpawn").get_to(obj.isRandomSpawn());
			j.at("shouldNotDie").get_to(obj.isShouldNotDie());
			j.at("stayInSamePlace").get_to(obj.isStayInSamePlace());
			j.at("kindaStayInSamePlace").get_to(obj.isKindaStayInSamePlace());
			j.at("headshot").get_to(obj.isHeadshot());
			j.at("dropWeapons").get_to(obj.isDropWeapons());
			j.at("dressUp").get_to(obj.isDressUp());
			j.at("clotherMAnModelKeys").get_to(obj.getClotherMAnModelKeys());
			j.at("clotherMAnTextureKeys").get_to(obj.getClotherMAnTextureKeys());
			j.at("musculeStat").get_to(obj.getMusculeStat());
			j.at("fatStat").get_to(obj.getFatStat());
			j.at("interiorId").get_to(obj.getInteriorId());
			j.at("showHealthBarCounter").get_to(obj.isShowHealthBarCounter());
			j.at("healthBarCounterText").get_to(obj.getHealthBarCounterText());
		}
	};

NLOHMANN_JSON_NAMESPACE_END
