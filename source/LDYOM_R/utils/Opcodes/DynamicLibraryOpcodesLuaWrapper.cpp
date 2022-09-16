//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void DynamicLibraryOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("DynamicLibraryOp");
    //CLEO
	table.set_function("load", [](const char* _libraryFileName) {int handle_; auto result = Command<0x0AA2>(_libraryFileName, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("free", [](const int& _self) { auto result = Command<0x0AA3>(_self); return std::make_tuple(result);});
	table.set_function("getProcedure", [](const char* _procName, const int& _self) {int address_; auto result = Command<0x0AA4>(_procName, _self, &address_); return std::make_tuple(address_, result);});

}