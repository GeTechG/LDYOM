//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void GangOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("GangOp");
    //default
	table.set_function("setWeapons", [](const int& _gangId, const int& _weaponType1, const int& _weaponType2, const int& _weaponType3) { auto result = Command<0x0237>(_gangId, _weaponType1, _weaponType2, _weaponType3); return std::make_tuple(result);});

}