//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void FindFileOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("FindFileOp");
    //CLEO
	table.set_function("first", [](const char* _searchMask, const char* _fileName) {int handle_; auto result = Command<0x0AE6>(_searchMask, _fileName, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("next", [](const int& _self) {const char* fileName_; auto result = Command<0x0AE7>(_self, (int*)&fileName_); return std::make_tuple(fileName_, result);});
	table.set_function("close", [](const int& _self) { auto result = Command<0x0AE8>(_self); return std::make_tuple(result);});

}