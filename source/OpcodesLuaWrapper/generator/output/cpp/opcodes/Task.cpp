// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindTask(sol::state &state) {
  auto table = state.create_table("TaskOp");
  // default
  table.set_function("toggleDuck", [](const int &_handle, const bool &_state) { Command<0x04EB>(_handle, _state); });
  table.set_function("pause", [](const int &_handle, const int &_time) { Command<0x05B9>(_handle, _time); });
  table.set_function("standStill", [](const int &_handle, const int &_time) { Command<0x05BA>(_handle, _time); });
  table.set_function("fallAndGetUp", [](const int &_handle, const bool &_fallDown, const int &_timeOnGround) {
    Command<0x05BB>(_handle, _fallDown, _timeOnGround);
  });
  table.set_function("jump", [](const int &_handle, const bool &_state) { Command<0x05BC>(_handle, _state); });
  table.set_function("tired", [](const int &_handle, const int &_time) { Command<0x05BD>(_handle, _time); });
  table.set_function("die", [](const int &_handle) { Command<0x05BE>(_handle); });
  table.set_function("lookAtChar", [](const int &_observer, const int &_target, const int &_time) {
    Command<0x05BF>(_observer, _target, _time);
  });
  table.set_function("lookAtVehicle", [](const int &_char, const int &_vehicle, const int &_time) {
    Command<0x05C0>(_char, _vehicle, _time);
  });
  table.set_function("say", [](const int &_handle, const int &_phraseId) { Command<0x05C1>(_handle, _phraseId); });
  table.set_function("shakeFist", [](const int &_handle) { Command<0x05C2>(_handle); });
  table.set_function("cower", [](const int &_handle) { Command<0x05C3>(_handle); });
  table.set_function("handsUp", [](const int &_handle, const int &_time) { Command<0x05C4>(_handle, _time); });
  table.set_function("duck", [](const int &_handle, const int &_time) { Command<0x05C5>(_handle, _time); });
  table.set_function("useAtm", [](const int &_handle) { Command<0x05C7>(_handle); });
  table.set_function("scratchHead", [](const int &_handle) { Command<0x05C8>(_handle); });
  table.set_function("lookAbout", [](const int &_handle, const int &_time) { Command<0x05C9>(_handle, _time); });
  table.set_function("enterCarAsPassenger",
                     [](const int &_char, const int &_vehicle, const int &_time, const int &_seatId) {
                       Command<0x05CA>(_char, _vehicle, _time, _seatId);
                     });
  table.set_function("enterCarAsDriver", [](const int &_char, const int &_vehicle, const int &_time) {
    Command<0x05CB>(_char, _vehicle, _time);
  });
  table.set_function("leaveCar", [](const int &_char, const int &_vehicle) { Command<0x05CD>(_char, _vehicle); });
  table.set_function("leaveCarAndFlee", [](const int &_char, const int &_vehicle, const float &_x, const float &_y,
                                           const float &_z) { Command<0x05CF>(_char, _vehicle, _x, _y, _z); });
  table.set_function("carDriveToCoord",
                     [](const int &_driver, const int &_vehicle, const float &_x, const float &_y, const float &_z,
                        const float &_speed, const int &__p7, const int &__p8, const int &_drivingStyle) {
                       Command<0x05D1>(_driver, _vehicle, _x, _y, _z, _speed, __p7, __p8, _drivingStyle);
                     });
  table.set_function("carDriveWander",
                     [](const int &_char, const int &_vehicle, const float &_speed, const int &_drivingStyle) {
                       Command<0x05D2>(_char, _vehicle, _speed, _drivingStyle);
                     });
  table.set_function("goStraightToCoord",
                     [](const int &_handle, const float &_x, const float &_y, const float &_z, const int &_moveState,
                        const int &_time) { Command<0x05D3>(_handle, _x, _y, _z, _moveState, _time); });
  table.set_function("achieveHeading",
                     [](const int &_handle, const float &_heading) { Command<0x05D4>(_handle, _heading); });
  table.set_function("followPointRoute", [](const int &_handle, const int &_walkSpeed, const int &_flag) {
    Command<0x05D8>(_handle, _walkSpeed, _flag);
  });
  table.set_function("gotoChar", [](const int &_walking, const int &_target, const int &_time, const float &_radius) {
    Command<0x05D9>(_walking, _target, _time, _radius);
  });
  table.set_function("fleePoint",
                     [](const int &_handle, const float &_x, const float &_y, const float &_z, const float &_radius,
                        const int &_time) { Command<0x05DA>(_handle, _x, _y, _z, _radius, _time); });
  table.set_function("fleeChar", [](const int &_handle, const int &_threat, const float &_radius, const int &_time) {
    Command<0x05DB>(_handle, _threat, _radius, _time);
  });
  table.set_function("smartFleePoint",
                     [](const int &_handle, const float &_x, const float &_y, const float &_z, const float &_radius,
                        const int &_time) { Command<0x05DC>(_handle, _x, _y, _z, _radius, _time); });
  table.set_function("smartFleeChar", [](const int &_handle, const int &_threat, const float &_radius,
                                         const int &_time) { Command<0x05DD>(_handle, _threat, _radius, _time); });
  table.set_function("wanderStandard", [](const int &_handle) { Command<0x05DE>(_handle); });
  table.set_function("killCharOnFoot",
                     [](const int &_killer, const int &_target) { Command<0x05E2>(_killer, _target); });
  table.set_function("followPathNodesToCoord",
                     [](const int &_handle, const float &_x, const float &_y, const float &_z, const int &_walkSpeed,
                        const int &_time) { Command<0x05F5>(_handle, _x, _y, _z, _walkSpeed, _time); });
  table.set_function("goToCoordAnyMeans",
                     [](const int &_char, const float &_x, const float &_y, const float &_z, const int &_walkSpeed,
                        const int &_vehicle) { Command<0x0603>(_char, _x, _y, _z, _walkSpeed, _vehicle); });
  table.set_function("playAnim", [](const int &_handle, const char *_animationName, const char *_animationFile,
                                    const float &_frameDelta, const bool &_loop, const bool &_lockX, const bool &_lockY,
                                    const bool &_lockF, const int &_time) {
    Command<0x0605>(_handle, _animationName, _animationFile, _frameDelta, _loop, _lockX, _lockY, _lockF, _time);
  });
  table.set_function("leaveCarImmediately",
                     [](const int &_char, const int &_vehicle) { Command<0x0622>(_char, _vehicle); });
  table.set_function("leaveAnyCar", [](const int &_handle) { Command<0x0633>(_handle); });
  table.set_function("killCharOnFootWhileDucking", [](const int &_char, const int &_target, const int &_flags,
                                                      const int &_actionDelay, const int &_actionChance) {
    Command<0x0634>(_char, _target, _flags, _actionDelay, _actionChance);
  });
  table.set_function("aimGunAtChar", [](const int &_char, const int &_target, const int &_time) {
    Command<0x0635>(_char, _target, _time);
  });
  table.set_function("goToCoordWhileShooting",
                     [](const int &_char, const float &_x, const float &_y, const float &_z, const int &_mode,
                        const float &_turnRadius, const float &_stopRadius, const int &_target) {
                       Command<0x0637>(_char, _x, _y, _z, _mode, _turnRadius, _stopRadius, _target);
                     });
  table.set_function("stayInSamePlace",
                     [](const int &_handle, const bool &_state) { Command<0x0638>(_handle, _state); });
  table.set_function("turnCharToFaceChar",
                     [](const int &_char, const int &_target) { Command<0x0639>(_char, _target); });
  table.set_function("lookAtObject", [](const int &_char, const int &_object, const int &_time) {
    Command<0x0655>(_char, _object, _time);
  });
  table.set_function("aimGunAtCoord", [](const int &_handle, const float &_x, const float &_y, const float &_z,
                                         const int &_time) { Command<0x0667>(_handle, _x, _y, _z, _time); });
  table.set_function("shootAtCoord", [](const int &_handle, const float &_x, const float &_y, const float &_z,
                                        const int &_time) { Command<0x0668>(_handle, _x, _y, _z, _time); });
  table.set_function("destroyCar", [](const int &_char, const int &_vehicle) { Command<0x0672>(_char, _vehicle); });
  table.set_function("diveAndGetUp", [](const int &_handle, const float &_directionX, const float &_directionY,
                                        const int &_timeOnGround) {
    Command<0x0673>(_handle, _directionX, _directionY, _timeOnGround);
  });
  table.set_function("shuffleToNextCarSeat",
                     [](const int &_char, const int &_vehicle) { Command<0x0676>(_char, _vehicle); });
  table.set_function("chatWithChar", [](const int &_char, const int &_other, const bool &_leadSpeaker,
                                        const int &__p4) { Command<0x0677>(_char, _other, _leadSpeaker, __p4); });
  table.set_function("togglePedThreatScanner", [](const int &_handle, const bool &__p2, const bool &__p3,
                                                  const bool &__p4) { Command<0x0688>(_handle, __p2, __p3, __p4); });
  table.set_function("diveFromAttachmentAndGetUp",
                     [](const int &_handle, const int &_time) { Command<0x06A5>(_handle, _time); });
  table.set_function("gotoCharOffset",
                     [](const int &_char, const int &_target, const int &_time, const float &_radius,
                        const float &_heading) { Command<0x06A8>(_char, _target, _time, _radius, _heading); });
  table.set_function("lookAtCoord", [](const int &_handle, const float &_x, const float &_y, const float &_z,
                                       const int &_time) { Command<0x06A9>(_handle, _x, _y, _z, _time); });
  table.set_function("sitDown", [](const int &_handle, const int &_time) { Command<0x06B0>(_handle, _time); });
  table.set_function("turnCharToFaceCoord", [](const int &_handle, const float &_x, const float &_y, const float &_z) {
    Command<0x06BA>(_handle, _x, _y, _z);
  });
  table.set_function("drivePointRoute", [](const int &_char, const int &_vehicle, const int &_speed) {
    Command<0x06BB>(_char, _vehicle, _speed);
  });
  table.set_function("goToCoordWhileAiming", [](const int &_char, const float &_x, const float &_y, const float &_z,
                                                const int &_mode, const float &_turnRadius, const float &_stopRadius,
                                                const int &_target, const float &_xOffset, const float &_yOffset,
                                                const float &_zOffset) {
    Command<0x06C2>(_char, _x, _y, _z, _mode, _turnRadius, _stopRadius, _target, _xOffset, _yOffset, _zOffset);
  });
  table.set_function("carTempAction", [](const int &_char, const int &_vehicle, const int &_actionId,
                                         const int &_time) { Command<0x06C7>(_char, _vehicle, _actionId, _time); });
  table.set_function("carMission", [](const int &_char, const int &_vehicle, const int &_targetVehicle,
                                      const int &_missionId, const float &_cruiseSpeed, const int &_drivingStyle) {
    Command<0x06E1>(_char, _vehicle, _targetVehicle, _missionId, _cruiseSpeed, _drivingStyle);
  });
  table.set_function("goToObject", [](const int &_char, const int &_object, const int &_time, const float &_radius) {
    Command<0x06E2>(_char, _object, _time, _radius);
  });
  table.set_function("weaponRoll",
                     [](const int &_handle, const bool &_direction) { Command<0x06E3>(_handle, _direction); });
  table.set_function("charArrestChar", [](const int &_char, const int &_target) { Command<0x06E4>(_char, _target); });
  table.set_function("pickUpObject", [](const int &_char, const int &_object, const float &_xOffset,
                                        const float &_yOffset, const float &_zOffset, const int &_boneId,
                                        const int &__p7, const char *_animationName, const char *_animationFile,
                                        const int &_time) {
    Command<0x070A>(_char, _object, _xOffset, _yOffset, _zOffset, _boneId, __p7, _animationName, _animationFile, _time);
  });
  table.set_function("driveBy", [](const int &_handle, const int &_targetChar, const int &_targetVehicle,
                                   const float &_x, const float &_y, const float &_z, const float &_radius,
                                   const int &_style, const bool &_rightHandCarSeat, const int &_fireRate) {
    Command<0x0713>(_handle, _targetChar, _targetVehicle, _x, _y, _z, _radius, _style, _rightHandCarSeat, _fireRate);
  });
  table.set_function("useMobilePhone",
                     [](const int &_handle, const bool &_start) { Command<0x0729>(_handle, _start); });
  table.set_function("warpCharIntoCarAsDriver",
                     [](const int &_char, const int &_vehicle) { Command<0x072A>(_char, _vehicle); });
  table.set_function("warpCharIntoCarAsPassenger", [](const int &_char, const int &_vehicle, const int &_seatId) {
    Command<0x072B>(_char, _vehicle, _seatId);
  });
  table.set_function("useAttractor",
                     [](const int &_char, const int &_attractor) { Command<0x074C>(_char, _attractor); });
  table.set_function("shootAtChar", [](const int &_handle, const int &_target, const int &_time) {
    Command<0x074D>(_handle, _target, _time);
  });
  table.set_function("fleeCharAnyMeans",
                     [](const int &_handle, const int &_threat, const float &_runDistance, const int &_time,
                        const bool &_changeCourse, const int &__p6, const int &__p7, const float &_radius) {
                       Command<0x0751>(_handle, _threat, _runDistance, _time, _changeCourse, __p6, __p7, _radius);
                     });
  table.set_function("dead", [](const int &_handle) { Command<0x0762>(_handle); });
  table.set_function("gotoCar", [](const int &_char, const int &_vehicle, const int &_time, const float &_radius) {
    Command<0x0772>(_char, _vehicle, _time, _radius);
  });
  table.set_function("climb", [](const int &_handle, const bool &_flag) { Command<0x078F>(_handle, _flag); });
  table.set_function("gotoCharAiming",
                     [](const int &_handle, const int &_target, const float &_radiusFrom, const float &_radiusTo) {
                       Command<0x07A3>(_handle, _target, _radiusFrom, _radiusTo);
                     });
  table.set_function("killCharOnFootTimed", [](const int &_handle, const int &_target, const int &_time) {
    Command<0x07A5>(_handle, _target, _time);
  });
  table.set_function("jetpack", [](const int &_handle) { Command<0x07A7>(_handle); });
  table.set_function("setCharDecisionMaker", [](const int &_char, const int &_decisionMakerChar) {
    Command<0x07BC>(_char, _decisionMakerChar);
  });
  table.set_function("complexPickupObject",
                     [](const int &_char, const int &_object) { Command<0x07C9>(_char, _object); });
  table.set_function("charSlideToCoord",
                     [](const int &_handle, const float &_x, const float &_y, const float &_z, const float &_angle,
                        const float &_radius) { Command<0x07CD>(_handle, _x, _y, _z, _angle, _radius); });
  table.set_function("swimToCoord", [](const int &_handle, const float &_x, const float &_y, const float &_z) {
    Command<0x07E1>(_handle, _x, _y, _z);
  });
  table.set_function("drivePointRouteAdvanced",
                     [](const int &_char, const int &_vehicle, const float &_speed, const int &__p4, const int &__p5,
                        const int &__p6) { Command<0x07E7>(_char, _vehicle, _speed, __p4, __p5, __p6); });
  table.set_function("charSlideToCoordAndPlayAnim",
                     [](const int &_handle, const float &_x, const float &_y, const float &_z, const float &_angle,
                        const float &_radius, const char *_animationName, const char *_animationFile,
                        const float &_frameDelta, const bool &_loop, const bool &_lockX, const bool &_lockY,
                        const bool &_lockF, const int &_time) {
                       Command<0x0804>(_handle, _x, _y, _z, _angle, _radius, _animationName, _animationFile,
                                       _frameDelta, _loop, _lockX, _lockY, _lockF, _time);
                     });
  table.set_function("playAnimNonInterruptable", [](const int &_handle, const char *_animationName,
                                                    const char *_animationFile, const float &_frameDelta,
                                                    const bool &_loop, const bool &_lockX, const bool &_lockY,
                                                    const bool &_lockF, const int &_time) {
    Command<0x0812>(_handle, _animationName, _animationFile, _frameDelta, _loop, _lockX, _lockY, _lockF, _time);
  });
  table.set_function("followPatrolRoute", [](const int &_handle, const int &_walkSpeed, const int &_routeMode) {
    Command<0x0817>(_handle, _walkSpeed, _routeMode);
  });
  table.set_function("greetPartner", [](const int &_handle, const int &_partner, const float &__p3, const int &__p4) {
    Command<0x0823>(_handle, _partner, __p3, __p4);
  });
  table.set_function("dieNamedAnim", [](const int &_handle, const char *_animationName, const char *_animationFile,
                                        const float &_frameDelta, const int &_time) {
    Command<0x0829>(_handle, _animationName, _animationFile, _frameDelta, _time);
  });
  table.set_function("followFootsteps",
                     [](const int &_handle, const int &_target) { Command<0x0850>(_handle, _target); });
  table.set_function("walkAlongsideChar",
                     [](const int &_handle, const int &_target) { Command<0x0859>(_handle, _target); });
  table.set_function("kindaStayInSamePlace",
                     [](const int &_handle, const bool &_state) { Command<0x085B>(_handle, _state); });
  table.set_function("playAnimWithFlags",
                     [](const int &_handle, const char *_animationName, const char *_animationFile,
                        const float &_frameDelta, const bool &_loop, const bool &_lockX, const bool &_lockY,
                        const bool &_lockF, const int &_time, const bool &_disableForce, const bool &_disableLockZ) {
                       Command<0x088A>(_handle, _animationName, _animationFile, _frameDelta, _loop, _lockX, _lockY,
                                       _lockF, _time, _disableForce, _disableLockZ);
                     });
  table.set_function("useClosestMapAttractor",
                     [](const int &_handle, const float &_radius, const int &_modelId, const float &_fromX,
                        const float &_fromY, const float &_fromZ, const char *_name) {
                       Command<0x08A0>(_handle, _radius, _modelId, _fromX, _fromY, _fromZ, _name);
                     });
  table.set_function("setIgnoreWeaponRangeFlag",
                     [](const int &_handle, const bool &_state) { Command<0x099F>(_handle, _state); });
  table.set_function("pickUpSecondObject", [](const int &_char, const int &_object, const float &_xOffset,
                                              const float &_yOffset, const float &_zOffset, const int &_boneId,
                                              const int &__p7, const char *_animationName, const char *_animationFile,
                                              const int &_time) {
    Command<0x09A0>(_char, _object, _xOffset, _yOffset, _zOffset, _boneId, __p7, _animationName, _animationFile, _time);
  });
  table.set_function("playAnimSecondary", [](const int &_handle, const char *_animationFile, const char *_animationName,
                                             const float &_frameDelta, const bool &_loop, const bool &_lockX,
                                             const bool &_lockY, const bool &_lockF, const int &_time) {
    Command<0x0A1A>(_handle, _animationFile, _animationName, _frameDelta, _loop, _lockX, _lockY, _lockF, _time);
  });
  table.set_function("handGesture", [](const int &_handle, const int &_target) { Command<0x0A1D>(_handle, _target); });
  table.set_function("followPathNodesToCoordWithRadius",
                     [](const int &_handle, const float &_x, const float &_y, const float &_z, const int &_mode,
                        const int &_time,
                        const float &_radius) { Command<0x0A2E>(_handle, _x, _y, _z, _mode, _time, _radius); });
}