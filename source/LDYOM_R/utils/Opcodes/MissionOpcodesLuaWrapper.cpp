//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void MissionOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("MissionOp");
    //default
	table.set_function("finish", []() { auto result = Command<0x00D8>(); return std::make_tuple(result);});
	table.set_function("loadAndLaunchInternal", [](const int& _index) { auto result = Command<0x0417>(_index); return std::make_tuple(result);});
	table.set_function("fail", []() { auto result = Command<0x045C>(); return std::make_tuple(result);});
	
	//CLEO+
	table.set_function("isOn", []() { auto result = Command<0x0E1D>(); return std::make_tuple(result);});
	table.set_function("isOnScriptedCutscene", []() { auto result = Command<0x0EB7>(); return std::make_tuple(result);});

}