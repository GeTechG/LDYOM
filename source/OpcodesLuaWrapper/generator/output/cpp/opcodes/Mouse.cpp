// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindMouse(sol::state &state) {
  auto table = state.create_table("MouseOp");
  // default
  table.set_function("getMovement", []() {
    float offsetX_;
    float offsetY_;
    Command<0x0A4A>(&offsetX_, &offsetY_);
    return std::make_tuple(offsetX_, offsetY_);
  });
  table.set_function("isUsingVerticalInversion", []() {
    auto result = Command<0x0A4C>();
    return std::make_tuple(result);
  });

  // CLEO+
  table.set_function("isWheelUp", []() {
    auto result = Command<0x0E10>();
    return std::make_tuple(result);
  });
  table.set_function("isWheelDown", []() {
    auto result = Command<0x0E11>();
    return std::make_tuple(result);
  });
  table.set_function("getSensibility", []() {
    float sensibility_;
    Command<0x0E23>(&sensibility_);
    return std::make_tuple(sensibility_);
  });
}