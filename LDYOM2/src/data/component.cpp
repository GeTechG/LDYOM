#include "component.h"
#include "entity.h"
#include <localization.h>

void Component::sol_lua_register(sol::state_view lua_state) {
	auto ut = lua_state.new_usertype<Component>("Component");
	SOL_LUA_FOR_EACH(SOL_LUA_BIND_MEMBER_ACTION, ut, Component, type, entity, getName);
}

std::string Component::tr(const std::string_view key) {
	return _("components." + std::string(this->getType()) + "." + std::string(key));
}