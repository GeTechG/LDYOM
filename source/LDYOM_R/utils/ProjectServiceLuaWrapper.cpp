
#include <sol.hpp>

#include "ProjectsService.h"

void ProjectsServiceLuaWrap(sol::state& state) {
	auto clazz = state.new_usertype<ProjectsService>("ProjectsService");
	clazz["getInstance"] = &ProjectsService::getInstance;
	clazz["Init"] = &ProjectsService::Init;
	clazz["update"] = &ProjectsService::update;
	clazz["Reset"] = &ProjectsService::Reset;
	clazz["createNewProject"] = &ProjectsService::createNewProject;
	clazz["saveCurrentProject"] = &ProjectsService::saveCurrentProject;
	clazz["loadProject"] = &ProjectsService::loadProject;
	clazz["deleteProject"] = &ProjectsService::deleteProject;
	clazz["getProjectsInfos"] = &ProjectsService::getProjectsInfos;
	clazz["onUpdate"] = &ProjectsService::onUpdate;
	clazz["getCurrentProject"] = &ProjectsService::getCurrentProject;
}
