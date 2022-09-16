//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void PadOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("PadOp");
    //default
	table.set_function("isButtonPressed", [](const int& _pad, const int& _buttonId) { auto result = Command<0x00E1>(_pad, _buttonId); return std::make_tuple(result);});
	table.set_function("getState", [](const int& _pad, const int& _buttonId) {int state_; auto result = Command<0x00E2>(_pad, _buttonId, &state_); return std::make_tuple(state_, result);});
	table.set_function("shake", [](const int& _pad, const int& _time, const int& _intensity) { auto result = Command<0x015B>(_pad, _time, _intensity); return std::make_tuple(result);});
	table.set_function("getControllerMode", []() {int mode_; auto result = Command<0x0293>(&mode_); return std::make_tuple(mode_, result);});
	table.set_function("setDrunkInputDelay", [](const int& _pad, const int& _delay) { auto result = Command<0x03FD>(_pad, _delay); return std::make_tuple(result);});
	table.set_function("getPositionOfAnalogueSticks", [](const int& _pad) {int leftStickX_;int leftStickY_;int rightStickX_;int rightStickY_; auto result = Command<0x0494>(_pad, &leftStickX_, &leftStickY_, &rightStickX_, &rightStickY_); return std::make_tuple(leftStickX_, leftStickY_, rightStickX_, rightStickY_, result);});
	table.set_function("setPlayerEnterCarButton", [](const int& _playerId, const bool& _state) { auto result = Command<0x07CC>(_playerId, _state); return std::make_tuple(result);});
	table.set_function("setPlayerDuckButton", [](const int& _playerId, const bool& _state) { auto result = Command<0x082A>(_playerId, _state); return std::make_tuple(result);});
	table.set_function("setPlayerFireButton", [](const int& _playerId, const bool& _state) { auto result = Command<0x0881>(_playerId, _state); return std::make_tuple(result);});
	table.set_function("isSkipCutsceneButtonPressed", []() { auto result = Command<0x08D0>(); return std::make_tuple(result);});
	table.set_function("setPlayerJumpButton", [](const int& _playerId, const bool& _state) { auto result = Command<0x0901>(_playerId, _state); return std::make_tuple(result);});
	table.set_function("setPlayerDisplayVitalStatsButton", [](const int& _playerId, const bool& _state) { auto result = Command<0x0960>(_playerId, _state); return std::make_tuple(result);});
	table.set_function("setPlayerCycleWeaponButton", [](const int& _playerId, const bool& _state) { auto result = Command<0x0992>(_playerId, _state); return std::make_tuple(result);});
	
	//CLEO
	table.set_function("isKeyPressed", [](const int& _keyCode) { auto result = Command<0x0AB0>(_keyCode); return std::make_tuple(result);});
	table.set_function("testCheat", [](const char* _input) { auto result = Command<0x0ADC>(_input); return std::make_tuple(result);});

}