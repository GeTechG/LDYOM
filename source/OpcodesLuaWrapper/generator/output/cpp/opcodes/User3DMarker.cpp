// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindUser3DMarker(sol::state &state) {
  auto table = state.create_table("User3DMarkerOp");
  // default
  table.set_function("create", [](const float &_x, const float &_y, const float &_z, const int &_color) {
    int handle_;
    Command<0x0A40>(_x, _y, _z, _color, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("remove", [](const int &_self) { Command<0x0A41>(_self); });
}