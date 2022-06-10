#include "BaseObjective.h"

#include <boost/uuid/random_generator.hpp>

#include "ProjectsService.h"
#include "strUtils.h"
#include "Localization/Localization.h"
#include "fmt/core.h"

BaseObjective::BaseObjective(): uuid_(boost::uuids::random_generator()()) {}

BaseObjective::BaseObjective(void* _new): uuid_(boost::uuids::random_generator()()) {
	const auto name = fmt::format("{} #{}", Localization::getInstance().get("objective.title"),
		ProjectsService::getInstance().getCurrentProject().getCurrentScene()->getObjectives().size());
	strlcpy(this->name_, name.c_str(), sizeof this->name_);
}

BaseObjective::BaseObjective(const BaseObjective& other): uuid_(boost::uuids::random_generator()()) {
	strlcpy(this->name_, other.name_, sizeof this->name_);
	strlcat(this->name_, "C", sizeof this->name_);
}

boost::uuids::uuid& BaseObjective::getUuid() {
	return uuid_;
}

char* BaseObjective::getName() {
	return this->name_;
}
