#include "UtilsLuaWrapper.h"

extern void ProjectsServiceLuaWrap(sol::state& state);

void UtilsLuaWrapper::wrap(sol::state& state) {
	ProjectsServiceLuaWrap(state);
}
