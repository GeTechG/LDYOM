#pragma once
#include <array>
#include <CVector.h>
#include <ktcoro_wait.hpp>
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

class VisualEffect final : public ObjectiveDependent, public INameable, public IPositionable, public IUuidable {
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar & boost::serialization::base_object<ObjectiveDependent>(*this);
		ar & this->uuid_;
		ar & boost::serialization::make_array(this->name_.data(), this->name_.size());
		ar & boost::serialization::make_array(this->pos_.data(), this->pos_.size());
		ar & type_;
		ar & size_;
		ar & effectType_;
		ar & flare_;
		ar & boost::serialization::make_array(color_.data(), color_.size());
		ar& drawing_;
		ar & angle_;
	}

	boost::uuids::uuid uuid_{};

	std::optional<unsigned> editorVisualEffect_;
	std::optional<unsigned> projectVisualEffect_;

	std::array<char, NAME_SIZE> name_{};
	std::array<float, 3> pos_{};
	int type_ = 0;
	float size_ = 1.0f;
	int effectType_ = 0;
	int flare_ = 0;
	std::array<float, 4> color_ = { 1.f, 1.f, 1.f, 1.f };
	bool drawing_ = false;
	float angle_ = 0.f;

public:
	VisualEffect() = default;
	VisualEffect(const char* name, const CVector& pos);

	VisualEffect(const VisualEffect& other);
	VisualEffect& operator=(const VisualEffect& other);

	~VisualEffect() override;

	boost::uuids::uuid& getUuid() override;
	int& getType();
	float& getSize();
	int& getEffectType();
	int& getFlare();
	std::array<float, 4>& getColor();
	bool& isDrawing();
	float& getAngle();

	char* getName() override;
	float* getPosition() override;

	void spawnEditorVisualEffect();
	void deleteEditorVisualEffect();

	void spawnProjectEntity() override;
	void deleteProjectEntity() override;

	void drawVisualEffect();
};
