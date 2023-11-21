#include "BaseObjective.h"

#include <boost/uuid/random_generator.hpp>

#include "ProjectsService.h"
#include "fmt/core.h"
#include "Localization/Localization.h"

BaseObjective::BaseObjective(): uuid(boost::uuids::random_generator()()), name() {}

BaseObjective::BaseObjective(void *_new): uuid(boost::uuids::random_generator()()) {
	this->name = fmt::format("{} #{}", Localization::getInstance().get("objective.title"),
	                         ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives().
	                                                        size());
}

boost::uuids::uuid& BaseObjective::getUuid() {
	return uuid;
}

std::string& BaseObjective::getName() {
	return this->name;
}
