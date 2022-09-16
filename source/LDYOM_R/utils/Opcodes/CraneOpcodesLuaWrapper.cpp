//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void CraneOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("CraneOp");
    //default
	table.set_function("playerEnteredDockCrane", []() { auto result = Command<0x079D>(); return std::make_tuple(result);});
	table.set_function("playerEnteredBuildingsiteCrane", []() { auto result = Command<0x079E>(); return std::make_tuple(result);});
	table.set_function("playerLeftCrane", []() { auto result = Command<0x079F>(); return std::make_tuple(result);});
	table.set_function("playerEnteredQuarryCrane", []() { auto result = Command<0x07F9>(); return std::make_tuple(result);});
	table.set_function("playerEnteredLasVegasCrane", []() { auto result = Command<0x07FA>(); return std::make_tuple(result);});
	table.set_function("enableControls", [](const bool& _up, const bool& _down, const bool& _release) { auto result = Command<0x0898>(_up, _down, _release); return std::make_tuple(result);});

}