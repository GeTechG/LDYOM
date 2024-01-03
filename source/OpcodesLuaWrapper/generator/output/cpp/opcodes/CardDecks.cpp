// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindCardDecks(sol::state &state) {
  auto table = state.create_table("CardDecksOp");
  // default
  table.set_function("shuffle", [](const int &_type) { Command<0x059D>(_type); });
  table.set_function("fetchNextCard", []() {
    int number_;
    Command<0x059E>(&number_);
    return std::make_tuple(number_);
  });
}