#include <sol.hpp>
#include "../Data/BaseObjective.h"

void objectiveWrapper(sol::state &state) {
	state.new_usertype<BaseObjective>("LDObjective",
	                                  sol::no_constructor,
	                                  "getUuid", &BaseObjective::getUuid,
	                                  "getName", &BaseObjective::getName,
	                                  "getCategory", &BaseObjective::getCategory,
	                                  "getTypeCategory", &BaseObjective::getTypeCategory,
	                                  "open", &BaseObjective::open,
	                                  "close", &BaseObjective::close,
	                                  "draw", &BaseObjective::draw,
	                                  "execute", &BaseObjective::execute
	);
}
