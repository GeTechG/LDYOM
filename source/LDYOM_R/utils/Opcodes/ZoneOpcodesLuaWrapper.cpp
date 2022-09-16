//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void ZoneOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("ZoneOp");
    //default
	table.set_function("getRandomChar", [](const int& _zone, const bool& _civilian, const bool& _gang, const bool& _criminalOrProstitute) {int handle_; auto result = Command<0x02DD>(_zone, _civilian, _gang, _criminalOrProstitute, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("setPopulationType", [](const int& _zone, const int& _type) { auto result = Command<0x0767>(_zone, _type); return std::make_tuple(result);});
	table.set_function("setDealerStrength", [](const int& _zone, const int& _strength) { auto result = Command<0x076A>(_zone, _strength); return std::make_tuple(result);});
	table.set_function("getDealerStrength", [](const int& _zone) {int density_; auto result = Command<0x076B>(_zone, &density_); return std::make_tuple(density_, result);});
	table.set_function("setGangStrength", [](const int& _zoneId, const int& _gangId, const int& _density) { auto result = Command<0x076C>(_zoneId, _gangId, _density); return std::make_tuple(result);});
	table.set_function("getGangStrength", [](const int& _zone, const int& _gangId) {int density_; auto result = Command<0x076D>(_zone, _gangId, &density_); return std::make_tuple(density_, result);});
	table.set_function("getName", [](const float& _x, const float& _y, const float& _z) {const char* name_; auto result = Command<0x0843>(_x, _y, _z, (int*)&name_); return std::make_tuple(name_, result);});
	table.set_function("setPopulationRace", [](const int& _zone, const int& __p2) { auto result = Command<0x0874>(_zone, __p2); return std::make_tuple(result);});
	table.set_function("setForGangWarsTraining", [](const int& _zone) { auto result = Command<0x08B3>(_zone); return std::make_tuple(result);});
	table.set_function("initPopulationSettings", []() { auto result = Command<0x08CA>(); return std::make_tuple(result);});
	table.set_function("getCurrentPopulationZoneType", []() {int type_; auto result = Command<0x08D3>(&type_); return std::make_tuple(type_, result);});
	table.set_function("getName", [](const float& _x, const float& _y, const float& _z) {const char* name_; auto result = Command<0x08F1>(_x, _y, _z, (int*)&name_); return std::make_tuple(name_, result);});
	table.set_function("setTriggerGangWar", [](const int& _zone) { auto result = Command<0x090C>(_zone); return std::make_tuple(result);});
	table.set_function("switchAudio", [](const int& _zone, const bool& _state) { auto result = Command<0x0917>(_zone, _state); return std::make_tuple(result);});
	table.set_function("setNoCops", [](const int& _zone, const bool& _state) { auto result = Command<0x09B7>(_zone, _state); return std::make_tuple(result);});
	table.set_function("setDisableMilitaryZones", [](const bool& _state) { auto result = Command<0x0A24>(_state); return std::make_tuple(result);});

}