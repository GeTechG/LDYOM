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

class Object final : public ObjectiveDependent, public INameable, public IPositionable, public IUuidable {
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar & boost::serialization::base_object<ObjectiveDependent>(*this);
		ar & this->uuid_;
		ar & this->name_;
		ar & this->pos_;
		ar & this->rotate;
		ar & boost::serialization::make_array(this->scale_.data(), this->scale_.size());
		ar & this->modelId_;
	}

	boost::uuids::uuid uuid_{};

	std::optional<CObject*> editorObject_;
	std::optional<CObject*> projectObject_;

	char name_[NAME_SIZE]{};
	float pos_[3]{};
	CQuaternion rotate;
	std::array<float, 3> scale_ = { 1.f, 1.f, 1.f };
	int modelId_{};

	std::optional<CObject*> spawnObject();
public:
	Object() = default;
	Object(const char* name, const CVector& pos);

	Object(const Object& other);

	Object& operator=(Object other);

	~Object() override;

	boost::uuids::uuid& getUuid() override;

	std::optional<CObject*>& getEditorObject();
	std::optional<CObject*>& getProjectObject();

	CQuaternion& getRotations();
	std::array<float, 3>& getScale();
	int& getModelId();

	void updateLocation();

	char* getName() override;
	float* getPosition() override;

	void spawnEditorObject();
	void deleteEditorObject();

	void spawnProjectEntity() override;
	void deleteProjectEntity() override;
};
