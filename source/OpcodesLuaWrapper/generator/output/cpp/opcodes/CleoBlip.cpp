// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindCleoBlip(sol::state &state) {
  auto table = state.create_table("CleoBlipOp");
  // CLEO+
  table.set_function("add", [](const int &_rwTextureOrRadarSprite, const float &_x, const float &_y, const bool &_short,
                               const int &_red, const int &_green, const int &_blue, const int &_alpha) {
    int handle_;
    Command<0x0E2A>(_rwTextureOrRadarSprite, _x, _y, _short, _red, _green, _blue, _alpha, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("remove", [](const int &_self) { Command<0x0E2B>(_self); });
}