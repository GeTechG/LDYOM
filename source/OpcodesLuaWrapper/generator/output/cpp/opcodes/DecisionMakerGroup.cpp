// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindDecisionMakerGroup(sol::state &state) {
  auto table = state.create_table("DecisionMakerGroupOp");
  // default
  table.set_function("load", [](const int &_type) {
    int handle_;
    Command<0x06AE>(_type, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("clearEventResponse",
                     [](const int &_self, const int &_eventId) { Command<0x0749>(_self, _eventId); });
  table.set_function("addEventResponse", [](const int &_self, const int &_eventId, const int &_taskId,
                                            const float &_respect, const float &_hate, const float &_like,
                                            const float &_dislike, const bool &_inCar, const bool &_onFoot) {
    Command<0x074A>(_self, _eventId, _taskId, _respect, _hate, _like, _dislike, _inCar, _onFoot);
  });
  table.set_function("copy", [](const int &_mask) {
    int handle_;
    Command<0x07E6>(_mask, &handle_);
    return std::make_tuple(handle_);
  });
}