#include <sol.hpp>

extern void bindEnums(sol::state &state);
extern void bindStructs(sol::state &state);
extern void bindCustomImGui(sol::state &state);
extern void bindImColor(sol::state &state);
extern void bindCustomCimGui(sol::state &state);

void imguiLuaWrapper(sol::state &state) {
	bindEnums(state);
	bindStructs(state);
	bindCustomImGui(state);
	bindCustomCimGui(state);
	bindImColor(state);
}
