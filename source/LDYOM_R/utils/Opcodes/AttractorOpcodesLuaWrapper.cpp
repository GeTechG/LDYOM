//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void AttractorOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("AttractorOp");
    //default
	table.set_function("add", [](const float& _x, const float& _y, const float& _z, const float& _angle, const float& __p5, const int& _sequence) {int handle_; auto result = Command<0x061D>(_x, _y, _z, _angle, __p5, _sequence, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("clear", [](const int& _self) { auto result = Command<0x061E>(_self); return std::make_tuple(result);});
	table.set_function("addPedTypeAsUser", [](const int& _self, const int& _pedType) { auto result = Command<0x0680>(_self, _pedType); return std::make_tuple(result);});

}