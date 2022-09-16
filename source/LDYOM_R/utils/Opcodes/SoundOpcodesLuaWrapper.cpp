//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void SoundOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("SoundOp");
    //default
	table.set_function("addOneOffSound", [](const float& _x, const float& _y, const float& _z, const int& _soundId) { auto result = Command<0x018C>(_x, _y, _z, _soundId); return std::make_tuple(result);});

}