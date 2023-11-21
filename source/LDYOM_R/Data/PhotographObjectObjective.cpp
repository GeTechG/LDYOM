#include "PhotographObjectObjective.h"

#include <extensions/ScriptCommands.h>

#include "Object.h"
#include "strUtils.h"

PhotographObjectObjective::PhotographObjectObjective(void *_new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.photograph_object"));
	this->name += suffix;
}

ktwait PhotographObjectObjective::execute(Scene *scene, Object *object, Result &result, ktcoro_tasklist &tasklist) {
	using namespace plugin;

	while (!Command<Commands::HAS_OBJECT_BEEN_PHOTOGRAPHED>(object->getProjectObject().value())) {
		co_await 1;
	}
}
