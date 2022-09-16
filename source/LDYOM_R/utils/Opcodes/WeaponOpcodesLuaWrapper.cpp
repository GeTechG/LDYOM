//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void WeaponOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("WeaponOp");
    //default
	table.set_function("getModel", [](const int& _weaponType) {int modelId_; auto result = Command<0x0781>(_weaponType, &modelId_); return std::make_tuple(modelId_, result);});
	table.set_function("getSlot", [](const int& _weaponType) {int slot_; auto result = Command<0x0782>(_weaponType, &slot_); return std::make_tuple(slot_, result);});
	
	//CLEO+
	table.set_function("isFireType", [](const int& _weaponType, const int& _weaponFire) { auto result = Command<0x0E26>(_weaponType, _weaponFire); return std::make_tuple(result);});

}