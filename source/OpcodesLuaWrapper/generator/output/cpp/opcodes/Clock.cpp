// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindClock(sol::state &state) {
  auto table = state.create_table("ClockOp");
  // default
  table.set_function("getTimeOfDay", []() {
    int hours_;
    int minutes_;
    Command<0x00BF>(&hours_, &minutes_);
    return std::make_tuple(hours_, minutes_);
  });
  table.set_function("setTimeOfDay", [](const int &_hours, const int &_minutes) { Command<0x00C0>(_hours, _minutes); });
  table.set_function("getMinutesToTimeOfDay", [](const int &_hours, const int &_minutes) {
    int minutesLeft_;
    Command<0x00C1>(_hours, _minutes, &minutesLeft_);
    return std::make_tuple(minutesLeft_);
  });
  table.set_function("setTimeScale", [](const float &_scale) { Command<0x015D>(_scale); });
  table.set_function("getGameTimer", []() {
    int time_;
    Command<0x01BD>(&time_);
    return std::make_tuple(time_);
  });
  table.set_function("store", []() { Command<0x0253>(); });
  table.set_function("restore", []() { Command<0x0254>(); });
  table.set_function("getCurrentDayOfWeek", []() {
    int day_;
    Command<0x07D0>(&day_);
    return std::make_tuple(day_);
  });
  table.set_function("getCurrentDate", []() {
    int day_;
    int month_;
    Command<0x0835>(&day_, &month_);
    return std::make_tuple(day_, month_);
  });
  table.set_function("setTimeOneDayForward", []() { Command<0x088E>(); });

  // CLEO+
  table.set_function("getLocalTime", []() {
    int year_;
    int month_;
    int weekDay_;
    int day_;
    int hour_;
    int minute_;
    int second_;
    int millisecond_;
    Command<0x0D2D>(&year_, &month_, &weekDay_, &day_, &hour_, &minute_, &second_, &millisecond_);
    return std::make_tuple(year_, month_, weekDay_, day_, hour_, minute_, second_, millisecond_);
  });
  table.set_function("getCurrentHour", []() {
    int hour_;
    Command<0x0E40>(&hour_);
    return std::make_tuple(hour_);
  });
  table.set_function("getCurrentMinute", []() {
    int minute_;
    Command<0x0E41>(&minute_);
    return std::make_tuple(minute_);
  });
  table.set_function("passTime", [](const int &_minutes) { Command<0x0EBB>(_minutes); });
}