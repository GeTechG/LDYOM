//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void WeatherOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("WeatherOp");
    //default
	table.set_function("force", [](const int& _type) { auto result = Command<0x01B5>(_type); return std::make_tuple(result);});
	table.set_function("forceNow", [](const int& _type) { auto result = Command<0x01B6>(_type); return std::make_tuple(result);});
	table.set_function("release", []() { auto result = Command<0x01B7>(); return std::make_tuple(result);});
	table.set_function("setHeathazeEffect", [](const bool& _state) { auto result = Command<0x08FD>(_state); return std::make_tuple(result);});
	table.set_function("setToAppropriateTypeNow", []() { auto result = Command<0x0915>(); return std::make_tuple(result);});
	
	//CLEO+
	table.set_function("getCurrent", []() {int type_; auto result = Command<0x0D59>(&type_); return std::make_tuple(type_, result);});
	table.set_function("getNext", []() {int type_; auto result = Command<0x0E04>(&type_); return std::make_tuple(type_, result);});
	table.set_function("setNext", [](const int& _type) { auto result = Command<0x0E05>(_type); return std::make_tuple(result);});
	table.set_function("getRainIntensity", []() {float intensity_; auto result = Command<0x0E06>(&intensity_); return std::make_tuple(intensity_, result);});
	table.set_function("setRainIntensity", [](const float& _intensity) { auto result = Command<0x0E07>(_intensity); return std::make_tuple(result);});

}