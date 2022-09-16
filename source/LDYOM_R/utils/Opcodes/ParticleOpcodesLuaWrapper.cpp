//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void ParticleOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("ParticleOp");
    //default
	table.set_function("create", [](const char* _name, const float& _x, const float& _y, const float& _z, const int& _type) {int handle_; auto result = Command<0x064B>(_name, _x, _y, _z, _type, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("play", [](const int& _self) { auto result = Command<0x064C>(_self); return std::make_tuple(result);});
	table.set_function("stop", [](const int& _self) { auto result = Command<0x064E>(_self); return std::make_tuple(result);});
	table.set_function("playAndKill", [](const int& _self) { auto result = Command<0x064F>(_self); return std::make_tuple(result);});
	table.set_function("kill", [](const int& _self) { auto result = Command<0x0650>(_self); return std::make_tuple(result);});
	table.set_function("createOnChar", [](const char* _name, const int& _char, const float& _xOffset, const float& _yOffset, const float& _zOffset, const int& _type) {int handle_; auto result = Command<0x0669>(_name, _char, _xOffset, _yOffset, _zOffset, _type, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("createOnCharWithDirection", [](const char* _name, const int& _char, const float& _xOffset, const float& _yOffset, const float& _zOffset, const float& _xDirection, const float& _yDirection, const float& _zDirection, const int& _type) {int handle_; auto result = Command<0x066A>(_name, _char, _xOffset, _yOffset, _zOffset, _xDirection, _yDirection, _zDirection, _type, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("createOnCar", [](const char* _name, const int& _vehicle, const float& _xOffset, const float& _yOffset, const float& _zOffset, const float& _type) {int handle_; auto result = Command<0x066B>(_name, _vehicle, _xOffset, _yOffset, _zOffset, _type, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("createOnCarWithDirection", [](const char* _name, const int& _vehicle, const float& _xOffset, const float& _yOffset, const float& _zOffset, const float& _xDirection, const float& _yDirection, const float& _zDirection, const int& _type) {int handle_; auto result = Command<0x066C>(_name, _vehicle, _xOffset, _yOffset, _zOffset, _xDirection, _yDirection, _zDirection, _type, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("createOnObject", [](const char* _name, const int& _object, const float& _xOffset, const float& _yOffset, const float& _zOffset, const int& _type) {int handle_; auto result = Command<0x066D>(_name, _object, _xOffset, _yOffset, _zOffset, _type, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("createOnObjectWithDirection", [](const char* _name, const int& _object, const float& _xOffset, const float& _yOffset, const float& _zOffset, const float& _xDirection, const float& _yDirection, const float& _zDirection, const int& _type) {int handle_; auto result = Command<0x066E>(_name, _object, _xOffset, _yOffset, _zOffset, _xDirection, _yDirection, _zDirection, _type, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("attachToCharBone", [](const int& _self, const int& _handle, const int& _boneId) { auto result = Command<0x0883>(_self, _handle, _boneId); return std::make_tuple(result);});
	table.set_function("killNow", [](const int& _self) { auto result = Command<0x0976>(_self); return std::make_tuple(result);});

}