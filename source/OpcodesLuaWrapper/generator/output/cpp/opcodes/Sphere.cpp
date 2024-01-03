// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindSphere(sol::state &state) {
  auto table = state.create_table("SphereOp");
  // default
  table.set_function("draw", [](const float &_x, const float &_y, const float &_z, const float &_radius) {
    Command<0x03A1>(_x, _y, _z, _radius);
  });
  table.set_function("create", [](const float &_x, const float &_y, const float &_z, const float &_radius) {
    int handle_;
    Command<0x03BC>(_x, _y, _z, _radius, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("remove", [](const int &_self) { Command<0x03BD>(_self); });
}