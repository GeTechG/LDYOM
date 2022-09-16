//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void DecisionMakerOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("DecisionMakerOp");
    //default
	table.set_function("remove", [](const int& _self) { auto result = Command<0x065C>(_self); return std::make_tuple(result);});
	table.set_function("doesExist", [](const int& _handle) { auto result = Command<0x09F2>(_handle); return std::make_tuple(result);});

}