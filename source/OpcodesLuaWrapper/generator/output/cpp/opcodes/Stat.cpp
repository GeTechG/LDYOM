// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindStat(sol::state &state) {
  auto table = state.create_table("StatOp");
  // default
  table.set_function("playerMadeProgress", [](const int &_progress) { Command<0x030C>(_progress); });
  table.set_function("setProgressTotal", [](const int &_maxProgress) { Command<0x030D>(_maxProgress); });
  table.set_function("registerMissionGiven", []() { Command<0x0317>(); });
  table.set_function("registerMissionPassed", [](const int &_key) { Command<0x0318>(_key); });
  table.set_function("setTotalNumberOfMissions", [](const int &_numMissions) { Command<0x042C>(_numMissions); });
  table.set_function("registerFastestTime",
                     [](const int &_statId, const int &_value) { Command<0x042E>(_statId, _value); });
  table.set_function("registerBestPosition",
                     [](const int &_statId, const int &_position) { Command<0x0582>(_statId, _position); });
  table.set_function("getProgressPercentage", []() {
    float percentage_;
    Command<0x058C>(&percentage_);
    return std::make_tuple(percentage_);
  });
  table.set_function("registerOddjobMissionPassed", []() { Command<0x0595>(); });
  table.set_function("incrementInt", [](const int &_statId, const int &_value) { Command<0x0623>(_statId, _value); });
  table.set_function("incrementFloat",
                     [](const int &_statId, const float &_value) { Command<0x0624>(_statId, _value); });
  table.set_function("decrementInt", [](const int &_statId, const int &_value) { Command<0x0625>(_statId, _value); });
  table.set_function("decrementFloat",
                     [](const int &_statId, const float &_value) { Command<0x0626>(_statId, _value); });
  table.set_function("registerInt", [](const int &_statId, const int &_value) { Command<0x0627>(_statId, _value); });
  table.set_function("registerFloat",
                     [](const int &_statId, const float &_value) { Command<0x0628>(_statId, _value); });
  table.set_function("setInt", [](const int &_statId, const int &_value) { Command<0x0629>(_statId, _value); });
  table.set_function("setFloat", [](const int &_statId, const float &_value) { Command<0x062A>(_statId, _value); });
  table.set_function("getInt", [](const int &_statId) {
    int value_;
    Command<0x0652>(_statId, &value_);
    return std::make_tuple(value_);
  });
  table.set_function("getFloat", [](const int &_statId) {
    float value_;
    Command<0x0653>(_statId, &value_);
    return std::make_tuple(value_);
  });
  table.set_function("findNumberTagsTagged", []() {
    int numTags_;
    Command<0x08E1>(&numTags_);
    return std::make_tuple(numTags_);
  });
  table.set_function("getTerritoryUnderControlPercentage", []() {
    int percentage_;
    Command<0x08E2>(&percentage_);
    return std::make_tuple(percentage_);
  });
  table.set_function("showUpdateStats", [](const int &_state) { Command<0x08F8>(_state); });
  table.set_function("setMissionRespectTotal", [](const int &_totalRespect) { Command<0x0997>(_totalRespect); });
  table.set_function("awardPlayerMissionRespect", [](const int &_value) { Command<0x0998>(_value); });
  table.set_function("incrementIntNoMessage",
                     [](const int &_statId, const int &_value) { Command<0x0A10>(_statId, _value); });
  table.set_function("incrementFloatNoMessage",
                     [](const int &_statId, const float &_value) { Command<0x0A1F>(_statId, _value); });
}