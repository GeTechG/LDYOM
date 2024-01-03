// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindKillFrenzy(sol::state &state) {
  auto table = state.create_table("KillFrenzyOp");
  // default
  table.set_function("start", [](const int &_text, const int &_weaponType, const int &_timeInMs, const int &_targetsNum,
                                 const int &_targetModel1, const int &_targetModel2, const int &_targetModel3,
                                 const int &_targetModel4, const int &_betaSoundsAndMessages) {
    Command<0x01F9>(_text, _weaponType, _timeInMs, _targetsNum, _targetModel1, _targetModel2, _targetModel3,
                    _targetModel4, _betaSoundsAndMessages);
  });
  table.set_function("readStatus", []() {
    int status_;
    Command<0x01FA>(&status_);
    return std::make_tuple(status_);
  });
  table.set_function("fail", []() { Command<0x09C2>(); });
}