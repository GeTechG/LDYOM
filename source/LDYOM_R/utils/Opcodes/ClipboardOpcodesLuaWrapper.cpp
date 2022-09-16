//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void ClipboardOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("ClipboardOp");
    //clipboard
	table.set_function("readData", [](const int& _memory, const int& _size) { auto result = Command<0x0B20>(_memory, _size); return std::make_tuple(result);});
	table.set_function("writeData", [](const int& _memory, const int& _size) { auto result = Command<0x0B21>(_memory, _size); return std::make_tuple(result);});

}