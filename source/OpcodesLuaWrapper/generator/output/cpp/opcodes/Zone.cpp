// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindZone(sol::state &state) {
  auto table = state.create_table("ZoneOp");
  // default
  table.set_function("getRandomChar", [](const char *_zone, const bool &_civilian, const bool &_gang,
                                         const bool &_criminalOrProstitute) {
    int handle_;
    Command<0x02DD>(_zone, _civilian, _gang, _criminalOrProstitute, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("setPopulationType", [](const char *_zone, const int &_type) { Command<0x0767>(_zone, _type); });
  table.set_function("setDealerStrength",
                     [](const char *_zone, const int &_strength) { Command<0x076A>(_zone, _strength); });
  table.set_function("getDealerStrength", [](const char *_zone) {
    int density_;
    Command<0x076B>(_zone, &density_);
    return std::make_tuple(density_);
  });
  table.set_function("setGangStrength", [](const char *_zoneId, const int &_gangId, const int &_density) {
    Command<0x076C>(_zoneId, _gangId, _density);
  });
  table.set_function("getGangStrength", [](const char *_zone, const int &_gangId) {
    int density_;
    Command<0x076D>(_zone, _gangId, &density_);
    return std::make_tuple(density_);
  });
  table.set_function("getName", [](const float &_x, const float &_y, const float &_z) {
    const char *name_;
    Command<0x0843>(_x, _y, _z, (int *)&name_);
    return std::make_tuple(name_);
  });
  table.set_function("setPopulationRace", [](const char *_zone, const int &__p2) { Command<0x0874>(_zone, __p2); });
  table.set_function("setForGangWarsTraining", [](const char *_zone) { Command<0x08B3>(_zone); });
  table.set_function("initPopulationSettings", []() { Command<0x08CA>(); });
  table.set_function("getCurrentPopulationZoneType", []() {
    int type_;
    Command<0x08D3>(&type_);
    return std::make_tuple(type_);
  });
  table.set_function("getName", [](const float &_x, const float &_y, const float &_z) {
    const char *name_;
    Command<0x08F1>(_x, _y, _z, (int *)&name_);
    return std::make_tuple(name_);
  });
  table.set_function("setTriggerGangWar", [](const char *_zone) { Command<0x090C>(_zone); });
  table.set_function("switchAudio", [](const char *_zone, const bool &_state) { Command<0x0917>(_zone, _state); });
  table.set_function("setNoCops", [](const char *_zone, const bool &_state) { Command<0x09B7>(_zone, _state); });
  table.set_function("setDisableMilitaryZones", [](const bool &_state) { Command<0x0A24>(_state); });
}