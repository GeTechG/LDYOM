//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void AudioStreamOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("AudioStreamOp");
    //CLEO
	table.set_function("load", [](const char* _audioFileName) {int handle_; auto result = Command<0x0AAC>(_audioFileName, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("setState", [](const int& _self, const int& _state) { auto result = Command<0x0AAD>(_self, _state); return std::make_tuple(result);});
	table.set_function("remove", [](const int& _self) { auto result = Command<0x0AAE>(_self); return std::make_tuple(result);});
	table.set_function("getLength", [](const int& _self) {int length_; auto result = Command<0x0AAF>(_self, &length_); return std::make_tuple(length_, result);});
	table.set_function("getState", [](const int& _self) {int state_; auto result = Command<0x0AB9>(_self, &state_); return std::make_tuple(state_, result);});
	table.set_function("getVolume", [](const int& _self) {float volume_; auto result = Command<0x0ABB>(_self, &volume_); return std::make_tuple(volume_, result);});
	table.set_function("setVolume", [](const int& _self, const float& _volume) { auto result = Command<0x0ABC>(_self, _volume); return std::make_tuple(result);});
	table.set_function("setLooped", [](const int& _self, const bool& _state) { auto result = Command<0x0AC0>(_self, _state); return std::make_tuple(result);});

}