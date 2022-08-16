#include "TouchObjectObjective.h"

#include <extensions/ScriptCommands.h>
#include "common.h"
#include "Object.h"

#include "strUtils.h"

TouchObjectObjective::TouchObjectObjective(void* _new): BaseObjective(_new) {
	const auto suffix = fmt::format(" : {}", Localization::getInstance().get("objective.touch_object"));
	strlcat(this->name_.data(), suffix.c_str(), sizeof this->name_);
}

ktwait TouchObjectObjective::execute(Scene* scene, Object* object, Result& result, ktcoro_tasklist& tasklist) {
	using namespace plugin;

	while (!Command<Commands::IS_CHAR_TOUCHING_OBJECT>(static_cast<CPed*>(FindPlayerPed()), object->getProjectObject().value())) {
		co_await 1;
	}
}
