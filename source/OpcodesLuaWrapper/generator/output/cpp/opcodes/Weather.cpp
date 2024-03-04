// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindWeather(sol::state &state) {
  auto table = state.create_table("WeatherOp");
  // default
  table.set_function("force", [](const int &_type) { Command<0x01B5>(_type); });
  table.set_function("forceNow", [](const int &_type) { Command<0x01B6>(_type); });
  table.set_function("release", []() { Command<0x01B7>(); });
  table.set_function("setHeathazeEffect", [](const bool &_state) { Command<0x08FD>(_state); });
  table.set_function("setToAppropriateTypeNow", []() { Command<0x0915>(); });

  // CLEO+
  table.set_function("getCurrent", []() {
    int type_;
    Command<0x0D59>(&type_);
    return std::make_tuple(type_);
  });
  table.set_function("getNext", []() {
    int type_;
    Command<0x0E04>(&type_);
    return std::make_tuple(type_);
  });
  table.set_function("setNext", [](const int &_type) { Command<0x0E05>(_type); });
  table.set_function("getRainIntensity", []() {
    float intensity_;
    Command<0x0E06>(&intensity_);
    return std::make_tuple(intensity_);
  });
  table.set_function("setRainIntensity", [](const float &_intensity) { Command<0x0E07>(_intensity); });
  table.set_function("getDayNightBalance", []() {
    float intensity_;
    Command<0x0E6C>(&intensity_);
    return std::make_tuple(intensity_);
  });
  table.set_function("getUnderwaterness", []() {
    float intensity_;
    Command<0x0E6D>(&intensity_);
    return std::make_tuple(intensity_);
  });
  table.set_function("getForced", []() {
    int weather_;
    Command<0x0EB0>(&weather_);
    return std::make_tuple(weather_);
  });
}