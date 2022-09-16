//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void GroupOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("GroupOp");
    //default
	table.set_function("create", [](const int& _defaultTaskAllocator) {int handle_; auto result = Command<0x062F>(_defaultTaskAllocator, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("setLeader", [](const int& _self, const int& _handle) { auto result = Command<0x0630>(_self, _handle); return std::make_tuple(result);});
	table.set_function("setMember", [](const int& _self, const int& _handle) { auto result = Command<0x0631>(_self, _handle); return std::make_tuple(result);});
	table.set_function("remove", [](const int& _self) { auto result = Command<0x0632>(_self); return std::make_tuple(result);});
	table.set_function("setDecisionMaker", [](const int& _self, const int& _maskOrHandle) { auto result = Command<0x06AD>(_self, _maskOrHandle); return std::make_tuple(result);});
	table.set_function("setSeparationRange", [](const int& _self, const float& _range) { auto result = Command<0x06F0>(_self, _range); return std::make_tuple(result);});
	table.set_function("setDefaultTaskAllocator", [](const int& _self, const int& _defaultTaskAllocator) { auto result = Command<0x07B3>(_self, _defaultTaskAllocator); return std::make_tuple(result);});
	table.set_function("getSize", [](const int& _self) {int numLeaders_;int numMembers_; auto result = Command<0x07F6>(_self, &numLeaders_, &numMembers_); return std::make_tuple(numLeaders_, numMembers_, result);});
	table.set_function("doesExist", [](const int& _handle) { auto result = Command<0x07FD>(_handle); return std::make_tuple(result);});
	table.set_function("setSequence", [](const int& _self, const int& _sequence) { auto result = Command<0x087D>(_self, _sequence); return std::make_tuple(result);});
	table.set_function("getMember", [](const int& _self, const int& _slotId) {int handle_; auto result = Command<0x092B>(_self, _slotId, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("setFollowStatus", [](const int& _self, const bool& _state) { auto result = Command<0x0940>(_self, _state); return std::make_tuple(result);});

}