#pragma once
#include <array>
#include <CCheckpoint.h>
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

class Checkpoint final : public ObjectiveDependent, public INameable, public IPositionable, public IUuidable {
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar & boost::serialization::base_object<ObjectiveDependent>(*this);
		ar & this->uuid_;
		ar& boost::serialization::make_array(this->name_.data(), this->name_.size());
		ar& boost::serialization::make_array(this->pos_.data(), this->pos_.size());
		ar & type;
		ar & radius_;
		ar & blipColor_;
		ar & boost::serialization::make_array(color_.data(), color_.size());
		ar & checkpointType_;
		ar & pulsePeriod;
		ar & pulseFraction;
		ar & rotateRate;
		ar & blipType_;
		ar & blipSprite_;
		ar & angle;
	}

	boost::uuids::uuid uuid_{};

	std::optional<int> editorCheckpoint_;
	std::optional<int> projectCheckpoint_;
	std::optional<int> editorBlip;
	std::optional<int> projectBlip;
	std::optional<unsigned> editorSphere;
	std::optional<unsigned> projectSphere;

	std::array<char, NAME_SIZE> name_{};
	std::array<float, 3> pos_{};
	int type = 0;
	float radius_ = 1.f;
	int   blipColor_ = 0;
	std::array<float, 4> color_ = { 1.f, 0.f, 0.f, 0.894f };
	int checkpointType_ = 0;
	int pulsePeriod = 2048;
	float pulseFraction = 0.1f;
	int rotateRate = 0;
	int blipType_ = 0;
	int blipSprite_ = 0;
	float angle = 0.f;
	bool rerender = false;

	std::optional<int> spawnCheckpoint();
public:
	Checkpoint() = default;
	Checkpoint(const char* name, const CVector& pos);

	Checkpoint(const Checkpoint& other);
	Checkpoint& operator=(const Checkpoint& other);

	~Checkpoint() override;

	boost::uuids::uuid& getUuid() override;

	std::optional<CCheckpoint*> getEditorCheckpoint();
	std::optional<CCheckpoint*> getProjectCheckpoint();
	std::optional<unsigned>& getEditorSphere();
	std::optional<unsigned>& getProjectSphere();

	void updateLocation();

	char* getName() override;
	float* getPosition() override;
	int& getType();
	float& getRadius();
	int& getBlipColor();
	std::array<float, 4>& getColor();
	int& getCheckpointType();
	int& getPulsePeriod();
	float& getPulseFraction();
	int& getRotateRate();
	int& getBlipType();
	int& getBlipSprite();
	float& getAngle();


	void spawnEditorCheckpoint();
	void deleteEditorCheckpoint();

	void spawnProjectEntity() override;
	void deleteProjectEntity() override;

	void spawnEditorBlip();
	void deleteEditorBlip();

	void spawnProjectBlip();
	void deleteProjectBlip();

	void drawSphere();
	bool existProjectEntity();
	CVector getProjectEntityPosition();
};
