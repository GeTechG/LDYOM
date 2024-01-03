// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindSkip(sol::state &state) {
  auto table = state.create_table("SkipOp");
  // default
  table.set_function("setUp", [](const float &_x, const float &_y, const float &_z, const float &_heading) {
    Command<0x0950>(_x, _y, _z, _heading);
  });
  table.set_function("clear", []() { Command<0x0951>(); });
  table.set_function("setUpAfterMission", [](const float &_x, const float &_y, const float &_z, const float &_heading) {
    Command<0x09AF>(_x, _y, _z, _heading);
  });
  table.set_function("setUpForSpecificVehicle",
                     [](const float &_x, const float &_y, const float &_z, const float &_heading, const int &_handle) {
                       Command<0x09E0>(_x, _y, _z, _heading, _handle);
                     });
  table.set_function("setUpForVehicleFinishedByScript",
                     [](const float &_x, const float &_y, const float &_z, const float &_heading, const int &_vehicle) {
                       Command<0x0A35>(_x, _y, _z, _heading, _vehicle);
                     });
  table.set_function("isWaitingForScriptToFadeIn", []() {
    auto result = Command<0x0A36>();
    return std::make_tuple(result);
  });
}