//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void RcOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("RcOp");
    //default
	table.set_function("giveModelToPlayer", [](const int& _handle, const float& _x, const float& _y, const float& _z, const float& _angle, const int& _modelId) { auto result = Command<0x046E>(_handle, _x, _y, _z, _angle, _modelId); return std::make_tuple(result);});
	table.set_function("getCar", [](const int& _player) {int car_; auto result = Command<0x0484>(_player, &car_); return std::make_tuple(car_, result);});
	table.set_function("setEnableDetonate", [](const bool& _state) { auto result = Command<0x048A>(_state); return std::make_tuple(result);});
	table.set_function("setEnableDetonateOnContact", [](const bool& _state) { auto result = Command<0x04D6>(_state); return std::make_tuple(result);});
	table.set_function("removeBuggy", []() { auto result = Command<0x04DB>(); return std::make_tuple(result);});
	table.set_function("takeCar", [](const int& _player, const int& _vehicle) { auto result = Command<0x0715>(_player, _vehicle); return std::make_tuple(result);});

}