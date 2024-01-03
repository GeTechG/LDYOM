// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindBoat(sol::state &state) {
  auto table = state.create_table("BoatOp");
  // default
  table.set_function("goto", [](const int &_self, const float &_x, const float &_y, const float &_z) {
    Command<0x02D3>(_self, _x, _y, _z);
  });
  table.set_function("stop", [](const int &_self) { Command<0x02D4>(_self); });
  table.set_function("setCruiseSpeed",
                     [](const int &_self, const float &_maxSpeed) { Command<0x02DB>(_self, _maxSpeed); });
  table.set_function("anchor", [](const int &_self, const int &_state) { Command<0x0323>(_self, _state); });
}