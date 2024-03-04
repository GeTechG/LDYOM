// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindGarage(sol::state &state) {
  auto table = state.create_table("GarageOp");
  // default
  table.set_function("setTargetCarForMission",
                     [](const char *_garageName, const int &_vehicle) { Command<0x021B>(_garageName, _vehicle); });
  table.set_function("activate", [](const char *_garageId) { Command<0x0299>(_garageId); });
  table.set_function("deactivate", [](const char *_garageId) { Command<0x02B9>(_garageId); });
  table.set_function("changeType", [](const char *_garageId, const int &_type) { Command<0x02FA>(_garageId, _type); });
  table.set_function("open", [](const char *_garageId) { Command<0x0360>(_garageId); });
  table.set_function("close", [](const char *_garageId) { Command<0x0361>(_garageId); });
  table.set_function("isOpen", [](const char *_garageId) {
    auto result = Command<0x03B0>(_garageId);
    return std::make_tuple(result);
  });
  table.set_function("isClosed", [](const char *_garageId) {
    auto result = Command<0x03B1>(_garageId);
    return std::make_tuple(result);
  });
  table.set_function("setResprayFree",
                     [](const char *_garageId, const bool &_state) { Command<0x093A>(_garageId, _state); });
}