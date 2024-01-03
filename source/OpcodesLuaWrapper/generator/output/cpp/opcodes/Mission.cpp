// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindMission(sol::state &state) {
  auto table = state.create_table("MissionOp");
  // default
  table.set_function("finish", []() { Command<0x00D8>(); });
  table.set_function("loadAndLaunchInternal", [](const int &_index) { Command<0x0417>(_index); });
  table.set_function("fail", []() { Command<0x045C>(); });

  // CLEO+
  table.set_function("isOn", []() {
    auto result = Command<0x0E1D>();
    return std::make_tuple(result);
  });
  table.set_function("isOnScriptedCutscene", []() {
    auto result = Command<0x0EB7>();
    return std::make_tuple(result);
  });
}