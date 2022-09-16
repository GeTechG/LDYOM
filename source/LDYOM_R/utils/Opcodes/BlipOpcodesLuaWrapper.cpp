//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void BlipOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("BlipOp");
    //default
	table.set_function("addForCarOld", [](const int& _vehicle, const int& _color, const int& _display) {int handle_; auto result = Command<0x0161>(_vehicle, _color, _display, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("remove", [](const int& _self) { auto result = Command<0x0164>(_self); return std::make_tuple(result);});
	table.set_function("changeColor", [](const int& _self, const int& _color) { auto result = Command<0x0165>(_self, _color); return std::make_tuple(result);});
	table.set_function("addForCoordOld", [](const float& _x, const float& _y, const float& _z, const int& _colour, const int& _display) {int handle_; auto result = Command<0x0167>(_x, _y, _z, _colour, _display, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("changeScale", [](const int& _self, const int& _size) { auto result = Command<0x0168>(_self, _size); return std::make_tuple(result);});
	table.set_function("addForCar", [](const int& _vehicle) {int handle_; auto result = Command<0x0186>(_vehicle, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("addForChar", [](const int& _char) {int handle_; auto result = Command<0x0187>(_char, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("addForObject", [](const int& _object) {int handle_; auto result = Command<0x0188>(_object, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("addForCoord", [](const float& _x, const float& _y, const float& _z) {int handle_; auto result = Command<0x018A>(_x, _y, _z, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("changeDisplay", [](const int& _self, const int& _display) { auto result = Command<0x018B>(_self, _display); return std::make_tuple(result);});
	table.set_function("addSpriteForContactPoint", [](const float& _x, const float& _y, const float& _z, const int& _sprite) {int handle_; auto result = Command<0x02A7>(_x, _y, _z, _sprite, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("addSpriteForCoord", [](const float& _x, const float& _y, const float& _z, const int& _sprite) {int handle_; auto result = Command<0x02A8>(_x, _y, _z, _sprite, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("addForPickup", [](const int& _pickup) {int handle_; auto result = Command<0x03DC>(_pickup, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("addShortRangeSpriteForCoord", [](const float& _x, const float& _y, const float& _z, const int& _sprite) {int handle_; auto result = Command<0x04CE>(_x, _y, _z, _sprite, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("addShortRangeSpriteForContactPoint", [](const float& _x, const float& _y, const float& _z, const int& _sprite) {int handle_; auto result = Command<0x0570>(_x, _y, _z, _sprite, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("addForSearchlight", [](const int& _searchlight) {int handle_; auto result = Command<0x06C4>(_searchlight, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("doesExist", [](const int& _handle) { auto result = Command<0x075C>(_handle); return std::make_tuple(result);});
	table.set_function("setAlwaysDisplayOnZoomedRadar", [](const int& _self, const bool& _state) { auto result = Command<0x07BF>(_self, _state); return std::make_tuple(result);});
	table.set_function("setAsFriendly", [](const int& _self, const bool& _state) { auto result = Command<0x07E0>(_self, _state); return std::make_tuple(result);});
	table.set_function("addForDeadChar", [](const int& _char) {int handle_; auto result = Command<0x0888>(_char, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("setEntryExit", [](const int& _self, const float& _x, const float& _y, const float& _radius) { auto result = Command<0x08DC>(_self, _x, _y, _radius); return std::make_tuple(result);});
	table.set_function("setCoordAppearance", [](const int& _self, const int& _color) { auto result = Command<0x08FB>(_self, _color); return std::make_tuple(result);});

}