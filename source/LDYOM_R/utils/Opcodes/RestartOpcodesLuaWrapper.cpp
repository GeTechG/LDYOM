//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void RestartOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("RestartOp");
    //default
	table.set_function("addHospital", [](const float& _x, const float& _y, const float& _z, const float& _heading, const int& _townId) { auto result = Command<0x016C>(_x, _y, _z, _heading, _townId); return std::make_tuple(result);});
	table.set_function("addPolice", [](const float& _x, const float& _y, const float& _z, const float& _heading, const int& _townId) { auto result = Command<0x016D>(_x, _y, _z, _heading, _townId); return std::make_tuple(result);});
	table.set_function("overrideNext", [](const float& _x, const float& _y, const float& _z, const float& _heading) { auto result = Command<0x016E>(_x, _y, _z, _heading); return std::make_tuple(result);});
	table.set_function("cancelOverride", []() { auto result = Command<0x01F6>(); return std::make_tuple(result);});
	table.set_function("setExtraHospitalRestartPoint", [](const float& _x, const float& _y, const float& _z, const float& _radius, const float& _heading) { auto result = Command<0x08DF>(_x, _y, _z, _radius, _heading); return std::make_tuple(result);});
	table.set_function("setExtraPoliceStationRestartPoint", [](const float& _x, const float& _y, const float& _z, const float& _radius, const float& _heading) { auto result = Command<0x08E0>(_x, _y, _z, _radius, _heading); return std::make_tuple(result);});
	table.set_function("setRespawnPointForDurationOfMission", [](const float& _x, const float& _y, const float& _z) { auto result = Command<0x09FF>(_x, _y, _z); return std::make_tuple(result);});

}