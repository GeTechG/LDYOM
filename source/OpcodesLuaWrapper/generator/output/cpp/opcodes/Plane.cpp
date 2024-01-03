// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindPlane(sol::state &state) {
  auto table = state.create_table("PlaneOp");
  // default
  table.set_function("gotoCoords",
                     [](const int &_self, const float &_x, const float &_y, const float &_z, const float &_minAltitude,
                        const float &_maxAltitude) { Command<0x04D2>(_self, _x, _y, _z, _minAltitude, _maxAltitude); });
  table.set_function("attackPlayer", [](const int &_self, const int &_handle, const float &_radius) {
    Command<0x070E>(_self, _handle, _radius);
  });
  table.set_function("flyInDirection",
                     [](const int &_self, const float &_heading, const float &_minAltitude, const float &_maxAltitude) {
                       Command<0x070F>(_self, _heading, _minAltitude, _maxAltitude);
                     });
  table.set_function("followEntity", [](const int &_self, const int &_char, const int &_vehicle, const float &_radius) {
    Command<0x0710>(_self, _char, _vehicle, _radius);
  });
  table.set_function("setThrottle",
                     [](const int &_self, const float &_throttle) { Command<0x0742>(_self, _throttle); });
  table.set_function("startsInAir", [](const int &_self) { Command<0x0745>(_self); });
  table.set_function("attackPlayerUsingDogFight", [](const int &_self, const int &_player, const float &_radius) {
    Command<0x08A2>(_self, _player, _radius);
  });
  table.set_function("setUndercarriageUp", [](const int &_self, const int &_state) { Command<0x08E6>(_self, _state); });
  table.set_function("getUndercarriagePosition", [](const int &_self) {
    float position_;
    Command<0x091F>(_self, &position_);
    return std::make_tuple(position_);
  });
}