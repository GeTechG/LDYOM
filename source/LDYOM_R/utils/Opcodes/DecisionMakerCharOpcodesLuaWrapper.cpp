//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void DecisionMakerCharOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("DecisionMakerCharOp");
    //default
	table.set_function("load", [](const int& _type) {int handle_; auto result = Command<0x060A>(_type, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("clearEventResponse", [](const int& _self, const int& _eventId) { auto result = Command<0x0708>(_self, _eventId); return std::make_tuple(result);});
	table.set_function("addEventResponse", [](const int& _self, const int& _eventId, const int& _taskId, const float& _respect, const float& _hate, const float& _like, const float& _dislike, const bool& _inCar, const bool& _onFoot) { auto result = Command<0x0709>(_self, _eventId, _taskId, _respect, _hate, _like, _dislike, _inCar, _onFoot); return std::make_tuple(result);});
	table.set_function("copy", [](const int& _maskOrOtherHandle) {int handle_; auto result = Command<0x07E5>(_maskOrOtherHandle, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("copyShared", [](const int& _mask) {int handle_; auto result = Command<0x0978>(_mask, &handle_); return std::make_tuple(handle_, result);});

}