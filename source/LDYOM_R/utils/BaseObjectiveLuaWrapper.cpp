#include <sol.hpp>
#include "../Data/BaseObjective.h"

void BaseObjectiveLuaWrap(sol::state& state) {
	auto clazz = state.new_usertype<BaseObjective>("BaseObjective");
	clazz["getUuid"] = &BaseObjective::getUuid;
	clazz["getName"] = &BaseObjective::getName;
	clazz["getCategory"] = &BaseObjective::getCategory;
	clazz["getTypeCategory"] = &BaseObjective::getTypeCategory;
	clazz["open"] = &BaseObjective::open;
	clazz["close"] = &BaseObjective::close;
	clazz["draw"] = &BaseObjective::draw;
	clazz["execute"] = &BaseObjective::execute;
}
