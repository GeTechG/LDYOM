// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindTrailer(sol::state &state) {
  auto table = state.create_table("TrailerOp");
  // default
  table.set_function("isAttachedToCab", [](const int &_self, const int &_cab) {
    auto result = Command<0x07AB>(_self, _cab);
    return std::make_tuple(result);
  });
  table.set_function("detachFromCab", [](const int &_self, const int &_cab) { Command<0x07AC>(_self, _cab); });
  table.set_function("attachToCab", [](const int &_self, const int &_cab) { Command<0x0893>(_self, _cab); });
}