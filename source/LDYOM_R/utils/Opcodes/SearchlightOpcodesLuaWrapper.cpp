//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void SearchlightOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("SearchlightOp");
    //default
	table.set_function("create", [](const float& _x, const float& _y, const float& _z, const float& _xPoint, const float& _yPoint, const float& _zPoint, const float& _radius, const float& _radiusPoint) {int handle_; auto result = Command<0x06B1>(_x, _y, _z, _xPoint, _yPoint, _zPoint, _radius, _radiusPoint, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("delete", [](const int& _self) { auto result = Command<0x06B2>(_self); return std::make_tuple(result);});
	table.set_function("doesExist", [](const int& _handle) { auto result = Command<0x06B3>(_handle); return std::make_tuple(result);});
	table.set_function("moveBetweenCoords", [](const int& _self, const float& _fromX, const float& _fromY, const float& _fromZ, const float& _toX, const float& _toY, const float& _toZ, const float& _speed) { auto result = Command<0x06B4>(_self, _fromX, _fromY, _fromZ, _toX, _toY, _toZ, _speed); return std::make_tuple(result);});
	table.set_function("pointAtCoord", [](const int& _self, const float& _x, const float& _y, const float& _z, const float& _speed) { auto result = Command<0x06B5>(_self, _x, _y, _z, _speed); return std::make_tuple(result);});
	table.set_function("pointAtChar", [](const int& _self, const int& _handle, const float& _speed) { auto result = Command<0x06B6>(_self, _handle, _speed); return std::make_tuple(result);});
	table.set_function("isCharIn", [](const int& _self, const int& _handle) { auto result = Command<0x06B7>(_self, _handle); return std::make_tuple(result);});
	table.set_function("pointAtVehicle", [](const int& _self, const int& _handle, const float& _speed) { auto result = Command<0x06BF>(_self, _handle, _speed); return std::make_tuple(result);});
	table.set_function("isVehicleIn", [](const int& _self, const int& _handle) { auto result = Command<0x06C0>(_self, _handle); return std::make_tuple(result);});
	table.set_function("createOnVehicle", [](const int& _vehicle, const float& _xOffset, const float& _yOffset, const float& _zOffset, const float& _xPoint, const float& _yPoint, const float& _zPoint, const float& _pointRadius, const float& _radius) {int handle_; auto result = Command<0x06C1>(_vehicle, _xOffset, _yOffset, _zOffset, _xPoint, _yPoint, _zPoint, _pointRadius, _radius, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("attachToObject", [](const int& _self, const int& _spotTower, const int& _spotHousing, const int& _spotBulb, const float& _xOffset, const float& _yOffset, const float& _zOffset) { auto result = Command<0x06CA>(_self, _spotTower, _spotHousing, _spotBulb, _xOffset, _yOffset, _zOffset); return std::make_tuple(result);});
	table.set_function("setClipIfColliding", [](const int& _self, const bool& _state) { auto result = Command<0x0941>(_self, _state); return std::make_tuple(result);});
	table.set_function("switchOnGround", [](const int& _self, const bool& _state) { auto result = Command<0x0A02>(_self, _state); return std::make_tuple(result);});

}