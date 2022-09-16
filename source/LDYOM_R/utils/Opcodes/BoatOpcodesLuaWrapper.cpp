//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void BoatOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("BoatOp");
    //default
	table.set_function("goto", [](const int& _self, const float& _x, const float& _y, const float& _z) { auto result = Command<0x02D3>(_self, _x, _y, _z); return std::make_tuple(result);});
	table.set_function("stop", [](const int& _self) { auto result = Command<0x02D4>(_self); return std::make_tuple(result);});
	table.set_function("setCruiseSpeed", [](const int& _self, const float& _maxSpeed) { auto result = Command<0x02DB>(_self, _maxSpeed); return std::make_tuple(result);});
	table.set_function("anchor", [](const int& _self, const bool& _state) { auto result = Command<0x0323>(_self, _state); return std::make_tuple(result);});

}