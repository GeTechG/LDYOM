// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindSequence(sol::state &state) {
  auto table = state.create_table("SequenceOp");
  // default
  table.set_function("open", []() {
    int handle_;
    Command<0x0615>(&handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("close", [](const int &_self) { Command<0x0616>(_self); });
  table.set_function("clear", [](const int &_self) { Command<0x061B>(_self); });
  table.set_function("setToRepeat", [](const int &_self, const int &_state) { Command<0x0643>(_self, _state); });
}