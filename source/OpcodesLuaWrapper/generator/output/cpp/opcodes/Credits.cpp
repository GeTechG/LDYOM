// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindCredits(sol::state &state) {
  auto table = state.create_table("CreditsOp");
  // default
  table.set_function("start", []() { Command<0x0434>(); });
  table.set_function("stop", []() { Command<0x0435>(); });
  table.set_function("areFinished", []() {
    auto result = Command<0x0436>();
    return std::make_tuple(result);
  });
}