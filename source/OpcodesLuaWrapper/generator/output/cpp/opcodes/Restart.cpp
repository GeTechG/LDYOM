// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindRestart(sol::state &state) {
  auto table = state.create_table("RestartOp");
  // default
  table.set_function("addHospital", [](const float &_x, const float &_y, const float &_z, const float &_heading,
                                       const int &_townId) { Command<0x016C>(_x, _y, _z, _heading, _townId); });
  table.set_function("addPolice", [](const float &_x, const float &_y, const float &_z, const float &_heading,
                                     const int &_townId) { Command<0x016D>(_x, _y, _z, _heading, _townId); });
  table.set_function("overrideNext", [](const float &_x, const float &_y, const float &_z, const float &_heading) {
    Command<0x016E>(_x, _y, _z, _heading);
  });
  table.set_function("cancelOverride", []() { Command<0x01F6>(); });
  table.set_function("setExtraHospitalRestartPoint",
                     [](const float &_x, const float &_y, const float &_z, const float &_radius,
                        const float &_heading) { Command<0x08DF>(_x, _y, _z, _radius, _heading); });
  table.set_function("setExtraPoliceStationRestartPoint",
                     [](const float &_x, const float &_y, const float &_z, const float &_radius,
                        const float &_heading) { Command<0x08E0>(_x, _y, _z, _radius, _heading); });
  table.set_function("setRespawnPointForDurationOfMission",
                     [](const float &_x, const float &_y, const float &_z) { Command<0x09FF>(_x, _y, _z); });
}