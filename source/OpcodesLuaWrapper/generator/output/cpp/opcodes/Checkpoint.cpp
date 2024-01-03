// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindCheckpoint(sol::state &state) {
  auto table = state.create_table("CheckpointOp");
  // default
  table.set_function("create",
                     [](const int &_type, const float &_x, const float &_y, const float &_z, const float &_pointX,
                        const float &_pointY, const float &_pointZ, const float &_radius) {
                       int handle_;
                       Command<0x06D5>(_type, _x, _y, _z, _pointX, _pointY, _pointZ, _radius, &handle_);
                       return std::make_tuple(handle_);
                     });
  table.set_function("delete", [](const int &_self) { Command<0x06D6>(_self); });
  table.set_function("setCoords", [](const int &_self, const float &_x, const float &_y, const float &_z) {
    Command<0x07F3>(_self, _x, _y, _z);
  });
  table.set_function("setHeading", [](const int &_self, const float &_heading) { Command<0x0996>(_self, _heading); });
}