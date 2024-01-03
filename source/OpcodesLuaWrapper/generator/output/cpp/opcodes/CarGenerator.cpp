// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindCarGenerator(sol::state &state) {
  auto table = state.create_table("CarGeneratorOp");
  // default
  table.set_function("create", [](const float &_x, const float &_y, const float &_z, const float &_heading,
                                  const int &_modelId, const int &_primaryColor, const int &_secondaryColor,
                                  const int &_forceSpawn, const int &_alarmChance, const int &_doorLockChance,
                                  const int &_minDelay, const int &_maxDelay) {
    int handle_;
    Command<0x014B>(_x, _y, _z, _heading, _modelId, _primaryColor, _secondaryColor, _forceSpawn, _alarmChance,
                    _doorLockChance, _minDelay, _maxDelay, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("switch", [](const int &_self, const int &_amount) { Command<0x014C>(_self, _amount); });
  table.set_function("suppressCarModel", [](const int &_model) { Command<0x0732>(_model); });
  table.set_function("dontSuppressCarModel", [](const int &_modelId) { Command<0x0733>(_modelId); });
  table.set_function("dontSuppressAnyCarModels", []() { Command<0x0734>(); });
  table.set_function("createWithPlate", [](const float &_x, const float &_y, const float &_z, const float &_heading,
                                           const int &_modelId, const int &_primaryColor, const int &_secondaryColor,
                                           const int &_forceSpawn, const int &_alarmChance, const int &_doorLockChance,
                                           const int &_minDelay, const int &_maxDelay, const char *_plateName) {
    int handle_;
    Command<0x09E2>(_x, _y, _z, _heading, _modelId, _primaryColor, _secondaryColor, _forceSpawn, _alarmChance,
                    _doorLockChance, _minDelay, _maxDelay, _plateName, &handle_);
    return std::make_tuple(handle_);
  });
  table.set_function("setHasBeenOwned", [](const int &_self, const int &_state) { Command<0x0A17>(_self, _state); });

  // CLEO+
  table.set_function("setNoSave", [](const int &_self) { Command<0x0E02>(_self); });
}