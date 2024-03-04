// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindColPoint(sol::state &state) {
  auto table = state.create_table("ColPointOp");
  // CLEO+
  table.set_function("getCollisionBetweenPoints",
                     [](const float &_fromX, const float &_fromY, const float &_fromZ, const float &_toX,
                        const float &_toY, const float &_toZ, const bool &_buildings, const bool &_vehicles,
                        const bool &_peds, const bool &_objects, const bool &_dummies, const bool &_seeThroughCheck,
                        const bool &_cameraIgnoreCheck, const bool &_shotThroughCheck, const int &_entityToIgnore) {
                       int address_;
                       float outX_;
                       float outY_;
                       float outZ_;
                       int entity_;
                       auto result =
                           Command<0x0D3A>(_fromX, _fromY, _fromZ, _toX, _toY, _toZ, _buildings, _vehicles, _peds,
                                           _objects, _dummies, _seeThroughCheck, _cameraIgnoreCheck, _shotThroughCheck,
                                           _entityToIgnore, &address_, &outX_, &outY_, &outZ_, &entity_);
                       return std::make_tuple(result, address_, outX_, outY_, outZ_, entity_);
                     });
  table.set_function("getNormalVector", [](const int &_self) {
    float x_;
    float y_;
    float z_;
    Command<0x0D3B>(_self, &x_, &y_, &z_);
    return std::make_tuple(x_, y_, z_);
  });
  table.set_function("getSurface", [](const int &_self) {
    int surfaceType_;
    Command<0x0D3C>(_self, &surfaceType_);
    return std::make_tuple(surfaceType_);
  });
  table.set_function("getDepth", [](const int &_self) {
    float depth_;
    Command<0x0D3E>(_self, &depth_);
    return std::make_tuple(depth_);
  });
  table.set_function("getLighting", [](const int &_self, const bool &_fromNight) {
    int lighting_;
    Command<0x0E6B>(_self, _fromNight, &lighting_);
    return std::make_tuple(lighting_);
  });
  table.set_function("getCoordinates", [](const int &_self) {
    float x_;
    float y_;
    float z_;
    Command<0x0EE1>(_self, &x_, &y_, &z_);
    return std::make_tuple(x_, y_, z_);
  });
}