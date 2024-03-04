// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindStuckCarCheck(sol::state &state) {
  auto table = state.create_table("StuckCarCheckOp");
  // default
  table.set_function("add", [](const int &_vehicle, const float &_distance, const int &_time) {
    Command<0x03CC>(_vehicle, _distance, _time);
  });
  table.set_function("remove", [](const int &_vehicle) { Command<0x03CD>(_vehicle); });
  table.set_function("isCarStuck", [](const int &_vehicle) {
    auto result = Command<0x03CE>(_vehicle);
    return std::make_tuple(result);
  });
  table.set_function("addWithWarp",
                     [](const int &_vehicle, const float &_distance, const int &_time, const bool &_stuck,
                        const bool &_flipped, const bool &_warp, const int &_pathId) {
                       Command<0x072F>(_vehicle, _distance, _time, _stuck, _flipped, _warp, _pathId);
                     });
}