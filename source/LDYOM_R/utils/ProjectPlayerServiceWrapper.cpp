#include <sol.hpp>

#include "ProjectPlayerService.h"
#include "../Data/BaseObjective.h"
#include "../Data/Scene.h"

void projectPlayerServiceWrapper(sol::state &state) {
	auto ldyomTable = state["ld"].get_or_create<sol::table>();
	ldyomTable["projectPlayerService"] = &ProjectPlayerService::getInstance();
	state.new_usertype<ProjectPlayerService>("LDProjectPlayerService", sol::no_constructor,
	                                         "getCurrentScene", &ProjectPlayerService::getCurrentScene,
	                                         "getCurrentObjective", &ProjectPlayerService::getCurrentObjective,
	                                         "setNextScene", &ProjectPlayerService::setNextScene,
	                                         "setNextObjective", &ProjectPlayerService::setNextObjective,
	                                         "stopCurrentScene", &ProjectPlayerService::stopCurrentScene,
	                                         "getSceneTasklist", &ProjectPlayerService::getSceneTasklist
	);
}
