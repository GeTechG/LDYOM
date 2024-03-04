// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindRc(sol::state &state) {
  auto table = state.create_table("RcOp");
  // default
  table.set_function("giveModelToPlayer",
                     [](const int &_handle, const float &_x, const float &_y, const float &_z, const float &_angle,
                        const int &_modelId) { Command<0x046E>(_handle, _x, _y, _z, _angle, _modelId); });
  table.set_function("getCar", [](const int &_player) {
    int car_;
    Command<0x0484>(_player, &car_);
    return std::make_tuple(car_);
  });
  table.set_function("setEnableDetonate", [](const bool &_state) { Command<0x048A>(_state); });
  table.set_function("setEnableDetonateOnContact", [](const bool &_state) { Command<0x04D6>(_state); });
  table.set_function("removeBuggy", []() { Command<0x04DB>(); });
  table.set_function("takeCar", [](const int &_player, const int &_vehicle) { Command<0x0715>(_player, _vehicle); });
}