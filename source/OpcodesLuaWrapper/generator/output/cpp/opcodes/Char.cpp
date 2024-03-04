// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindChar(sol::state &state) {
  auto table = state.create_table("CharOp");
  // default
  table.set_function("create",
                     [](const int &_pedType, const int &_modelId, const float &_x, const float &_y, const float &_z) {
                       int handle_;
                       Command<0x009A>(_pedType, _modelId, _x, _y, _z, &handle_);
                       return std::make_tuple(handle_);
                     });
  table.set_function("delete", [](const int &_self) { Command<0x009B>(_self); });
  table.set_function("getCoordinates", [](const int &_self) {
    float x_;
    float y_;
    float z_;
    Command<0x00A0>(_self, &x_, &y_, &z_);
    return std::make_tuple(x_, y_, z_);
  });
  table.set_function("setCoordinates", [](const int &_self, const float &_x, const float &_y, const float &_z) {
    Command<0x00A1>(_self, _x, _y, _z);
  });
  table.set_function("isInArea2D", [](const int &_self, const float &_leftBottomX, const float &_leftBottomY,
                                      const float &_rightTopX, const float &_rightTopY, const bool &_drawSphere) {
    auto result = Command<0x00A3>(_self, _leftBottomX, _leftBottomY, _rightTopX, _rightTopY, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("isInArea3D", [](const int &_self, const float &_leftBottomX, const float &_leftBottomY,
                                      const float &_leftBottomZ, const float &_rightTopX, const float &_rightTopY,
                                      const float &_rightTopZ, const bool &_drawSphere) {
    auto result = Command<0x00A4>(_self, _leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX, _rightTopY, _rightTopZ,
                                  _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("storeCarIsIn", [](const int &_self) {
    int handle_;
    Command<0x00D9>(_self, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("isInCar", [](const int &_self, const int &_vehicle) {
    auto result = Command<0x00DB>(_self, _vehicle);
    return std::make_tuple(result);
  });
  table.set_function("isInModel", [](const int &_self, const int &_modelId) {
    auto result = Command<0x00DD>(_self, _modelId);
    return std::make_tuple(result);
  });
  table.set_function("isInAnyCar", [](const int &_self) {
    auto result = Command<0x00DF>(_self);
    return std::make_tuple(result);
  });
  table.set_function("locateAnyMeans2D", [](const int &_self, const float &_x, const float &_y, const float &_xRadius,
                                            const float &_yRadius, const bool &_drawSphere) {
    auto result = Command<0x00EC>(_self, _x, _y, _xRadius, _yRadius, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("locateOnFoot2D", [](const int &_self, const float &_x, const float &_y, const float &_xRadius,
                                          const float &_yRadius, const bool &_drawSphere) {
    auto result = Command<0x00ED>(_self, _x, _y, _xRadius, _yRadius, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("locateInCar2D", [](const int &_self, const float &_x, const float &_y, const float &_xRadius,
                                         const float &_yRadius, const bool &_drawSphere) {
    auto result = Command<0x00EE>(_self, _x, _y, _xRadius, _yRadius, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("locateStoppedAnyMeans2D",
                     [](const int &_self, const float &_x, const float &_y, const float &_xRadius,
                        const float &_yRadius, const bool &_drawSphere) {
                       auto result = Command<0x00EF>(_self, _x, _y, _xRadius, _yRadius, _drawSphere);
                       return std::make_tuple(result);
                     });
  table.set_function("locateStoppedOnFoot2D",
                     [](const int &_self, const float &_x, const float &_y, const float &_xRadius,
                        const float &_yRadius, const bool &_drawSphere) {
                       auto result = Command<0x00F0>(_self, _x, _y, _xRadius, _yRadius, _drawSphere);
                       return std::make_tuple(result);
                     });
  table.set_function("locateStoppedInCar2D", [](const int &_self, const float &_x, const float &_y,
                                                const float &_xRadius, const float &_yRadius, const bool &_drawSphere) {
    auto result = Command<0x00F1>(_self, _x, _y, _xRadius, _yRadius, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("locateAnyMeansChar2D", [](const int &_self, const int &_target, const float &_xRadius,
                                                const float &_yRadius, const bool &_drawSphere) {
    auto result = Command<0x00F2>(_self, _target, _xRadius, _yRadius, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("locateOnFootChar2D", [](const int &_self, const int &_target, const float &_xRadius,
                                              const float &_yRadius, const bool &_drawSphere) {
    auto result = Command<0x00F3>(_self, _target, _xRadius, _yRadius, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("locateInCarChar2D", [](const int &_self, const int &_otherChar, const float &_xRadius,
                                             const float &_yRadius, const bool &_drawSphere) {
    auto result = Command<0x00F4>(_self, _otherChar, _xRadius, _yRadius, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("locateAnyMeans3D",
                     [](const int &_self, const float &_x, const float &_y, const float &_z, const float &_xRadius,
                        const float &_yRadius, const float &_zRadius, const bool &_drawSphere) {
                       auto result = Command<0x00FE>(_self, _x, _y, _z, _xRadius, _yRadius, _zRadius, _drawSphere);
                       return std::make_tuple(result);
                     });
  table.set_function("locateOnFoot3D",
                     [](const int &_self, const float &_x, const float &_y, const float &_z, const float &_xRadius,
                        const float &_yRadius, const float &_zRadius, const bool &_drawSphere) {
                       auto result = Command<0x00FF>(_self, _x, _y, _z, _xRadius, _yRadius, _zRadius, _drawSphere);
                       return std::make_tuple(result);
                     });
  table.set_function("locateInCar3D",
                     [](const int &_self, const float &_x, const float &_y, const float &_z, const float &_xRadius,
                        const float &_yRadius, const float &_zRadius, const bool &_drawSphere) {
                       auto result = Command<0x0100>(_self, _x, _y, _z, _xRadius, _yRadius, _zRadius, _drawSphere);
                       return std::make_tuple(result);
                     });
  table.set_function("locateStoppedAnyMeans3D",
                     [](const int &_self, const float &_x, const float &_y, const float &_z, const float &_xRadius,
                        const float &_yRadius, const float &_zRadius, const bool &_drawSphere) {
                       auto result = Command<0x0101>(_self, _x, _y, _z, _xRadius, _yRadius, _zRadius, _drawSphere);
                       return std::make_tuple(result);
                     });
  table.set_function("locateStoppedOnFoot3D",
                     [](const int &_self, const float &_x, const float &_y, const float &_z, const float &_xRadius,
                        const float &_yRadius, const float &_zRadius, const bool &_drawSphere) {
                       auto result = Command<0x0102>(_self, _x, _y, _z, _xRadius, _yRadius, _zRadius, _drawSphere);
                       return std::make_tuple(result);
                     });
  table.set_function("locateStoppedInCar3D",
                     [](const int &_self, const float &_x, const float &_y, const float &_z, const float &_xRadius,
                        const float &_yRadius, const float &_zRadius, const bool &_drawSphere) {
                       auto result = Command<0x0103>(_self, _x, _y, _z, _xRadius, _yRadius, _zRadius, _drawSphere);
                       return std::make_tuple(result);
                     });
  table.set_function("locateAnyMeansChar3D", [](const int &_self, const int &_target, const float &_xRadius,
                                                const float &_yRadius, const float &_zRadius, const bool &_drawSphere) {
    auto result = Command<0x0104>(_self, _target, _xRadius, _yRadius, _zRadius, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("locateOnFootChar3D", [](const int &_self, const int &_target, const float &_xRadius,
                                              const float &_yRadius, const float &_zRadius, const bool &_drawSphere) {
    auto result = Command<0x0105>(_self, _target, _xRadius, _yRadius, _zRadius, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("locateInCarChar3D", [](const int &_self, const int &_target, const float &_xRadius,
                                             const float &_yRadius, const float &_zRadius, const bool &_drawSphere) {
    auto result = Command<0x0106>(_self, _target, _xRadius, _yRadius, _zRadius, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("addAmmo", [](const int &_self, const int &_weaponType, const int &_ammo) {
    Command<0x0114>(_self, _weaponType, _ammo);
  });
  table.set_function("isDead", [](const int &_handle) {
    auto result = Command<0x0118>(_handle);
    return std::make_tuple(result);
  });
  table.set_function("createInsideCar", [](const int &_vehicle, const int &_pedType, const int &_modelId) {
    int handle_;
    Command<0x0129>(_vehicle, _pedType, _modelId, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("isInZone", [](const int &_self, const char *_zone) {
    auto result = Command<0x0154>(_self, _zone);
    return std::make_tuple(result);
  });
  table.set_function("getHeading", [](const int &_self) {
    float heading_;
    Command<0x0172>(_self, &heading_);
    return std::make_tuple(heading_);
  });
  table.set_function("setHeading", [](const int &_self, const float &_heading) { Command<0x0173>(_self, _heading); });
  table.set_function("isTouchingObject", [](const int &_self, const int &_object) {
    auto result = Command<0x0179>(_self, _object);
    return std::make_tuple(result);
  });
  table.set_function("setAmmo", [](const int &_self, const int &_weaponType, const int &_ammo) {
    Command<0x017B>(_self, _weaponType, _ammo);
  });
  table.set_function("isHealthGreater", [](const int &_self, const int &_health) {
    auto result = Command<0x0184>(_self, _health);
    return std::make_tuple(result);
  });
  table.set_function("isInAreaOnFoot2D", [](const int &_self, const float &_leftBottomX, const float &_leftBottomY,
                                            const float &_rightTopX, const float &_rightTopY, const bool &_drawSphere) {
    auto result = Command<0x01A1>(_self, _leftBottomX, _leftBottomY, _rightTopX, _rightTopY, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("isInAreaInCar2D", [](const int &_self, const float &_leftBottomX, const float &_leftBottomY,
                                           const float &_rightTopX, const float &_rightTopY, const bool &_drawSphere) {
    auto result = Command<0x01A2>(_self, _leftBottomX, _leftBottomY, _rightTopX, _rightTopY, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("isStoppedInArea2D", [](const int &_self, const float &_leftBottomX, const float &_leftBottomY,
                                             const float &_rightTopX, const float &_rightTopY,
                                             const bool &_drawSphere) {
    auto result = Command<0x01A3>(_self, _leftBottomX, _leftBottomY, _rightTopX, _rightTopY, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("isStoppedInAreaOnFoot2D", [](const int &_self, const float &_leftBottomX,
                                                   const float &_leftBottomY, const float &_rightTopX,
                                                   const float &_rightTopY, const bool &_drawSphere) {
    auto result = Command<0x01A4>(_self, _leftBottomX, _leftBottomY, _rightTopX, _rightTopY, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("isStoppedInAreaInCar2D", [](const int &_self, const float &_leftBottomX,
                                                  const float &_leftBottomY, const float &_rightTopX,
                                                  const float &_rightTopY, const bool &_drawSphere) {
    auto result = Command<0x01A5>(_self, _leftBottomX, _leftBottomY, _rightTopX, _rightTopY, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("isInAreaOnFoot3D", [](const int &_self, const float &_leftBottomX, const float &_leftBottomY,
                                            const float &_leftBottomZ, const float &_rightTopX, const float &_rightTopY,
                                            const float &_rightTopZ, const bool &_drawSphere) {
    auto result = Command<0x01A6>(_self, _leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX, _rightTopY, _rightTopZ,
                                  _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("isInAreaInCar3D", [](const int &_self, const float &_leftBottomX, const float &_leftBottomY,
                                           const float &_leftBottomZ, const float &_rightTopX, const float &_rightTopY,
                                           const float &_rightTopZ, const bool &_drawSphere) {
    auto result = Command<0x01A7>(_self, _leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX, _rightTopY, _rightTopZ,
                                  _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("isStoppedInArea3D",
                     [](const int &_self, const float &_leftBottomX, const float &_leftBottomY,
                        const float &_leftBottomZ, const float &_rightTopX, const float &_rightTopY,
                        const float &_rightTopZ, const bool &_drawSphere) {
                       auto result = Command<0x01A8>(_self, _leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX,
                                                     _rightTopY, _rightTopZ, _drawSphere);
                       return std::make_tuple(result);
                     });
  table.set_function("isStoppedInAreaOnFoot3D",
                     [](const int &_self, const float &_leftBottomX, const float &_leftBottomY,
                        const float &_leftBottomZ, const float &_rightTopX, const float &_rightTopY,
                        const float &_rightTopZ, const bool &_drawSphere) {
                       auto result = Command<0x01A9>(_self, _leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX,
                                                     _rightTopY, _rightTopZ, _drawSphere);
                       return std::make_tuple(result);
                     });
  table.set_function("isStoppedInAreaInCar3D",
                     [](const int &_self, const float &_leftBottomX, const float &_leftBottomY,
                        const float &_leftBottomZ, const float &_rightTopX, const float &_rightTopY,
                        const float &_rightTopZ, const bool &_drawSphere) {
                       auto result = Command<0x01AA>(_self, _leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX,
                                                     _rightTopY, _rightTopZ, _drawSphere);
                       return std::make_tuple(result);
                     });
  table.set_function("giveWeapon", [](const int &_self, const int &_weaponType, const int &_ammo) {
    Command<0x01B2>(_self, _weaponType, _ammo);
  });
  table.set_function("setCurrentWeapon",
                     [](const int &_self, const int &_weaponType) { Command<0x01B9>(_self, _weaponType); });
  table.set_function("markAsNoLongerNeeded", [](const int &_self) { Command<0x01C2>(_self); });
  table.set_function("dontRemove", [](const int &_self) { Command<0x01C5>(_self); });
  table.set_function("createAsPassenger",
                     [](const int &_vehicle, const int &_pedType, const int &_modelId, const int &_seatId) {
                       int handle_;
                       Command<0x01C8>(_vehicle, _pedType, _modelId, _seatId, &handle_);
                       return std::make_tuple(handle_);
                     });
  table.set_function("locateAnyMeansCar2D", [](const int &_self, const int &_vehicle, const float &_xRadius,
                                               const float &_yRadius, const bool &_drawSphere) {
    auto result = Command<0x0202>(_self, _vehicle, _xRadius, _yRadius, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("locateOnFootCar2D", [](const int &_self, const int &_vehicle, const float &_xRadius,
                                             const float &_yRadius, const bool &_drawSphere) {
    auto result = Command<0x0203>(_self, _vehicle, _xRadius, _yRadius, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("locateInCarCar2D", [](const int &_self, const int &_handle, const float &_xRadius,
                                            const float &_yRadius, const bool &_drawSphere) {
    auto result = Command<0x0204>(_self, _handle, _xRadius, _yRadius, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("locateAnyMeansCar3D", [](const int &_self, const int &_vehicle, const float &_xRadius,
                                               const float &_yRadius, const float &_zRadius, const bool &_drawSphere) {
    auto result = Command<0x0205>(_self, _vehicle, _xRadius, _yRadius, _zRadius, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("locateOnFootCar3D", [](const int &_self, const int &_vehicle, const float &_xRadius,
                                             const float &_yRadius, const float &_zRadius, const bool &_drawSphere) {
    auto result = Command<0x0206>(_self, _vehicle, _xRadius, _yRadius, _zRadius, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("locateInCarCar3D", [](const int &_self, const int &_vehicle, const float &_xRadius,
                                            const float &_yRadius, const float &_zRadius, const bool &_drawSphere) {
    auto result = Command<0x0207>(_self, _vehicle, _xRadius, _yRadius, _zRadius, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("setHealth", [](const int &_self, const int &_health) { Command<0x0223>(_self, _health); });
  table.set_function("getHealth", [](const int &_self) {
    int health_;
    Command<0x0226>(_self, &health_);
    return std::make_tuple(health_);
  });
  table.set_function("isTouchingObjectOnFoot", [](const int &_self, const int &_object) {
    auto result = Command<0x023B>(_self, _object);
    return std::make_tuple(result);
  });
  table.set_function("setAnimGroup",
                     [](const int &_self, const char *_animGroup) { Command<0x0245>(_self, _animGroup); });
  table.set_function("isStopped", [](const int &_self) {
    auto result = Command<0x02A0>(_self);
    return std::make_tuple(result);
  });
  table.set_function("setOnlyDamagedByPlayer",
                     [](const int &_self, const bool &_state) { Command<0x02A9>(_self, _state); });
  table.set_function("setProofs",
                     [](const int &_self, const bool &_bulletProof, const bool &_fireProof, const bool &_explosionProof,
                        const bool &_collisionProof, const bool &_meleeProof) {
                       Command<0x02AB>(_self, _bulletProof, _fireProof, _explosionProof, _collisionProof, _meleeProof);
                     });
  table.set_function("isOnScreen", [](const int &_self) {
    auto result = Command<0x02CB>(_self);
    return std::make_tuple(result);
  });
  table.set_function("isShootingInArea", [](const int &_self, const float &_leftBottomX, const float &_leftBottomY,
                                            const float &_topRightX, const float &_topRightY, const bool &_drawSphere) {
    auto result = Command<0x02D6>(_self, _leftBottomX, _leftBottomY, _topRightX, _topRightY, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("isCurrentWeapon", [](const int &_self, const int &_weaponType) {
    auto result = Command<0x02D8>(_self, _weaponType);
    return std::make_tuple(result);
  });
  table.set_function("isShooting", [](const int &_self) {
    auto result = Command<0x02E0>(_self);
    return std::make_tuple(result);
  });
  table.set_function("setAccuracy", [](const int &_self, const int &_accuracy) { Command<0x02E2>(_self, _accuracy); });
  table.set_function("isModel", [](const int &_self, const int &_modelId) {
    auto result = Command<0x02F2>(_self, _modelId);
    return std::make_tuple(result);
  });
  table.set_function("hasBeenDamagedByWeapon", [](const int &_self, const int &_weaponType) {
    auto result = Command<0x031D>(_self, _weaponType);
    return std::make_tuple(result);
  });
  table.set_function("explodeHead", [](const int &_self) { Command<0x0321>(_self); });
  table.set_function("setBleeding", [](const int &_self, const bool &_state) { Command<0x0332>(_self, _state); });
  table.set_function("setVisible", [](const int &_self, const bool &_state) { Command<0x0337>(_self, _state); });
  table.set_function("removeElegantly", [](const int &_self) { Command<0x034F>(_self); });
  table.set_function("setStayInSamePlace",
                     [](const int &_self, const bool &_state) { Command<0x0350>(_self, _state); });
  table.set_function("addArmor", [](const int &_self, const int &_amount) { Command<0x035F>(_self, _amount); });
  table.set_function("warpFromCarToCoord", [](const int &_self, const float &_x, const float &_y, const float &_z) {
    Command<0x0362>(_self, _x, _y, _z);
  });
  table.set_function("hasSpottedChar", [](const int &_self, const int &_target) {
    auto result = Command<0x0364>(_self, _target);
    return std::make_tuple(result);
  });
  table.set_function("warpIntoCar", [](const int &_self, const int &_vehicle) { Command<0x036A>(_self, _vehicle); });
  table.set_function("createRandom", [](const float &_x, const float &_y, const float &_z) {
    int handle_;
    Command<0x0376>(_x, _y, _z, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("setAnimSpeed", [](const int &_self, const char *_animName, const float &_animSpeed) {
    Command<0x0393>(_self, _animName, _animSpeed);
  });
  table.set_function("setCantBeDraggedOut",
                     [](const int &_self, const bool &_state) { Command<0x039E>(_self, _state); });
  table.set_function("isMale", [](const int &_self) {
    auto result = Command<0x03A3>(_self);
    return std::make_tuple(result);
  });
  table.set_function("storeCarIsInNoSave", [](const int &_self) {
    int handle_;
    Command<0x03C0>(_self, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("setMoney", [](const int &_self, const int &_amount) { Command<0x03FE>(_self, _amount); });
  table.set_function("getAmmoInWeapon", [](const int &_self, const int &_weaponType) {
    int ammo_;
    Command<0x041A>(_self, _weaponType, &ammo_);
    return std::make_tuple(ammo_);
  });
  table.set_function("warpIntoCarAsPassenger", [](const int &_self, const int &_handle, const int &_seatId) {
    Command<0x0430>(_self, _handle, _seatId);
  });
  table.set_function("setIsChrisCriminal",
                     [](const int &_self, const bool &_state) { Command<0x0433>(_self, _state); });
  table.set_function("setSuffersCriticalHits",
                     [](const int &_self, const bool &_state) { Command<0x0446>(_self, _state); });
  table.set_function("isSittingInCar", [](const int &_self, const int &_vehicle) {
    auto result = Command<0x0448>(_self, _vehicle);
    return std::make_tuple(result);
  });
  table.set_function("isSittingInAnyCar", [](const int &_self) {
    auto result = Command<0x0449>(_self);
    return std::make_tuple(result);
  });
  table.set_function("isOnFoot", [](const int &_self) {
    auto result = Command<0x044B>(_self);
    return std::make_tuple(result);
  });
  table.set_function("attachToCar", [](const int &_self, const int &_vehicle, const float &_xOffset,
                                       const float &_yOffset, const float &_zOffset, const int &_position,
                                       const float &_angleLimit, const int &_weaponType) {
    Command<0x0464>(_self, _vehicle, _xOffset, _yOffset, _zOffset, _position, _angleLimit, _weaponType);
  });
  table.set_function("detachFromCar", [](const int &_self) { Command<0x0465>(_self); });
  table.set_function("clearLastWeaponDamage", [](const int &_self) { Command<0x0467>(_self); });
  table.set_function("getNumberOfFollowers", [](const int &_self) {
    int number_;
    Command<0x046D>(_self, &number_);
    return std::make_tuple(number_);
  });
  table.set_function("getCurrentWeapon", [](const int &_self) {
    int weaponType_;
    Command<0x0470>(_self, &weaponType_);
    return std::make_tuple(weaponType_);
  });
  table.set_function("locateAnyMeansObject2D", [](const int &_self, const int &_object, const float &_xRadius,
                                                  const float &_yRadius, const bool &_drawSphere) {
    auto result = Command<0x0471>(_self, _object, _xRadius, _yRadius, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("locateOnFootObject2D", [](const int &_self, const int &_object, const float &_xRadius,
                                                const float &_yRadius, const bool &_drawSphere) {
    auto result = Command<0x0472>(_self, _object, _xRadius, _yRadius, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("locateInCarObject2D", [](const int &_self, const int &_object, const float &_xRadius,
                                               const float &_yRadius, const bool &_drawSphere) {
    auto result = Command<0x0473>(_self, _object, _xRadius, _yRadius, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("locateAnyMeansObject3D",
                     [](const int &_self, const int &_object, const float &_xRadius, const float &_yRadius,
                        const float &_zRadius, const bool &_drawSphere) {
                       auto result = Command<0x0474>(_self, _object, _xRadius, _yRadius, _zRadius, _drawSphere);
                       return std::make_tuple(result);
                     });
  table.set_function("locateOnFootObject3D", [](const int &_self, const int &_object, const float &_xRadius,
                                                const float &_yRadius, const float &_zRadius, const bool &_drawSphere) {
    auto result = Command<0x0475>(_self, _object, _xRadius, _yRadius, _zRadius, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("locateInCarObject3D", [](const int &_self, const int &_object, const float &_xRadius,
                                               const float &_yRadius, const float &_zRadius, const bool &_drawSphere) {
    auto result = Command<0x0476>(_self, _object, _xRadius, _yRadius, _zRadius, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("isOnAnyBike", [](const int &_self) {
    auto result = Command<0x047A>(_self);
    return std::make_tuple(result);
  });
  table.set_function("canSeeDeadChar", [](const int &_self, const int &_pedType) {
    auto result = Command<0x0480>(_self, _pedType);
    return std::make_tuple(result);
  });
  table.set_function("shutUp", [](const int &_self, const bool &_state) { Command<0x0489>(_self, _state); });
  table.set_function("removeAllWeapons", [](const int &_self) { Command<0x048F>(_self); });
  table.set_function("hasGotWeapon", [](const int &_self, const int &_weaponType) {
    auto result = Command<0x0491>(_self, _weaponType);
    return std::make_tuple(result);
  });
  table.set_function("isInAnyBoat", [](const int &_self) {
    auto result = Command<0x04A7>(_self);
    return std::make_tuple(result);
  });
  table.set_function("isInAnyHeli", [](const int &_self) {
    auto result = Command<0x04A9>(_self);
    return std::make_tuple(result);
  });
  table.set_function("isInAnyPlane", [](const int &_self) {
    auto result = Command<0x04AB>(_self);
    return std::make_tuple(result);
  });
  table.set_function("isInWater", [](const int &_self) {
    auto result = Command<0x04AD>(_self);
    return std::make_tuple(result);
  });
  table.set_function("getWeaponInSlot", [](const int &_self, const int &_weaponSlotId) {
    int weaponType_;
    int weaponAmmo_;
    int weaponModel_;
    Command<0x04B8>(_self, _weaponSlotId, &weaponType_, &weaponAmmo_, &weaponModel_);
    return std::make_tuple(weaponType_, weaponAmmo_, weaponModel_);
  });
  table.set_function("getOffsetInWorldCoords",
                     [](const int &_self, const float &_xOffset, const float &_yOffset, const float &_zOffset) {
                       float x_;
                       float y_;
                       float z_;
                       Command<0x04C4>(_self, _xOffset, _yOffset, _zOffset, &x_, &y_, &z_);
                       return std::make_tuple(x_, y_, z_);
                     });
  table.set_function("hasBeenPhotographed", [](const int &_self) {
    auto result = Command<0x04C5>(_self);
    return std::make_tuple(result);
  });
  table.set_function("isInFlyingVehicle", [](const int &_self) {
    auto result = Command<0x04C8>(_self);
    return std::make_tuple(result);
  });
  table.set_function("freezePosition", [](const int &_self, const bool &_state) { Command<0x04D7>(_self, _state); });
  table.set_function("setDrownsInWater", [](const int &_self, const bool &_state) { Command<0x04D8>(_self, _state); });
  table.set_function("getArmor", [](const int &_self) {
    int armor_;
    Command<0x04DD>(_self, &armor_);
    return std::make_tuple(armor_);
  });
  table.set_function("isWaitingForWorldCollision", [](const int &_self) {
    auto result = Command<0x04F0>(_self);
    return std::make_tuple(result);
  });
  table.set_function("attachToObject", [](const int &_self, const int &_handle, const float &_xOffset,
                                          const float &_yOffset, const float &_zOffset, const int &_orientation,
                                          const float &_angleLimit, const int &_weaponType) {
    Command<0x04F4>(_self, _handle, _xOffset, _yOffset, _zOffset, _orientation, _angleLimit, _weaponType);
  });
  table.set_function("createSwatRope",
                     [](const int &_pedType, const int &_modelId, const float &_x, const float &_y, const float &_z) {
                       int handle_;
                       Command<0x0503>(_pedType, _modelId, _x, _y, _z, &handle_);
                       return std::make_tuple(handle_);
                     });
  table.set_function("hasBeenDamagedByChar", [](const int &_self, const int &_handle) {
    auto result = Command<0x051A>(_self, _handle);
    return std::make_tuple(result);
  });
  table.set_function("hasBeenDamagedByCar", [](const int &_self, const int &_handle) {
    auto result = Command<0x051B>(_self, _handle);
    return std::make_tuple(result);
  });
  table.set_function("setStayInCarWhenJacked",
                     [](const int &_self, const bool &_state) { Command<0x0526>(_self, _state); });
  table.set_function("isTouchingVehicle", [](const int &_self, const int &_handle) {
    auto result = Command<0x0547>(_self, _handle);
    return std::make_tuple(result);
  });
  table.set_function("setCanBeShotInVehicle",
                     [](const int &_self, const bool &_state) { Command<0x054A>(_self, _state); });
  table.set_function("clearLastDamageEntity", [](const int &_self) { Command<0x054E>(_self); });
  table.set_function("removeWeapon",
                     [](const int &_self, const int &_weaponType) { Command<0x0555>(_self, _weaponType); });
  table.set_function("createRandomAsDriver", [](const int &_vehicle) {
    int handle_;
    Command<0x0560>(_vehicle, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("createRandomAsPassenger", [](const int &_vehicle, const int &_seatId) {
    int handle_;
    Command<0x0561>(_vehicle, _seatId, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("setNeverTargeted", [](const int &_self, const bool &_state) { Command<0x0568>(_self, _state); });
  table.set_function("isInAnyPoliceVehicle", [](const int &_self) {
    auto result = Command<0x056C>(_self);
    return std::make_tuple(result);
  });
  table.set_function("doesExist", [](const int &_handle) {
    auto result = Command<0x056D>(_handle);
    return std::make_tuple(result);
  });
  table.set_function("freezePositionAndDontLoadCollision",
                     [](const int &_self, const bool &_state) { Command<0x0575>(_self, _state); });
  table.set_function("setLoadCollisionFlag",
                     [](const int &_self, const bool &_state) { Command<0x0588>(_self, _state); });
  table.set_function("isDucking", [](const int &_self) {
    auto result = Command<0x0597>(_self);
    return std::make_tuple(result);
  });
  table.set_function("isInAngledArea2D", [](const int &_self, const float &_leftBottomX, const float &_leftBottomY,
                                            const float &_rightTopX, const float &_rightTopY, const float &_angle,
                                            const bool &_drawSphere) {
    auto result = Command<0x05F6>(_self, _leftBottomX, _leftBottomY, _rightTopX, _rightTopY, _angle, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("isInAngledAreaOnFoot2D",
                     [](const int &_self, const float &_leftBottomX, const float &_leftBottomY, const float &_rightTopX,
                        const float &_rightTopY, const float &_angle, const bool &_drawSphere) {
                       auto result = Command<0x05F7>(_self, _leftBottomX, _leftBottomY, _rightTopX, _rightTopY, _angle,
                                                     _drawSphere);
                       return std::make_tuple(result);
                     });
  table.set_function("isInAngledAreaInCar2D", [](const int &_self, const float &_leftBottomX, const float &_leftBottomY,
                                                 const float &_rightTopX, const float &_rightTopY, const float &_angle,
                                                 const bool &_drawSphere) {
    auto result = Command<0x05F8>(_self, _leftBottomX, _leftBottomY, _rightTopX, _rightTopY, _angle, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("isStoppedInAngledArea2D",
                     [](const int &_self, const float &_leftBottomX, const float &_leftBottomY, const float &_rightTopX,
                        const float &_rightTopY, const float &_angle, const bool &_drawSphere) {
                       auto result = Command<0x05F9>(_self, _leftBottomX, _leftBottomY, _rightTopX, _rightTopY, _angle,
                                                     _drawSphere);
                       return std::make_tuple(result);
                     });
  table.set_function("isStoppedInAngledAreaOnFoot2D",
                     [](const int &_self, const float &_leftBottomX, const float &_leftBottomY, const float &_rightTopX,
                        const float &_rightTopY, const float &_angle, const bool &_drawSphere) {
                       auto result = Command<0x05FA>(_self, _leftBottomX, _leftBottomY, _rightTopX, _rightTopY, _angle,
                                                     _drawSphere);
                       return std::make_tuple(result);
                     });
  table.set_function("isStoppedInAngledAreaInCar2D",
                     [](const int &_self, const float &_leftBottomX, const float &_leftBottomY, const float &_rightTopX,
                        const float &_rightTopY, const float &_angle, const bool &_drawSphere) {
                       auto result = Command<0x05FB>(_self, _leftBottomX, _leftBottomY, _rightTopX, _rightTopY, _angle,
                                                     _drawSphere);
                       return std::make_tuple(result);
                     });
  table.set_function("isInAngledArea3D", [](const int &_self, const float &_leftBottomX, const float &_leftBottomY,
                                            const float &_leftBottomZ, const float &_rightTopX, const float &_rightTopY,
                                            const float &_rightTopZ, const float &_angle, const bool &_drawSphere) {
    auto result = Command<0x05FC>(_self, _leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX, _rightTopY, _rightTopZ,
                                  _angle, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("isInAngledAreaOnFoot3D",
                     [](const int &_self, const float &_leftBottomX, const float &_leftBottomY,
                        const float &_leftBottomZ, const float &_rightTopX, const float &_rightTopY,
                        const float &_rightTopZ, const float &_angle, const bool &_drawSphere) {
                       auto result = Command<0x05FD>(_self, _leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX,
                                                     _rightTopY, _rightTopZ, _angle, _drawSphere);
                       return std::make_tuple(result);
                     });
  table.set_function("isInAngledAreaInCar3D",
                     [](const int &_self, const float &_leftBottomX, const float &_leftBottomY,
                        const float &_leftBottomZ, const float &_rightTopX, const float &_rightTopY,
                        const float &_rightTopZ, const float &_angle, const bool &_drawSphere) {
                       auto result = Command<0x05FE>(_self, _leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX,
                                                     _rightTopY, _rightTopZ, _angle, _drawSphere);
                       return std::make_tuple(result);
                     });
  table.set_function("isStoppedInAngledArea3D",
                     [](const int &_self, const float &_leftBottomX, const float &_leftBottomY,
                        const float &_leftBottomZ, const float &_rightTopX, const float &_rightTopY,
                        const float &_rightTopZ, const float &_angle, const bool &_drawSphere) {
                       auto result = Command<0x05FF>(_self, _leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX,
                                                     _rightTopY, _rightTopZ, _angle, _drawSphere);
                       return std::make_tuple(result);
                     });
  table.set_function("isStoppedInAngledAreaOnFoot3D",
                     [](const int &_self, const float &_leftBottomX, const float &_leftBottomY,
                        const float &_leftBottomZ, const float &_rightTopX, const float &_rightTopY,
                        const float &_rightTopZ, const float &_angle, const bool &_drawSphere) {
                       auto result = Command<0x0600>(_self, _leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX,
                                                     _rightTopY, _rightTopZ, _angle, _drawSphere);
                       return std::make_tuple(result);
                     });
  table.set_function("isStoppedInAngledAreaInCar3D",
                     [](const int &_self, const float &_leftBottomX, const float &_leftBottomY,
                        const float &_leftBottomZ, const float &_rightTopX, const float &_rightTopY,
                        const float &_rightTopZ, const float &_angle, const bool &_drawSphere) {
                       auto result = Command<0x0601>(_self, _leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX,
                                                     _rightTopY, _rightTopZ, _angle, _drawSphere);
                       return std::make_tuple(result);
                     });
  table.set_function("isInTaxi", [](const int &_self) {
    auto result = Command<0x0602>(_self);
    return std::make_tuple(result);
  });
  table.set_function("setDecisionMaker",
                     [](const int &_self, const int &_maskOrHandle) { Command<0x060B>(_self, _maskOrHandle); });
  table.set_function("setSenseRange",
                     [](const int &_self, const float &_meleeAccuracy) { Command<0x060F>(_self, _meleeAccuracy); });
  table.set_function("isPlayingAnim", [](const int &_self, const char *_animationName) {
    auto result = Command<0x0611>(_self, _animationName);
    return std::make_tuple(result);
  });
  table.set_function("setAnimPlayingFlag", [](const int &_self, const char *_animationName, const bool &_flag) {
    Command<0x0612>(_self, _animationName, _flag);
  });
  table.set_function("getAnimCurrentTime", [](const int &_self, const char *_animationName) {
    float time_;
    Command<0x0613>(_self, _animationName, &time_);
    return std::make_tuple(time_);
  });
  table.set_function("setAnimCurrentTime", [](const int &_self, const char *_animationName, const float &_time) {
    Command<0x0614>(_self, _animationName, _time);
  });
  table.set_function("performSequence",
                     [](const int &_self, const int &_sequence) { Command<0x0618>(_self, _sequence); });
  table.set_function("setCollision", [](const int &_self, const bool &_state) { Command<0x0619>(_self, _state); });
  table.set_function("getAnimTotalTime", [](const int &_self, const char *_animationName) {
    float totalTime_;
    Command<0x061A>(_self, _animationName, &totalTime_);
    return std::make_tuple(totalTime_);
  });
  table.set_function("createAtAttractor",
                     [](const int &_pedType, const int &_modelId, const int &_taskId, const int &_attractor) {
                       int handle_;
                       Command<0x0621>(_pedType, _modelId, _taskId, _attractor, &handle_);
                       return std::make_tuple(handle_);
                     });
  table.set_function("getScriptTaskStatus", [](const int &_self, const int &_taskId) {
    int status_;
    Command<0x062E>(_self, _taskId, &status_);
    return std::make_tuple(status_);
  });
  table.set_function("isAtScriptedAttractor", [](const int &_self, const int &_handle) {
    auto result = Command<0x0642>(_self, _handle);
    return std::make_tuple(result);
  });
  table.set_function("getSequenceProgress", [](const int &_self) {
    int progress_;
    Command<0x0646>(_self, &progress_);
    return std::make_tuple(progress_);
  });
  table.set_function("clearLookAt", [](const int &_self) { Command<0x0647>(_self); });
  table.set_function("setFollowNodeThresholdDistance",
                     [](const int &_self, const float &_range) { Command<0x0648>(_self, _range); });
  table.set_function("getModel", [](const int &_self) {
    int modelId_;
    Command<0x0665>(_self, &modelId_);
    return std::make_tuple(modelId_);
  });
  table.set_function("clearTasks", [](const int &_self) { Command<0x0687>(_self); });
  table.set_function("attachToBike", [](const int &_self, const int &_vehicle, const float &_xOffset,
                                        const float &_yOffset, const float &_zOffset, const int &_position,
                                        const float &__p7, const float &__p8, const int &_weaponType) {
    Command<0x06A7>(_self, _vehicle, _xOffset, _yOffset, _zOffset, _position, __p7, __p8, _weaponType);
  });
  table.set_function("hideWeaponForScriptedCutscene",
                     [](const int &_self, const bool &_state) { Command<0x06AB>(_self, _state); });
  table.set_function("getSpeed", [](const int &_self) {
    float speed_;
    Command<0x06AC>(_self, &speed_);
    return std::make_tuple(speed_);
  });
  table.set_function("removeFromGroup", [](const int &_self) { Command<0x06C9>(_self); });
  table.set_function("isGroupMember", [](const int &_self, const int &_handle) {
    auto result = Command<0x06EE>(_self, _handle);
    return std::make_tuple(result);
  });
  table.set_function("isGroupLeader", [](const int &_self, const int &_handle) {
    auto result = Command<0x06EF>(_self, _handle);
    return std::make_tuple(result);
  });
  table.set_function("isNearAnyChars", [](const int &_self, const float &_radius) {
    auto result = Command<0x06FF>(_self, _radius);
    return std::make_tuple(result);
  });
  table.set_function("dropObject", [](const int &_self, const bool &_state) { Command<0x070B>(_self, _state); });
  table.set_function("isHoldingObject", [](const int &_self, const int &_handle) {
    auto result = Command<0x0737>(_self, _handle);
    return std::make_tuple(result);
  });
  table.set_function("hasBeenArrested", [](const int &_self) {
    auto result = Command<0x0741>(_self);
    return std::make_tuple(result);
  });
  table.set_function("setInformRespectedFriends", [](const int &_self, const float &_radius, const int &__p3) {
    Command<0x074E>(_self, _radius, __p3);
  });
  table.set_function("isRespondingToEvent", [](const int &_self, const int &_eventId) {
    auto result = Command<0x074F>(_self, _eventId);
    return std::make_tuple(result);
  });
  table.set_function("setIsTargetPriority",
                     [](const int &_self, const bool &_state) { Command<0x0770>(_self, _state); });
  table.set_function("setRelationship", [](const int &_self, const int &_relationshipType, const int &_pedType) {
    Command<0x077A>(_self, _relationshipType, _pedType);
  });
  table.set_function("clearRelationship", [](const int &_self, const int &_relationshipType, const int &_toPedType) {
    Command<0x077B>(_self, _relationshipType, _toPedType);
  });
  table.set_function("clearAllRelationships",
                     [](const int &_self, const int &_relationshipType) { Command<0x077C>(_self, _relationshipType); });
  table.set_function("clearTasksImmediately", [](const int &_self) { Command<0x0792>(_self); });
  table.set_function("performSequenceFromProgress",
                     [](const int &_self, const int &_sequence, const int &__p3, const int &__p4) {
                       Command<0x07A0>(_self, _sequence, __p3, __p4);
                     });
  table.set_function("setNextDesiredMoveState", [](const int &_moveState) { Command<0x07A1>(_moveState); });
  table.set_function("getSequenceProgressRecursive", [](const int &_self) {
    int _p2_;
    int _p3_;
    Command<0x07A4>(_self, &_p2_, &_p3_);
    return std::make_tuple(_p2_, _p3_);
  });
  table.set_function("isInAnySearchlight", [](const int &_self) {
    int handle_;
    auto result = Command<0x07A9>(_self, &handle_);
    return std::make_tuple(result, handle_);
  });
  table.set_function("listenToPlayerGroupCommands",
                     [](const int &_self, const bool &_state) { Command<0x07CB>(_self, _state); });
  table.set_function("setShootRate", [](const int &_self, const int &_rate) { Command<0x07DD>(_self, _rate); });
  table.set_function("giveMeleeAttack", [](const int &_self, const int &_fightStyle, const int &_moveId) {
    Command<0x07FE>(_self, _fightStyle, _moveId);
  });
  table.set_function("getHighestPriorityEvent", [](const int &_self) {
    int eventId_;
    Command<0x080E>(_self, &eventId_);
    return std::make_tuple(eventId_);
  });
  table.set_function("getCarIsUsing", [](const int &_self) {
    int handle_;
    Command<0x0811>(_self, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("setKindaStayInSamePlace",
                     [](const int &_self, const bool &_state) { Command<0x0816>(_self, _state); });
  table.set_function("isInAir", [](const int &_self) {
    auto result = Command<0x0818>(_self);
    return std::make_tuple(result);
  });
  table.set_function("getHeightAboveGround", [](const int &_self) {
    float height_;
    Command<0x0819>(_self, &height_);
    return std::make_tuple(height_);
  });
  table.set_function("setWeaponSkill", [](const int &_self, const int &_skillId) { Command<0x081A>(_self, _skillId); });
  table.set_function("setVelocity", [](const int &_self, const float &_x, const float &_y, const float &_z) {
    Command<0x083C>(_self, _x, _y, _z);
  });
  table.set_function("getVelocity", [](const int &_self) {
    float x_;
    float y_;
    float z_;
    Command<0x083D>(_self, &x_, &y_, &z_);
    return std::make_tuple(x_, y_, z_);
  });
  table.set_function("setRotation", [](const int &_self, const float &_x, const float &_y, const float &_z) {
    Command<0x083E>(_self, _x, _y, _z);
  });
  table.set_function("damage", [](const int &_self, const int &_amount, const bool &_damageArmour) {
    Command<0x0851>(_self, _amount, _damageArmour);
  });
  table.set_function("setAllowedToDuck", [](const int &_self, const bool &_state) { Command<0x0856>(_self, _state); });
  table.set_function("setAreaVisible",
                     [](const int &_self, const int &_interiorId) { Command<0x0860>(_self, _interiorId); });
  table.set_function("setDropsWeaponsWhenDead",
                     [](const int &_self, const bool &_state) { Command<0x087E>(_self, _state); });
  table.set_function("setNeverLeavesGroup",
                     [](const int &_self, const bool &_state) { Command<0x087F>(_self, _state); });
  table.set_function("setHeadingLimitForAttached",
                     [](const int &_self, const int &_orientation, const float &_headingLimit) {
                       Command<0x0887>(_self, _orientation, _headingLimit);
                     });
  table.set_function("getCoordinatesOfDied", [](const int &_self) {
    float x_;
    float y_;
    float z_;
    Command<0x0889>(_self, &x_, &y_, &z_);
    return std::make_tuple(x_, y_, z_);
  });
  table.set_function("getPedType", [](const int &_self) {
    int pedType_;
    Command<0x089F>(_self, &pedType_);
    return std::make_tuple(pedType_);
  });
  table.set_function("setHasUsedEntryExit", [](const int &_self, const float &_x, const float &_y,
                                               const float &_radius) { Command<0x08AD>(_self, _x, _y, _radius); });
  table.set_function("setMaxHealth",
                     [](const int &_self, const int &_maxHealth) { Command<0x08AF>(_self, _maxHealth); });
  table.set_function("setCanBeKnockedOffBike",
                     [](const int &_self, const bool &_stayOnBike) { Command<0x08C6>(_self, _stayOnBike); });
  table.set_function("setCoordinatesDontWarpGang", [](const int &_self, const float &_x, const float &_y,
                                                      const float &_z) { Command<0x08C7>(_self, _x, _y, _z); });
  table.set_function("setBulletproofVest",
                     [](const int &_self, const bool &_state) { Command<0x093B>(_self, _state); });
  table.set_function("setUsesUpperbodyDamageAnimsOnly",
                     [](const int &_self, const bool &_state) { Command<0x0946>(_self, _state); });
  table.set_function("setSayContext", [](const int &_self, const int &_speech) {
    int _p3_;
    Command<0x0947>(_self, _speech, &_p3_);
    return std::make_tuple(_p3_);
  });
  table.set_function("getNameOfEntryExitUsed", [](const int &_self) {
    const char *interiorName_;
    Command<0x094B>(_self, (int *)&interiorName_);
    return std::make_tuple(interiorName_);
  });
  table.set_function("getPositionOfEntryExitCharUsed", [](const int &_self) {
    float x_;
    float y_;
    float z_;
    float heading_;
    Command<0x094C>(_self, &x_, &y_, &z_, &heading_);
    return std::make_tuple(x_, y_, z_, heading_);
  });
  table.set_function("isTalking", [](const int &_self) {
    auto result = Command<0x094D>(_self);
    return std::make_tuple(result);
  });
  table.set_function("disableSpeech", [](const int &_self, const bool &_stopNow) { Command<0x094E>(_self, _stopNow); });
  table.set_function("enableSpeech", [](const int &_self) { Command<0x094F>(_self); });
  table.set_function("isStuckUnderCar", [](const int &_self) {
    auto result = Command<0x095D>(_self);
    return std::make_tuple(result);
  });
  table.set_function("setKeepTask", [](const int &_self, const bool &_state) { Command<0x0961>(_self, _state); });
  table.set_function("isSwimming", [](const int &_self) {
    auto result = Command<0x0965>(_self);
    return std::make_tuple(result);
  });
  table.set_function("getSwimState", [](const int &_self) {
    int state_;
    Command<0x0966>(_self, &state_);
    return std::make_tuple(state_);
  });
  table.set_function("startFacialTalk",
                     [](const int &_self, const int &_duration) { Command<0x0967>(_self, _duration); });
  table.set_function("stopFacialTalk", [](const int &_self) { Command<0x0968>(_self); });
  table.set_function("setCoordinatesNoOffset", [](const int &_self, const float &_x, const float &_y, const float &_z) {
    Command<0x0972>(_self, _x, _y, _z);
  });
  table.set_function("setForceDieInCar", [](const int &_self, const bool &_state) { Command<0x0982>(_self, _state); });
  table.set_function("dropSecondObject", [](const int &_self, const bool &_state) { Command<0x09A1>(_self, _state); });
  table.set_function("setDruggedUp", [](const int &_self, const bool &_state) { Command<0x09A7>(_self, _state); });
  table.set_function("isHeadMissing", [](const int &_self) {
    auto result = Command<0x09A8>(_self);
    return std::make_tuple(result);
  });
  table.set_function("isInAnyTrain", [](const int &_self) {
    auto result = Command<0x09AE>(_self);
    return std::make_tuple(result);
  });
  table.set_function("setSignalAfterKill",
                     [](const int &_self, const bool &_state) { Command<0x09B5>(_self, _state); });
  table.set_function("setWantedByPolice", [](const int &_self, const bool &_state) { Command<0x09B6>(_self, _state); });
  table.set_function("setCoordinatesDontWarpGangNoOffset", [](const int &_self, const float &_x, const float &_y,
                                                              const float &_z) { Command<0x09BC>(_self, _x, _y, _z); });
  table.set_function("isUsingMapAttractor", [](const int &_self) {
    auto result = Command<0x09C5>(_self);
    return std::make_tuple(result);
  });
  table.set_function("removeFromCarMaintainPosition",
                     [](const int &_self, const int &_handle) { Command<0x09C9>(_self, _handle); });
  table.set_function("setSayContextImportant",
                     [](const int &_self, const int &__p2, const bool &__p3, const bool &__p4, const bool &__p5) {
                       int _p6_;
                       Command<0x09D5>(_self, __p2, __p3, __p4, __p5, &_p6_);
                       return std::make_tuple(_p6_);
                     });
  table.set_function("setSayScript", [](const int &_self, const int &__p2, const bool &__p3, const bool &__p4,
                                        const bool &__p5) { Command<0x09D6>(_self, __p2, __p3, __p4, __p5); });
  table.set_function("isGettingInToACar", [](const int &_self) {
    auto result = Command<0x09DE>(_self);
    return std::make_tuple(result);
  });
  table.set_function("getAreaVisible", [](const int &_self) {
    int interiorId_;
    Command<0x09E8>(_self, &interiorId_);
    return std::make_tuple(interiorId_);
  });
  table.set_function("hasSpottedCharInFront", [](const int &_self, const int &_handle) {
    auto result = Command<0x09ED>(_self, _handle);
    return std::make_tuple(result);
  });
  table.set_function("ignoreHeightDifferenceFollowingNodes",
                     [](const int &_self, const bool &_state) { Command<0x09F4>(_self, _state); });
  table.set_function("setGetOutUpsideDownCar",
                     [](const int &_self, const bool &_state) { Command<0x09F6>(_self, _state); });
  table.set_function("shutUpForScriptedSpeech",
                     [](const int &_self, const bool &_state) { Command<0x0A09>(_self, _state); });
  table.set_function("isTouchingChar", [](const int &_self, const int &_handle) {
    auto result = Command<0x0A1B>(_self, _handle);
    return std::make_tuple(result);
  });
  table.set_function("setDeathWeaponsPersist",
                     [](const int &_self, const bool &_state) { Command<0x0A27>(_self, _state); });
  table.set_function("setSwimSpeed", [](const int &_self, const float &_speed) { Command<0x0A28>(_self, _speed); });
  table.set_function("isAttachedToAnyCar", [](const int &_self) {
    auto result = Command<0x0A32>(_self);
    return std::make_tuple(result);
  });
  table.set_function("storeCarIsAttachedToNoSave", [](const int &_self) {
    int handle_;
    Command<0x0A33>(_self, &handle_);
    return std::make_tuple(handle_);
  });

  // CLEO
  table.set_function("storeClosestEntities", [](const int &_self) {
    int carHandle_;
    int charHandle_;
    Command<0x0AB5>(_self, &carHandle_, &charHandle_);
    return std::make_tuple(carHandle_, charHandle_);
  });

  // CLEO+
  table.set_function("getBoneMatrix", [](const int &_self, const int &_pedBone) {
    int matrix_;
    auto result = Command<0x0D0B>(_self, _pedBone, &matrix_);
    return std::make_tuple(result, matrix_);
  });
  table.set_function("setModelAlpha", [](const int &_self, const int &_alpha) { Command<0x0D10>(_self, _alpha); });
  table.set_function("getBone", [](const int &_self, const int &_pedBone) {
    int address_;
    auto result = Command<0x0D30>(_self, _pedBone, &address_);
    return std::make_tuple(result, address_);
  });
  table.set_function("getBoneOffsetVector", [](const int &_pedBone) {
    int offsetVector_;
    Command<0x0D31>(_pedBone, &offsetVector_);
    return std::make_tuple(offsetVector_);
  });
  table.set_function("getBoneQuat", [](const int &_pedBone) {
    int quat_;
    Command<0x0D32>(_pedBone, &quat_);
    return std::make_tuple(quat_);
  });
  table.set_function("getMaxHealth", [](const int &_self) {
    float maxHealth_;
    Command<0x0D39>(_self, &maxHealth_);
    return std::make_tuple(maxHealth_);
  });
  table.set_function("isScriptControlled", [](const int &_self) {
    auto result = Command<0x0E0A>(_self);
    return std::make_tuple(result);
  });
  table.set_function("markAsNeeded", [](const int &_self) { Command<0x0E0B>(_self); });
  table.set_function("initExtendedVars", [](const int &_self, const char *_identifier, const int &_totalVars) {
    auto result = Command<0x0E14>(_self, _identifier, _totalVars);
    return std::make_tuple(result);
  });
  table.set_function("setExtendedVar",
                     [](const int &_self, const char *_identifier, const int &_varNumber, const int &_value) {
                       auto result = Command<0x0E15>(_self, _identifier, _varNumber, _value);
                       return std::make_tuple(result);
                     });
  table.set_function("getExtendedVar", [](const int &_self, const char *_identifier, const int &_varNumber) {
    int value_;
    auto result = Command<0x0E16>(_self, _identifier, _varNumber, &value_);
    return std::make_tuple(result, value_);
  });
  table.set_function("fixGroundBrightnessAndFadeIn",
                     [](const int &_self, const bool &_fixGround, const bool &_fixBrightness, const bool &_fadeIn) {
                       Command<0x0E24>(_self, _fixGround, _fixBrightness, _fadeIn);
                     });
  table.set_function("setRenderObjectAutoHide", [](const int &_self, const bool &_dead, const bool &_weapon,
                                                   const bool &_car) { Command<0x0E30>(_self, _dead, _weapon, _car); });
  table.set_function("setCoordinatesSimple", [](const int &_self, const float &_x, const float &_y, const float &_z) {
    Command<0x0E32>(_self, _x, _y, _z);
  });
  table.set_function("isDoingTaskId", [](const int &_self, const int &_taskId) {
    auto result = Command<0x0E42>(_self, _taskId);
    return std::make_tuple(result);
  });
  table.set_function("getTaskPointerById", [](const int &_self, const int &_taskId) {
    int address_;
    auto result = Command<0x0E43>(_self, _taskId, &address_);
    return std::make_tuple(result, address_);
  });
  table.set_function("getKillTargetChar", [](const int &_self) {
    int killTarget_;
    auto result = Command<0x0E44>(_self, &killTarget_);
    return std::make_tuple(result, killTarget_);
  });
  table.set_function("isUsingGun", [](const int &_self) {
    auto result = Command<0x0E46>(_self);
    return std::make_tuple(result);
  });
  table.set_function("isFighting", [](const int &_self) {
    auto result = Command<0x0E47>(_self);
    return std::make_tuple(result);
  });
  table.set_function("isFallenOnGround", [](const int &_self) {
    auto result = Command<0x0E48>(_self);
    return std::make_tuple(result);
  });
  table.set_function("isEnteringAnyCar", [](const int &_self) {
    auto result = Command<0x0E49>(_self);
    return std::make_tuple(result);
  });
  table.set_function("isExitingAnyCar", [](const int &_self) {
    auto result = Command<0x0E4A>(_self);
    return std::make_tuple(result);
  });
  table.set_function("isPlayingAnyScriptAnimation", [](const int &_self) {
    auto result = Command<0x0E4B>(_self);
    return std::make_tuple(result);
  });
  table.set_function("isDoingAnyImportantTask", [](const int &_self) {
    auto result = Command<0x0E4C>(_self);
    return std::make_tuple(result);
  });
  table.set_function("getHealthPercent", [](const int &_self) {
    float healthPercent_;
    Command<0x0E5C>(_self, &healthPercent_);
    return std::make_tuple(healthPercent_);
  });
  table.set_function("getCurrentWeaponinfo", [](const int &_self) {
    int weaponInfo_;
    auto result = Command<0x0E83>(_self, &weaponInfo_);
    return std::make_tuple(result, weaponInfo_);
  });
  table.set_function("getWeaponState", [](const int &_self) {
    int weaponState_;
    Command<0x0E8B>(_self, &weaponState_);
    return std::make_tuple(weaponState_);
  });
  table.set_function("getCharWeaponClip", [](const int &_self) {
    int weaponClip_;
    Command<0x0E8C>(_self, &weaponClip_);
    return std::make_tuple(weaponClip_);
  });
  table.set_function("getCollisionSurface", [](const int &_self) {
    int surfaceType_;
    Command<0x0E8E>(_self, &surfaceType_);
    return std::make_tuple(surfaceType_);
  });
  table.set_function("getCollisionLighting", [](const int &_self) {
    float lighting_;
    Command<0x0E8F>(_self, &lighting_);
    return std::make_tuple(lighting_);
  });
  table.set_function("isReallyInAir", [](const int &_self) {
    auto result = Command<0x0E92>(_self);
    return std::make_tuple(result);
  });
  table.set_function("clearPrimaryTasks", [](const int &_self) { Command<0x0E96>(_self); });
  table.set_function("clearSecondaryTasks", [](const int &_self) { Command<0x0E97>(_self); });
  table.set_function("setSecondPlayer", [](const int &_self, const bool &_enableCamera, const bool &_separateCars) {
    Command<0x0EA0>(_self, _enableCamera, _separateCars);
  });
  table.set_function("isOnFire", [](const int &_self) {
    auto result = Command<0x0EA4>(_self);
    return std::make_tuple(result);
  });
  table.set_function("getClosestCop", [](const int &_self, const float &_radius, const bool &_alive, const bool &_inCar,
                                         const bool &_onFoot, const bool &_seenInFront) {
    int closestCop_;
    auto result = Command<0x0EA5>(_self, _radius, _alive, _inCar, _onFoot, _seenInFront, &closestCop_);
    return std::make_tuple(result, closestCop_);
  });
  table.set_function("setArrested", [](const int &_self) { Command<0x0EAA>(_self); });
  table.set_function("getPedState", [](const int &_self) {
    int pedState_;
    Command<0x0EAB>(_self, &pedState_);
    return std::make_tuple(pedState_);
  });
  table.set_function("getProofs", [](const int &_self) {
    bool bullet_;
    bool fire_;
    bool explosion_;
    bool collision_;
    bool melee_;
    Command<0x0EAC>(_self, (int *)&bullet_, (int *)&fire_, (int *)&explosion_, (int *)&collision_, (int *)&melee_);
    return std::make_tuple(bullet_, fire_, explosion_, collision_, melee_);
  });
  table.set_function("isWeaponVisibleSet", [](const int &_self) {
    auto result = Command<0x0EAF>(_self);
    return std::make_tuple(result);
  });
  table.set_function("getStatId", [](const int &_self) {
    int pedStat_;
    Command<0x0EB1>(_self, &pedStat_);
    return std::make_tuple(pedStat_);
  });
  table.set_function("getDamageLastFrame", [](const int &_self) {
    int entity_;
    int weaponType_;
    int bodyPart_;
    float intensity_;
    auto result = Command<0x0EB5>(_self, &entity_, &weaponType_, &bodyPart_, &intensity_);
    return std::make_tuple(result, entity_, weaponType_, bodyPart_, intensity_);
  });
  table.set_function("getRandomSeed", [](const int &_self) {
    int randomSeed_;
    Command<0x0EC8>(_self, &randomSeed_);
    return std::make_tuple(randomSeed_);
  });
  table.set_function("getMoveState", [](const int &_self) {
    int moveState_;
    Command<0x0ECB>(_self, &moveState_);
    return std::make_tuple(moveState_);
  });
  table.set_function("dontDeleteUntilTime",
                     [](const int &_self, const int &_msFromNow) { Command<0x0ECC>(_self, _msFromNow); });
  table.set_function("getTimeIsDead", [](const int &_self) {
    int timeIsDead_;
    Command<0x0ECE>(_self, &timeIsDead_);
    return std::make_tuple(timeIsDead_);
  });
  table.set_function("setIgnoreDamageAnims",
                     [](const int &_self, const bool &_bool) { Command<0x0ED9>(_self, _bool); });
  table.set_function("locateDistanceToChar", [](const int &_self, const int &_character, const float &_radius) {
    auto result = Command<0x0EE4>(_self, _character, _radius);
    return std::make_tuple(result);
  });
  table.set_function("locateDistanceToCar", [](const int &_self, const int &_car, const float &_radius) {
    auto result = Command<0x0EE5>(_self, _car, _radius);
    return std::make_tuple(result);
  });
  table.set_function("locateDistanceToObject", [](const int &_self, const int &_object, const float &_radius) {
    auto result = Command<0x0EE6>(_self, _object, _radius);
    return std::make_tuple(result);
  });
  table.set_function("locateDistanceToCoordinates",
                     [](const int &_self, const float &_x, const float &_y, const float &_z, const float &_radius) {
                       auto result = Command<0x0EEA>(_self, _x, _y, _z, _radius);
                       return std::make_tuple(result);
                     });
  table.set_function("getFear", [](const int &_self) {
    int fear_;
    Command<0x0EFA>(_self, &fear_);
    return std::make_tuple(fear_);
  });
  table.set_function("getSimplestActiveTask", [](const int &_self) {
    int taskId_;
    int address_;
    auto result = Command<0x0EFF>(_self, &taskId_, &address_);
    return std::make_tuple(result, taskId_, address_);
  });
  table.set_function("createRenderObjectToCharBoneFromSpecial",
                     [](const int &_self, const int &_specialModel, const int &_pedBone, const float &_x,
                        const float &_y, const float &_z, const float &_rx, const float &_ry, const float &_rz) {
                       int renderobject_;
                       Command<0x0F02>(_self, _specialModel, _pedBone, _x, _y, _z, _rx, _ry, _rz, &renderobject_);
                       return std::make_tuple(renderobject_);
                     });
}