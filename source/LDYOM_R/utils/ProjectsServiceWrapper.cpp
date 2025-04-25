#include <sol.hpp>

#include "ProjectsService.h"

void projectsServiceWrapper(sol::state &state) {
	auto ldTable = state["ld"].get<sol::table>();
	ldTable["projectsService"] = &ProjectsService::getInstance;

	auto classProjectsService = state.new_usertype<ProjectsService>("LDProjectsService", sol::no_constructor);
	classProjectsService["createNewProject"] = &ProjectsService::createNewProject;
	classProjectsService["saveCurrentProject"] = &ProjectsService::saveCurrentProject;
	classProjectsService["loadProject"] = &ProjectsService::loadProject;
	classProjectsService["loadProductionProject"] = &ProjectsService::loadProductionProject;
	classProjectsService["deleteProject"] = &ProjectsService::deleteProject;
	classProjectsService["deleteProductionProject"] = &ProjectsService::deleteProductionProject;
	classProjectsService["makeProjectProduction"] = &ProjectsService::makeProjectProduction;
	classProjectsService["getProjectsInfos"] = &ProjectsService::getProjectsInfos;
	classProjectsService["getProductionProjectsInfos"] = &ProjectsService::getProductionProjectsInfos;
	classProjectsService["onUpdate"] = &ProjectsService::onUpdate;
	classProjectsService["getCurrentProject"] = &ProjectsService::getCurrentProject;
}
