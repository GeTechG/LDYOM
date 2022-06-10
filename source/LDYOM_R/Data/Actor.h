#pragma once
#include "plugin.h"
#include <optional>
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

#include "constants.h"
#include "INameable.h"
#include "IPositionable.h"
#include "IUuidable.h"
#include "ObjectiveDependent.h"
#include "Weapon.h"

class Actor final : public ObjectiveDependent, public INameable, public IPositionable, public IUuidable {
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<ObjectiveDependent>(*this);
		ar & this->uuid_;
		ar & this->name_;
		ar & this->pos_;
		ar & this->headingAngle_;
		ar & this->group_;
		ar & this->modelType_;
		ar & this->slot_;
		ar & this->modelId_;
		ar & this->weapons_;
		ar & this->defaultWeapon_;
		ar & this->accuracy_;
		ar & this->health_;
		ar & this->randomSpawn_;
		ar & this->shouldNotDie_;
		ar & this->stayInSamePlace_;
		ar & this->kindaStayInSamePlace_;
		ar & this->headshot_;
		ar & this->dropWeapons_;
	}

	boost::uuids::uuid uuid_{};

	std::optional<CPed*> editorPed_;
	std::optional<CPed*> projectPed_;

	char name_[NAME_SIZE]{};
	float pos_[3]{};
	float headingAngle_{};
	int group_{};
	unsigned char modelType_{};
	int slot_{};
	int modelId_{};
	std::vector<Weapon> weapons_;
	int defaultWeapon_{};
	int accuracy_{};
	int health_{};
	bool randomSpawn_{};
	bool shouldNotDie_{};
	bool stayInSamePlace_{};
	bool kindaStayInSamePlace_{};
	bool headshot_{};
	bool dropWeapons_{};

	CPed* spawnPed();
public:
	Actor() = default;
	Actor(const char* name, const CVector& pos, float headingAngle);

	Actor(const Actor& other);

	Actor& operator=(Actor&& other) noexcept;

	~Actor() override;

	char* getName() override;
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
	boost::uuids::uuid& getUuid() override;

	void updateLocation() const;

	void spawnEditorPed();
	void deleteEditorPed();

	void spawnProjectEntity() override;
	void deleteProjectEntity() override;
};
