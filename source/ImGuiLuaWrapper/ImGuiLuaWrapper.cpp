#include <sol.hpp>

extern void bindEnums(sol::state &state);
extern void bindStructs(sol::state &state);

void imguiLuaWrapper(sol::state &state) {
	bindEnums(state);
	bindStructs(state);
}
