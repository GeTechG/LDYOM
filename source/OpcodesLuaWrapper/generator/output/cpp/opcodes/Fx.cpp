// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindFx(sol::state &state) {
  auto table = state.create_table("FxOp");
  // default
  table.set_function("drawShadow",
                     [](const int &_textureType, const float &_x, const float &_y, const float &_z, const float &_angle,
                        const float &_length, const int &_intensity, const int &_r, const int &_g, const int &_b) {
                       Command<0x016F>(_textureType, _x, _y, _z, _angle, _length, _intensity, _r, _g, _b);
                     });
  table.set_function("addExplosion", [](const float &_x, const float &_y, const float &_z, const int &_type) {
    Command<0x020C>(_x, _y, _z, _type);
  });
  table.set_function("drawCorona",
                     [](const float &_x, const float &_y, const float &_z, const float &_size, const int &_coronaType,
                        const int &_flareType, const int &_r, const int &_g,
                        const int &_b) { Command<0x024F>(_x, _y, _z, _size, _coronaType, _flareType, _r, _g, _b); });
  table.set_function("drawWeaponshopCorona",
                     [](const float &_x, const float &_y, const float &_z, const float &_size, const int &_coronaType,
                        const int &_flareType, const int &_r, const int &_g,
                        const int &_b) { Command<0x04D5>(_x, _y, _z, _size, _coronaType, _flareType, _r, _g, _b); });
  table.set_function("addExplosionNoSound", [](const float &_x, const float &_y, const float &_z, const int &_type) {
    Command<0x0565>(_x, _y, _z, _type);
  });
  table.set_function("addBigGunFlash", [](const float &_fromX, const float &_fromY, const float &_fromZ,
                                          const float &_toX, const float &_toY, const float &_toZ) {
    Command<0x058A>(_fromX, _fromY, _fromZ, _toX, _toY, _toZ);
  });
  table.set_function("addSparks", [](const float &_x, const float &_y, const float &_z, const float &_velocityX,
                                     const float &_velocityY, const float &_velocityZ, const int &_density) {
    Command<0x08EB>(_x, _y, _z, _velocityX, _velocityY, _velocityZ, _density);
  });
  table.set_function("addExplosionVariableShake",
                     [](const float &_x, const float &_y, const float &_z, const int &_type, const float &_shake) {
                       Command<0x0948>(_x, _y, _z, _type, _shake);
                     });
  table.set_function("addSmokeParticle", [](const float &_x, const float &_y, const float &_z, const float &_velocityX,
                                            const float &_velocityY, const float &_velocityZ, const int &_red,
                                            const int &_green, const int &_blue, const int &_alpha, const float &_size,
                                            const float &_lastFactor) {
    Command<0x095C>(_x, _y, _z, _velocityX, _velocityY, _velocityZ, _red, _green, _blue, _alpha, _size, _lastFactor);
  });
  table.set_function("addBlood",
                     [](const float &_x, const float &_y, const float &_z, const float &_offsetX, const float &_offsetY,
                        const float &_offsetZ, const int &_density, const int &_handle) {
                       Command<0x09B8>(_x, _y, _z, _offsetX, _offsetY, _offsetZ, _density, _handle);
                     });
  table.set_function("drawLightWithRange", [](const float &_x, const float &_y, const float &_z, const int &_red,
                                              const int &_green, const int &_blue, const float &_radius) {
    Command<0x09E5>(_x, _y, _z, _red, _green, _blue, _radius);
  });

  // CLEO+
  table.set_function("getAddress", [](const int &_self) {
    int address_;
    Command<0x0EBF>(_self, &address_);
    return std::make_tuple(address_);
  });
  table.set_function("addParticle", [](const int &_self, const float &_posX, const float &_posY, const float &_posZ,
                                       const float &_velX, const float &_velY, const float &_velZ, const float &_size,
                                       const float &_brightness, const float &_r, const float &_g, const float &_b,
                                       const float &_a, const float &_lastFactor) {
    Command<0x0EC0>(_self, _posX, _posY, _posZ, _velX, _velY, _velZ, _size, _brightness, _r, _g, _b, _a, _lastFactor);
  });
  table.set_function("isAvailableWithName", [](const char *_name) {
    auto result = Command<0x0EC1>(_name);
    return std::make_tuple(result);
  });
}