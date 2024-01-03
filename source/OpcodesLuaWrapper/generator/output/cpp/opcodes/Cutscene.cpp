// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindCutscene(sol::state &state) {
  auto table = state.create_table("CutsceneOp");
  // default
  table.set_function("setOffset",
                     [](const float &_x, const float &_y, const float &_z) { Command<0x0244>(_x, _y, _z); });
  table.set_function("load", [](const char *_name) { Command<0x02E4>(_name); });
  table.set_function("start", []() { Command<0x02E7>(); });
  table.set_function("getTime", []() {
    int time_;
    Command<0x02E8>(&time_);
    return std::make_tuple(time_);
  });
  table.set_function("hasFinished", []() {
    auto result = Command<0x02E9>();
    return std::make_tuple(result);
  });
  table.set_function("clear", []() { Command<0x02EA>(); });
  table.set_function("wasSkipped", []() {
    auto result = Command<0x056A>();
    return std::make_tuple(result);
  });
  table.set_function("hasLoaded", []() {
    auto result = Command<0x06B9>();
    return std::make_tuple(result);
  });
  table.set_function("getOffset", []() {
    float xOffset_;
    float yOffset_;
    float zOffset_;
    Command<0x08D1>(&xOffset_, &yOffset_, &zOffset_);
    return std::make_tuple(xOffset_, yOffset_, zOffset_);
  });
  table.set_function("appendToNext", [](const char *__p1, const char *__p2) { Command<0x08F0>(__p1, __p2); });

  // CLEO+
  table.set_function("isOn", []() {
    auto result = Command<0x0E25>();
    return std::make_tuple(result);
  });
}