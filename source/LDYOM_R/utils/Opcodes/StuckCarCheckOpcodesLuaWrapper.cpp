//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void StuckCarCheckOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("StuckCarCheckOp");
    //default
	table.set_function("add", [](const int& _vehicle, const float& _speed, const int& _duration) { auto result = Command<0x03CC>(_vehicle, _speed, _duration); return std::make_tuple(result);});
	table.set_function("remove", [](const int& _vehicle) { auto result = Command<0x03CD>(_vehicle); return std::make_tuple(result);});
	table.set_function("isCarStuck", [](const int& _vehicle) { auto result = Command<0x03CE>(_vehicle); return std::make_tuple(result);});
	table.set_function("addWithWarp", [](const int& _vehicle, const float& _speed, const int& _duration, const bool& _stuck, const bool& _flipped, const bool& _warp, const int& _pathId) { auto result = Command<0x072F>(_vehicle, _speed, _duration, _stuck, _flipped, _warp, _pathId); return std::make_tuple(result);});

}