#pragma once
#include <CVehicle.h>
#include <optional>
#include <boost/serialization/access.hpp>
#include "boost/uuid/uuid.hpp"
#include <boost/serialization/base_object.hpp>

#include "constants.h"
#include "INameable.h"
#include "IPositionable.h"
#include "IUuidable.h"
#include "ObjectiveDependent.h"

class Vehicle final : public ObjectiveDependent, public INameable, public IPositionable, public IUuidable {
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar & boost::serialization::base_object<ObjectiveDependent>(*this);
		ar & this->uuid_;
		ar & this->name_;
		ar & this->pos_;
		ar & this->headingAngle_;
		ar & this->modelId_;
		ar & this->shouldNotDie_;
		ar & this->health_;
		ar & this->bulletproof_;
		ar & this->fireproof_;
		ar & this->explosionproof_;
		ar & this->collisionproof_;
		ar & this->meleeproof_;
		ar & this->tiresVulnerability_;
		ar & this->extendedColor_;
		ar & this->primaryColor_;
		ar & this->secondaryColor_;
		ar & this->locked_;
		ar & this->colors_;
		ar & this->componentTypeA_;
		ar & this->componentTypeB_;
		ar & this->numberplateCity_;
		ar & this->numberplate_;
	}

	boost::uuids::uuid uuid_{};

	std::optional<CVehicle*> editorVehicle_;
	std::optional<CVehicle*> projectVehicle_;

	char name_[NAME_SIZE]{};
	float pos_[3]{};
	float headingAngle_{};
	int modelId_{};
	bool shouldNotDie_{};
	int health_{};
	bool bulletproof_{};
	bool fireproof_{};
	bool explosionproof_{};
	bool collisionproof_{};
	bool meleeproof_{};
	bool tiresVulnerability_{};
	bool extendedColor_{};
	float primaryColor_[4]{};
	float secondaryColor_[4]{};
	bool locked_{};
	std::vector<std::pair<unsigned char, std::array<float, 4>>> colors_;
	int componentTypeA_{};
	int componentTypeB_{};
	int numberplateCity_{};
	char numberplate_[9]{};

	void recolorVehicle(bool recolor, CVehicle* newVehicle);
	CVehicle* spawnVehicle(bool recolor);
public:
	Vehicle() = default;
	Vehicle(const char* name, const CVector& pos, float headingAngle);

	Vehicle(const Vehicle& other);

	Vehicle& operator=(Vehicle other);

	~Vehicle() override;

	boost::uuids::uuid& getUuid() override;

	std::optional<CVehicle*>& getEditorVehicle();
	std::optional<CVehicle*>& getProjectVehicle();

	int& getModelId();
	bool& isShouldNotDie();
	int& getHealth();
	bool& isBulletproof();
	bool& isFireproof();
	bool& isExplosionproof();
	bool& isCollisionproof();
	bool& isMeleeproof();
	bool& isTiresVulnerability();
	bool& isExtendedColor();
	bool& isLocked();
	std::vector<std::pair<unsigned char, std::array<float, 4>>>& getColors();
	int& getComponentTypeA();
	int& getComponentTypeB();
	int& getNumberplateCity();
	float* getPrimaryColor();
	float* getSecondaryColor();
	char* getNumberplate();

	void updateLocation() const;

	char* getName() override;
	float* getPosition() override;

	float& getHeadingAngle();

	void setEditorPrimaryColor();
	void setEditorSecondaryColor();

	void spawnEditorVehicle(bool recolor = false);
	void deleteEditorVehicle();

	void spawnProjectEntity() override;
	void deleteProjectEntity() override;
};
