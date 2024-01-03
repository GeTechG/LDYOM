// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindObject(sol::state &state) {
  auto table = state.create_table("ObjectOp");
  // default
  table.set_function("create", [](const int &_modelId, const float &_x, const float &_y, const float &_z) {
    int handle_;
    Command<0x0107>(_modelId, _x, _y, _z, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("delete", [](const int &_self) { Command<0x0108>(_self); });
  table.set_function("getHeading", [](const int &_self) {
    float heading_;
    Command<0x0176>(_self, &heading_);
    return std::make_tuple(heading_);
  });
  table.set_function("setHeading", [](const int &_self, const float &_heading) { Command<0x0177>(_self, _heading); });
  table.set_function("getCoordinates", [](const int &_self) {
    float x_;
    float y_;
    float z_;
    Command<0x01BB>(_self, &x_, &y_, &z_);
    return std::make_tuple(x_, y_, z_);
  });
  table.set_function("setCoordinates", [](const int &_self, const float &_x, const float &_y, const float &_z) {
    Command<0x01BC>(_self, _x, _y, _z);
  });
  table.set_function("markAsNoLongerNeeded", [](const int &_self) { Command<0x01C4>(_self); });
  table.set_function("dontRemove", [](const int &_self) { Command<0x01C7>(_self); });
  table.set_function("createNoOffset", [](const int &_modelId, const float &_x, const float &_y, const float &_z) {
    int handle_;
    Command<0x029B>(_modelId, _x, _y, _z, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("isOnScreen", [](const int &_self) {
    auto result = Command<0x02CC>(_self);
    return std::make_tuple(result);
  });
  table.set_function("rotate",
                     [](const int &_self, const float &_fromAngle, const float &_toAngle, const int &_collisionCheck) {
                       auto result = Command<0x034D>(_self, _fromAngle, _toAngle, _collisionCheck);
                       return std::make_tuple(result);
                     });
  table.set_function("slide", [](const int &_self, const float &_fromX, const float &_fromY, const float &_fromZ,
                                 const float &_xSpeed, const float &_ySpeed, const float &_zSpeed,
                                 const int &_collisionCheck) {
    auto result = Command<0x034E>(_self, _fromX, _fromY, _fromZ, _xSpeed, _ySpeed, _zSpeed, _collisionCheck);
    return std::make_tuple(result);
  });
  table.set_function("placeRelativeToCar",
                     [](const int &_self, const int &_vehicle, const float &_xOffset, const float &_yOffset,
                        const float &_zOffset) { Command<0x035C>(_self, _vehicle, _xOffset, _yOffset, _zOffset); });
  table.set_function("makeTargetable", [](const int &_self, const int &_state) { Command<0x035D>(_self, _state); });
  table.set_function("hasBeenDamaged", [](const int &_self) {
    auto result = Command<0x0366>(_self);
    return std::make_tuple(result);
  });
  table.set_function("setVelocity", [](const int &_self, const float &_xSpeed, const float &_ySpeed,
                                       const float &_zSpeed) { Command<0x0381>(_self, _xSpeed, _ySpeed, _zSpeed); });
  table.set_function("setCollision", [](const int &_self, const int &_state) { Command<0x0382>(_self, _state); });
  table.set_function("addToVelocity", [](const int &_self, const float &_x, const float &_y, const float &_z) {
    Command<0x038C>(_self, _x, _y, _z);
  });
  table.set_function("setDynamic", [](const int &_self, const int &_state) { Command<0x0392>(_self, _state); });
  table.set_function("doesExist", [](const int &_handle) {
    auto result = Command<0x03CA>(_handle);
    return std::make_tuple(result);
  });
  table.set_function("getOffsetInWorldCoords",
                     [](const int &_self, const float &_xOffset, const float &_yOffset, const float &_zOffset) {
                       float x_;
                       float y_;
                       float z_;
                       Command<0x0400>(_self, _xOffset, _yOffset, _zOffset, &x_, &y_, &z_);
                       return std::make_tuple(x_, y_, z_);
                     });
  table.set_function("setDrawLast", [](const int &_self, const int &_state) { Command<0x0418>(_self, _state); });
  table.set_function("setRotation", [](const int &_self, const float &_x, const float &_y, const float &_z) {
    Command<0x0453>(_self, _x, _y, _z);
  });
  table.set_function("setRecordsCollisions",
                     [](const int &_self, const int &_state) { Command<0x04D9>(_self, _state); });
  table.set_function("hasCollidedWithAnything", [](const int &_self) {
    auto result = Command<0x04DA>(_self);
    return std::make_tuple(result);
  });
  table.set_function("locate2D", [](const int &_self, const float &_x, const float &_y, const float &_xRadius,
                                    const float &_yRadius, const int &_drawSphere) {
    auto result = Command<0x04E5>(_self, _x, _y, _xRadius, _yRadius, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("locate3D",
                     [](const int &_self, const float &_x, const float &_y, const float &_z, const float &_xRadius,
                        const float &_yRadius, const float &_zRadius, const int &_drawSphere) {
                       auto result = Command<0x04E6>(_self, _x, _y, _z, _xRadius, _yRadius, _zRadius, _drawSphere);
                       return std::make_tuple(result);
                     });
  table.set_function("isInWater", [](const int &_self) {
    auto result = Command<0x04E7>(_self);
    return std::make_tuple(result);
  });
  table.set_function("isInArea2D", [](const int &_self, const float &_leftBottomX, const float &_leftBottomY,
                                      const float &_rightTopX, const float &_rightTopY, const int &_drawSphere) {
    auto result = Command<0x04E9>(_self, _leftBottomX, _leftBottomY, _rightTopX, _rightTopY, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("isInArea3D", [](const int &_self, const float &_leftBottomX, const float &_leftBottomY,
                                      const float &_leftBottomZ, const float &_rightTopX, const float &_rightTopY,
                                      const float &_rightTopZ, const int &_drawSphere) {
    auto result = Command<0x04EA>(_self, _leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX, _rightTopY, _rightTopZ,
                                  _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("sortOutCollisionWithCar",
                     [](const int &_self, const int &_handle) { Command<0x050E>(_self, _handle); });
  table.set_function("freezePosition", [](const int &_self, const int &_state) { Command<0x0550>(_self, _state); });
  table.set_function("setAreaVisible", [](const int &_self, const int &_areaId) { Command<0x0566>(_self, _areaId); });
  table.set_function("getVelocity", [](const int &_self) {
    float x_;
    float y_;
    float z_;
    Command<0x059F>(_self, &x_, &y_, &z_);
    return std::make_tuple(x_, y_, z_);
  });
  table.set_function("addToRotationVelocity", [](const int &_self, const float &_x, const float &_y, const float &_z) {
    Command<0x05A1>(_self, _x, _y, _z);
  });
  table.set_function("setRotationVelocity", [](const int &_self, const float &_x, const float &_y, const float &_z) {
    Command<0x05A2>(_self, _x, _y, _z);
  });
  table.set_function("isStatic", [](const int &_self) {
    auto result = Command<0x05A3>(_self);
    return std::make_tuple(result);
  });
  table.set_function("getRotationVelocity", [](const int &_self) {
    float x_;
    float y_;
    float z_;
    Command<0x05A6>(_self, &x_, &y_, &z_);
    return std::make_tuple(x_, y_, z_);
  });
  table.set_function("addVelocityRelative", [](const int &_self, const float &_x, const float &_y, const float &_z) {
    Command<0x05A7>(_self, _x, _y, _z);
  });
  table.set_function("getSpeed", [](const int &_self) {
    float speed_;
    Command<0x05A8>(_self, &speed_);
    return std::make_tuple(speed_);
  });
  table.set_function("setRenderScorched", [](const int &_self, const int &_state) { Command<0x0654>(_self, _state); });
  table.set_function("attachToCar", [](const int &_self, const int &_handle, const float &_xOffset,
                                       const float &_yOffset, const float &_zOffset, const float &_xRotation,
                                       const float &_yRotation, const float &_zRotation) {
    Command<0x0681>(_self, _handle, _xOffset, _yOffset, _zOffset, _xRotation, _yRotation, _zRotation);
  });
  table.set_function("detach",
                     [](const int &_self, const float &_x, const float &_y, const float &_z,
                        const int &_collisionDetection) { Command<0x0682>(_self, _x, _y, _z, _collisionDetection); });
  table.set_function("isAttached", [](const int &_self) {
    auto result = Command<0x0685>(_self);
    return std::make_tuple(result);
  });
  table.set_function("attachToObject", [](const int &_self, const int &_handle, const float &_xOffset,
                                          const float &_yOffset, const float &_zOffset, const float &_xRotation,
                                          const float &_yRotation, const float &_zRotation) {
    Command<0x069A>(_self, _handle, _xOffset, _yOffset, _zOffset, _xRotation, _yRotation, _zRotation);
  });
  table.set_function("attachToChar", [](const int &_self, const int &_handle, const float &_xOffset,
                                        const float &_yOffset, const float &_zOffset, const float &_xRotation,
                                        const float &_yRotation, const float &_zRotation) {
    Command<0x069B>(_self, _handle, _xOffset, _yOffset, _zOffset, _xRotation, _yRotation, _zRotation);
  });
  table.set_function("getHealth", [](const int &_self) {
    int health_;
    Command<0x071E>(_self, &health_);
    return std::make_tuple(health_);
  });
  table.set_function("setHealth", [](const int &_self, const int &_health) { Command<0x071F>(_self, _health); });
  table.set_function("break", [](const int &_self, const int &_intensity) { Command<0x0723>(_self, _intensity); });
  table.set_function("setVisible", [](const int &_self, const int &_state) { Command<0x0750>(_self, _state); });
  table.set_function("playAnim", [](const int &_self, const char *_animationName, const char *_animationFile,
                                    const float &_frameDelta, const int &_lockF, const int &_loop) {
    Command<0x075A>(_self, _animationName, _animationFile, _frameDelta, _lockF, _loop);
  });
  table.set_function("getRopeHeight", [](const int &_self) {
    float height_;
    Command<0x0796>(_self, &height_);
    return std::make_tuple(height_);
  });
  table.set_function("setRopeHeight", [](const int &_self, const float &_height) { Command<0x0797>(_self, _height); });
  table.set_function("grabEntityOnRope", [](const int &_self) {
    int vehicle_;
    int char_;
    int object_;
    Command<0x0798>(_self, &vehicle_, &char_, &object_);
    return std::make_tuple(vehicle_, char_, object_);
  });
  table.set_function("releaseEntityFromRope", [](const int &_self) { Command<0x0799>(_self); });
  table.set_function("getQuaternion", [](const int &_self) {
    float x_;
    float y_;
    float z_;
    float w_;
    Command<0x07C3>(_self, &x_, &y_, &z_, &w_);
    return std::make_tuple(x_, y_, z_, w_);
  });
  table.set_function("setQuaternion", [](const int &_self, const float &_x, const float &_y, const float &_z,
                                         const float &_w) { Command<0x07C4>(_self, _x, _y, _z, _w); });
  table.set_function("setCollisionDamageEffect",
                     [](const int &_self, const int &_state) { Command<0x07F7>(_self, _state); });
  table.set_function("getLevelDesignCoords", [](const int &_self, const int &_nth) {
    float x_;
    float y_;
    float z_;
    Command<0x080A>(_self, _nth, &x_, &y_, &z_);
    return std::make_tuple(x_, y_, z_);
  });
  table.set_function("setCoordinatesAndVelocity", [](const int &_self, const float &_x, const float &_y,
                                                     const float &_z) { Command<0x0815>(_self, _x, _y, _z); });
  table.set_function("connectLods",
                     [](const int &_self, const int &_lodObject) { Command<0x0827>(_self, _lodObject); });
  table.set_function("hasBeenPhotographed", [](const int &_self) {
    auto result = Command<0x0833>(_self);
    return std::make_tuple(result);
  });
  table.set_function("setAnimSpeed", [](const int &_self, const char *_animationName, const float &_speed) {
    Command<0x0836>(_self, _animationName, _speed);
  });
  table.set_function("isPlayingAnim", [](const int &_self, const char *_animationName) {
    auto result = Command<0x0837>(_self, _animationName);
    return std::make_tuple(result);
  });
  table.set_function("getAnimCurrentTime", [](const int &_self, const char *_animationName) {
    float time_;
    Command<0x0839>(_self, _animationName, &time_);
    return std::make_tuple(time_);
  });
  table.set_function("setAnimCurrentTime", [](const int &_self, const char *_animationName, const float &_time) {
    Command<0x083A>(_self, _animationName, _time);
  });
  table.set_function("setOnlyDamagedByPlayer",
                     [](const int &_self, const int &_state) { Command<0x0875>(_self, _state); });
  table.set_function("setScale", [](const int &_self, const float &_scale) { Command<0x08D2>(_self, _scale); });
  table.set_function("isInAngledArea2D", [](const int &_self, const float &_leftBottomX, const float &_leftBottomY,
                                            const float &_rightTopX, const float &_rightTopY, const float &_angle,
                                            const int &_drawSphere) {
    auto result = Command<0x08E3>(_self, _leftBottomX, _leftBottomY, _rightTopX, _rightTopY, _angle, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("isInAngledArea3D", [](const int &_self, const float &_leftBottomX, const float &_leftBottomY,
                                            const float &_leftBottomZ, const float &_rightTopX, const float &_rightTopY,
                                            const float &_rightTopZ, const float &_angle, const int &_drawSphere) {
    auto result = Command<0x08E4>(_self, _leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX, _rightTopY, _rightTopZ,
                                  _angle, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("setAsStealable", [](const int &_self, const int &_state) { Command<0x08E9>(_self, _state); });
  table.set_function("hasBeenDamagedByWeapon", [](const int &_self, const int &_weaponType) {
    auto result = Command<0x08FF>(_self, _weaponType);
    return std::make_tuple(result);
  });
  table.set_function("clearLastWeaponDamage", [](const int &_self) { Command<0x0900>(_self); });
  table.set_function("lockDoor", [](const int &_self, const int &_state) { Command<0x0905>(_self, _state); });
  table.set_function("setMass", [](const int &_self, const float &_mass) { Command<0x0906>(_self, _mass); });
  table.set_function("getMass", [](const int &_self) {
    float mass_;
    Command<0x0907>(_self, &mass_);
    return std::make_tuple(mass_);
  });
  table.set_function("setTurnMass",
                     [](const int &_self, const float &_turnMass) { Command<0x0908>(_self, _turnMass); });
  table.set_function("getTurnMass", [](const int &_self) {
    float turnMass_;
    Command<0x0909>(_self, &turnMass_);
    return std::make_tuple(turnMass_);
  });
  table.set_function("winchCanPickUp", [](const int &_self, const int &_state) { Command<0x0916>(_self, _state); });
  table.set_function("hasBeenUprooted", [](const int &_self) {
    auto result = Command<0x095B>(_self);
    return std::make_tuple(result);
  });
  table.set_function("isWithinBrainActivationRange", [](const int &_self) {
    auto result = Command<0x0977>(_self);
    return std::make_tuple(result);
  });
  table.set_function("getModel", [](const int &_self) {
    int model_;
    Command<0x0984>(_self, &model_);
    return std::make_tuple(model_);
  });
  table.set_function("removeElegantly", [](const int &_self) { Command<0x09A2>(_self); });
  table.set_function("setProofs", [](const int &_self, const int &_bulletProof, const int &_fireProof,
                                     const int &_explosionProof, const int &_collisionProof, const int &_meleeProof) {
    Command<0x09CA>(_self, _bulletProof, _fireProof, _explosionProof, _collisionProof, _meleeProof);
  });
  table.set_function("doesHaveThisModel", [](const int &_self, const int &_modelId) {
    auto result = Command<0x09CC>(_self, _modelId);
    return std::make_tuple(result);
  });
  table.set_function("isIntersectingWorld", [](const int &_self) {
    auto result = Command<0x09FC>(_self);
    return std::make_tuple(result);
  });
  table.set_function("enableDisabledAttractors",
                     [](const int &_self, const int &_state) { Command<0x0A0A>(_self, _state); });

  // CLEO+
  table.set_function("setModelAlpha", [](const int &_self, const int &_alpha) { Command<0x0D11>(_self, _alpha); });
  table.set_function("createNoSave", [](const int &_modelId, const float &_x, const float &_y, const float &_z,
                                        const int &_useOffset, const int &_useGround) {
    int handle_;
    Command<0x0E01>(_modelId, _x, _y, _z, _useOffset, _useGround, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("isScriptControlled", [](const int &_self) {
    auto result = Command<0x0E0C>(_self);
    return std::make_tuple(result);
  });
  table.set_function("markAsNeeded", [](const int &_self) { Command<0x0E0D>(_self); });
  table.set_function("initExtendedVars", [](const int &_self, const char *_identifier, const int &_totalVars) {
    auto result = Command<0x0E1A>(_self, _identifier, _totalVars);
    return std::make_tuple(result);
  });
  table.set_function("setExtendedVar",
                     [](const int &_self, const char *_identifier, const int &_varNumber, const int &_value) {
                       auto result = Command<0x0E1B>(_self, _identifier, _varNumber, _value);
                       return std::make_tuple(result);
                     });
  table.set_function("getExtendedVar", [](const int &_self, const char *_identifier, const int &_varNumber) {
    int value_;
    auto result = Command<0x0E1C>(_self, _identifier, _varNumber, &value_);
    return std::make_tuple(result, value_);
  });
  table.set_function("getDistanceFromCenterOfMassToBaseOfModel", [](const int &_self) {
    float distance_;
    Command<0x0E71>(_self, &distance_);
    return std::make_tuple(distance_);
  });
  table.set_function("isReallyInAir", [](const int &_self) {
    auto result = Command<0x0E94>(_self);
    return std::make_tuple(result);
  });
  table.set_function("simulateDamage", [](const int &_self, const float &_damage, const int &_weaponType) {
    Command<0x0E95>(_self, _damage, _weaponType);
  });
  table.set_function("getProofs", [](const int &_self) {
    int bullet_;
    int fire_;
    int explosion_;
    int collision_;
    int melee_;
    Command<0x0EAE>(_self, &bullet_, &fire_, &explosion_, &collision_, &melee_);
    return std::make_tuple(bullet_, fire_, explosion_, collision_, melee_);
  });
  table.set_function("getRandomSeed", [](const int &_self) {
    int randomSeed_;
    Command<0x0ECA>(_self, &randomSeed_);
    return std::make_tuple(randomSeed_);
  });
  table.set_function("locateDistanceToObject", [](const int &_self, const int &_object, const float &_radius) {
    auto result = Command<0x0EE9>(_self, _object, _radius);
    return std::make_tuple(result);
  });
  table.set_function("locateDistanceToCoordinates",
                     [](const int &_self, const float &_x, const float &_y, const float &_z, const float &_radius) {
                       auto result = Command<0x0EEC>(_self, _x, _y, _z, _radius);
                       return std::make_tuple(result);
                     });
  table.set_function("createRenderObjectToObject",
                     [](const int &_self, const int &_modelId, const float &_x, const float &_y, const float &_z,
                        const float &_rx, const float &_ry, const float &_rz) {
                       int renderobject_;
                       Command<0x0F03>(_self, _modelId, _x, _y, _z, _rx, _ry, _rz, &renderobject_);
                       return std::make_tuple(renderobject_);
                     });
  table.set_function("createRenderObjectToObjectFromSpecial",
                     [](const int &_self, const int &_specialModel, const float &_x, const float &_y, const float &_z,
                        const float &_rx, const float &_ry, const float &_rz) {
                       int renderobject_;
                       Command<0x0F04>(_self, _specialModel, _x, _y, _z, _rx, _ry, _rz, &renderobject_);
                       return std::make_tuple(renderobject_);
                     });
}