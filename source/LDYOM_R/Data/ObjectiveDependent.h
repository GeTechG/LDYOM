#pragma once
#include <boost/serialization/access.hpp>
#include <boost/uuid/uuid.hpp>

class ObjectiveDependent {
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar & this->useObjective;
		ar & this->spawnObjectiveUuid_;
		ar & this->deleteObjectiveUuid_;
	}
protected:
	bool useObjective = true;
	boost::uuids::uuid spawnObjectiveUuid_{};
	boost::uuids::uuid deleteObjectiveUuid_{};
public:
	ObjectiveDependent() = default;
	ObjectiveDependent(const ObjectiveDependent& other) = default;
	ObjectiveDependent(ObjectiveDependent&& other) noexcept;
	ObjectiveDependent& operator=(const ObjectiveDependent& other);
	ObjectiveDependent& operator=(ObjectiveDependent&& other) noexcept;
	virtual ~ObjectiveDependent() = default;

	virtual void spawnProjectEntity() = 0;
	virtual void deleteProjectEntity() = 0;

	bool& isUseObjective();
	boost::uuids::uuid& getSpawnObjectiveUuid();
	boost::uuids::uuid& getDeleteObjectiveUuid();
};
