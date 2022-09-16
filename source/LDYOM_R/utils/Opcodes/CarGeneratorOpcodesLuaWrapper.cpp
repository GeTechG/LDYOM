//This file is generating, it cannot be changed.
#include <sol.hpp>
#include <extensions/ScriptCommands.h>

using namespace plugin;

void CarGeneratorOpcodesLuaWrapper(sol::state& state) {
    auto table = state.create_table("CarGeneratorOp");
    //default
	table.set_function("create", [](const float& _x, const float& _y, const float& _z, const float& _heading, const int& _modelId, const int& _primaryColor, const int& _secondaryColor, const bool& _forceSpawn, const int& _alarmChance, const int& _doorLockChance, const int& _minDelay, const int& _maxDelay) {int handle_; auto result = Command<0x014B>(_x, _y, _z, _heading, _modelId, _primaryColor, _secondaryColor, _forceSpawn, _alarmChance, _doorLockChance, _minDelay, _maxDelay, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("switch", [](const int& _self, const int& _amount) { auto result = Command<0x014C>(_self, _amount); return std::make_tuple(result);});
	table.set_function("suppressCarModel", [](const int& _model) { auto result = Command<0x0732>(_model); return std::make_tuple(result);});
	table.set_function("dontSuppressCarModel", [](const int& _modelId) { auto result = Command<0x0733>(_modelId); return std::make_tuple(result);});
	table.set_function("dontSuppressAnyCarModels", []() { auto result = Command<0x0734>(); return std::make_tuple(result);});
	table.set_function("createWithPlate", [](const float& _x, const float& _y, const float& _z, const float& _heading, const int& _modelId, const int& _primaryColor, const int& _secondaryColor, const bool& _forceSpawn, const int& _alarmChance, const int& _doorLockChance, const int& _minDelay, const int& _maxDelay, const char* _plateName) {int handle_; auto result = Command<0x09E2>(_x, _y, _z, _heading, _modelId, _primaryColor, _secondaryColor, _forceSpawn, _alarmChance, _doorLockChance, _minDelay, _maxDelay, _plateName, &handle_); return std::make_tuple(handle_, result);});
	table.set_function("setHasBeenOwned", [](const int& _self, const bool& _state) { auto result = Command<0x0A17>(_self, _state); return std::make_tuple(result);});
	
	//CLEO+
	table.set_function("setNoSave", [](const int& _self) { auto result = Command<0x0E02>(_self); return std::make_tuple(result);});

}