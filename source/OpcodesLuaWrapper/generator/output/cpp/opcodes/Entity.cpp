// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindEntity(sol::state &state) {
  auto table = state.create_table("EntityOp");
  // CLEO+
  table.set_function("getType", [](const int &_entity) {
    int type_;
    Command<0x0E13>(_entity, &type_);
    return std::make_tuple(type_);
  });
  table.set_function("locateDistanceToEntity", [](const int &_entityA, const int &_entityB, const float &_radius) {
    auto result = Command<0x0EED>(_entityA, _entityB, _radius);
    return std::make_tuple(result);
  });
  table.set_function("getCoordinates", [](const int &_address) {
    float x_;
    float y_;
    float z_;
    Command<0x0EEE>(_address, &x_, &y_, &z_);
    return std::make_tuple(x_, y_, z_);
  });
  table.set_function("getHeading", [](const int &_address) {
    float heading_;
    Command<0x0EEF>(_address, &heading_);
    return std::make_tuple(heading_);
  });
}