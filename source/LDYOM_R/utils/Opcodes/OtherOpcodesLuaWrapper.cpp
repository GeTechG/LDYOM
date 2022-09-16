//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void OtherOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("OtherOp");
    //default
	table.set_function("syncWater", []() { auto result = Command<0x0971>(); return std::make_tuple(result);});

}