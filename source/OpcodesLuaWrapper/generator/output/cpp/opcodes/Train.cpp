// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindTrain(sol::state &state) {
  auto table = state.create_table("TrainOp");
  // default
  table.set_function("create",
                     [](const int &_type, const float &_x, const float &_y, const float &_z, const int &_direction) {
                       int handle_;
                       Command<0x06D8>(_type, _x, _y, _z, _direction, &handle_);
                       return std::make_tuple(handle_);
                     });
  table.set_function("setSpeed", [](const int &_self, const float &_speed) { Command<0x06DC>(_self, _speed); });
  table.set_function("setCruiseSpeed", [](const int &_self, const float &_speed) { Command<0x06DD>(_self, _speed); });
  table.set_function("getCaboose", [](const int &_self) {
    int caboose_;
    Command<0x06DE>(_self, &caboose_);
    return std::make_tuple(caboose_);
  });
  table.set_function("getCarriage", [](const int &_self, const int &_number) {
    int carriage_;
    Command<0x078A>(_self, _number, &carriage_);
    return std::make_tuple(carriage_);
  });
  table.set_function("delete", [](const int &_self) { Command<0x07BD>(_self); });
  table.set_function("markAsNoLongerNeeded", [](const int &_self) { Command<0x07BE>(_self); });
  table.set_function("setCoordinates", [](const int &_self, const float &_x, const float &_y, const float &_z) {
    Command<0x07C7>(_self, _x, _y, _z);
  });
  table.set_function("hasDerailed", [](const int &_self) {
    auto result = Command<0x0981>(_self);
    return std::make_tuple(result);
  });
  table.set_function("setForcedToSlowDown",
                     [](const int &_self, const int &_state) { Command<0x09CF>(_self, _state); });
  table.set_function("findDirection", [](const int &_self) {
    auto result = Command<0x09E3>(_self);
    return std::make_tuple(result);
  });
  table.set_function("isNextStationAllowed", [](const int &_self) {
    auto result = Command<0x0A06>(_self);
    return std::make_tuple(result);
  });
  table.set_function("skipToNextAllowedStation", [](const int &_self) { Command<0x0A07>(_self); });
}