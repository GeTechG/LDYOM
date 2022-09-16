//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void DebuggerOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("DebuggerOp");
    //default
	table.set_function("isDebugCameraOn", []() { auto result = Command<0x05A0>(); return std::make_tuple(result);});
	table.set_function("saveStringToDebugFile", [](const char* _msg) { auto result = Command<0x05B6>(_msg); return std::make_tuple(result);});

}