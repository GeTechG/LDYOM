//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void SphereOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("SphereOp");
    //default
	table.set_function("draw", [](const float& _x, const float& _y, const float& _z, const float& _radius) { auto result = Command<0x03A1>(_x, _y, _z, _radius); return std::make_tuple(result);});
	table.set_function("create", [](const float& _x, const float& _y, const float& _z, const float& _radius) {int handle_; auto result = Command<0x03BC>(_x, _y, _z, _radius, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("remove", [](const int& _self) { auto result = Command<0x03BD>(_self); return std::make_tuple(result);});

}