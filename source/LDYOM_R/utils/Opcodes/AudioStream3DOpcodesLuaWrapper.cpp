//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void AudioStream3DOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("AudioStream3DOp");
    //CLEO
	table.set_function("load", [](const char* _audioFileName) {int handle_; auto result = Command<0x0AC1>(_audioFileName, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("setPlayAtCoords", [](const int& _self, const float& _x, const float& _y, const float& _z) { auto result = Command<0x0AC2>(_self, _x, _y, _z); return std::make_tuple(result);});
	table.set_function("setPlayAtObject", [](const int& _self, const int& _handle) { auto result = Command<0x0AC3>(_self, _handle); return std::make_tuple(result);});
	table.set_function("setPlayAtChar", [](const int& _self, const int& _handle) { auto result = Command<0x0AC4>(_self, _handle); return std::make_tuple(result);});
	table.set_function("setPlayAtCar", [](const int& _self, const int& _handle) { auto result = Command<0x0AC5>(_self, _handle); return std::make_tuple(result);});

}