//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void ScriptFireOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("ScriptFireOp");
    //default
	table.set_function("start", [](const float& _x, const float& _y, const float& _z, const int& _propagation, const int& _size) {int handle_; auto result = Command<0x02CF>(_x, _y, _z, _propagation, _size, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("isExtinguished", [](const int& _self) { auto result = Command<0x02D0>(_self); return std::make_tuple(result);});
	table.set_function("remove", [](const int& _self) { auto result = Command<0x02D1>(_self); return std::make_tuple(result);});
	table.set_function("createCarFire", [](const int& _vehicle) {int handle_; auto result = Command<0x0325>(_vehicle, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("createCharFire", [](const int& _char) {int handle_; auto result = Command<0x0326>(_char, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("getCoords", [](const int& _self, const float& _x, const float& _y, const float& _z) { auto result = Command<0x06F5>(_self, _x, _y, _z); return std::make_tuple(result);});
	table.set_function("doesExist", [](const int& _handle) { auto result = Command<0x0973>(_handle); return std::make_tuple(result);});

}