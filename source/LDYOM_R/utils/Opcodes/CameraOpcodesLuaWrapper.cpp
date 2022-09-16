//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void CameraOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("CameraOp");
    //default
	table.set_function("shake", [](const int& _intensity) { auto result = Command<0x0003>(_intensity); return std::make_tuple(result);});
	table.set_function("isPointOnScreen", [](const float& _x, const float& _y, const float& _z, const float& _radius) { auto result = Command<0x00C2>(_x, _y, _z, _radius); return std::make_tuple(result);});
	table.set_function("pointAtCar", [](const int& _vehicle, const int& _mode, const int& _switchStyle) { auto result = Command<0x0158>(_vehicle, _mode, _switchStyle); return std::make_tuple(result);});
	table.set_function("pointAtChar", [](const int& _char, const int& _mode, const int& _switchStyle) { auto result = Command<0x0159>(_char, _mode, _switchStyle); return std::make_tuple(result);});
	table.set_function("restore", []() { auto result = Command<0x015A>(); return std::make_tuple(result);});
	table.set_function("setFixedPosition", [](const float& _x, const float& _y, const float& _z, const float& _xRotation, const float& _yRotation, const float& _zRotation) { auto result = Command<0x015F>(_x, _y, _z, _xRotation, _yRotation, _zRotation); return std::make_tuple(result);});
	table.set_function("pointAtPoint", [](const float& _x, const float& _y, const float& _z, const int& _switchStyle) { auto result = Command<0x0160>(_x, _y, _z, _switchStyle); return std::make_tuple(result);});
	table.set_function("setFadingColor", [](const int& _r, const int& _g, const int& _b) { auto result = Command<0x0169>(_r, _g, _b); return std::make_tuple(result);});
	table.set_function("doFade", [](const int& _time, const int& _direction) { auto result = Command<0x016A>(_time, _direction); return std::make_tuple(result);});
	table.set_function("getFadingStatus", []() { auto result = Command<0x016B>(); return std::make_tuple(result);});
	table.set_function("restoreJumpcut", []() { auto result = Command<0x02EB>(); return std::make_tuple(result);});
	table.set_function("setZoom", [](const int& _zoom) { auto result = Command<0x032A>(_zoom); return std::make_tuple(result);});
	table.set_function("setBehindPlayer", []() { auto result = Command<0x0373>(); return std::make_tuple(result);});
	table.set_function("setInFrontOfPlayer", []() { auto result = Command<0x03C8>(); return std::make_tuple(result);});
	table.set_function("setNearClip", [](const float& _value) { auto result = Command<0x041D>(_value); return std::make_tuple(result);});
	table.set_function("getDebugCoordinates", []() {float x_;float y_;float z_; auto result = Command<0x0454>(&x_, &y_, &z_); return std::make_tuple(x_, y_, z_, result);});
	table.set_function("setInterpolationParameters", [](const float& __p1, const int& _time) { auto result = Command<0x0460>(__p1, _time); return std::make_tuple(result);});
	table.set_function("getDebugPointAt", []() {float x_;float y_;float z_; auto result = Command<0x0463>(&x_, &y_, &z_); return std::make_tuple(x_, y_, z_, result);});
	table.set_function("attachToVehicle", [](const int& _handle, const float& _xOffset, const float& _yOffset, const float& _zOffset, const float& _xRotation, const float& _yRotation, const float& _zRotation, const float& _tilt, const int& _switchStyle) { auto result = Command<0x0679>(_handle, _xOffset, _yOffset, _zOffset, _xRotation, _yRotation, _zRotation, _tilt, _switchStyle); return std::make_tuple(result);});
	table.set_function("attachToVehicleLookAtVehicle", [](const int& _handle, const float& _xOffset, const float& _yOffset, const float& _zOffset, const int& _vehicle, const float& _tilt, const int& _switchStyle) { auto result = Command<0x067A>(_handle, _xOffset, _yOffset, _zOffset, _vehicle, _tilt, _switchStyle); return std::make_tuple(result);});
	table.set_function("attachToVehicleLookAtChar", [](const int& _handle, const float& _xOffset, const float& _yOffset, const float& _zOffset, const int& _char, const float& _tilt, const int& _switchStyle) { auto result = Command<0x067B>(_handle, _xOffset, _yOffset, _zOffset, _char, _tilt, _switchStyle); return std::make_tuple(result);});
	table.set_function("attachToChar", [](const int& _handle, const float& _xOffset, const float& _yOffset, const float& _zOffset, const float& _xRotation, const float& _yRotation, const float& _zRotation, const float& _tilt, const int& _switchStyle) { auto result = Command<0x067C>(_handle, _xOffset, _yOffset, _zOffset, _xRotation, _yRotation, _zRotation, _tilt, _switchStyle); return std::make_tuple(result);});
	table.set_function("attachToCharLookAtVehicle", [](const int& _char, const float& _xOffset, const float& _yOffset, const float& _zOffset, const int& _vehicle, const float& _tilt, const int& _switchStyle) { auto result = Command<0x067D>(_char, _xOffset, _yOffset, _zOffset, _vehicle, _tilt, _switchStyle); return std::make_tuple(result);});
	table.set_function("attachToCharLookAtChar", [](const int& _handle, const float& _xOffset, const float& _yOffset, const float& _zOffset, const int& _char, const float& _tilt, const int& _switchStyle) { auto result = Command<0x067E>(_handle, _xOffset, _yOffset, _zOffset, _char, _tilt, _switchStyle); return std::make_tuple(result);});
	table.set_function("getActiveCoordinates", []() {float x_;float y_;float z_; auto result = Command<0x068D>(&x_, &y_, &z_); return std::make_tuple(x_, y_, z_, result);});
	table.set_function("getActivePointAt", []() {float x_;float y_;float z_; auto result = Command<0x068E>(&x_, &y_, &z_); return std::make_tuple(x_, y_, z_, result);});
	table.set_function("setTwoPlayerMode", [](const bool& _state) { auto result = Command<0x06E0>(_state); return std::make_tuple(result);});
	table.set_function("getFov", []() {float fov_; auto result = Command<0x0801>(&fov_); return std::make_tuple(fov_, result);});
	table.set_function("setFirstPersonInCarMode", [](const bool& _state) { auto result = Command<0x0822>(_state); return std::make_tuple(result);});
	table.set_function("doBump", [](const float& _xOffset, const float& _yOffset) { auto result = Command<0x0834>(_xOffset, _yOffset); return std::make_tuple(result);});
	table.set_function("setVectorTrack", [](const float& _fromX, const float& _fromY, const float& _fromZ, const float& _toX, const float& _toY, const float& _toZ, const int& _time, const bool& _ease) { auto result = Command<0x0920>(_fromX, _fromY, _fromZ, _toX, _toY, _toZ, _time, _ease); return std::make_tuple(result);});
	table.set_function("setLerpFov", [](const float& _from, const float& _to, const int& _time, const bool& _ease) { auto result = Command<0x0922>(_from, _to, _time, _ease); return std::make_tuple(result);});
	table.set_function("setDarknessEffect", [](const bool& _enable, const int& _pitchBlack) { auto result = Command<0x0924>(_enable, _pitchBlack); return std::make_tuple(result);});
	table.set_function("resetNewScriptables", []() { auto result = Command<0x0925>(); return std::make_tuple(result);});
	table.set_function("persistTrack", [](const bool& _state) { auto result = Command<0x092F>(_state); return std::make_tuple(result);});
	table.set_function("persistPos", [](const bool& _state) { auto result = Command<0x0930>(_state); return std::make_tuple(result);});
	table.set_function("persistFov", [](const bool& _state) { auto result = Command<0x0931>(_state); return std::make_tuple(result);});
	table.set_function("isVectorMoveRunning", []() { auto result = Command<0x0933>(); return std::make_tuple(result);});
	table.set_function("isVectorTrackRunning", []() { auto result = Command<0x0934>(); return std::make_tuple(result);});
	table.set_function("setVectorMove", [](const float& _fromX, const float& _fromY, const float& _fromZ, const float& _toX, const float& _toY, const float& _toZ, const int& _time, const bool& _ease) { auto result = Command<0x0936>(_fromX, _fromY, _fromZ, _toX, _toY, _toZ, _time, _ease); return std::make_tuple(result);});
	table.set_function("setCinema", [](const bool& _state) { auto result = Command<0x093D>(_state); return std::make_tuple(result);});
	table.set_function("setInFrontOfChar", [](const int& _handle) { auto result = Command<0x0944>(_handle); return std::make_tuple(result);});
	table.set_function("setShakeSimulationSimple", [](const int& _type, const float& _timeInMs, const float& _intensity) { auto result = Command<0x099C>(_type, _timeInMs, _intensity); return std::make_tuple(result);});
	table.set_function("setPlayerInCarMode", [](const int& _mode) { auto result = Command<0x09AD>(_mode); return std::make_tuple(result);});
	table.set_function("allowFixedCollision", [](const bool& _state) { auto result = Command<0x09EC>(_state); return std::make_tuple(result);});
	table.set_function("setVehicleTweak", [](const int& _modelId, const float& _distance, const float& _altitude, const float& _angle) { auto result = Command<0x09EF>(_modelId, _distance, _altitude, _angle); return std::make_tuple(result);});
	table.set_function("resetVehicleTweak", []() { auto result = Command<0x09F0>(); return std::make_tuple(result);});
	table.set_function("setPositionUnfixed", [](const float& _xOffset, const float& _yOffset) { auto result = Command<0x0A25>(_xOffset, _yOffset); return std::make_tuple(result);});
	table.set_function("setPhotoEffect", [](const bool& _state) { auto result = Command<0x0A2F>(_state); return std::make_tuple(result);});
	table.set_function("getPlayerInCarMode", []() {int mode_; auto result = Command<0x0A39>(&mode_); return std::make_tuple(mode_, result);});

}