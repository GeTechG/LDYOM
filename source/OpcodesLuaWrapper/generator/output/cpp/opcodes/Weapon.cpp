// This file is generating, it cannot be changed.
#include <extensions/ScriptCommands.h>
#include <sol.hpp>

using namespace plugin;

void bindWeapon(sol::state &state) {
  auto table = state.create_table("WeaponOp");
  // default
  table.set_function("getModel", [](const int &_weaponType) {
    int modelId_;
    Command<0x0781>(_weaponType, &modelId_);
    return std::make_tuple(modelId_);
  });
  table.set_function("getSlot", [](const int &_weaponType) {
    int slot_;
    Command<0x0782>(_weaponType, &slot_);
    return std::make_tuple(slot_);
  });

  // CLEO+
  table.set_function("isFireType", [](const int &_weaponType, const int &_weaponFire) {
    auto result = Command<0x0E26>(_weaponType, _weaponFire);
    return std::make_tuple(result);
  });
  table.set_function("getWeaponInfo", [](const int &_weaponType, const int &_weaponSkill) {
    int weaponInfo_;
    auto result = Command<0x0E84>(_weaponType, _weaponSkill, &weaponInfo_);
    return std::make_tuple(result, weaponInfo_);
  });
  table.set_function("getWeaponInfoModels", [](const int &_weaponInfo) {
    int model1_;
    int model2_;
    Command<0x0E85>(_weaponInfo, &model1_, &model2_);
    return std::make_tuple(model1_, model2_);
  });
  table.set_function("getWeaponInfoFlags", [](const int &_weaponInfo) {
    int flags_;
    Command<0x0E86>(_weaponInfo, &flags_);
    return std::make_tuple(flags_);
  });
  table.set_function("getWeaponInfoAnimGroup", [](const int &_weaponInfo) {
    int animGroup_;
    Command<0x0E87>(_weaponInfo, &animGroup_);
    return std::make_tuple(animGroup_);
  });
  table.set_function("getWeaponInfoTotalClip", [](const int &_weaponInfo) {
    int totalClip_;
    Command<0x0E88>(_weaponInfo, &totalClip_);
    return std::make_tuple(totalClip_);
  });
  table.set_function("getWeaponinfoFireType", [](const int &_weaponInfo) {
    int fireType_;
    Command<0x0E89>(_weaponInfo, &fireType_);
    return std::make_tuple(fireType_);
  });
  table.set_function("getWeaponInfoSlot", [](const int &_weaponInfo) {
    int weaponSlot_;
    Command<0x0E8A>(_weaponInfo, &weaponSlot_);
    return std::make_tuple(weaponSlot_);
  });
}