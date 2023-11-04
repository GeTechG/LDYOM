#include "ObjectiveDependent.h"
#include "WindowsRenderService.h"
#include "../Windows/ObjectivesWindow.h"

ObjectiveDependent::ObjectiveDependent(void *new_) {
	const int currentObjective = Windows::WindowsRenderService::getInstance().getWindow<Windows::ObjectivesWindow>()->
	                                                                          getElement();
	if (currentObjective != -1)
		this->spawnObjectiveUuid_ = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->
		                                                           getObjectives().at(currentObjective)->getUuid();
}

ObjectiveDependent::ObjectiveDependent(ObjectiveDependent &&other) noexcept: useObjective(other.useObjective),
                                                                             spawnObjectiveUuid_{
	                                                                             other.spawnObjectiveUuid_
                                                                             },
                                                                             deleteObjectiveUuid_{
	                                                                             other.deleteObjectiveUuid_
                                                                             } {}

ObjectiveDependent& ObjectiveDependent::operator=(const ObjectiveDependent &other) {
	if (this == &other)
		return *this;
	useObjective = other.useObjective;
	spawnObjectiveUuid_ = other.spawnObjectiveUuid_;
	deleteObjectiveUuid_ = other.deleteObjectiveUuid_;
	return *this;
}

ObjectiveDependent& ObjectiveDependent::operator=(ObjectiveDependent &&other) noexcept {
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
