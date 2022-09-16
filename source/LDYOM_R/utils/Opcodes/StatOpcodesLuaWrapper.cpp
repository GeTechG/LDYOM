//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void StatOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("StatOp");
    //default
	table.set_function("playerMadeProgress", [](const int& _progress) { auto result = Command<0x030C>(_progress); return std::make_tuple(result);});
	table.set_function("setProgressTotal", [](const int& _maxProgress) { auto result = Command<0x030D>(_maxProgress); return std::make_tuple(result);});
	table.set_function("registerMissionGiven", []() { auto result = Command<0x0317>(); return std::make_tuple(result);});
	table.set_function("registerMissionPassed", [](const char* _key) { auto result = Command<0x0318>(_key); return std::make_tuple(result);});
	table.set_function("setTotalNumberOfMissions", [](const int& _numMissions) { auto result = Command<0x042C>(_numMissions); return std::make_tuple(result);});
	table.set_function("registerFastestTime", [](const int& _statId, const int& _value) { auto result = Command<0x042E>(_statId, _value); return std::make_tuple(result);});
	table.set_function("registerBestPosition", [](const int& _statId, const int& _position) { auto result = Command<0x0582>(_statId, _position); return std::make_tuple(result);});
	table.set_function("getProgressPercentage", []() {float percentage_; auto result = Command<0x058C>(&percentage_); return std::make_tuple(percentage_, result);});
	table.set_function("registerOddjobMissionPassed", []() { auto result = Command<0x0595>(); return std::make_tuple(result);});
	table.set_function("incrementInt", [](const int& _statId, const int& _value) { auto result = Command<0x0623>(_statId, _value); return std::make_tuple(result);});
	table.set_function("incrementFloat", [](const int& _statId, const float& _value) { auto result = Command<0x0624>(_statId, _value); return std::make_tuple(result);});
	table.set_function("decrementInt", [](const int& _statId, const int& _value) { auto result = Command<0x0625>(_statId, _value); return std::make_tuple(result);});
	table.set_function("decrementFloat", [](const int& _statId, const float& _value) { auto result = Command<0x0626>(_statId, _value); return std::make_tuple(result);});
	table.set_function("registerInt", [](const int& _statId, const int& _value) { auto result = Command<0x0627>(_statId, _value); return std::make_tuple(result);});
	table.set_function("registerFloat", [](const int& _statId, const float& _value) { auto result = Command<0x0628>(_statId, _value); return std::make_tuple(result);});
	table.set_function("setInt", [](const int& _statId, const int& _value) { auto result = Command<0x0629>(_statId, _value); return std::make_tuple(result);});
	table.set_function("setFloat", [](const int& _statId, const float& _value) { auto result = Command<0x062A>(_statId, _value); return std::make_tuple(result);});
	table.set_function("getInt", [](const int& _statId) {int value_; auto result = Command<0x0652>(_statId, &value_); return std::make_tuple(value_, result);});
	table.set_function("getFloat", [](const int& _statId) {float value_; auto result = Command<0x0653>(_statId, &value_); return std::make_tuple(value_, result);});
	table.set_function("findNumberTagsTagged", []() {int numTags_; auto result = Command<0x08E1>(&numTags_); return std::make_tuple(numTags_, result);});
	table.set_function("getTerritoryUnderControlPercentage", []() {int percentage_; auto result = Command<0x08E2>(&percentage_); return std::make_tuple(percentage_, result);});
	table.set_function("showUpdateStats", [](const bool& _state) { auto result = Command<0x08F8>(_state); return std::make_tuple(result);});
	table.set_function("setMissionRespectTotal", [](const int& _totalRespect) { auto result = Command<0x0997>(_totalRespect); return std::make_tuple(result);});
	table.set_function("awardPlayerMissionRespect", [](const int& _value) { auto result = Command<0x0998>(_value); return std::make_tuple(result);});
	table.set_function("incrementIntNoMessage", [](const int& _statId, const int& _value) { auto result = Command<0x0A10>(_statId, _value); return std::make_tuple(result);});
	table.set_function("incrementFloatNoMessage", [](const int& _statId, const float& _value) { auto result = Command<0x0A1F>(_statId, _value); return std::make_tuple(result);});

}