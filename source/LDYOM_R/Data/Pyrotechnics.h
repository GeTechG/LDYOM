#pragma once
#include <array>
#include <CObject.h>
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

class Pyrotechnics final : public ObjectiveDependent, public INameable, public IPositionable, public IUuidable {
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar & boost::serialization::base_object<ObjectiveDependent>(*this);
		ar & this->uuid_;
		ar& boost::serialization::make_array(this->name_.data(), this->name_.size());
		ar& boost::serialization::make_array(this->pos_.data(), this->pos_.size());
		ar& this->type_;
		ar& this->typeExplosion_;
		ar& this->sizeFire_;
		ar & this->propagationFire_;
	}

	boost::uuids::uuid uuid_{};

	std::optional<CObject*> editorExplosionObject_;
	std::optional<int> editorFire_;
	std::optional<int> projectFire_;

	std::array<char, NAME_SIZE> name_{};
	std::array<float, 3> pos_{};
	int type_ = 0;
	int typeExplosion_ = 0;
	int sizeFire_ = 0;
	int propagationFire_ = 0;


	std::optional<int> spawnFire(bool editor = false);
public:
	Pyrotechnics() = default;
	Pyrotechnics(const char* name, const CVector& pos);

	Pyrotechnics(const Pyrotechnics& other);

	Pyrotechnics& operator=(const Pyrotechnics& other);

	~Pyrotechnics() override;

	boost::uuids::uuid& getUuid() override;

	std::optional<CFire*> getEditorFire();
	std::optional<CFire*> getProjectFire();

	int& getType();
	int& getTypeExplosion();
	int& getSizeFire();
	int& getPropagationFire();
	std::optional<int>& getProjectFireId();

	void updateLocation();

	char* getName() override;
	float* getPosition() override;

	void spawnEditorPyrotechnics();
	void deleteEditorPyrotechnics();

	void spawnProjectEntity() override;
	void deleteProjectEntity() override;
};
