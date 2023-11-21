#include "ObjectiveDependent.h"
#include "WindowsRenderService.h"
#include "../Windows/ObjectivesWindow.h"

ObjectiveDependent::ObjectiveDependent(void *new_) {
	const int currentObjective = Windows::WindowsRenderService::getInstance().getWindow<Windows::ObjectivesWindow>()->
	                                                                          getElement();
	if (currentObjective != -1)
		this->spawnObjectiveUuid = ProjectsService::getInstance().getCurrentProject().getCurrentScene()->
		                                                          getObjectives().at(currentObjective)->getUuid();
}

bool& ObjectiveDependent::isUseObjective() {
	return useObjective;
}

boost::uuids::uuid& ObjectiveDependent::getSpawnObjectiveUuid() {
	return spawnObjectiveUuid;
}

boost::uuids::uuid& ObjectiveDependent::getDeleteObjectiveUuid() {
	return deleteObjectiveUuid;
}
