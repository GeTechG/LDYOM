// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindCrane(sol::state &state) {
  auto table = state.create_table("CraneOp");
  // default
  table.set_function("playerEnteredDockCrane", []() { Command<0x079D>(); });
  table.set_function("playerEnteredBuildingsiteCrane", []() { Command<0x079E>(); });
  table.set_function("playerLeftCrane", []() { Command<0x079F>(); });
  table.set_function("playerEnteredQuarryCrane", []() { Command<0x07F9>(); });
  table.set_function("playerEnteredLasVegasCrane", []() { Command<0x07FA>(); });
  table.set_function("enableControls", [](const bool &_up, const bool &_down, const bool &_release) {
    Command<0x0898>(_up, _down, _release);
  });
}