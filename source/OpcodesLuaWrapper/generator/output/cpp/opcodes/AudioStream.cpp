// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindAudioStream(sol::state &state) {
  auto table = state.create_table("AudioStreamOp");
  // CLEO
  table.set_function("load", [](const char *_audioFileName) {
    int handle_;
    auto result = Command<0x0AAC>(_audioFileName, &handle_);
    return std::make_tuple(result, handle_);
  });
  table.set_function("setState", [](const int &_self, const int &_state) { Command<0x0AAD>(_self, _state); });
  table.set_function("remove", [](const int &_self) { Command<0x0AAE>(_self); });
  table.set_function("getLength", [](const int &_self) {
    int length_;
    Command<0x0AAF>(_self, &length_);
    return std::make_tuple(length_);
  });
  table.set_function("getState", [](const int &_self) {
    int state_;
    Command<0x0AB9>(_self, &state_);
    return std::make_tuple(state_);
  });
  table.set_function("getVolume", [](const int &_self) {
    float volume_;
    Command<0x0ABB>(_self, &volume_);
    return std::make_tuple(volume_);
  });
  table.set_function("setVolume", [](const int &_self, const float &_volume) { Command<0x0ABC>(_self, _volume); });
  table.set_function("setLooped", [](const int &_self, const int &_state) { Command<0x0AC0>(_self, _state); });

  // CLEO+
  table.set_function("getInternal", [](const int &_self) {
    int address_;
    Command<0x0E3B>(_self, &address_);
    return std::make_tuple(address_);
  });
}