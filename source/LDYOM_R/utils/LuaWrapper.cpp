#include "LuaWrapper.h"
#include <ImGuiLuaWrapper.h>
#include "OpcodesLuaWrapper.h"

extern void convertWrapper(sol::state &state);
extern void filesystemWrapper(sol::state &state);
extern void imguiLdyomCustomWrapper(sol::state &state);
extern void indexUuidWrapper(sol::state &state);
extern void localizationWrapper(sol::state &state);
extern void pluginSdkWrapper(sol::state &state);
extern void asyncWrapper(sol::state &state);
extern void otherWrapper(sol::state &state);
extern void nodeEditorWrapper(sol::state &state);
extern void luaWindowWrapper(sol::state &state);
extern void blueprintNodeBuilder(sol::state &state);
extern void ldyomWrapper(sol::state &state);
extern void boostWrapper(sol::state &state);
extern void ktcoroWrapper(sol::state &state);
extern void mathConditionWrapper(sol::state &state);
extern void globalVariableWrapper(sol::state &state);
extern void projectPlayerServiceWrapper(sol::state &state);
extern void glmWrapper(sol::state &state);

void LuaWrapper::wrap(sol::state &state) {
	imguiLuaWrapper(state);
	opcodesLuaWrapper(state);
	convertWrapper(state);
	filesystemWrapper(state);
	imguiLdyomCustomWrapper(state);
	indexUuidWrapper(state);
	localizationWrapper(state);
	pluginSdkWrapper(state);
	asyncWrapper(state);
	otherWrapper(state);
	nodeEditorWrapper(state);
	luaWindowWrapper(state);
	blueprintNodeBuilder(state);
	ldyomWrapper(state);
	boostWrapper(state);
	ktcoroWrapper(state);
	mathConditionWrapper(state);
	globalVariableWrapper(state);
	projectPlayerServiceWrapper(state);
	glmWrapper(state);
}
