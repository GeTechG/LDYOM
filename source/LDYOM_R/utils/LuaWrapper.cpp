#include "LuaWrapper.h"
#include <ImGuiLuaWrapper.h>
#include "OpcodesLuaWrapper.h"

extern void convertWrapper(sol::state &state);
extern void filesystemWrapper(sol::state &state);
extern void imguiCustomWrapper(sol::state &state);
extern void indexUuidWrapper(sol::state &state);
extern void localizationWrapper(sol::state &state);
extern void pluginSdkWrapper(sol::state &state);
extern void asyncWrapper(sol::state &state);
extern void otherWrapper(sol::state &state);

void LuaWrapper::wrap(sol::state &state) {
	imguiLuaWrapper(state);
	opcodesLuaWrapper(state);
	convertWrapper(state);
	filesystemWrapper(state);
	imguiCustomWrapper(state);
	indexUuidWrapper(state);
	localizationWrapper(state);
	pluginSdkWrapper(state);
	asyncWrapper(state);
	otherWrapper(state);
}
