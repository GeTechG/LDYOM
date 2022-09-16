//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void CardDecksOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("CardDecksOp");
    //default
	table.set_function("shuffle", [](const int& _type) { auto result = Command<0x059D>(_type); return std::make_tuple(result);});
	table.set_function("fetchNextCard", []() {int number_; auto result = Command<0x059E>(&number_); return std::make_tuple(number_, result);});

}