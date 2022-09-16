//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void HeliOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("HeliOp");
    //default
	table.set_function("gotoCoords", [](const int& _self, const float& _x, const float& _y, const float& _z, const float& _minAltitude, const float& _maxAltitude) { auto result = Command<0x04A2>(_self, _x, _y, _z, _minAltitude, _maxAltitude); return std::make_tuple(result);});
	table.set_function("setOrientation", [](const int& _self, const float& _angle) { auto result = Command<0x04D0>(_self, _angle); return std::make_tuple(result);});
	table.set_function("clearOrientation", [](const int& _self) { auto result = Command<0x04D1>(_self); return std::make_tuple(result);});
	table.set_function("setStabiliser", [](const int& _self, const bool& _state) { auto result = Command<0x04DF>(_self, _state); return std::make_tuple(result);});
	table.set_function("fireHunterGun", [](const int& _self) { auto result = Command<0x0541>(_self); return std::make_tuple(result);});
	table.set_function("makeComeCrashingDown", [](const int& _self) { auto result = Command<0x0564>(_self); return std::make_tuple(result);});
	table.set_function("attackPlayer", [](const int& _self, const int& _handle, const float& _radius) { auto result = Command<0x0724>(_self, _handle, _radius); return std::make_tuple(result);});
	table.set_function("followEntity", [](const int& _self, const int& _char, const int& _vehicle, const float& _radius) { auto result = Command<0x0726>(_self, _char, _vehicle, _radius); return std::make_tuple(result);});
	table.set_function("chaseEntity", [](const int& _self, const int& _char, const int& _vehicle, const float& _radius) { auto result = Command<0x0727>(_self, _char, _vehicle, _radius); return std::make_tuple(result);});
	table.set_function("landAtCoords", [](const int& _self, const float& _x, const float& _y, const float& _z, const float& _minAltitude, const float& _maxAltitude) { auto result = Command<0x0743>(_self, _x, _y, _z, _minAltitude, _maxAltitude); return std::make_tuple(result);});
	table.set_function("keepEntityInView", [](const int& _self, const int& _char, const int& _vehicle, const float& _minAltitude, const int& _maxAltitude) { auto result = Command<0x0780>(_self, _char, _vehicle, _minAltitude, _maxAltitude); return std::make_tuple(result);});
	table.set_function("attachWinch", [](const int& _self, const bool& _state) { auto result = Command<0x0788>(_self, _state); return std::make_tuple(result);});
	table.set_function("releaseEntityFromWinch", [](const int& _self) { auto result = Command<0x0789>(_self); return std::make_tuple(result);});
	table.set_function("grabEntityOnWinch", [](const int& _self) {int char_;int vehicle_;int object_; auto result = Command<0x078B>(_self, &char_, &vehicle_, &object_); return std::make_tuple(char_, vehicle_, object_, result);});
	table.set_function("activateSpeedCheat", [](const int& _self, const int& _power) { auto result = Command<0x07BB>(_self, _power); return std::make_tuple(result);});
	table.set_function("setBladesFullSpeed", [](const int& _self) { auto result = Command<0x0825>(_self); return std::make_tuple(result);});
	table.set_function("setReachedTargetDistance", [](const int& _self, const int& _distance) { auto result = Command<0x0853>(_self, _distance); return std::make_tuple(result);});
	table.set_function("disableAudio", [](const int& _self, const bool& _state) { auto result = Command<0x0A1C>(_self, _state); return std::make_tuple(result);});

}