//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void ClockOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("ClockOp");
    //default
	table.set_function("getTimeOfDay", []() {int hours_;int minutes_; auto result = Command<0x00BF>(&hours_, &minutes_); return std::make_tuple(hours_, minutes_, result);});
	table.set_function("setTimeOfDay", [](const int& _hours, const int& _minutes) { auto result = Command<0x00C0>(_hours, _minutes); return std::make_tuple(result);});
	table.set_function("getMinutesToTimeOfDay", [](const int& _hours, const int& _minutes) {int minutesLeft_; auto result = Command<0x00C1>(_hours, _minutes, &minutesLeft_); return std::make_tuple(minutesLeft_, result);});
	table.set_function("setTimeScale", [](const float& _scale) { auto result = Command<0x015D>(_scale); return std::make_tuple(result);});
	table.set_function("getGameTimer", []() {int time_; auto result = Command<0x01BD>(&time_); return std::make_tuple(time_, result);});
	table.set_function("store", []() { auto result = Command<0x0253>(); return std::make_tuple(result);});
	table.set_function("restore", []() { auto result = Command<0x0254>(); return std::make_tuple(result);});
	table.set_function("getCurrentDayOfWeek", []() {int day_; auto result = Command<0x07D0>(&day_); return std::make_tuple(day_, result);});
	table.set_function("getCurrentDate", []() {int day_;int month_; auto result = Command<0x0835>(&day_, &month_); return std::make_tuple(day_, month_, result);});
	table.set_function("setTimeOneDayForward", []() { auto result = Command<0x088E>(); return std::make_tuple(result);});

}