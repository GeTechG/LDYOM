#pragma once
#include <array>
#include <CPickup.h>
#include <optional>
#include <boost/serialization/access.hpp>
#include "boost/uuid/uuid.hpp"
#include <boost/serialization/base_object.hpp>

#include "constants.h"
#include "INameable.h"
#include "IPositionable.h"
#include "IUuidable.h"
#include "ObjectiveDependent.h"
#include <boost/serialization/array.hpp>

class Pickup final : public ObjectiveDependent, public INameable, public IPositionable, public IUuidable {
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar & boost::serialization::base_object<ObjectiveDependent>(*this);
		ar & this->uuid_;
		ar& boost::serialization::make_array(this->name_.data(), this->name_.size());
		ar& boost::serialization::make_array(this->pos_.data(), this->pos_.size());
		ar& this->modelId_;
		ar& this->type_;
		ar& this->spawnType_;
		ar& this->weapon_;
		ar& this->ammo_;
	}

	boost::uuids::uuid uuid_{};

	std::optional<int> editorPickup_;
	std::optional<int> projectPickup_;

	std::array<char, NAME_SIZE> name_{};
	std::array<float, 3> pos_{};
	int type_ = 0;
	int spawnType_ = 0;
	int weapon_ = 1;
	int ammo_ = 0;
	int modelId_ = 0;

	std::optional<int> spawnPickup(int pickupType);
public:
	Pickup() = default;
	Pickup(const char* name, const CVector& pos);

	Pickup(const Pickup& other);
	Pickup& operator=(const Pickup& other);

	~Pickup() override;

	boost::uuids::uuid& getUuid() override;

	std::optional<CPickup*> getEditorPickup();
	std::optional<CPickup*> getProjectPickup();

	int& getModelId();
	int& getType();
	int& getSpawnType();
	int& getWeapon();
	int& getAmmo();
	std::optional<int>& getProjectPickupIndex();
	std::optional<int>& getEditorPickupIndex();

	void updateLocation();

	char* getName() override;
	float* getPosition() override;

	void spawnEditorPickup();
	void deleteEditorPickup();

	void spawnProjectEntity() override;
	void deleteProjectEntity() override;
};
