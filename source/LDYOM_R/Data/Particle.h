#pragma once
#include <array>
#include <CMatrix.h>
#include <FxSystem_c.h>
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
#include <boost/signals2.hpp>

class Particle final : public ObjectiveDependent, public INameable, public IPositionable, public IUuidable {
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar & boost::serialization::base_object<ObjectiveDependent>(*this);
		ar & this->uuid_;
		ar& boost::serialization::make_array(this->name_.data(), this->name_.size());
		ar& boost::serialization::make_array(this->pos_.data(), this->pos_.size());
		ar & this->rotate;
		ar & boost::serialization::make_array(this->scale_.data(), this->scale_.size());
		ar & this->particleType_;
		ar & this->attachType_;
		ar& this->attachUuid_;
		ar & this->pedBodeId_;
	}

	boost::uuids::uuid uuid_{};

	std::optional<int> editorParticle_;
	std::optional<int> projectParticle_;

	std::array<char, NAME_SIZE> name_{};
	std::array<float, 3> pos_{};
	CQuaternion rotate;
	std::array<float, 3> scale_ = { 1.f, 1.f, 1.f };
	int particleType_{};
	int attachType_ = 0;
	boost::uuids::uuid attachUuid_{};
	int pedBodeId_ = 1;

	std::optional<int> spawnParticle();
public:
	Particle() = default;
	Particle(const char* name, const CVector& pos);

	Particle(const Particle& other);

	Particle& operator=(const Particle& other);

	~Particle() override;

	boost::uuids::uuid& getUuid() override;

	std::optional<FxSystem_c*> getEditorParticle();
	std::optional<FxSystem_c*> getProjectParticle();

	CQuaternion& getRotations();
	std::array<float, 3>& getScale();
	int& getParticleType();
	int& getAttachType();
	boost::uuids::uuid& getAttachUuid();
	int& getPedBodeId();
	std::optional<int>& getProjectParticleId();

	void updateLocation();

	char* getName() override;
	float* getPosition() override;

	void spawnEditorParticle();
	void deleteEditorParticle();

	void spawnProjectEntity() override;
	void deleteProjectEntity() override;
};
