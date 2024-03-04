// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindGroup(sol::state &state) {
  auto table = state.create_table("GroupOp");
  // default
  table.set_function("create", [](const int &_defaultTaskAllocator) {
    int handle_;
    Command<0x062F>(_defaultTaskAllocator, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("setLeader", [](const int &_self, const int &_handle) { Command<0x0630>(_self, _handle); });
  table.set_function("setMember", [](const int &_self, const int &_handle) { Command<0x0631>(_self, _handle); });
  table.set_function("remove", [](const int &_self) { Command<0x0632>(_self); });
  table.set_function("setDecisionMaker",
                     [](const int &_self, const int &_maskOrHandle) { Command<0x06AD>(_self, _maskOrHandle); });
  table.set_function("setSeparationRange",
                     [](const int &_self, const float &_range) { Command<0x06F0>(_self, _range); });
  table.set_function("setDefaultTaskAllocator", [](const int &_self, const int &_defaultTaskAllocator) {
    Command<0x07B3>(_self, _defaultTaskAllocator);
  });
  table.set_function("getSize", [](const int &_self) {
    int numLeaders_;
    int numMembers_;
    Command<0x07F6>(_self, &numLeaders_, &numMembers_);
    return std::make_tuple(numLeaders_, numMembers_);
  });
  table.set_function("doesExist", [](const int &_handle) {
    auto result = Command<0x07FD>(_handle);
    return std::make_tuple(result);
  });
  table.set_function("setSequence", [](const int &_self, const int &_sequence) { Command<0x087D>(_self, _sequence); });
  table.set_function("getMember", [](const int &_self, const int &_slotId) {
    int handle_;
    Command<0x092B>(_self, _slotId, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("setFollowStatus", [](const int &_self, const bool &_state) { Command<0x0940>(_self, _state); });
}