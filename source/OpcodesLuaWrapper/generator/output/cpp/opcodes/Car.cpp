// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindCar(sol::state &state) {
  auto table = state.create_table("CarOp");
  // default
  table.set_function("create", [](const int &_modelId, const float &_x, const float &_y, const float &_z) {
    int handle_;
    Command<0x00A5>(_modelId, _x, _y, _z, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("delete", [](const int &_self) { Command<0x00A6>(_self); });
  table.set_function("gotoCoordinates", [](const int &_self, const float &_x, const float &_y, const float &_z) {
    Command<0x00A7>(_self, _x, _y, _z);
  });
  table.set_function("wanderRandomly", [](const int &_self) { Command<0x00A8>(_self); });
  table.set_function("setIdle", [](const int &_self) { Command<0x00A9>(_self); });
  table.set_function("getCoordinates", [](const int &_self) {
    float x_;
    float y_;
    float z_;
    Command<0x00AA>(_self, &x_, &y_, &z_);
    return std::make_tuple(x_, y_, z_);
  });
  table.set_function("setCoordinates", [](const int &_self, const float &_x, const float &_y, const float &_z) {
    Command<0x00AB>(_self, _x, _y, _z);
  });
  table.set_function("setCruiseSpeed",
                     [](const int &_self, const float &_maxSpeed) { Command<0x00AD>(_self, _maxSpeed); });
  table.set_function("setDrivingStyle",
                     [](const int &_self, const int &_drivingStyle) { Command<0x00AE>(_self, _drivingStyle); });
  table.set_function("setMission",
                     [](const int &_self, const int &_carMission) { Command<0x00AF>(_self, _carMission); });
  table.set_function("isInArea2D", [](const int &_self, const float &_leftBottomX, const float &_leftBottomY,
                                      const float &_rightTopX, const float &_rightTopY, const int &_drawSphere) {
    auto result = Command<0x00B0>(_self, _leftBottomX, _leftBottomY, _rightTopX, _rightTopY, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("isInArea3D", [](const int &_self, const float &_leftBottomX, const float &_leftBottomY,
                                      const float &_leftBottomZ, const float &_rightTopX, const float &_rightTopY,
                                      const float &_rightTopZ, const int &_drawSphere) {
    auto result = Command<0x00B1>(_self, _leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX, _rightTopY, _rightTopZ,
                                  _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("isDead", [](const int &_handle) {
    auto result = Command<0x0119>(_handle);
    return std::make_tuple(result);
  });
  table.set_function("isModel", [](const int &_self, const int &_modelId) {
    auto result = Command<0x0137>(_self, _modelId);
    return std::make_tuple(result);
  });
  table.set_function("getHeading", [](const int &_self) {
    float heading_;
    Command<0x0174>(_self, &heading_);
    return std::make_tuple(heading_);
  });
  table.set_function("setHeading", [](const int &_self, const float &_heading) { Command<0x0175>(_self, _heading); });
  table.set_function("isHealthGreater", [](const int &_self, const int &_health) {
    auto result = Command<0x0185>(_self, _health);
    return std::make_tuple(result);
  });
  table.set_function("isStuckOnRoof", [](const int &_self) {
    auto result = Command<0x018F>(_self);
    return std::make_tuple(result);
  });
  table.set_function("addUpsidedownCheck", [](const int &_self) { Command<0x0190>(_self); });
  table.set_function("removeUpsidedownCheck", [](const int &_self) { Command<0x0191>(_self); });
  table.set_function("isStoppedInArea2D", [](const int &_self, const float &_leftBottomX, const float &_leftBottomY,
                                             const float &_rightTopX, const float &_rightTopY, const int &_drawSphere) {
    auto result = Command<0x01AB>(_self, _leftBottomX, _leftBottomY, _rightTopX, _rightTopY, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("isStoppedInArea3D", [](const int &_self, const float &_leftBottomX, const float &_leftBottomY,
                                             const float &_leftBottomZ, const float &_rightTopX,
                                             const float &_rightTopY, const float &_rightTopZ, const int &_drawSphere) {
    auto result = Command<0x01AC>(_self, _leftBottomX, _leftBottomY, _leftBottomZ, _rightTopX, _rightTopY, _rightTopZ,
                                  _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("locate2D", [](const int &_self, const float &_x, const float &_y, const float &_xRadius,
                                    const float &_yRadius, const int &_drawSphere) {
    auto result = Command<0x01AD>(_self, _x, _y, _xRadius, _yRadius, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("locateStopped2D", [](const int &_self, const float &_x, const float &_y, const float &_xRadius,
                                           const float &_yRadius, const int &_drawSphere) {
    auto result = Command<0x01AE>(_self, _x, _y, _xRadius, _yRadius, _drawSphere);
    return std::make_tuple(result);
  });
  table.set_function("locate3D",
                     [](const int &_self, const float &_x, const float &_y, const float &_z, const float &_xRadius,
                        const float &_yRadius, const float &_zRadius, const int &_drawSphere) {
                       auto result = Command<0x01AF>(_self, _x, _y, _z, _xRadius, _yRadius, _zRadius, _drawSphere);
                       return std::make_tuple(result);
                     });
  table.set_function("locateStopped3D",
                     [](const int &_self, const float &_x, const float &_y, const float &_z, const float &_xRadius,
                        const float &_yRadius, const float &_zRadius, const int &_drawSphere) {
                       auto result = Command<0x01B0>(_self, _x, _y, _z, _xRadius, _yRadius, _zRadius, _drawSphere);
                       return std::make_tuple(result);
                     });
  table.set_function("isStopped", [](const int &_self) {
    auto result = Command<0x01C1>(_self);
    return std::make_tuple(result);
  });
  table.set_function("markAsNoLongerNeeded", [](const int &_self) { Command<0x01C3>(_self); });
  table.set_function("getNumberOfPassengers", [](const int &_self) {
    int count_;
    Command<0x01E9>(_self, &count_);
    return std::make_tuple(count_);
  });
  table.set_function("getMaximumNumberOfPassengers", [](const int &_self) {
    int count_;
    Command<0x01EA>(_self, &count_);
    return std::make_tuple(count_);
  });
  table.set_function("setHeavy", [](const int &_self, const int &_state) { Command<0x01EC>(_self, _state); });
  table.set_function("isInAirProper", [](const int &_self) {
    auto result = Command<0x01F3>(_self);
    return std::make_tuple(result);
  });
  table.set_function("isUpsidedown", [](const int &_self) {
    auto result = Command<0x01F4>(_self);
    return std::make_tuple(result);
  });
  table.set_function("lockDoors",
                     [](const int &_self, const int &_lockStatus) { Command<0x020A>(_self, _lockStatus); });
  table.set_function("explode", [](const int &_self) { Command<0x020B>(_self); });
  table.set_function("isUpright", [](const int &_self) {
    auto result = Command<0x020D>(_self);
    return std::make_tuple(result);
  });
  table.set_function("setTaxiLights", [](const int &_self, const int &_state) { Command<0x0216>(_self, _state); });
  table.set_function("setHealth", [](const int &_self, const int &_health) { Command<0x0224>(_self, _health); });
  table.set_function("getHealth", [](const int &_self) {
    int health_;
    Command<0x0227>(_self, &health_);
    return std::make_tuple(health_);
  });
  table.set_function("changeColor", [](const int &_self, const int &_primaryColor, const int &_secondaryColor) {
    Command<0x0229>(_self, _primaryColor, _secondaryColor);
  });
  table.set_function("armWithBomb", [](const int &_self, const int &_bombType) { Command<0x0242>(_self, _bombType); });
  table.set_function("setCanRespray", [](const int &_self, const int &_state) { Command<0x0294>(_self, _state); });
  table.set_function("setOnlyDamagedByPlayer",
                     [](const int &_self, const int &_state) { Command<0x02AA>(_self, _state); });
  table.set_function("setProofs", [](const int &_self, const int &_bulletProof, const int &_fireProof,
                                     const int &_explosionProof, const int &_collisionProof, const int &_meleeProof) {
    Command<0x02AC>(_self, _bulletProof, _fireProof, _explosionProof, _collisionProof, _meleeProof);
  });
  table.set_function("isInWater", [](const int &_self) {
    auto result = Command<0x02BF>(_self);
    return std::make_tuple(result);
  });
  table.set_function("gotoCoordinatesAccurate", [](const int &_self, const float &_x, const float &_y,
                                                   const float &_z) { Command<0x02C2>(_self, _x, _y, _z); });
  table.set_function("isOnScreen", [](const int &_self) {
    auto result = Command<0x02CA>(_self);
    return std::make_tuple(result);
  });
  table.set_function("getSpeed", [](const int &_self) {
    float speed_;
    Command<0x02E3>(_self, &speed_);
    return std::make_tuple(speed_);
  });
  table.set_function("getForwardX", [](const int &_self) {
    float x_;
    Command<0x02F8>(_self, &x_);
    return std::make_tuple(x_);
  });
  table.set_function("getForwardY", [](const int &_self) {
    float y_;
    Command<0x02F9>(_self, &y_);
    return std::make_tuple(y_);
  });
  table.set_function("hasBeenDamagedByWeapon", [](const int &_self, const int &_weaponType) {
    auto result = Command<0x031E>(_self, _weaponType);
    return std::make_tuple(result);
  });
  table.set_function("setVisible", [](const int &_self, const int &_state) { Command<0x0338>(_self, _state); });
  table.set_function("switchSiren", [](const int &_self, const int &_state) { Command<0x0397>(_self, _state); });
  table.set_function("setWatertight", [](const int &_self, const int &_state) { Command<0x039C>(_self, _state); });
  table.set_function("turnToFaceCoord",
                     [](const int &_self, const float &_x, const float &_y) { Command<0x039F>(_self, _x, _y); });
  table.set_function("setStatus", [](const int &_self, const int &_status) { Command<0x03A2>(_self, _status); });
  table.set_function("setStrong", [](const int &_self, const int &_state) { Command<0x03AB>(_self, _state); });
  table.set_function("isVisiblyDamaged", [](const int &_self) {
    auto result = Command<0x03C9>(_self);
    return std::make_tuple(result);
  });
  table.set_function("setUpsidedownNotDamaged",
                     [](const int &_self, const int &_state) { Command<0x03ED>(_self, _state); });
  table.set_function("getColors", [](const int &_self) {
    int primaryColour_;
    int secondaryColour_;
    Command<0x03F3>(_self, &primaryColour_, &secondaryColour_);
    return std::make_tuple(primaryColour_, secondaryColour_);
  });
  table.set_function("setCanBeDamaged", [](const int &_self, const int &_state) { Command<0x03F5>(_self, _state); });
  table.set_function("getOffsetInWorldCoords",
                     [](const int &_self, const float &_xOffset, const float &_yOffset, const float &_zOffset) {
                       float x_;
                       float y_;
                       float z_;
                       Command<0x0407>(_self, _xOffset, _yOffset, _zOffset, &x_, &y_, &z_);
                       return std::make_tuple(x_, y_, z_);
                     });
  table.set_function("setTraction",
                     [](const int &_self, const float &_traction) { Command<0x0423>(_self, _traction); });
  table.set_function("setAvoidLevelTransitions",
                     [](const int &_self, const int &_state) { Command<0x0428>(_self, _state); });
  table.set_function("isPassengerSeatFree", [](const int &_self, const int &_seatIndex) {
    auto result = Command<0x0431>(_self, _seatIndex);
    return std::make_tuple(result);
  });
  table.set_function("getCharInPassengerSeat", [](const int &_self, const int &_seatIndex) {
    int handle_;
    Command<0x0432>(_self, _seatIndex, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("getModel", [](const int &_self) {
    int modelId_;
    Command<0x0441>(_self, &modelId_);
    return std::make_tuple(modelId_);
  });
  table.set_function("setStayInFastLane", [](const int &_self, const int &_state) { Command<0x0466>(_self, _state); });
  table.set_function("clearLastWeaponDamage", [](const int &_self) { Command<0x0468>(_self); });
  table.set_function("getDriver", [](const int &_self) {
    int handle_;
    Command<0x046C>(_self, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("setTempAction", [](const int &_self, const int &_actionId, const int &_timeInMs) {
    Command<0x0477>(_self, _actionId, _timeInMs);
  });
  table.set_function("setRandomRouteSeed",
                     [](const int &_self, const int &_routeSeed) { Command<0x048B>(_self, _routeSeed); });
  table.set_function("isOnFire", [](const int &_self) {
    auto result = Command<0x0495>(_self);
    return std::make_tuple(result);
  });
  table.set_function("isTireBurst", [](const int &_self, const int &_tireId) {
    auto result = Command<0x0496>(_self, _tireId);
    return std::make_tuple(result);
  });
  table.set_function("setForwardSpeed",
                     [](const int &_self, const float &_forwardSpeed) { Command<0x04BA>(_self, _forwardSpeed); });
  table.set_function("markAsConvoyCar", [](const int &_self, const int &_state) { Command<0x04BD>(_self, _state); });
  table.set_function("setStraightLineDistance",
                     [](const int &_self, const int &_distance) { Command<0x04E0>(_self, _distance); });
  table.set_function("popBoot", [](const int &_self) { Command<0x04E1>(_self); });
  table.set_function("isWaitingForWorldCollision", [](const int &_self) {
    auto result = Command<0x04F1>(_self);
    return std::make_tuple(result);
  });
  table.set_function("burstTire", [](const int &_self, const int &_tireId) { Command<0x04FE>(_self, _tireId); });
  table.set_function("setModelComponents", [](const int &__unused, const int &_component1, const int &_component2) {
    Command<0x0506>(__unused, _component1, _component2);
  });
  table.set_function("closeAllDoors", [](const int &_self) { Command<0x0508>(_self); });
  table.set_function("freezePosition", [](const int &_self, const int &_state) { Command<0x0519>(_self, _state); });
  table.set_function("hasBeenDamagedByChar", [](const int &_self, const int &_handle) {
    auto result = Command<0x051C>(_self, _handle);
    return std::make_tuple(result);
  });
  table.set_function("hasBeenDamagedByCar", [](const int &_self, const int &_other) {
    auto result = Command<0x051D>(_self, _other);
    return std::make_tuple(result);
  });
  table.set_function("setCanBurstTires", [](const int &_self, const int &_state) { Command<0x053F>(_self, _state); });
  table.set_function("clearLastDamageEntity", [](const int &_self) { Command<0x054F>(_self); });
  table.set_function("doesExist", [](const int &_handle) {
    auto result = Command<0x056E>(_handle);
    return std::make_tuple(result);
  });
  table.set_function("freezePositionAndDontLoadCollision",
                     [](const int &_self, const int &_state) { Command<0x0574>(_self, _state); });
  table.set_function("setLoadCollisionFlag",
                     [](const int &_self, const int &_state) { Command<0x0587>(_self, _state); });
  table.set_function("setToFadeIn", [](const int &_self, const int &_alpha) { Command<0x0594>(_self, _alpha); });
  table.set_function("startPlayback", [](const int &_self, const int &_path) { Command<0x05EB>(_self, _path); });
  table.set_function("stopPlayback", [](const int &_self) { Command<0x05EC>(_self); });
  table.set_function("pausePlayback", [](const int &_self) { Command<0x05ED>(_self); });
  table.set_function("unpausePlayback", [](const int &_self) { Command<0x05EE>(_self); });
  table.set_function("setEscortCarLeft", [](const int &_self, const int &_handle) { Command<0x05F1>(_self, _handle); });
  table.set_function("setEscortCarRight",
                     [](const int &_self, const int &_handle) { Command<0x05F2>(_self, _handle); });
  table.set_function("setEscortCarRear", [](const int &_self, const int &_handle) { Command<0x05F3>(_self, _handle); });
  table.set_function("setEscortCarFront",
                     [](const int &_self, const int &_handle) { Command<0x05F4>(_self, _handle); });
  table.set_function("isPlaybackGoingOn", [](const int &_self) {
    auto result = Command<0x060E>(_self);
    return std::make_tuple(result);
  });
  table.set_function("openDoor", [](const int &_self, const int &_door) { Command<0x0657>(_self, _door); });
  table.set_function("customPlateForNextCar",
                     [](const int &_modelId, const char *_plateText) { Command<0x0674>(_modelId, _plateText); });
  table.set_function("forceLights",
                     [](const int &_self, const int &_lightMode) { Command<0x067F>(_self, _lightMode); });
  table.set_function("attachToCar", [](const int &_self, const int &_handle, const float &_xOffset,
                                       const float &_yOffset, const float &_zOffset, const float &_xRotation,
                                       const float &_yRotation, const float &_zRotation) {
    Command<0x0683>(_self, _handle, _xOffset, _yOffset, _zOffset, _xRotation, _yRotation, _zRotation);
  });
  table.set_function("detach",
                     [](const int &_self, const float &_x, const float &_y, const float &_z,
                        const int &_collisionDetection) { Command<0x0684>(_self, _x, _y, _z, _collisionDetection); });
  table.set_function("isAttached", [](const int &_self) {
    auto result = Command<0x0686>(_self);
    return std::make_tuple(result);
  });
  table.set_function("popDoor", [](const int &_self, const int &_door, const int &_visibility) {
    Command<0x0689>(_self, _door, _visibility);
  });
  table.set_function("fixDoor", [](const int &_self, const int &_door) { Command<0x068A>(_self, _door); });
  table.set_function("taskEveryoneLeave", [](const int &_self) { Command<0x068B>(_self); });
  table.set_function("popPanel", [](const int &_self, const int &_panelId, const int &_visibility) {
    Command<0x0697>(_self, _panelId, _visibility);
  });
  table.set_function("fixPanel", [](const int &_self, const int &_panelId) { Command<0x0698>(_self, _panelId); });
  table.set_function("fixTire", [](const int &_self, const int &_typeId) { Command<0x0699>(_self, _typeId); });
  table.set_function("getSpeedVector", [](const int &_self) {
    float x_;
    float y_;
    float z_;
    Command<0x06A2>(_self, &x_, &y_, &z_);
    return std::make_tuple(x_, y_, z_);
  });
  table.set_function("getMass", [](const int &_self) {
    float mass_;
    Command<0x06A3>(_self, &mass_);
    return std::make_tuple(mass_);
  });
  table.set_function("getRoll", [](const int &_self) {
    float angle_;
    Command<0x06BE>(_self, &angle_);
    return std::make_tuple(angle_);
  });
  table.set_function("skipToEndAndStopPlayback", [](const int &_self) { Command<0x06C5>(_self); });
  table.set_function("getAvailableMod", [](const int &_self, const int &_slotId) {
    int modelId_;
    Command<0x06E5>(_self, _slotId, &modelId_);
    return std::make_tuple(modelId_);
  });
  table.set_function("addMod", [](const int &_self, const int &_modelId) {
    int handle_;
    Command<0x06E7>(_self, _modelId, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("removeMod", [](const int &_self, const int &_modelId) { Command<0x06E8>(_self, _modelId); });
  table.set_function("getNumAvailablePaintjobs", [](const int &_self) {
    int numPaintjobs_;
    Command<0x06EC>(_self, &numPaintjobs_);
    return std::make_tuple(numPaintjobs_);
  });
  table.set_function("givePaintjob",
                     [](const int &_self, const int &_paintjobId) { Command<0x06ED>(_self, _paintjobId); });
  table.set_function("doesHaveStuckCarCheck", [](const int &_self) {
    auto result = Command<0x06FC>(_self);
    return std::make_tuple(result);
  });
  table.set_function("setPlaybackSpeed", [](const int &_self, const float &_speed) { Command<0x06FD>(_self, _speed); });
  table.set_function("gotoCoordinatesRacing", [](const int &_self, const float &_x, const float &_y, const float &_z) {
    Command<0x0704>(_self, _x, _y, _z);
  });
  table.set_function("startPlaybackUsingAi",
                     [](const int &_self, const int &_pathId) { Command<0x0705>(_self, _pathId); });
  table.set_function("skipInPlayback", [](const int &_self, const float &_amount) { Command<0x0706>(_self, _amount); });
  table.set_function("explodeInCutscene", [](const int &_self) { Command<0x070C>(_self); });
  table.set_function("setStayInSlowLane", [](const int &_self, const int &_state) { Command<0x0714>(_self, _state); });
  table.set_function("damagePanel", [](const int &_self, const int &_panelId) { Command<0x0730>(_self, _panelId); });
  table.set_function("setRoll", [](const int &_self, const float &_yAngle) { Command<0x0731>(_self, _yAngle); });
  table.set_function("setCanGoAgainstTraffic",
                     [](const int &_self, const int &_state) { Command<0x073B>(_self, _state); });
  table.set_function("damageDoor", [](const int &_self, const int &_door) { Command<0x073C>(_self, _door); });
  table.set_function("setAsMissionCar", [](const int &_self) { Command<0x0763>(_self); });
  table.set_function("getPitch", [](const int &_self) {
    float angle_;
    Command<0x077D>(_self, &angle_);
    return std::make_tuple(angle_);
  });
  table.set_function("getQuaternion", [](const int &_self) {
    float x_;
    float y_;
    float z_;
    float w_;
    Command<0x07C5>(_self, &x_, &y_, &z_, &w_);
    return std::make_tuple(x_, y_, z_, w_);
  });
  table.set_function("setQuaternion", [](const int &_self, const float &_x, const float &_y, const float &_z,
                                         const float &_w) { Command<0x07C6>(_self, _x, _y, _z, _w); });
  table.set_function("applyForce",
                     [](const int &_self, const float &_xOffset, const float &_yOffset, const float &_zOffset,
                        const float &_xRotation, const float &_yRotation, const float &_zRotation) {
                       Command<0x07D5>(_self, _xOffset, _yOffset, _zOffset, _xRotation, _yRotation, _zRotation);
                     });
  table.set_function("addToRotationVelocity", [](const int &_self, const float &_x, const float &_y, const float &_z) {
    Command<0x07DA>(_self, _x, _y, _z);
  });
  table.set_function("setRotationVelocity", [](const int &_self, const float &_x, const float &_y, const float &_z) {
    Command<0x07DB>(_self, _x, _y, _z);
  });
  table.set_function("setAlwaysCreateSkids",
                     [](const int &_self, const int &_state) { Command<0x07EE>(_self, _state); });
  table.set_function("controlHydraulics", [](const int &_self, const float &__p2, const float &__p3, const float &__p4,
                                             const float &__p5) { Command<0x07F5>(_self, __p2, __p3, __p4, __p5); });
  table.set_function("setFollowCar", [](const int &_self, const int &_handle, const float &_radius) {
    Command<0x07F8>(_self, _handle, _radius);
  });
  table.set_function("setHydraulics", [](const int &_self, const int &_state) { Command<0x07FF>(_self, _state); });
  table.set_function("doesHaveHydraulics", [](const int &_self) {
    auto result = Command<0x0803>(_self);
    return std::make_tuple(result);
  });
  table.set_function("setEngineBroken", [](const int &_self, const int &_state) { Command<0x081D>(_self, _state); });
  table.set_function("getUprightValue", [](const int &_self) {
    float value_;
    Command<0x083F>(_self, &value_);
    return std::make_tuple(value_);
  });
  table.set_function("setAreaVisible",
                     [](const int &_self, const int &_interiorId) { Command<0x0840>(_self, _interiorId); });
  table.set_function("selectWeapons", [](const int &_self, const int &__p2) { Command<0x0841>(_self, __p2); });
  table.set_function("setCanBeTargeted", [](const int &_self, const int &_state) { Command<0x084E>(_self, _state); });
  table.set_function("setCanBeVisiblyDamaged",
                     [](const int &_self, const int &_state) { Command<0x0852>(_self, _state); });
  table.set_function("startPlaybackLooped",
                     [](const int &_self, const int &_pathId) { Command<0x085E>(_self, _pathId); });
  table.set_function("setDirtLevel", [](const int &_self, const float &_level) { Command<0x0878>(_self, _level); });
  table.set_function("setAirResistanceMultiplier",
                     [](const int &_self, const float &_multiplier) { Command<0x088B>(_self, _multiplier); });
  table.set_function("setCoordinatesNoOffset", [](const int &_self, const float &_x, const float &_y, const float &_z) {
    Command<0x088C>(_self, _x, _y, _z);
  });
  table.set_function("isTouchingObject", [](const int &_self, const int &_handle) {
    auto result = Command<0x0897>(_self, _handle);
    return std::make_tuple(result);
  });
  table.set_function("controlMovablePart",
                     [](const int &_self, const float &_range) { Command<0x08A4>(_self, _range); });
  table.set_function("winchCanPickUp", [](const int &_self, const int &_state) { Command<0x08A5>(_self, _state); });
  table.set_function("openDoorABit", [](const int &_self, const int &_door, const float &_value) {
    Command<0x08A6>(_self, _door, _value);
  });
  table.set_function("isDoorFullyOpen", [](const int &_self, const int &_door) {
    auto result = Command<0x08A7>(_self, _door);
    return std::make_tuple(result);
  });
  table.set_function("explodeInCutsceneShakeAndBits",
                     [](const int &_self, const int &_shake, const int &_effect, const int &_sound) {
                       Command<0x08CB>(_self, _shake, _effect, _sound);
                     });
  table.set_function("getClass", [](const int &_self) {
    int class_;
    Command<0x08EC>(_self, &class_);
    return std::make_tuple(class_);
  });
  table.set_function("canBeTargetedByHsMissile",
                     [](const int &_self, const int &_state) { Command<0x08F2>(_self, _state); });
  table.set_function("setFreebies", [](const int &_self, const int &_state) { Command<0x08F3>(_self, _state); });
  table.set_function("setEngineOn", [](const int &_self, const int &_state) { Command<0x0918>(_self, _state); });
  table.set_function("setLightsOn", [](const int &_self, const int &_state) { Command<0x0919>(_self, _state); });
  table.set_function("attachToObject", [](const int &_self, const int &_handle, const float &_xOffset,
                                          const float &_yOffset, const float &_zOffset, const float &_xRotation,
                                          const float &_yRotation, const float &_zRotation) {
    Command<0x0939>(_self, _handle, _xOffset, _yOffset, _zOffset, _xRotation, _yRotation, _zRotation);
  });
  table.set_function("doesProvideCover", [](const int &_self, const int &_state) { Command<0x0957>(_self, _state); });
  table.set_function("controlDoor", [](const int &_self, const int &_door, const int &_latch, const float &_angle) {
    Command<0x095E>(_self, _door, _latch, _angle);
  });
  table.set_function("getDoorAngleRatio", [](const int &_self, const int &_door) {
    float ratio_;
    Command<0x095F>(_self, _door, &ratio_);
    return std::make_tuple(ratio_);
  });
  table.set_function("isBig", [](const int &_self) {
    auto result = Command<0x0969>(_self);
    return std::make_tuple(result);
  });
  table.set_function("storeModState", []() { Command<0x096B>(); });
  table.set_function("restoreModState", []() { Command<0x096C>(); });
  table.set_function("getCurrentMod", [](const int &_self, const int &_slot) {
    int modelId_;
    Command<0x096D>(_self, _slot, &modelId_);
    return std::make_tuple(modelId_);
  });
  table.set_function("isLowRider", [](const int &_self) {
    auto result = Command<0x096E>(_self);
    return std::make_tuple(result);
  });
  table.set_function("isStreetRacer", [](const int &_self) {
    auto result = Command<0x096F>(_self);
    return std::make_tuple(result);
  });
  table.set_function("isEmergencyServices", [](const int &_self) {
    auto result = Command<0x0975>(_self);
    return std::make_tuple(result);
  });
  table.set_function("getNumColors", [](const int &_self) {
    int count_;
    Command<0x097D>(_self, &count_);
    return std::make_tuple(count_);
  });
  table.set_function("getBlockingCar", [](const int &_self) {
    int handle_;
    Command<0x0987>(_self, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("getCurrentPaintjob", [](const int &_self) {
    int paintjobNumber_;
    Command<0x0988>(_self, &paintjobNumber_);
    return std::make_tuple(paintjobNumber_);
  });
  table.set_function("getMovingComponentOffset", [](const int &_self) {
    float offset_;
    Command<0x098D>(_self, &offset_);
    return std::make_tuple(offset_);
  });
  table.set_function("setCollision", [](const int &_self, const int &_state) { Command<0x099A>(_self, _state); });
  table.set_function("changePlaybackToUseAi", [](const int &_self) { Command<0x099B>(_self); });
  table.set_function("randomPassengerSay",
                     [](const int &_self, const int &_speechId) { Command<0x09AB>(_self, _speechId); });
  table.set_function("setIsConsideredByPlayer",
                     [](const int &_self, const int &_state) { Command<0x09B0>(_self, _state); });
  table.set_function("getDoorLockStatus", [](const int &_self) {
    int lockStatus_;
    Command<0x09B3>(_self, &lockStatus_);
    return std::make_tuple(lockStatus_);
  });
  table.set_function("isDoorDamaged", [](const int &_self, const int &_door) {
    auto result = Command<0x09BB>(_self, _door);
    return std::make_tuple(result);
  });
  table.set_function("setPetrolTankWeakpoint",
                     [](const int &_self, const int &_state) { Command<0x09C4>(_self, _state); });
  table.set_function("isTouchingCar", [](const int &_self, const int &_handle) {
    auto result = Command<0x09CB>(_self, _handle);
    return std::make_tuple(result);
  });
  table.set_function("isOnAllWheels", [](const int &_self) {
    auto result = Command<0x09D0>(_self);
    return std::make_tuple(result);
  });
  table.set_function("getModelValue", [](const int &_model) {
    int value_;
    Command<0x09E1>(_model, &value_);
    return std::make_tuple(value_);
  });
  table.set_function("giveNonPlayerNitro", [](const int &_self) { Command<0x09E9>(_self); });
  table.set_function("resetHydraulics", [](const int &_self) { Command<0x09FE>(_self); });
  table.set_function("setExtraColors", [](const int &_self, const int &_color1, const int &_color2) {
    Command<0x0A11>(_self, _color1, _color2);
  });
  table.set_function("getExtraColors", [](const int &_self) {
    int color1_;
    int color2_;
    Command<0x0A12>(_self, &color1_, &color2_);
    return std::make_tuple(color1_, color2_);
  });
  table.set_function("hasBeenResprayed", [](const int &_self) {
    auto result = Command<0x0A15>(_self);
    return std::make_tuple(result);
  });
  table.set_function("improveByCheating", [](const int &_self, const int &_state) { Command<0x0A21>(_self, _state); });
  table.set_function("fix", [](const int &_self) { Command<0x0A30>(_self); });

  // CLEO
  table.set_function("getNumberOfGears", [](const int &_self) {
    int numGear_;
    Command<0x0AB7>(_self, &numGear_);
    return std::make_tuple(numGear_);
  });
  table.set_function("getCurrentGear", [](const int &_self) {
    int gear_;
    Command<0x0AB8>(_self, &gear_);
    return std::make_tuple(gear_);
  });
  table.set_function("isSirenOn", [](const int &_self) {
    auto result = Command<0x0ABD>(_self);
    return std::make_tuple(result);
  });
  table.set_function("isEngineOn", [](const int &_self) {
    auto result = Command<0x0ABE>(_self);
    return std::make_tuple(result);
  });
  table.set_function("cleoSetEngineOn", [](const int &_self, const int &_state) { Command<0x0ABF>(_self, _state); });

  // CLEO+
  table.set_function("setModelAlpha", [](const int &_self, const int &_alpha) { Command<0x0D0F>(_self, _alpha); });
  table.set_function("setDoorWindowState", [](const int &_self, const int &_door, const int &_state) {
    Command<0x0D33>(_self, _door, _state);
  });
  table.set_function("getAlarm", [](const int &_self) {
    int status_;
    Command<0x0E00>(_self, &status_);
    return std::make_tuple(status_);
  });
  table.set_function("isScriptControlled", [](const int &_self) {
    auto result = Command<0x0E08>(_self);
    return std::make_tuple(result);
  });
  table.set_function("markAsNeeded", [](const int &_self) { Command<0x0E09>(_self); });
  table.set_function("getSubclass", [](const int &_self) {
    int subclass_;
    Command<0x0E12>(_self, &subclass_);
    return std::make_tuple(subclass_);
  });
  table.set_function("initExtendedVars", [](const int &_self, const char *_identifier, const int &_totalVars) {
    auto result = Command<0x0E17>(_self, _identifier, _totalVars);
    return std::make_tuple(result);
  });
  table.set_function("setExtendedVar", [](const int &_self, const char *_identifier, const int &_varNumber) {
    auto result = Command<0x0E18>(_self, _identifier, _varNumber);
    return std::make_tuple(result);
  });
  table.set_function("getExtendedCarVar", [](const int &_self, const char *_identifier, const int &_varNumber) {
    int value_;
    auto result = Command<0x0E19>(_self, _identifier, _varNumber, &value_);
    return std::make_tuple(result, value_);
  });
  table.set_function("getTrailer", [](const int &_self) {
    int trailer_;
    auto result = Command<0x0E59>(_self, &trailer_);
    return std::make_tuple(result, trailer_);
  });
  table.set_function("getTractor", [](const int &_self) {
    int tractor_;
    auto result = Command<0x0E5A>(_self, &tractor_);
    return std::make_tuple(result, tractor_);
  });
  table.set_function("getDummyCoord",
                     [](const int &_self, const int &_vehicleDummy, const int &_worldCoords, const int &_invertX) {
                       float x_;
                       float y_;
                       float z_;
                       auto result = Command<0x0E5B>(_self, _vehicleDummy, _worldCoords, _invertX, &x_, &y_, &z_);
                       return std::make_tuple(result, x_, y_, z_);
                     });
  table.set_function("playHorn", [](const int &_self) { Command<0x0E5F>(_self); });
  table.set_function("setAlarm", [](const int &_self, const int &_status) { Command<0x0E61>(_self, _status); });
  table.set_function("getCollisionIntensity", [](const int &_self) {
    float intensity_;
    auto result = Command<0x0E65>(_self, &intensity_);
    return std::make_tuple(result, intensity_);
  });
  table.set_function("getCollisionCoordinates", [](const int &_self) {
    float x_;
    float y_;
    float z_;
    Command<0x0E66>(_self, &x_, &y_, &z_);
    return std::make_tuple(x_, y_, z_);
  });
  table.set_function("doesHavePartNode", [](const int &_self, const int &_carNode) {
    auto result = Command<0x0E82>(_self, _carNode);
    return std::make_tuple(result);
  });
  table.set_function("getCollisionSurface", [](const int &_self) {
    int surfaceType_;
    Command<0x0E90>(_self, &surfaceType_);
    return std::make_tuple(surfaceType_);
  });
  table.set_function("getCollisionLighting", [](const int &_self) {
    float lighting_;
    Command<0x0E91>(_self, &lighting_);
    return std::make_tuple(lighting_);
  });
  table.set_function("isReallyInAir", [](const int &_self) {
    auto result = Command<0x0E93>(_self);
    return std::make_tuple(result);
  });
  table.set_function("getProofs", [](const int &_self) {
    int bullet_;
    int fire_;
    int explosion_;
    int collision_;
    int melee_;
    Command<0x0EAD>(_self, &bullet_, &fire_, &explosion_, &collision_, &melee_);
    return std::make_tuple(bullet_, fire_, explosion_, collision_, melee_);
  });
  table.set_function("setCoordinatesSimple", [](const int &_self, const float &_x, const float &_y, const float &_z) {
    Command<0x0EB4>(_self, _x, _y, _z);
  });
  table.set_function("getWeaponDamageLastFrame", [](const int &_self) {
    int char_;
    int weaponType_;
    float intensity_;
    auto result = Command<0x0EB6>(_self, &char_, &weaponType_, &intensity_);
    return std::make_tuple(result, char_, weaponType_, intensity_);
  });
  table.set_function("getRandomSeed", [](const int &_self) {
    int randomSeed_;
    Command<0x0EC9>(_self, &randomSeed_);
    return std::make_tuple(randomSeed_);
  });
  table.set_function("dontDeleteUntilTime",
                     [](const int &_self, const int &_msFromNow) { Command<0x0ECD>(_self, _msFromNow); });
  table.set_function("getTimeIsDead", [](const int &_self) {
    int timeIsDead_;
    Command<0x0ECF>(_self, &timeIsDead_);
    return std::make_tuple(timeIsDead_);
  });
  table.set_function("locateDistanceToObject", [](const int &_self, const int &_object, const float &_radius) {
    auto result = Command<0x0EE7>(_self, _object, _radius);
    return std::make_tuple(result);
  });
  table.set_function("locateDistanceToCar", [](const int &_self, const int &_car, const float &_radius) {
    auto result = Command<0x0EE8>(_self, _car, _radius);
    return std::make_tuple(result);
  });
  table.set_function("locateDistanceToCoordinates",
                     [](const int &_self, const float &_x, const float &_y, const float &_z, const float &_radius) {
                       auto result = Command<0x0EEB>(_self, _x, _y, _z, _radius);
                       return std::make_tuple(result);
                     });
  table.set_function("isOwnedByPlayer", [](const int &_self) {
    auto result = Command<0x0EF5>(_self);
    return std::make_tuple(result);
  });
  table.set_function("setOwnedByPlayer",
                     [](const int &_self, const int &_ownedByPlayer) { Command<0x0EF6>(_self, _ownedByPlayer); });
  table.set_function("getAnimGroup", [](const int &_self) {
    int carAnimGroup_;
    Command<0x0EF9>(_self, &carAnimGroup_);
    return std::make_tuple(carAnimGroup_);
  });
  table.set_function("isConvertible", [](const int &_self) {
    auto result = Command<0x0EFB>(_self);
    return std::make_tuple(result);
  });
  table.set_function("getValue", [](const int &_self) {
    int value_;
    Command<0x0EFC>(_self, &value_);
    return std::make_tuple(value_);
  });
  table.set_function("getPedals", [](const int &_self) {
    float gas_;
    float brake_;
    Command<0x0EFD>(_self, &gas_, &brake_);
    return std::make_tuple(gas_, brake_);
  });
}