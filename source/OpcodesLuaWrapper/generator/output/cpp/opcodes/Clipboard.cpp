// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindClipboard(sol::state &state) {
  auto table = state.create_table("ClipboardOp");
  // clipboard
  table.set_function("readData", [](const int &_memory, const int &_size) { Command<0x0B20>(_memory, _size); });
  table.set_function("writeData", [](const int &_memory, const int &_size) { Command<0x0B21>(_memory, _size); });
}