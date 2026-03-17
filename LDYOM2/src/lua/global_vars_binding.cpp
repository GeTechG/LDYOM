#include "global_vars_binding.h"
#include "core/global_vars_service.h"

void register_global_vars_bindings(sol::state_view lua) {
	GlobalVarsService::instance().initialize(lua);
}
