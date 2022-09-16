//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void KillFrenzyOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("KillFrenzyOp");
    //default
	table.set_function("start", [](const char* _text, const int& _weaponType, const int& _timeInMs, const int& _targetsNum, const int& _targetModel1, const int& _targetModel2, const int& _targetModel3, const int& _targetModel4, const bool& _betaSoundsAndMessages) { auto result = Command<0x01F9>(_text, _weaponType, _timeInMs, _targetsNum, _targetModel1, _targetModel2, _targetModel3, _targetModel4, _betaSoundsAndMessages); return std::make_tuple(result);});
	table.set_function("readStatus", []() {int status_; auto result = Command<0x01FA>(&status_); return std::make_tuple(status_, result);});
	table.set_function("fail", []() { auto result = Command<0x09C2>(); return std::make_tuple(result);});

}