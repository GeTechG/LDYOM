// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindDecisionMaker(sol::state &state) {
  auto table = state.create_table("DecisionMakerOp");
  // default
  table.set_function("remove", [](const int &_self) { Command<0x065C>(_self); });
  table.set_function("doesExist", [](const int &_handle) {
    auto result = Command<0x09F2>(_handle);
    return std::make_tuple(result);
  });
}