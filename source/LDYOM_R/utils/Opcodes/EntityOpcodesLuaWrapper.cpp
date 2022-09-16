//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void EntityOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("EntityOp");
    //CLEO+
	table.set_function("getType", [](const int& _entity) {int type_; auto result = Command<0x0E13>(_entity, &type_); return std::make_tuple(type_, result);});

}