// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindSound(sol::state &state) {
  auto table = state.create_table("SoundOp");
  // default
  table.set_function("addOneOffSound", [](const float &_x, const float &_y, const float &_z, const int &_soundId) {
    Command<0x018C>(_x, _y, _z, _soundId);
  });
}