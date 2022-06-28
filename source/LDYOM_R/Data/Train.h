#pragma once
#include <optional>
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

#include "constants.h"
#include "INameable.h"
#include "IPositionable.h"
#include "IUuidable.h"
#include "ObjectiveDependent.h"
#include "CTrain.h"

class Train final : public ObjectiveDependent, public INameable, public IPositionable, public IUuidable {
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& boost::serialization::base_object<ObjectiveDependent>(*this);
		ar& this->uuid_;
		ar& this->name_;
		ar& this->pos_;
		ar& this->rotate_;
		ar& this->health_;
		ar& this->trainType_;
		ar& this->speed_;
		ar& this->cruiseSpeed_;
	}

	boost::uuids::uuid uuid_{};

	std::optional<CTrain*> editorTrain_;
	std::optional<CTrain*> projectTrain_;
	char name_[NAME_SIZE]{};
	float pos_[3]{};
	bool rotate_ = false;
	int health_ = 1000;
	int trainType_{};
	float speed_ = 0.0f;
	float cruiseSpeed_ = 0.0f;

	CTrain* spawnTrain();
public:
	Train() = default;
	Train(const char* name, const CVector & pos);
	Train(const Train& other);
	Train& operator=(const Train& other);
	~Train() override;

	boost::uuids::uuid& getUuid() override;

	std::optional<CTrain*>& getEditorTrain();
	std::optional<CTrain*>& getProjectTrain();

	bool& isRotate();
	int& getHealth();
	int& getTrainType();
	float& getSpeed();
	float& getCruiseSpeed();

	void updateLocation();

	char* getName() override;
	float* getPosition() override;

	void spawnEditorTrain();
	void deleteEditorTrain();

	void spawnProjectEntity() override;
	void deleteProjectEntity() override;
};
