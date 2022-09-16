//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void SequenceOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("SequenceOp");
    //default
	table.set_function("open", []() {int handle_; auto result = Command<0x0615>(&handle_); return std::make_tuple(handle_, result);});
	table.set_function("close", [](const int& _self) { auto result = Command<0x0616>(_self); return std::make_tuple(result);});
	table.set_function("clear", [](const int& _self) { auto result = Command<0x061B>(_self); return std::make_tuple(result);});
	table.set_function("setToRepeat", [](const int& _self, const bool& _state) { auto result = Command<0x0643>(_self, _state); return std::make_tuple(result);});

}