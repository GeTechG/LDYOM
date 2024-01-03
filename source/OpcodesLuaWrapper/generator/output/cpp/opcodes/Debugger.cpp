// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindDebugger(sol::state &state) {
  auto table = state.create_table("DebuggerOp");
  // default
  table.set_function("isDebugCameraOn", []() {
    auto result = Command<0x05A0>();
    return std::make_tuple(result);
  });

  // debug
  table.set_function("enable", []() { Command<0x00C3>(); });
  table.set_function("disable", []() { Command<0x00C4>(); });
  table.set_function("breakpoint", [](const int &_blocking, const char *_text, const int &_args) {
    Command<0x2100>(_blocking, _text, _args);
  });
  table.set_function("trace", [](const char *_text, const int &_args) { Command<0x2101>(_text, _args); });
  table.set_function("logLine", [](const char *_filename, const int &_timestamp, const char *_text, const int &_args) {
    Command<0x2102>(_filename, _timestamp, _text, _args);
  });
}