//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void CreditsOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("CreditsOp");
    //default
	table.set_function("start", []() { auto result = Command<0x0434>(); return std::make_tuple(result);});
	table.set_function("stop", []() { auto result = Command<0x0435>(); return std::make_tuple(result);});
	table.set_function("areFinished", []() { auto result = Command<0x0436>(); return std::make_tuple(result);});

}