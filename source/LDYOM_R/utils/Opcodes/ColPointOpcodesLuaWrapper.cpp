//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void ColPointOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("ColPointOp");
    //CLEO+
	table.set_function("getCoordinates", [](const int& _colPoint) {float x_;float y_;float z_; auto result = Command<0x0EE1>(_colPoint, &x_, &y_, &z_); return std::make_tuple(x_, y_, z_, result);});

}