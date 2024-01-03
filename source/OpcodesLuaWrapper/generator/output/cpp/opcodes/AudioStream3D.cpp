// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindAudioStream3D(sol::state &state) {
  auto table = state.create_table("AudioStream3DOp");
  // CLEO
  table.set_function("load", [](const char *_audioFileName) {
    int handle_;
    auto result = Command<0x0AC1>(_audioFileName, &handle_);
    return std::make_tuple(result, handle_);
  });
  table.set_function("setPlayAtCoords", [](const int &_self, const float &_x, const float &_y, const float &_z) {
    Command<0x0AC2>(_self, _x, _y, _z);
  });
  table.set_function("setPlayAtObject", [](const int &_self, const int &_handle) { Command<0x0AC3>(_self, _handle); });
  table.set_function("setPlayAtChar", [](const int &_self, const int &_handle) { Command<0x0AC4>(_self, _handle); });
  table.set_function("setPlayAtCar", [](const int &_self, const int &_handle) { Command<0x0AC5>(_self, _handle); });
}