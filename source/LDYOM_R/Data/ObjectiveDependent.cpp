#include "ObjectiveDependent.h"

ObjectiveDependent::ObjectiveDependent(ObjectiveDependent&& other) noexcept: useObjective(other.useObjective),
                                                                             spawnObjectiveUuid_{
	                                                                             other.spawnObjectiveUuid_
                                                                             },
                                                                             deleteObjectiveUuid_{
	                                                                             other.deleteObjectiveUuid_
                                                                             } {
}

ObjectiveDependent& ObjectiveDependent::operator=(const ObjectiveDependent& other) {
	if (this == &other)
		return *this;
	useObjective = other.useObjective;
	spawnObjectiveUuid_ = other.spawnObjectiveUuid_;
	deleteObjectiveUuid_ = other.deleteObjectiveUuid_;
	return *this;
}

ObjectiveDependent& ObjectiveDependent::operator=(ObjectiveDependent&& other) noexcept {
	if (this == &other)
		return *this;
	useObjective = other.useObjective;
	spawnObjectiveUuid_ = other.spawnObjectiveUuid_;
	deleteObjectiveUuid_ = other.deleteObjectiveUuid_;
	return *this;
}

bool& ObjectiveDependent::isUseObjective() {
	return useObjective;
}

boost::uuids::uuid& ObjectiveDependent::getSpawnObjectiveUuid() {
	return spawnObjectiveUuid_;
}

boost::uuids::uuid& ObjectiveDependent::getDeleteObjectiveUuid() {
	return deleteObjectiveUuid_;
}
