#include "DataLuaWrapper.h"

extern void ProjectDataLuaWrap(sol::state& state);
extern void SceneLuaWrap(sol::state& state);

void DataLuaWrapper::wrap(sol::state& state) {
	ProjectDataLuaWrap(state);
	SceneLuaWrap(state);
}
