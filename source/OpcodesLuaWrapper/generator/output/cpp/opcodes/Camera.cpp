// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindCamera(sol::state &state) {
  auto table = state.create_table("CameraOp");
  // default
  table.set_function("shake", [](const int &_intensity) { Command<0x0003>(_intensity); });
  table.set_function("isPointOnScreen", [](const float &_x, const float &_y, const float &_z, const float &_radius) {
    auto result = Command<0x00C2>(_x, _y, _z, _radius);
    return std::make_tuple(result);
  });
  table.set_function("pointAtCar", [](const int &_vehicle, const int &_mode, const int &_switchStyle) {
    Command<0x0158>(_vehicle, _mode, _switchStyle);
  });
  table.set_function("pointAtChar", [](const int &_char, const int &_mode, const int &_switchStyle) {
    Command<0x0159>(_char, _mode, _switchStyle);
  });
  table.set_function("restore", []() { Command<0x015A>(); });
  table.set_function("setFixedPosition", [](const float &_x, const float &_y, const float &_z, const float &_xRotation,
                                            const float &_yRotation, const float &_zRotation) {
    Command<0x015F>(_x, _y, _z, _xRotation, _yRotation, _zRotation);
  });
  table.set_function("pointAtPoint", [](const float &_x, const float &_y, const float &_z, const int &_switchStyle) {
    Command<0x0160>(_x, _y, _z, _switchStyle);
  });
  table.set_function("setFadingColor",
                     [](const int &_r, const int &_g, const int &_b) { Command<0x0169>(_r, _g, _b); });
  table.set_function("doFade", [](const int &_time, const int &_direction) { Command<0x016A>(_time, _direction); });
  table.set_function("getFadingStatus", []() {
    auto result = Command<0x016B>();
    return std::make_tuple(result);
  });
  table.set_function("restoreJumpcut", []() { Command<0x02EB>(); });
  table.set_function("setZoom", [](const int &_zoom) { Command<0x032A>(_zoom); });
  table.set_function("setBehindPlayer", []() { Command<0x0373>(); });
  table.set_function("setInFrontOfPlayer", []() { Command<0x03C8>(); });
  table.set_function("setNearClip", [](const float &_value) { Command<0x041D>(_value); });
  table.set_function("getDebugCoordinates", []() {
    float x_;
    float y_;
    float z_;
    Command<0x0454>(&x_, &y_, &z_);
    return std::make_tuple(x_, y_, z_);
  });
  table.set_function("setInterpolationParameters",
                     [](const float &__p1, const int &_time) { Command<0x0460>(__p1, _time); });
  table.set_function("getDebugPointAt", []() {
    float x_;
    float y_;
    float z_;
    Command<0x0463>(&x_, &y_, &z_);
    return std::make_tuple(x_, y_, z_);
  });
  table.set_function("attachToVehicle", [](const int &_handle, const float &_xOffset, const float &_yOffset,
                                           const float &_zOffset, const float &_xRotation, const float &_yRotation,
                                           const float &_zRotation, const float &_tilt, const int &_switchStyle) {
    Command<0x0679>(_handle, _xOffset, _yOffset, _zOffset, _xRotation, _yRotation, _zRotation, _tilt, _switchStyle);
  });
  table.set_function("attachToVehicleLookAtVehicle",
                     [](const int &_handle, const float &_xOffset, const float &_yOffset, const float &_zOffset,
                        const int &_vehicle, const float &_tilt, const int &_switchStyle) {
                       Command<0x067A>(_handle, _xOffset, _yOffset, _zOffset, _vehicle, _tilt, _switchStyle);
                     });
  table.set_function("attachToVehicleLookAtChar",
                     [](const int &_car, const float &_xOffset, const float &_yOffset, const float &_zOffset,
                        const int &_char, const float &_tilt, const int &_switchStyle) {
                       Command<0x067B>(_car, _xOffset, _yOffset, _zOffset, _char, _tilt, _switchStyle);
                     });
  table.set_function("attachToChar", [](const int &_handle, const float &_xOffset, const float &_yOffset,
                                        const float &_zOffset, const float &_xRotation, const float &_yRotation,
                                        const float &_zRotation, const float &_tilt, const int &_switchStyle) {
    Command<0x067C>(_handle, _xOffset, _yOffset, _zOffset, _xRotation, _yRotation, _zRotation, _tilt, _switchStyle);
  });
  table.set_function("attachToCharLookAtVehicle",
                     [](const int &_char, const float &_xOffset, const float &_yOffset, const float &_zOffset,
                        const int &_vehicle, const float &_tilt, const int &_switchStyle) {
                       Command<0x067D>(_char, _xOffset, _yOffset, _zOffset, _vehicle, _tilt, _switchStyle);
                     });
  table.set_function("attachToCharLookAtChar",
                     [](const int &_handle, const float &_xOffset, const float &_yOffset, const float &_zOffset,
                        const int &_char, const float &_tilt, const int &_switchStyle) {
                       Command<0x067E>(_handle, _xOffset, _yOffset, _zOffset, _char, _tilt, _switchStyle);
                     });
  table.set_function("getActiveCoordinates", []() {
    float x_;
    float y_;
    float z_;
    Command<0x068D>(&x_, &y_, &z_);
    return std::make_tuple(x_, y_, z_);
  });
  table.set_function("getActivePointAt", []() {
    float x_;
    float y_;
    float z_;
    Command<0x068E>(&x_, &y_, &z_);
    return std::make_tuple(x_, y_, z_);
  });
  table.set_function("setTwoPlayerMode", [](const int &_state) { Command<0x06E0>(_state); });
  table.set_function("getFov", []() {
    float fov_;
    Command<0x0801>(&fov_);
    return std::make_tuple(fov_);
  });
  table.set_function("setFirstPersonInCarMode", [](const int &_state) { Command<0x0822>(_state); });
  table.set_function("doBump",
                     [](const float &_xOffset, const float &_yOffset) { Command<0x0834>(_xOffset, _yOffset); });
  table.set_function("setVectorTrack",
                     [](const float &_fromX, const float &_fromY, const float &_fromZ, const float &_toX,
                        const float &_toY, const float &_toZ, const int &_time,
                        const int &_ease) { Command<0x0920>(_fromX, _fromY, _fromZ, _toX, _toY, _toZ, _time, _ease); });
  table.set_function("setLerpFov", [](const float &_from, const float &_to, const int &_time, const int &_ease) {
    Command<0x0922>(_from, _to, _time, _ease);
  });
  table.set_function("setDarknessEffect",
                     [](const int &_enable, const int &_pitchBlack) { Command<0x0924>(_enable, _pitchBlack); });
  table.set_function("resetNewScriptables", []() { Command<0x0925>(); });
  table.set_function("persistTrack", [](const int &_state) { Command<0x092F>(_state); });
  table.set_function("persistPos", [](const int &_state) { Command<0x0930>(_state); });
  table.set_function("persistFov", [](const int &_state) { Command<0x0931>(_state); });
  table.set_function("isVectorMoveRunning", []() {
    auto result = Command<0x0933>();
    return std::make_tuple(result);
  });
  table.set_function("isVectorTrackRunning", []() {
    auto result = Command<0x0934>();
    return std::make_tuple(result);
  });
  table.set_function("setVectorMove",
                     [](const float &_fromX, const float &_fromY, const float &_fromZ, const float &_toX,
                        const float &_toY, const float &_toZ, const int &_time,
                        const int &_ease) { Command<0x0936>(_fromX, _fromY, _fromZ, _toX, _toY, _toZ, _time, _ease); });
  table.set_function("setCinema", [](const int &_state) { Command<0x093D>(_state); });
  table.set_function("setInFrontOfChar", [](const int &_handle) { Command<0x0944>(_handle); });
  table.set_function("setShakeSimulationSimple", [](const int &_type, const float &_timeInMs, const float &_intensity) {
    Command<0x099C>(_type, _timeInMs, _intensity);
  });
  table.set_function("setPlayerInCarMode", [](const int &_mode) { Command<0x09AD>(_mode); });
  table.set_function("allowFixedCollision", [](const int &_state) { Command<0x09EC>(_state); });
  table.set_function("setVehicleTweak",
                     [](const int &_modelId, const float &_distance, const float &_altitude, const float &_angle) {
                       Command<0x09EF>(_modelId, _distance, _altitude, _angle);
                     });
  table.set_function("resetVehicleTweak", []() { Command<0x09F0>(); });
  table.set_function("takePhoto", [](const int &__p1) { Command<0x0A1E>(__p1); });
  table.set_function("setPositionUnfixed",
                     [](const float &_xOffset, const float &_yOffset) { Command<0x0A25>(_xOffset, _yOffset); });
  table.set_function("setPhotoEffect", [](const int &_state) { Command<0x0A2F>(_state); });
  table.set_function("getPlayerInCarMode", []() {
    int mode_;
    Command<0x0A39>(&mode_);
    return std::make_tuple(mode_);
  });

  // CLEO+
  table.set_function("setCameraControl", [](const int &_state) { Command<0x0E60>(_state); });
  table.set_function("getCurrentMode", []() {
    int mode_;
    Command<0x0E64>(&mode_);
    return std::make_tuple(mode_);
  });
  table.set_function("getOffsetFromCameraInWorldCoords",
                     [](const float &_offsetX, const float &_offsetY, const float &_offsetZ) {
                       float x_;
                       float y_;
                       float z_;
                       Command<0x0EB2>(_offsetX, _offsetY, _offsetZ, &x_, &y_, &z_);
                       return std::make_tuple(x_, y_, z_);
                     });
  table.set_function("locateDistanceToCoordinates",
                     [](const float &_x, const float &_y, const float &_z, const float &_radius) {
                       auto result = Command<0x0EBE>(_x, _y, _z, _radius);
                       return std::make_tuple(result);
                     });
  table.set_function("getFadeAlpha", []() {
    float alpha_;
    Command<0x0EC7>(&alpha_);
    return std::make_tuple(alpha_);
  });
  table.set_function("getStruct", [](const int &_cCamera) {
    int activeCCam_;
    Command<0x0F12>(_cCamera, &activeCCam_);
    return std::make_tuple(activeCCam_);
  });
  table.set_function("getRotationInputValues", []() {
    float x_;
    float y_;
    Command<0x0F14>(&x_, &y_);
    return std::make_tuple(x_, y_);
  });
  table.set_function("setRotationInputValues", [](const float &_x, const float &_y) { Command<0x0F15>(_x, _y); });
}