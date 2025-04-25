#include "ObjectiveDependent.h"

#include "Settings.h"
#include "WindowsRenderService.h"
#include "../Windows/ObjectivesWindow.h"

ObjectiveDependent::ObjectiveDependent(void *new_) {
	const int currentObjective = Windows::WindowsRenderService::getInstance().getWindow<Windows::ObjectivesWindow>()->
	                                                                          getElement();
	const auto &objectives = ProjectsService::getInstance()
	                         .getCurrentProject().getCurrentScene()->getObjectives();
	if (currentObjective != -1)
		this->spawnObjectiveUuid = objectives.at(currentObjective)->getUuid();

	if (Settings::getInstance().get<bool>(
		"main.autoBindDeleteAfterLastObjective").value_or(true)) {
		if (!objectives.empty()) {
			this->deleteObjectiveUuid = objectives
			                            .back()->getUuid();
		}
	}
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
