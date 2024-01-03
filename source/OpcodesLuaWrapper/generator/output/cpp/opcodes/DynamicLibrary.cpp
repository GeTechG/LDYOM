// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindDynamicLibrary(sol::state &state) {
  auto table = state.create_table("DynamicLibraryOp");
  // CLEO
  table.set_function("load", [](const char *_libraryFileName) {
    int handle_;
    auto result = Command<0x0AA2>(_libraryFileName, &handle_);
    return std::make_tuple(result, handle_);
  });
  table.set_function("free", [](const int &_self) { Command<0x0AA3>(_self); });
  table.set_function("getProcedure", [](const char *_procName, const int &_self) {
    int address_;
    auto result = Command<0x0AA4>(_procName, _self, &address_);
    return std::make_tuple(result, address_);
  });

  // CLEO+
  table.set_function("getLoadedLibrary", [](const char *_library) {
    int address_;
    auto result = Command<0x0EFE>(_library, &address_);
    return std::make_tuple(result, address_);
  });
}