//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void MenuGridOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("MenuGridOp");
    //default
	table.set_function("create", [](const char* _header, const float& _topLeftX, const float& _topLeftY, const float& _width, const int& _numColumns, const bool& _interactive, const bool& _background, const int& _alignment) {int handle_; auto result = Command<0x0964>(_header, _topLeftX, _topLeftY, _width, _numColumns, _interactive, _background, _alignment, &handle_); return std::make_tuple(handle_, result);});

}